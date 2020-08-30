// File: main.cpp
// Created: 07-07-20
// Author: Nicholas Gingerella
// Description:
//   Main entry point for "Tutris", a blatant Tetris knock off written using
//   SDL 2.0

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "my_utils/SDL_Utils.h"
#include "tutris/Game.h"
#include "tutris/Piece.h"
#include "tutris/tutris.h"

void display_game_over_prompt(SDL_Renderer* rend);
void display_title_prompt(SDL_Renderer* rend);
std::string get_countdown_timer(unsigned int elapsed_time_ms);

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 728;
const int TILE_SIZE = 80;
const int PAUSE_TIME_IN_SECONDS = 10;
const int MILLISECONDS_IN_SECONDS = 1000;
const int SDL_RENDERER_FIRST_AVAILABLE_DRIVER = -1;
const std::string WINDOW_TITLE = "Tutris";
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
std::stringstream score_str;
std::string game_timer_str;
Mix_Music *bgm_game = nullptr;
Mix_Chunk *sfx_drop = nullptr;
Mix_Chunk *sfx_collapse = nullptr;
Mix_Chunk *sfx_row_clear = nullptr;
Mix_Chunk *sfx_gameover = nullptr;
Mix_Chunk *sfx_victory = nullptr;
const std::string resource_path = "../resources/fonts/";
SDL_Texture *text_score;
SDL_Texture *text_timer;
SDL_Texture *text_title;
SDL_Texture *text_title_prompt_start;
SDL_Texture *text_game_over;
SDL_Texture *text_end_prompt;
SDL_Texture *text_end_prompt2;
SDL_Color COLOR_WHITE = {255, 255, 255, 255};




int main(int argc, char **argv)
{
    ///////////////////////////////
    // START SDL Setup Boilerplate
    ///////////////////////////////
    // Initialize SDL utilities, windows, and renderers
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        SDL_Utils::logSDLError(std::cout, "SDL_Init");
        return false;
    }

    if ( (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG )
    {
        SDL_Utils::logSDLError(std::cout, "IMG_Init");
        return false;
    }

    if (TTF_Init() != 0)
    {
        SDL_Utils::logSDLError(std::cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << Mix_GetError() << std::endl;;
        SDL_Quit();
        return 1;
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

    bgm_game = Mix_LoadMUS("../resources/music/bg_music.mp3");
    if (bgm_game == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    sfx_drop = Mix_LoadWAV("../resources/sounds/drop.wav");
    if (sfx_drop == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }
    sfx_collapse = Mix_LoadWAV("../resources/sounds/collapse.wav");
    if (sfx_collapse == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }
    sfx_row_clear = Mix_LoadWAV("../resources/sounds/row_clear.wav");
    if (sfx_row_clear == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    sfx_gameover = Mix_LoadWAV("../resources/sounds/gameover.wav");
    if (sfx_gameover == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    // sfx_victory = Mix_LoadWAV("../resources/sounds/victory.wav");
    // if (sfx_victory == nullptr)
    // {
    //     std::cout << Mix_GetError() << std::endl;
    //     SDL_Utils::cleanup(window);
    //     SDL_Quit();
    // }

    // Load Text
    text_game_over = SDL_Utils::renderText("GAME OVER",
        resource_path + "sample.ttf",
        COLOR_WHITE,
        72,
        renderer
    );
    if ( text_game_over == nullptr )
    {
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_end_prompt = SDL_Utils::renderText("Press ESC key to Quit",
        resource_path + "sample.ttf",
        COLOR_WHITE,
        24,
        renderer
    );
    if ( text_end_prompt == nullptr )
    {
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_end_prompt2 = SDL_Utils::renderText("Press SPACE key to start new game",
        resource_path + "sample.ttf",
        COLOR_WHITE,
        24,
        renderer
    );
    if ( text_end_prompt2 == nullptr )
    {
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_title = SDL_Utils::renderText("TUTRIS",
    resource_path + "sample.ttf",
    COLOR_WHITE,
    72,
    renderer
    );
    if ( text_title == nullptr )
    {
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_title_prompt_start = SDL_Utils::renderText("Press SPACE key to start",
    resource_path + "sample.ttf",
    COLOR_WHITE,
    24,
    renderer
    );
    if ( text_title_prompt_start == nullptr )
    {
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    ///////////////////////////////
    // END SDL Setup Boilerplate
    ///////////////////////////////


    // PROGRAM LOGIC STARTS HERE

    // Seed random generator
    srand(static_cast<unsigned>(time(NULL)));

    Game* game_instance = new Game(SCREEN_WIDTH/2 - ((ns_Tutris::FIELD_WIDTH/2)*ns_Tutris::BLOCK_SIZE_PIXEL), 100, ns_Tutris::FIELD_WIDTH, ns_Tutris::FIELD_HEIGHT);
    unsigned int piece_fall_counter = 5; // The lower the number, the faster the speed
    unsigned int speed_counter = 0;
    unsigned int elapsed_ms = 0;
    unsigned int max_speed_lock = 5*(60*1000); // used to max out speed if game is going on too long (5 mins)
    unsigned int speed_up_interval = 30*1000; // speed up piece every 30s
    bool force_down = false;
    SDL_Event event;
    bool game_running = true;
    bool game_over = false;
    unsigned int score = 0;

    score_str.str("");
    score_str << "SCORE: ";
    score_str << score;
    text_score = SDL_Utils::renderText(score_str.str(),
        resource_path + "sample.ttf",
        COLOR_WHITE,
        24,
        renderer);

    // Game loop
    bool on_title_screen = true;
    while (game_running)
    {
        // Time Step
        // Super simple, just sleep 50ms per step (20 steps per second)
        SDL_Delay(50);
        elapsed_ms += 50;
        
        std::string timer = get_countdown_timer(elapsed_ms);
        text_timer = SDL_Utils::renderText(timer,
            resource_path + "sample.ttf",
            COLOR_WHITE,
            24,
            renderer
        );

        if ((elapsed_ms >= max_speed_lock) && (piece_fall_counter > 2))
        {
            piece_fall_counter = 2; // make the game a bit more interesting
        }
        else if ( !game_over && !on_title_screen && (elapsed_ms >= ns_Tutris::MAX_TIME_MS))
        {
            // end game
            // maybe play a victory song instead!
            game_over = true;

            if (Mix_PlayingMusic() != 0)
            {
                Mix_HaltMusic();
            }

            if (Mix_Playing(-1) != 0)
            {
                Mix_HaltChannel(-1);
            }

            Mix_PlayChannel(-1, sfx_gameover, 0);
            game_over = true;
            std::cout << "NICE GAME!" << std::endl;
            std::cout << "SCORE: " << score << std::endl;
        }

        // update speed as long as game is still in progress
        if (!game_over)
        {
            speed_counter++;
            if (speed_counter >= piece_fall_counter)
            {
                force_down = true;
            }

            if (elapsed_ms % speed_up_interval == 0)
            {
                if (piece_fall_counter > 1)
                {
                    piece_fall_counter--;
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

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if (!game_over)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::left);
                        }
                        break;
                    case SDLK_RIGHT:
                        if (!game_over)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::right);
                        }
                        break;
                    case SDLK_DOWN:
                        if (!game_over)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::down);
                        }
                        break;
                    case SDLK_UP:
                        if (!game_over)
                        {
                            game_instance->rotatePiece();
                        }
                        break;
                    case SDLK_ESCAPE:
                        if (game_over)
                        {
                            game_running = false;
                        }
                        break;
                    case SDLK_SPACE:
                        if (game_over)
                        {
                            game_over = false;
                            delete game_instance;
                            game_instance = new Game(SCREEN_WIDTH/2 - ((ns_Tutris::FIELD_WIDTH/2)*ns_Tutris::BLOCK_SIZE_PIXEL), 100, ns_Tutris::FIELD_WIDTH, ns_Tutris::FIELD_HEIGHT);
                            elapsed_ms = 0;
                            score = 0;
                            score_str.str("");
                            score_str << "SCORE: ";
                            score_str << score;
                            SDL_Color color = {255, 255, 255, 255};
                            text_score = SDL_Utils::renderText(score_str.str(),
                                resource_path + "sample.ttf",
                                color,
                                24,
                                renderer
                            );

                            // If game over sound is still playing, stop it.
                            // (this will stop ALL channels)
                            if (Mix_Playing(-1) != 0)
                            {
                                Mix_HaltChannel(-1);
                            }

                            // Previous game over sound should be stopped,
                            // start playing music again
                            if (Mix_PlayingMusic() == 0)
                            {
                                Mix_PlayMusic(bgm_game, -1);
                            }
                        } 
                        else if (on_title_screen)
                        {
                            on_title_screen = false;
                            elapsed_ms = 0;
                        }
                        else
                        {
                            // in middle of game. Use ESC to pause game
                        }
                        
                        break;
                    default:
                        break;
                }
            }
        }

        // Game logic
        if (!game_over && !on_title_screen)
        {       
            // Start playing music if not already doing so
            Mix_VolumeMusic(MIX_MAX_VOLUME);
            if (Mix_PlayingMusic() == 0)
            {
                Mix_PlayMusic(bgm_game, -1);
            }

            if (!game_instance->isPieceActive())
            {
                // We are either at the very start of the game, or the current
                // piece has been set in place. Are there any full lines of blocks
                // that we can clear and award points for?
                std::vector<int> clear_rows = game_instance->scanField();
                while (!clear_rows.empty())
                {   
                    // Check the number of neighboring cleared rows
                    // to determine if this is a collapse, or a regular
                    // row clear
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

                    // Were two or more neighboring rows cleared at once?
                    // If so, trigger a collapse
                    if (neighbor_rows >= 2)
                    {
                        collapse = true;
                    }

                    // Give player a bonus if they triggered a collapse
                    if (collapse)
                    {
                        score += ns_Tutris::SCORE_INCREMENT_COLLAPSE;
                        if (piece_fall_counter < 5)
                        {
                            piece_fall_counter += 1; // slow the active piece speed down
                        }
                    }

                    // Add standard score for cleared rows
                    score += clear_rows.size()*ns_Tutris::SCORE_INCREMENT_BASIC;
                    if (piece_fall_counter < 5)
                    {
                        piece_fall_counter += 1; // slow the active piece speed down
                    }


                    // Load text
                    score_str.str("");
                    score_str << "SCORE: ";
                    score_str << score;
                    SDL_Color color = {255, 255, 255, 255};
                    text_score = SDL_Utils::renderText(score_str.str(),
                        resource_path + "sample.ttf",
                        color,
                        24,
                        renderer);

                    if ( text_score == nullptr )
                    {
                        SDL_Utils::cleanup(renderer, window);
                        TTF_Quit();
                        IMG_Quit();
                        Mix_Quit();
                        SDL_Quit();
                        return 1;
                    }

                    // Re-Render screen with new rows marked for clearing
                    // Mark rows for clearing
                    game_instance->markClearRows(clear_rows);
                    SDL_SetRenderDrawColor(renderer, ns_Tutris::COLOR_BACKGROUND.r, ns_Tutris::COLOR_BACKGROUND.g, ns_Tutris::COLOR_BACKGROUND.b, 0xFF);
                    SDL_RenderClear(renderer);
                    game_instance->render(renderer);
                    SDL_Utils::renderTexture(text_score, renderer, SCREEN_WIDTH/2 + (game_instance->getNumCols()*ns_Tutris::BLOCK_SIZE_PIXEL)/2 + 10, SCREEN_HEIGHT/2 - (game_instance->getNumRows()*ns_Tutris::BLOCK_SIZE_PIXEL)/2);
                    SDL_RenderPresent(renderer);

                    // Play sound effect
                    if (collapse)
                    {
                        Mix_PlayChannel(-1, sfx_row_clear, 0);
                    }
                    else
                    {
                        Mix_PlayChannel(-1, sfx_collapse, 0);
                    }
                    

                    // Delay 0.5s to emphasize cleared rows
                    if (!clear_rows.empty())
                    {
                        SDL_Delay(500);
                    }

                    // Remove marked rows from the field so blocks can fall
                    game_instance->removeRows(clear_rows);

                    // Trigger collapse fall logic, or regular fall logic
                    // for the blocks in the field
                    if (collapse)
                    {
                        game_instance->collapseBlocks();
                    }
                    else
                    {
                        game_instance->shiftBlocks(clear_rows);
                    }
                    
                    // Redraw Screen
                    SDL_SetRenderDrawColor(renderer, ns_Tutris::COLOR_BACKGROUND.r, ns_Tutris::COLOR_BACKGROUND.g, ns_Tutris::COLOR_BACKGROUND.b, 0xFF);
                    SDL_RenderClear(renderer);
                    game_instance->render(renderer);
                    SDL_Utils::renderTexture(text_score, renderer, SCREEN_WIDTH/2 + (game_instance->getNumCols()*ns_Tutris::BLOCK_SIZE_PIXEL)/2 + 10, SCREEN_HEIGHT/2 - (game_instance->getNumRows()*ns_Tutris::BLOCK_SIZE_PIXEL)/2);
                    SDL_RenderPresent(renderer);

                    // All cleared rows now handled an put in place.
                    // Empty our list and scan field for more cleared
                    // rows now that blocks have fallen in place.
                    clear_rows.clear();
                    clear_rows = game_instance->scanField();
                }

                // We failed to add a piece to the field.
                // the piece spawns at the top of the field.
                // if we failed to spawn a piece at the top due
                // to collision, then the game is over.
                if (!game_instance->addPiece(ns_Tutris::tetromino_shape::random))
                {
                    // stop all music
                    if (Mix_PlayingMusic() != 0)
                    {
                        Mix_HaltMusic();
                    }

                    // stop all sound effects
                    if (Mix_Playing(-1) != 0)
                    {
                        Mix_HaltChannel(-1);
                    }

                    Mix_PlayChannel(-1, sfx_gameover, 0);
                    game_over = true;
                    std::cout << "GAME OVER" << std::endl;
                    std::cout << "SCORE: " << score << std::endl;
                    continue;
                }
                else
                {
                    Mix_PlayChannel(-1, sfx_drop, 0);
                }
            }

            if (force_down && game_instance->isPieceActive())
            {
                game_instance->movePiece(ns_Tutris::move_direction::down);
                speed_counter = 0;
                force_down = false;
            }
        }

        // RENDER
        // clear screen to background color
        SDL_SetRenderDrawColor(renderer, ns_Tutris::COLOR_BACKGROUND.r, ns_Tutris::COLOR_BACKGROUND.g, ns_Tutris::COLOR_BACKGROUND.b, 0xFF);
        SDL_RenderClear(renderer);

        // Redraw game field
        if (!on_title_screen)
        {
            game_instance->render(renderer);
            int score_x_pos = SCREEN_WIDTH/2 + (game_instance->getNumCols()*ns_Tutris::BLOCK_SIZE_PIXEL)/2 + 10;
            int score_y_pos = SCREEN_HEIGHT/2 - (game_instance->getNumRows()*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
            SDL_Utils::renderTexture(text_score, renderer,score_x_pos ,score_y_pos );
            SDL_Utils::renderTexture(text_timer, renderer,score_x_pos ,score_y_pos + 20 );
        }

        if (game_over)
        {
            display_game_over_prompt(renderer);
        }

        if (on_title_screen)
        {
            display_title_prompt(renderer);
        }

        // Render Screen
        SDL_RenderPresent(renderer);

    } // end while(game_running)

    Mix_FreeChunk(sfx_drop);
    Mix_FreeChunk(sfx_collapse);
    Mix_FreeChunk(sfx_row_clear);
    SDL_Utils::cleanup(renderer, window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void display_game_over_prompt(SDL_Renderer* rend)
{
    // Draw box on center of screen
    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    int box_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2 - 100;
    int box_y_pos = SCREEN_HEIGHT/2 - box_height/2;
    SDL_Rect game_over_square = {
        box_x_pos,
        box_y_pos,
        box_width,
        box_height
    };
    SDL_SetRenderDrawColor( rend, 0x00,0x00,0x00,0xFF);
    SDL_RenderFillRect(rend, &game_over_square);
    SDL_SetRenderDrawColor( rend, 0xFF,0xFF,0xFF,0xFF);
    SDL_RenderDrawRect(rend, &game_over_square);

    SDL_Utils::renderTexture(text_game_over, rend, box_x_pos + 30, box_y_pos + 20);
    SDL_Utils::renderTexture(text_end_prompt, rend, box_x_pos + 30, box_y_pos + 100);
    SDL_Utils::renderTexture(text_end_prompt2, rend, box_x_pos + 30, box_y_pos + 140);
}

void display_title_prompt(SDL_Renderer* rend)
{
    // Draw box on center of screen
    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    int box_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2 - 100;
    int box_y_pos = SCREEN_HEIGHT/2 - box_height/2;
    SDL_Rect game_over_square = {
        box_x_pos,
        box_y_pos,
        box_width,
        box_height
    };
    SDL_SetRenderDrawColor( rend, 0x00,0x00,0x00,0xFF);
    SDL_RenderFillRect(rend, &game_over_square);
    SDL_SetRenderDrawColor( rend, 0xFF,0xFF,0xFF,0xFF);
    SDL_RenderDrawRect(rend, &game_over_square);

    SDL_Utils::renderTexture(text_title, rend, box_x_pos + 110, box_y_pos + 20);
    SDL_Utils::renderTexture(text_title_prompt_start, rend, box_x_pos + 95, box_y_pos + 120);
}

std::string get_countdown_timer(unsigned int elapsed_time_ms)
{
    unsigned int max_ms_countdown;
    if ((elapsed_time_ms >= 0) && (elapsed_time_ms <= ns_Tutris::MAX_TIME_MS))
    {
        max_ms_countdown = (ns_Tutris::MAX_TIME_MS) - elapsed_time_ms;
    }
    else
    {
        max_ms_countdown = 0;
    }
    
    unsigned int num_secs = (max_ms_countdown/1000);
    unsigned int num_mins = num_secs/60;
    unsigned int num_secs_display = num_secs % 60; // only want to display 0-60 seconds 

    std::stringstream mins_string;
    mins_string << std::setfill('0') << std::setw(2) << num_mins; 
    std::stringstream secs_string;
    secs_string << std::setfill('0') << std::setw(2) << num_secs_display;  

    return std::string(mins_string.str() + ":" + secs_string.str());
}
