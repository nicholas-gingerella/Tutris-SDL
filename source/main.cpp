// File: main.cpp
// Created: 07-07-20
// Author: Nicholas Gingerella
// Description:
//   Main entry point for "Tutris", a blatant Tetris knock off written using
//   SDL 2.0

#include <iostream>
#include <algorithm>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "my_utils/SDL_Utils.h"
#include "tutris/field.h"
#include "tutris/tetromino.h"
#include "tutris/tutris.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 728;
const int TILE_SIZE = 80;
const int PAUSE_TIME_IN_SECONDS = 10;
const int MILLISECONDS_IN_SECONDS = 1000;
const int SDL_RENDERER_FIRST_AVAILABLE_DRIVER = -1;
const std::string WINDOW_TITLE = "Tutris";
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int main(int argc, char **argv)
{
    ///////////////////////////////
    // START SDL Setup Boilerplate
    ///////////////////////////////
    // Initialize SDL utilities, windows, and renderers
    if ( (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG )
    {
        SDL_Utils::logSDLError(std::cout, "IMG_Init");
        return false;
    }

    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Utils::logSDLError(std::cout, "SDL_CreateWindow Error");
        SDL_Quit();
        return false;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
        SDL_RENDERER_FIRST_AVAILABLE_DRIVER,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_Utils::logSDLError(std::cout, "SDL_CreateRenderer Error");
        SDL_Utils::cleanup(window);
        SDL_Quit();
        return 1;
    }
    ///////////////////////////////
    // END SDL Setup Boilerplate
    ///////////////////////////////


    // PROGRAM LOGIC STARTS HERE

    // Seed random generator
    srand(static_cast<unsigned>(time(NULL)));

    Field game_field(SCREEN_WIDTH/2 - ((tutris::FIELD_WIDTH/2)*tutris::BLOCK_SIZE_PIXEL), 100, tutris::FIELD_WIDTH, tutris::FIELD_HEIGHT);
    unsigned int current_speed = 10; // piece moves down every 20 ticks
    unsigned int speed_counter = 0;
    unsigned int elapsed_ms = 0;
    unsigned int speed_up_interval = 30*1000; // speed up piece every 30s
    bool force_down = false;
    SDL_Event event;
    bool game_running = true;
    bool game_over = false;
    unsigned int score = 0;

    // Game loop
    while (game_running)
    {
        // Time Step
        // Super simple, just sleep 50ms per step (20 steps per second)
        SDL_Delay(50);
        elapsed_ms += 50;
        std::cout << elapsed_ms << std::endl;

        // if we lost the game, don't bother with the counter anymore
        if (!game_over)
        {
            speed_counter++;
            if (speed_counter >= current_speed)
            {
                force_down = true;
            }

            if (elapsed_ms % speed_up_interval == 0)
            {
                if (current_speed > 1)
                {
                    current_speed--;
                    std::cout << "SPEED UP: " << current_speed << std::endl;
                }
                else
                {
                    std::cout << "MAX SPEED REACHED " << current_speed << std::endl;
                }
                
            }
        }

        // input
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game_running = false;
            }

            if (game_over)
            {
                // once the game ends, wait for player to press X
                continue;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1:
                        break;
                    case SDLK_LEFT:
                        game_field.movePiece(tutris::move_direction::left);
                        break;
                    case SDLK_RIGHT:
                        game_field.movePiece(tutris::move_direction::right);
                        break;
                    case SDLK_DOWN:
                        game_field.movePiece(tutris::move_direction::down);
                        break;
                    case SDLK_z:
                        game_field.rotatePiece();
                        break;
                    default:
                        break;
                }
            }
        }

        // Game logic
        if (!game_field.isPieceActive())
        {
            // We are either at the very start of the game, or the current
            // piece has been set in place. Are there any full lines of blocks
            // that we can clear and award points for?
            std::vector<int> clear_rows = game_field.scanField();

            // Take note of what rows are marked for clearing.
            // 1. is it more than one row?
            //   * are they consecutive and more than 2? or are they single and spaced apart
            // 2. are the rows consecutive? (right next to eachother?)
            //   * cause a "collapse", all free floating pieces/caverns
            //     collapse or cave in by moving all blocks down until they
            //     can't move down anymore. Extra points
            // 3. are they spaced apart?
            //   * if marked rows are spaced apart and consist of only a single
            //     row, then just move the blocks above it down by 1.


            // if clearing rows (simple visual indicator)
            // render field ()
            // delay 1 second
            // remove rows marked for clearing
            // 
            //  if a collapse is triggered (2 or more consecutive rows cleared)
            //    scan field (bottom to top)
            //    if block has a piece in it and piece can move down
            //    move piece down (would cause hanging blocks to fall) //simple approach for now
            //  
            //  if no collapse triggered
            //    for each row found as traversing from bottom to top of field
            //      remove marked blocks in row
            //      move all blocks above down by 1 until next cleared row is found
            while (!clear_rows.empty())
            {   
                // Is was a collapse triggered?
                bool collapse = false;
                int neighbor_rows = 1;
                std::sort(clear_rows.begin(), clear_rows.end());
                for (unsigned int i = 0; i < clear_rows.size()-1; i++)
                {
                    int curr = clear_rows[i];
                    int next = clear_rows[i+1];
                    if (next == curr+1)
                    {
                        neighbor_rows++;
                    }
                }
                
                if (neighbor_rows >= 2)
                {
                    collapse = true;
                }
                
                // There are rows that need to be marked for clearing
                std::cout << "Rows cleared" << std::endl;
                std::vector<int>::iterator it;
                for(it = clear_rows.begin(); it != clear_rows.end(); it++)
                {
                    std::cout << *it << std::endl;
                }
                game_field.markClearRows(clear_rows);

                // Re-Render screen with new rows marked for clearing
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                game_field.render(renderer);
                SDL_RenderPresent(renderer);

                // Play sound effect
                // Delay 0.5s
                SDL_Delay(500);

                // Remove cleared rows (collapse blocks if necessary)
                game_field.removeRows(clear_rows);

                //if collapse logic
                if (collapse)
                {
                    std::cout << "collapse logic" << std::endl;
                    game_field.shiftFallingBlocks();
                }
                else
                {
                    std::cout << "standard fall" << std::endl;
                    game_field.regularFallLogic(clear_rows);
                }
                
                //NOTE: once the blocks shift into place (especially after collapse)
                //      we need to re-check for rows that can be cleared again. and go
                //      through the motions of clearing them from the field as well.
                // Re-Render screen with new rows marked for clearing
                SDL_SetRenderDrawColor(renderer, 0x26, 0x26, 0x26, 0xFF);
                SDL_RenderClear(renderer);
                game_field.render(renderer);
                SDL_RenderPresent(renderer);

                clear_rows.clear();
                clear_rows = game_field.scanField();
            }




            if (!game_field.addPiece(tutris::tetromino_shape::random))
            {
                // We failed to add a piece to the field.
                // the piece spawns at the top of the field.
                // if we failed to spawn a piece at the top due
                // to collision, then the game is over.
                std::cout << "GAME OVER!!!" << std::endl;
                game_over = true;
            }
        }

        if (force_down && game_field.isPieceActive())
        {
            game_field.movePiece(tutris::move_direction::down);
            speed_counter = 0;
            force_down = false;
        }

        // Render
        // Clear screen to white before drawing scene
        SDL_SetRenderDrawColor(renderer, 0x26, 0x26, 0x26, 0xFF);
        SDL_RenderClear(renderer);
        game_field.render(renderer);
        SDL_RenderPresent(renderer);
    }



    SDL_Utils::cleanup(renderer, window);
    SDL_Quit();
    return 0;
}