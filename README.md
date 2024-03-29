# Tutris
yet another Tetris implementation! This is a rework for the simple ncurses tetris I made a ways back, but expanding on game rules and bringing in SDL for graphics.

![First Tutris Screenshot](/../documentation/docs/screenshots/tutris_play_screenshot.png?raw=true "Tutris")

## Controls
right arrow key: Move piece right

left arrow key: Move piece left

down arrow key: Move piece down

up arrow key: Rotate piece


## Gameplay
Below are the basic rules of the game.

### Game Timer
A match lasts 10 minutes. If you keep the blocks from reaching the top of the field, you win!
If any blocks reach the top of the field before the timer is up, you lose.

### Fall Speed
The piece drop speed increases every 30 seconds, maxing out at a top speed after 5 speedups.
Clearing a row will slow your piece drop speed down. After the 5 minute mark of the game, the piece drop speed will be locked at the 2nd fastest speed.

### Score
Filling rows with blocks will clear the row and increase your score.
* Single row: 160 points (Regular clear)
* 2 or more consecutive rows: 1600 points per row cleared (Collapse clear)
  * Collapse: Any gaps or overhanging blocks are caved in and collapsed, leaving no gaps in the field.
  * The more rows you clear at once, the higher the score, but remember, the longer you go without clearing, the faster your active piece will fall.

## How to Build

### Install C++ Compilation Tools
* `sudo apt install g++`
* `sudo apt install make`

### Install Libraries
Install the necessary SDL libraries on your system (Currently only tested with Ubuntu OS)

SDL2: Core SDL2 libraries
* `sudo apt install libsdl2-2.0-0`
* `sudo apt install libsdl2-dev`

SDL2-Image: Extension for handling image files
* `sudo apt install libsdl2-image-2.0-0`
* `sudo apt install libsdl2-image-dev`

SDL2-Mixer: Extension for handling sound files
* `sudo apt install libsdl2-mixer-2.0-0`
* `sudo apt install libsdl2-mixer-dev`

SDL2-TTF: Extension for handling fonts
* `sudo apt install libsdl2-ttf-2.0-0`
* `sudo apt install libsdl2-ttf-dev`

### Build Project
To build, navigate to source folder in repo
* `cd repo_directory/source`
 
Run make to build project
* `make`

### Run Executable
To run executable generated in bin directory of project
* `../bin/Tutris`
