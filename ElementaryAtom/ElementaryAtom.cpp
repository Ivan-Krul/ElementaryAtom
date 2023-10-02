#include <iostream>
#include <raylib.h>
#include <vector>
#include <array>

constexpr int res_x = 640;
constexpr int res_y = 480;

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

void DrawNuclear(const Atom& atom, int x, int y) {
    float size = atom.nuclear.protons*2;
    
    size_t protons = atom.nuclear.protons;
    size_t neutrons = atom.nuclear.neutrons;

    float angle = 0.f;

    float lx;
    float ly;

    while ((protons + neutrons) != 0) {
        angle = rand();
        
        lx = std::cos(angle) * (size * (rand() % 1000) / 1000.f);
        ly = std::sin(angle) * (size * (rand() % 1000) / 1000.f);

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

void DrawElectrons(const Atom& atom, int x, int y) {
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
                    ((l + 1) * 4)* (s + 1)* size* size - 2,
                    ((l + 1) * 4)* (s + 1)* size* size,
                    0, 360, 15, DARKBLUE);

            for (size_t e = 0; e < electrons; e++) {
                angle = rand();

                float rx = rand() % uint16_t(std::ceil(electron_arrangement * size));
                float ry = rand() % uint16_t(std::ceil(electron_arrangement * size));

                lx = size * std::cos(angle) * ((l + 1) * 4) * (s + 1) * size + rx;
                ly = size * std::sin(angle) * ((l + 1) * 4) * (s + 1) * size + ry;

                DrawCircle(lx + x, ly + y, electron_radius, BLUE);
            }
        }
    }
}

int main()
{
    InitWindow(res_x, res_y, "Elementary Atom");
    SetTargetFPS(60);

    int atom_pos_x = res_x / 2;
    int atom_pos_y = res_y / 2;

    Atom atom;
    atom.nuclear.neutrons = 5;
    atom.nuclear.protons = 5;

    atom.electron.resize(2);
    atom.electron[0].s = 2;
    atom.electron[1].s = 2;
    atom.electron[1].p = 3;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawNuclear(atom, atom_pos_x, atom_pos_y);
        DrawElectrons(atom, atom_pos_x, atom_pos_y);

        EndDrawing();
    }

    CloseWindow();
}

