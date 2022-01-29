#pragma once

#include <raylib.h>

#define MAX_NOTES 1000
#define ALPHABET_COUNT 26
#define PIECE_SIZE 48
#define POOF_FRAMES 30

extern Texture blueLetter[ALPHABET_COUNT];
extern Texture yellowLetter[ALPHABET_COUNT];
extern Texture poof;
extern Rectangle viewport;
static char alphabet[ALPHABET_COUNT] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Notes are instructions for what key
// ^to expect at what moment
typedef struct NoteStruct {
    char letter;
    double birth_time;
    double lifetime;
    double x;
    double y;
} Note;

typedef struct FigureStruct {
    int x;
    int y;
    Note note;
    int live;
    int dead;
    int despawningFrame;
} Figure;


int noteIsDead(Note, double);
int noteBeReady(Note, double);
int noteIsLive(Note, double);
KeyboardKey noteKey(Note);
int noteLetter(Note);

int map_init(const char* fname, Note* m);
int figure_add(Figure*, int, Note);

void draw_figure(Figure f);
Rectangle rectText(Figure f);