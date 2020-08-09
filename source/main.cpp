// File: main.cpp
// Created: 07-07-20
// Author: Nicholas Gingerella
// Description:
//   Main entry point for "Tutris", a blatant Tetris knock off written using
//   SDL 2.0

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "my_utils/SDL_Utils.h"
#include "tutris/field.h"
#include "tutris/tetromino.h"
#include "tutris/tutris.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;
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
    Field game_field(100, 100, tutris::FIELD_WIDTH, tutris::FIELD_HEIGHT);
    SDL_Event event;
    bool game_running = true;

    // Game loop
    while (game_running)
    {
        // Time Step

        // input
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game_running = false;
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
            game_field.addPiece(tutris::tetromino_shape::el);
        }

        // Render
        // Clear screen to white before drawing scene
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        game_field.render(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_Utils::cleanup(renderer, window);
    SDL_Quit();
    return 0;
}