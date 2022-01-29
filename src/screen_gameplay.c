/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

#include "figure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int gaming = 0;

static Music song = {0};
static float songLength = 0;
Texture blueLetter[ALPHABET_COUNT] = {0};
Texture yellowLetter[ALPHABET_COUNT] = {0};
Texture poof = {0};
Rectangle viewport;

static Note map[MAX_NOTES] = {0};
static int mapLength = 0;
static int mapStart = 0;
static Figure figure[MAX_NOTES] = {0};
static int figureLength = 0;
int score = 0;
static int scoreGain = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    viewport = (Rectangle){
        10,
        70,
        GetScreenWidth()-20-PIECE_SIZE,
        GetScreenHeight()-80-PIECE_SIZE};
    framesCounter = 0;
    finishScreen = 0;
    gaming = 0;
    score = 100;
    scoreGain = 0;

    song = LoadMusicStream("resources/song.mp3");
    song.looping = 0;
    songLength = (int)GetMusicTimeLength(song);

    for (int i = 0; i < ALPHABET_COUNT; i++)
    {
        blueLetter[i] = LoadTexture(
            TextFormat(
                "resources/blue/letter_%c.png",
                alphabet[i]));
        yellowLetter[i] = LoadTexture(
            TextFormat(
                "resources/yellow/letter_%c.png",
                alphabet[i]));
    }
    poof = LoadTexture("resources/poof.png");

    mapLength = map_init("resources/level.txt", map);
    mapStart = 0;
    figureLength = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    framesCounter++;
    UpdateMusicStream(song);

    double songTime = GetMusicTimePlayed(song);
    if (gaming){
        for (int i = 0; i < figureLength; i++)
        {
            // Remove Deleted Figures
            if (figure[i].despawningFrame > POOF_FRAMES*2)
            {
                for (int c = i; c < figureLength-1; c++)
                {
                    figure[c] = figure[c + 1];
                }
                figureLength--;
                continue;
            }
            /////////////////////////////////////////////
            // Test for figures comming to life
            if (noteIsLive(figure[i].note, songTime))
            {
                figure[i].live = 1;
            }
            // Test for figures expiring
            if (noteIsDead(figure[i].note, songTime)){
                figure[i].dead = 1;
            }

            // Test for figures expired for long periods
            Note n = figure[i].note;
            double expiricyTime = songTime - (n.birth_time + n.lifetime);
            if (expiricyTime > 15 && figure[i].despawningFrame < 1)
            {
                scoreGain = -20;
                score += scoreGain;
                figure[i].despawningFrame = 1;
                continue;
            }

            // Test for SCORE
            if (figure[i].live && IsKeyPressed(noteKey(figure[i].note)) && figure[i].despawningFrame < 1)
            {
                if (figure[i].dead)
                {
                    // TODO: Kill Score
                    scoreGain = -1 - (int)expiricyTime;
                }
                else
                {
                    // TODO: add score
                    if (scoreGain > -.5)
                    {
                        scoreGain = 2;
                    } else {
                        scoreGain = 1;
                    }
                }
                score += scoreGain;
                figure[i].despawningFrame = 1;
                continue;
            }
        
            // Logic for Particles
            if (figure[i].despawningFrame>0)
            {
                figure[i].despawningFrame++;
            }
            
        }
        
        // Push Soon to spawn notes to active drawing list
        // ^Skips them in the next iteration
        for (int i = mapStart; i < mapLength; i++)
        {
            Note n = map[i];
            if (noteBeReady(n, songTime))
            {
                figureLength = figure_add(figure, figureLength, n);
                mapStart++;
            }
        }
        
    }

    // Ending Condition
    if ((gaming && songTime / songLength >= 1) || score <= -10)
    {
        finishScreen = 1;
        // for (int i = 0; i < figureLength; i++)
        // {
        //     if (figure[i].despawningFrame < 1)
        //     {
        //         Note n = figure[i].note;
        //         double hitTime = songTime - (n.birth_time + n.lifetime);
        //         figure[i].dead = 1;
        //         figure[i].despawningFrame++;
        //         if (hitTime > 0)
        //         {
        //             scoreGain = -1 - (int)hitTime;
        //             score -= scoreGain;
        //         }
                
        //     }
        // }
    }

    // Dispatch Start
    if (!gaming && (IsKeyPressed(KEY_ENTER)))
    {
        gaming = 1;
        PlayMusicStream(song);
    }    
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    if (!gaming)
    {
        DrawTextEx(
            font,
            "INACURATE AND OVERWHELMING\nPress ENTER to START\n(audio warning!)",
            (Vector2){100, GetScreenHeight() / 2},
            32, 1, BLACK);
    } else {
        Color lifeColor = BLUE;
        if (score <= 10)
        {
            lifeColor = RED;
        }

        DrawTextEx(
            font,
            TextFormat("Life: %d", score),
            (Vector2){GetScreenWidth() / 2 - 100, 20},
            50,
            2,
            lifeColor);

        if (scoreGain > 0)
        {
            DrawTextEx(
                font,
                TextFormat("+%d", scoreGain),
                (Vector2){GetScreenWidth() / 2 +30, 20},
                50,
                2,
                GREEN);
        }
        else if (scoreGain < 0)
        {
            DrawTextEx(
                font,
                TextFormat("%d", scoreGain),
                (Vector2){GetScreenWidth() / 2 + 30, 20},
                50,
                2,
                RED);
        }

        for (int i = 0; i < figureLength; i++)
        {
            Figure f = figure[i];

            if (f.live)
            {
                draw_figure(f);
            }
        }
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    UnloadMusicStream(song);
    for (int i = 0; i < 26; i++)
    {
        UnloadTexture(blueLetter[i]);
        UnloadTexture(yellowLetter[i]);
    }
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}