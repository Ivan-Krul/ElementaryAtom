#include <iostream>
#include <raylib.h>
#include <vector>
#include <array>

constexpr int nuclear_radius = 5;

struct ElectronLayer {
    size_t s : 2; // 1 element  (2)
    size_t p : 3; // 3 elements (6)
    size_t d : 4; // 5 elements (10)
    size_t f : 4; // 7 elements (14)
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

void DrawAtom(Atom& atom) {

}

int main()
{
    InitWindow(640, 480, "Elementary Atom");
    SetTargetFPS(60);

    Atom atom;
    atom.nuclear.neutrons = 5;
    atom.nuclear.protons = 5;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawNuclear(atom, 100, 100);

        EndDrawing();
    }

    CloseWindow();
}

