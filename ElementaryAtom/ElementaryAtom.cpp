#include <iostream>
#include <raylib.h>
#include <vector>
#include <array>
#include <fstream>
#include <string>

const std::string atom_path = "atom.txt";

constexpr int res_x = 1280;
constexpr int res_y = 720;

constexpr int delta_arrow_shift = 5;
constexpr float delta_scale = 0.05f;

constexpr int nuclear_radius = 5;

constexpr float electron_arrangement = 0.1f; // precentage of circle radius
constexpr int electron_radius = 5;

struct ElectronLayer {
    uint16_t s; // 1 element  (2)
    uint16_t p; // 3 elements (6)
    uint16_t d; // 5 elements (10)
    uint16_t f; // 7 elements (14)
};

struct Atom {
    struct {
        size_t protons : 7;
        size_t neutrons : 8;
    } nuclear;

    std::vector<ElectronLayer> electron;
};

void DrawNuclear(const Atom& atom, int x, int y, float scale) {
    float size = atom.nuclear.protons;
    
    size_t protons = atom.nuclear.protons;
    size_t neutrons = atom.nuclear.neutrons;

    float angle = 0.f;

    float lx;
    float ly;

    while ((protons + neutrons) != 0) {
        angle = rand();
        
        lx = std::cos(angle) * (size * (rand() % 1000) / 1000.f) * scale;
        ly = std::sin(angle) * (size * (rand() % 1000) / 1000.f) * scale;

        if (rand() % (protons + neutrons) > protons) {
            DrawCircle(lx + x, ly + y, nuclear_radius, RED);
            protons--;
        }
        else {
            DrawCircle(lx + x, ly + y, nuclear_radius, GRAY);
            neutrons--;
        }
    }
}

uint16_t& ElectronStage(ElectronLayer& el_layer, int choice) {
    switch (choice) {
    case 0:
        return el_layer.s;
    case 1:
        return el_layer.p;
    case 2:
        return el_layer.d;
    case 3:
        return el_layer.f;
    default:
        return el_layer.s;
    }
}

uint16_t ElectronStage(const ElectronLayer& el_layer, int choice) {
    switch (choice) {
    case 0:
        return el_layer.s;
    case 1:
        return el_layer.p;
    case 2:
        return el_layer.d;
    case 3:
        return el_layer.f;
    default:
        return el_layer.s;
    }
}

void DrawElectrons(const Atom& atom, int x, int y, float scale) {
    float size = atom.nuclear.protons / 1.75;

    uint16_t electrons = 0;

    float angle = 0.f;

    float lx;
    float ly;

    for (size_t l = 0; l < atom.electron.size(); l++) {
        for (size_t s = 0; s < 4; s++) {
            electrons = ElectronStage(atom.electron[l], s);

            if(electrons != 0)
                DrawRing(
                    Vector2{ (float)x,(float)y },
                    ((l + 1) * 4)* (s + 1)* size* size * scale - 2,
                    ((l + 1) * 4)* (s + 1)* size* size * scale,
                    0, 360, 32, DARKBLUE);

            for (size_t e = 0; e < electrons; e++) {
                angle = rand();

                float rx = rand() % uint16_t(std::ceil(electron_arrangement * size));
                float ry = rand() % uint16_t(std::ceil(electron_arrangement * size));

                lx = size * std::cos(angle) * ((l + 1) * 4) * (s + 1) * size + rx;
                ly = size * std::sin(angle) * ((l + 1) * 4) * (s + 1) * size + ry;

                lx *= scale;
                ly *= scale;

                DrawCircle(lx + x, ly + y, electron_radius, BLUE);
            }
        }
    }
}

void InitAtomFile() {
    std::ofstream fout;
    fout.open(atom_path);

    fout << "# Atom\n";

    fout << "# Nuclear\n";
    fout << "# Protons\n";
    fout << 5 << '\n';
    fout << "# Neutrons\n";
    fout << 5 << '\n';

    fout << "# Electrons\n";
    fout << "# Layer count\n";

    fout << "# Each layer should have at least 1 electron to be validated\n";

    

    fout << "S" << '\n';
    fout << 2 << '\n';
    fout << "P" << '\n';
    fout << 0 << '\n';
    fout << "D" << '\n';
    fout << 0 << '\n';
    fout << "F" << '\n';
    fout << 0 << '\n';

    fout << "END\n";

    fout << "S" << '\n';
    fout << 2 << '\n';
    fout << "P" << '\n';
    fout << 3 << '\n';
    fout << "D" << '\n';
    fout << 0 << '\n';
    fout << "F" << '\n';
    fout << 0 << '\n';

    fout << "END\n";

    fout.close();
}

Atom SyncAtomTopology() {
    std::ifstream fin;

    fin.open(atom_path);

    if (!fin.is_open()) {
        InitAtomFile();
        fin.open(atom_path);
    }

    Atom atom;
    std::string line;

    int stage = 0;

    while (!fin.eof()) {
        std::getline(fin, line);

        if (line[0] == '#')
            continue;

        switch (stage) {
        case 0: // protons
            atom.nuclear.protons = std::stoi(line);
            stage++;
            break;
        case 1: // neutrons
            atom.nuclear.neutrons = std::stoi(line);
            stage++;
            break;
        case 2: // electrons
            std::getline(fin, line);
            atom.electron.push_back({ 0,0,0,0 });

            while (line != "END") {
                char saved_char = line[0];
                std::getline(fin, line);

                switch (saved_char) {
                case 'S': atom.electron.back().s = std::stoi(line); break;
                case 'P':atom.electron.back().p = std::stoi(line); break;
                case 'D':atom.electron.back().d = std::stoi(line); break;
                case 'F':atom.electron.back().f = std::stoi(line); break;
                }

                std::getline(fin, line);
            }
        default:
            break;
        }
    }
}

void HandleArrowKeys(int& x, int& y) {
    if (IsKeyDown(KEY_UP))
        y += delta_arrow_shift;
    if (IsKeyDown(KEY_DOWN))
        y -= delta_arrow_shift;
    if (IsKeyDown(KEY_LEFT))
        x += delta_arrow_shift;
    if (IsKeyDown(KEY_RIGHT))
        x -= delta_arrow_shift;
}

void HandleScaleKeys(float& scale) {

    if (IsKeyDown(KEY_MINUS) && scale > 0.1f)
        scale *= (1.f - delta_scale);
    if (IsKeyDown(KEY_EQUAL) && scale < 4.f)
        scale *= (1.f + delta_scale);
}

int main()
{
    InitWindow(res_x, res_y, "Elementary Atom");
    SetTargetFPS(60);

    int atom_pos_x = res_x / 2;
    int atom_pos_y = res_y / 2;

    float scale = 2.f;

    Atom atom;
    atom.nuclear.neutrons = 5;
    atom.nuclear.protons = 5;

    atom.electron.resize(2);
    atom.electron[0].s = 2;
    atom.electron[1].s = 2;
    atom.electron[1].p = 3;

    while (!WindowShouldClose()) {

        HandleArrowKeys(atom_pos_x, atom_pos_y);
        HandleScaleKeys(scale);

        BeginDrawing();

        ClearBackground(BLACK);

        DrawNuclear(atom, atom_pos_x, atom_pos_y,scale);
        DrawElectrons(atom, atom_pos_x, atom_pos_y, scale);

        EndDrawing();
    }

    CloseWindow();
}

