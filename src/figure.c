#include "figure.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Rectangle poofAnim[POOF_FRAMES] = {
    {0, 0, 64, 64},
    {64, 0, 64, 64},
    {128, 0, 64, 64},
    {192, 0, 64, 64},
    {256, 0, 64, 64},
    {320, 0, 64, 64},
    {0, 64, 64, 64},
    {64, 64, 64, 64},
    {128, 64, 64, 64},
    {192, 64, 64, 64},
    {256, 64, 64, 64},
    {320, 64, 64, 64},
    {0, 128, 64, 64},
    {64, 128, 64, 64},
    {128, 128, 64, 64},
    {192, 128, 64, 64},
    {256, 128, 64, 64},
    {320, 128, 64, 64},
    {0, 192, 64, 64},
    {64, 192, 64, 64},
    {128, 192, 64, 64},
    {192, 192, 64, 64},
    {256, 192, 64, 64},
    {320, 192, 64, 64},
    {0, 256, 64, 64},
    {64, 256, 64, 64},
    {128, 256, 64, 64},
    {192, 256, 64, 64},
    {256, 256, 64, 64},
    {320, 256, 64, 64}};

KeyboardKey
noteKey(Note n)
{
    switch (n.letter)
    {
    case 'A': return KEY_A;
    case 'B': return KEY_B;
    case 'C': return KEY_C;
    case 'D': return KEY_D;
    case 'E': return KEY_E;
    case 'F': return KEY_F;
    case 'G': return KEY_G;
    case 'H': return KEY_H;
    case 'I': return KEY_I;
    case 'J': return KEY_J;
    case 'K': return KEY_K;
    case 'L': return KEY_L;
    case 'M': return KEY_M;
    case 'N': return KEY_N;
    case 'O': return KEY_O;
    case 'P': return KEY_P;
    case 'Q': return KEY_Q;
    case 'R': return KEY_R;
    case 'S': return KEY_S;
    case 'T': return KEY_T;
    case 'U': return KEY_U;
    case 'V': return KEY_V; 
    case 'W': return KEY_W;
    case 'X': return KEY_X;
    case 'Y': return KEY_Y;
    case 'Z': return KEY_Z;
    default: return KEY_NULL;
    }
}
int noteLetter(Note n) {
    return noteKey(n) - 65;
}
int noteIsLive(Note n, double songTime) {
    return n.birth_time <= songTime;
}

int noteBeReady(Note n, double songTime)
{
    return n.birth_time <= songTime + 2;
}

int noteIsDead(Note n, double songTime) {
    return n.birth_time + n.lifetime <= songTime;
}

int noteBirthTimeComparator(const void *p, const void *q)
{
    const Note* pp = p;
    const Note* qq = q;
    double l = pp->birth_time;
    double r = qq->birth_time;
    return l-r;
}

int map_init(const char *fname, Note *m)
{
    char letter;
    float birth;
    float life;
    float x;
    float y;
    FILE* pFile = fopen(fname, "r");
    int fileLength = 0;
    while (5 == fscanf(pFile, "%c %f %f %f %f\n", &letter, &birth, &life, &x, &y))
    {
        printf("%c %f %f %f %f\n", letter, birth, life, x, y);
        m[fileLength++] = (Note){
            letter,
            birth,
            life,
            (GetRandomValue(0, 50) * 2) / 100.0,
            (GetRandomValue(0, 50) * 2) / 100.0
        };
    }

    fclose(pFile);
    qsort(m, fileLength, sizeof(Note), noteBirthTimeComparator);
    return fileLength;
}


int figure_add(Figure* f, int length, Note n) {
    f[length].x = viewport.x + n.x * viewport.width;
    f[length].y = viewport.y + n.y * viewport.height;
    f[length].note = n;
    f[length].live = 0;
    f[length].dead = 0;
    f[length].despawningFrame = 0;
    return length + 1;
}

void draw_figure(Figure f) {
    if (f.despawningFrame>0)
    {
        DrawTextureRec(
            poof,
            poofAnim[f.despawningFrame/2],
            (Vector2){f.x, f.y},
            WHITE);
    }else{
        int letternum = noteLetter(f.note);
        Texture txt = blueLetter[letternum];
        if (f.dead)
        {
            txt = yellowLetter[letternum];
        }
        DrawTexture(
            txt,
            f.x,
            f.y,
            WHITE);
    }
}
