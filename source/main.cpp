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

void display_game_over_prompt(SDL_Renderer*);
void display_victory_prompt(SDL_Renderer*);
void display_title_prompt(SDL_Renderer*);
void display_Pause_prompt(SDL_Renderer*);
std::string get_countdown_timer(unsigned int);
void game_update(SDL_Renderer*);
void renderBox(SDL_Renderer*, int, int, int, int, ns_Tutris::tutris_color, ns_Tutris::tutris_color);
void renderBoxCenter(SDL_Renderer*, int, int, ns_Tutris::tutris_color, ns_Tutris::tutris_color);
void updateGameScoreText(SDL_Renderer*, unsigned int);
void clearScreen(SDL_Renderer*);
void renderScoreText(SDL_Renderer*);
void renderTimerText(SDL_Renderer*);
void redrawGameField(SDL_Renderer*);



// Main window and renderer for SDL
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 728;
const int TILE_SIZE = 80;
const int PAUSE_TIME_IN_SECONDS = 10;
const int MILLISECONDS_IN_SECONDS = 1000;
const int SDL_RENDERER_FIRST_AVAILABLE_DRIVER = -1;

const std::string WINDOW_TITLE = "Tutris";
std::stringstream score_str;
std::string game_timer_str;



Mix_Music *bgm_game = nullptr;
Mix_Chunk *sfx_drop = nullptr;
Mix_Chunk *sfx_pause_on = nullptr;
Mix_Chunk *sfx_pause_off = nullptr;
Mix_Chunk *sfx_collapse = nullptr;
Mix_Chunk *sfx_row_clear = nullptr;
Mix_Chunk *sfx_gameover = nullptr;
Mix_Chunk *sfx_victory = nullptr;

const std::string resource_path_fonts = "../resources/fonts/";
const std::string resource_path_music = "../resources/music/";
const std::string resource_path_sounds = "../resources/sounds/";
const std::string main_font_path = resource_path_fonts + "sample.ttf";

SDL_Texture *text_score;
SDL_Texture *text_timer;
SDL_Texture *text_title;
SDL_Texture *text_title_prompt_start;
SDL_Texture *text_game_over;
SDL_Texture *text_victory_prompt_quit;
SDL_Texture *text_victory_prompt_start;
SDL_Texture *text_pause;
SDL_Texture *text_victory;

SDL_Color SDL_COLOR_WHITE = {255, 255, 255, 255};


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
bool game_paused = false;
bool game_victory = false;
bool on_title_screen = true;
unsigned int score = 0;

enum class game_state
{
    title,
    playing,
    paused,
    gameover,
    victory
};
game_state tutris_state = game_state::title;




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

    // Load Music and Sound Effects
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

    sfx_pause_on = Mix_LoadWAV("../resources/sounds/pause_on.wav");
    if (sfx_pause_on == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    sfx_pause_off = Mix_LoadWAV("../resources/sounds/pause_off.wav");
    if (sfx_pause_off == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    sfx_victory = Mix_LoadWAV("../resources/sounds/victory.wav");
    if (sfx_victory == nullptr)
    {
        std::cout << Mix_GetError() << std::endl;
        SDL_Utils::cleanup(window);
        SDL_Quit();
    }

    // Load/Render in-game text
    text_game_over = SDL_Utils::renderText("GAME OVER",
        main_font_path,
        SDL_COLOR_WHITE,
        72,
        renderer
    );
    if ( text_game_over == nullptr )
    {
        std::cout << "Error loading GAME OVER text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_victory = SDL_Utils::renderText("NICE GAME!",
        main_font_path,
        SDL_COLOR_WHITE,
        72,
        renderer
    );
    if ( text_victory == nullptr )
    {
        std::cout << "Error loading Victory text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_victory_prompt_quit = SDL_Utils::renderText("Press ESC key to Quit",
        main_font_path,
        SDL_COLOR_WHITE,
        24,
        renderer
    );
    if ( text_victory_prompt_quit == nullptr )
    {
        std::cout << "Error loading QUIT text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_victory_prompt_start = SDL_Utils::renderText("Press SPACE key to start new game",
        main_font_path,
        SDL_COLOR_WHITE,
        24,
        renderer
    );
    if ( text_victory_prompt_start == nullptr )
    {
        std::cout << "Error loading Start text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_title = SDL_Utils::renderText("TUTRIS",
    main_font_path,
    SDL_COLOR_WHITE,
    72,
    renderer
    );
    if ( text_title == nullptr )
    {
        std::cout << "Error loading TITLE text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_title_prompt_start = SDL_Utils::renderText("Press SPACE key to start",
    main_font_path,
    SDL_COLOR_WHITE,
    24,
    renderer
    );
    if ( text_title_prompt_start == nullptr )
    {
        std::cout << "Error loading START text" << std::endl;
        SDL_Utils::cleanup(renderer, window);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    text_pause = SDL_Utils::renderText("PAUSED",
    main_font_path,
    SDL_COLOR_WHITE,
    72,
    renderer
    );
    if ( text_pause == nullptr )
    {
        std::cout << "Error loading START text" << std::endl;
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


    ///////////////////////////////
    // PROGRAM LOGIC STARTS HERE
    ///////////////////////////////

    // Seed random generator
    srand(static_cast<unsigned>(time(NULL)));

    updateGameScoreText(renderer, score);

    // Game loop
    while (game_running)
    {
        // Time Step
        // Super simple, just sleep 50ms per step (20 steps per second)
        SDL_Delay(50);

        // Game State Transitions
        switch(tutris_state)
        {
            case game_state::title:
            {
                if (!on_title_screen)
                {
                    tutris_state = game_state::playing;

                    // cleanup current game instance and create a new one
                    if (game_instance != nullptr)
                    {
                        delete game_instance;
                    }

                    game_instance = new Game(SCREEN_WIDTH/2 - ((ns_Tutris::FIELD_WIDTH/2)*ns_Tutris::BLOCK_SIZE_PIXEL), 100, ns_Tutris::FIELD_WIDTH, ns_Tutris::FIELD_HEIGHT);
                    elapsed_ms = 0;
                    score = 0;
                    updateGameScoreText(renderer, score);

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
                break;
            }
            case game_state::playing:
            {
                if (game_over)
                {
                    tutris_state = game_state::gameover;
                    
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

                    // Play game over sound
                    Mix_PlayChannel(-1, sfx_gameover, 0);

                    // Display game over screen (in action control)
                }
                else if (!on_title_screen && (elapsed_ms >= ns_Tutris::MAX_TIME_MS))
                {
                    tutris_state = game_state::victory;

                    // Run logic on state transition so it only runs once
                    // stop current plying sounds/music
                    if (Mix_PlayingMusic() != 0)
                    {
                        Mix_HaltMusic();
                    }

                    if (Mix_Playing(-1) != 0)
                    {
                        Mix_HaltChannel(-1);
                    }

                    // Play victory sound and display victory message
                    Mix_PlayChannel(-1, sfx_victory, 0);

                    // Display victory screen (in action control)
                }
                else if (game_paused)
                {
                    tutris_state = game_state::paused;
                    
                    // play pause sound
                    Mix_PlayChannel(-1, sfx_pause_on, 0);

                    // display pause screen ( in action control)
                }
                else
                {
                    tutris_state = game_state::playing;
                }
                break;
            }
            case game_state::paused:
            {
                if (!game_paused)
                {
                    tutris_state = game_state::playing;
                    
                    Mix_PlayChannel(-1, sfx_pause_off, 0);
                }
                else
                {
                    tutris_state = game_state::paused;
                }
                
                break;
            }
            case game_state::victory:
            {
                if (!game_victory)
                {
                    if (game_running)
                    {
                        tutris_state = game_state::playing;

                        // cleanup current game instance and create a new one
                        delete game_instance;
                        game_instance = new Game(SCREEN_WIDTH/2 - ((ns_Tutris::FIELD_WIDTH/2)*ns_Tutris::BLOCK_SIZE_PIXEL), 100, ns_Tutris::FIELD_WIDTH, ns_Tutris::FIELD_HEIGHT);
                        elapsed_ms = 0;
                        score = 0;
                        updateGameScoreText(renderer, score);

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
                }
                else
                {
                    tutris_state = game_state::victory;
                }
                
                break;
            }
            case game_state::gameover:
            {
                if (!game_over)
                {
                    if (game_running)
                    {
                        tutris_state = game_state::playing;

                        // cleanup current game instance and create a new one
                        delete game_instance;
                        game_instance = new Game(SCREEN_WIDTH/2 - ((ns_Tutris::FIELD_WIDTH/2)*ns_Tutris::BLOCK_SIZE_PIXEL), 100, ns_Tutris::FIELD_WIDTH, ns_Tutris::FIELD_HEIGHT);
                        elapsed_ms = 0;
                        score = 0;
                        updateGameScoreText(renderer, score);

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
                }
                else
                {
                    tutris_state = game_state::gameover;
                }
                
                break;
            }
        }

        //state actions
        switch(tutris_state)
        {
            case game_state::title:
            {
                display_title_prompt(renderer);
                break;
            }
            case game_state::playing:
            {
                game_update(renderer);

                game_instance->render(renderer);
                renderScoreText(renderer);
                renderTimerText(renderer);
                break;
            }
            case game_state::paused:
            {
                display_Pause_prompt(renderer);
                break;
            }
            case game_state::victory:
            {
                display_victory_prompt(renderer);
                break;
            }
            case game_state::gameover:
            {
                display_game_over_prompt(renderer);
                break;
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
                        if (tutris_state == game_state::playing)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::left);
                        }
                        break;
                    case SDLK_RIGHT:
                        if (tutris_state == game_state::playing)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::right);
                        }
                        break;
                    case SDLK_DOWN:
                        if (tutris_state == game_state::playing)
                        {
                            game_instance->movePiece(ns_Tutris::move_direction::down);
                        }
                        break;
                    case SDLK_UP:
                        if (tutris_state == game_state::playing)
                        {
                            game_instance->rotatePiece();
                        }
                        break;
                    case SDLK_ESCAPE:
                        if (tutris_state == game_state::gameover ||
                            tutris_state == game_state::victory)
                        {
                            game_running = false;
                            game_paused = false;
                        }
                        else if (tutris_state == game_state::playing)
                        {
                            game_running = true;
                            game_paused = true;
                        }
                        else if (tutris_state == game_state::paused)
                        {
                            game_running = true;
                            game_paused = false;
                        }
                        break;
                    case SDLK_SPACE:
                        if (tutris_state == game_state::gameover ||
                            tutris_state == game_state::victory)
                        {
                            game_over = false;
                            game_victory = false;
                            game_running = true;
                        } 
                        else if (tutris_state == game_state::title)
                        {
                            on_title_screen = false;
                            game_over = false;
                            game_running = true;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        // RENDER
        SDL_RenderPresent(renderer);
        clearScreen(renderer);

    } // end while(game_running)

    return 0;
}

void game_update(SDL_Renderer* rend)
{
    elapsed_ms += 50;
    
    std::string timer = get_countdown_timer(elapsed_ms);
    text_timer = SDL_Utils::renderText(timer,
        main_font_path,
        SDL_COLOR_WHITE,
        24,
        rend
    );

    // If the max play time was passed, go to game win state
    if (elapsed_ms >= ns_Tutris::MAX_TIME_MS)
    {
        game_victory = true;
        return;
    }

    // Force piece down after certain interval
    speed_counter++;
    if (speed_counter >= piece_fall_counter)
    {
        force_down = true;
    }

    // Speed up piece drop interval after certain amount
    // of time passes
    if (elapsed_ms % speed_up_interval == 0)
    {
        if (piece_fall_counter > 1)
        {
            piece_fall_counter--;
        }
    }

    // Lock to top speed of the piece at a certain max
    if ((elapsed_ms >= max_speed_lock) && (piece_fall_counter < 2))
    {
        piece_fall_counter = 2; // speed up game for 2nd half
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
                score += (ns_Tutris::SCORE_INCREMENT_COLLAPSE * neighbor_rows);
                if ((piece_fall_counter < 5) && (elapsed_ms < max_speed_lock))
                {
                    // slow down piece, unless we're halfway through the match
                    piece_fall_counter += 1;
                }
            }

            // Add standard score for cleared rows
            score += clear_rows.size()*ns_Tutris::SCORE_INCREMENT_BASIC;
            if (piece_fall_counter < 5)
            {
                piece_fall_counter += 1; // slow the active piece speed down
            }

            updateGameScoreText(rend, score);

            // Re-Render screen with new rows marked for clearing
            // Mark rows for clearing
            game_instance->markClearRows(clear_rows);

            redrawGameField(rend);

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
            
            redrawGameField(rend);

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
            game_over = true;
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

void display_game_over_prompt(SDL_Renderer* rend)
{
    game_instance->render(rend);
    renderScoreText(rend);
    renderTimerText(rend);

    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    renderBoxCenter(rend, box_width, box_height, ns_Tutris::TUTRIS_COLOR_BLACK, ns_Tutris::TUTRIS_COLOR_RED);

    int text_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2 - 70;
    int text_y_pos = SCREEN_HEIGHT/2 - box_height/2;

    SDL_Utils::renderTexture(text_game_over, rend, text_x_pos, text_y_pos + 20);
    SDL_Utils::renderTexture(text_victory_prompt_quit, rend, text_x_pos, text_y_pos + 100);
    SDL_Utils::renderTexture(text_victory_prompt_start, rend, text_x_pos, text_y_pos + 140);

    SDL_RenderPresent(rend);
}

void display_victory_prompt(SDL_Renderer* rend)
{
    game_instance->render(rend);
    renderScoreText(rend);
    renderTimerText(rend);

    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    renderBoxCenter(rend, box_width, box_height, ns_Tutris::TUTRIS_COLOR_BLACK, ns_Tutris::TUTRIS_COLOR_GREEN);

    int text_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2 - 60;
    int text_y_pos = SCREEN_HEIGHT/2 - box_height/2;
    SDL_Utils::renderTexture(text_victory, rend, text_x_pos, text_y_pos + 20);
    SDL_Utils::renderTexture(text_victory_prompt_quit, rend, text_x_pos, text_y_pos + 100);
    SDL_Utils::renderTexture(text_victory_prompt_start, rend, text_x_pos, text_y_pos + 140);

    SDL_RenderPresent(rend);
}

void display_title_prompt(SDL_Renderer* rend)
{
    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    renderBoxCenter(rend, box_width, box_height, ns_Tutris::TUTRIS_COLOR_BLACK, ns_Tutris::TUTRIS_COLOR_WHITE);

    int text_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
    int text_y_pos = SCREEN_HEIGHT/2 - box_height/2;
    SDL_Utils::renderTexture(text_title, rend, text_x_pos + 10, text_y_pos + 40);
    SDL_Utils::renderTexture(text_title_prompt_start, rend, text_x_pos, text_y_pos + 120);

    SDL_RenderPresent(rend);
}

void display_Pause_prompt(SDL_Renderer* rend)
{
    game_instance->render(rend);
    renderScoreText(rend);
    renderTimerText(rend);

    int box_width = ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL + 200;
    int box_height = 200;
    renderBoxCenter(rend, box_width, box_height, ns_Tutris::TUTRIS_COLOR_BLACK, ns_Tutris::TUTRIS_COLOR_WHITE);

    int text_x_pos = SCREEN_WIDTH/2 - (ns_Tutris::FIELD_WIDTH*ns_Tutris::BLOCK_SIZE_PIXEL)/2 - 100;
    int text_y_pos = SCREEN_HEIGHT/2 - box_height/2;
    SDL_Utils::renderTexture(text_pause, rend, text_x_pos + 100, text_y_pos + 70);

    SDL_RenderPresent(rend);
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

void renderBox(SDL_Renderer *rend, 
                int x_pos, 
                int y_pox, 
                int w, 
                int h, 
                ns_Tutris::tutris_color bg, 
                ns_Tutris::tutris_color outline )
{
    SDL_Rect box = {
        x_pos,
        y_pox,
        w,
        h
    };
    SDL_SetRenderDrawColor( rend, bg.r ,bg.g ,bg.b ,0xFF);
    SDL_RenderFillRect(rend, &box);
    SDL_SetRenderDrawColor( rend, outline.r,outline.g,outline.b,0xFF);
    SDL_RenderDrawRect(rend, &box);
}

void renderBoxCenter(SDL_Renderer *rend,
                    int w, 
                    int h, 
                    ns_Tutris::tutris_color bg, 
                    ns_Tutris::tutris_color outline )
{
    int box_x_pos = SCREEN_WIDTH/2 - w/2;
    int box_y_pos = SCREEN_HEIGHT/2 - h/2;
    renderBox(rend, box_x_pos, box_y_pos, w, h, bg, outline);
}

void updateGameScoreText(SDL_Renderer* r, unsigned int s)
{
    std::stringstream ss;
    ss.str("");
    ss << "SCORE: ";
    ss << s;
    text_score = SDL_Utils::renderText(ss.str(),
        main_font_path,
        SDL_COLOR_WHITE,
        24,
        r);
}

void clearScreen(SDL_Renderer* r)
{
    SDL_SetRenderDrawColor(r, ns_Tutris::COLOR_BACKGROUND.r, ns_Tutris::COLOR_BACKGROUND.g, ns_Tutris::COLOR_BACKGROUND.b, 0xFF);
    SDL_RenderClear(r);
}

void renderScoreText(SDL_Renderer* r)
{
    int score_x_pos = SCREEN_WIDTH/2 + (game_instance->getNumCols()*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
    score_x_pos += 10; // shift a few pixels away from game grid
    int score_y_pos = SCREEN_HEIGHT/2 - (game_instance->getNumRows()*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
    SDL_Utils::renderTexture(text_score, r, score_x_pos, score_y_pos);
}

void renderTimerText(SDL_Renderer* r)
{
    int timer_x_pos = SCREEN_WIDTH/2 + (game_instance->getNumCols()*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
    timer_x_pos += 10; // shift a few pixels away from game grid
    int timer_y_pos = SCREEN_HEIGHT/2 - (game_instance->getNumRows()*ns_Tutris::BLOCK_SIZE_PIXEL)/2;
    timer_y_pos += 20; // shift down below score text
    SDL_Utils::renderTexture(text_timer, r, timer_x_pos, timer_y_pos);
}

void redrawGameField(SDL_Renderer* r)
{
    clearScreen(r);
    game_instance->render(r);
    renderScoreText(r);
    renderTimerText(r);
    SDL_RenderPresent(r);
}


