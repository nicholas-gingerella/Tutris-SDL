CXX = g++
CXXFLAGS = -ggdb -Wall -Wextra -O2 -std=c++17 -static-libgcc -static-libstdc++ -I$(SDL_INCLUDE) -I$(SDL_IMAGE_INCLUDE) -I$(SDL_MIXER_INCLUDE) -I$(SDL_TTF_INCLUDE)

SDL_LIB = -L../lib/SDL2-2.0.18/x86_64-w64-mingw32/lib
SDL_IMAGE_LIB = -L../lib/SDL2_image-2.0.5/x86_64-w64-mingw32/lib
SDL_MIXER_LIB = -L../lib/SDL2_mixer-2.0.4/x86_64-w64-mingw32/lib
SDL_TTF_LIB = -L../lib/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib
LDFLAGS = $(SDL_LIB) $(SDL_IMAGE_LIB) $(SDL_MIXER_LIB) $(SDL_TTF_LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -mwindows

SDL_INCLUDE = ../lib/SDL2-2.0.18/x86_64-w64-mingw32/include
SDL_IMAGE_INCLUDE = ../lib/SDL2_image-2.0.5/x86_64-w64-mingw32/include
SDL_MIXER_INCLUDE = ../lib/SDL2_mixer-2.0.4/x86_64-w64-mingw32/include
SDL_TTF_INCLUDE = ../lib/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include

OBJ_DIR = ./
BIN_DIR = ../bin
EXE = Tutris

_OBJS = main.o Game.o Piece.o 
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

$(OBJ_DIR)/%.o: %.cpp $(UTILS_INCLUDE) $(TUTRIS_INCLUDES)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJS)
	$(CXX) -o $(BIN_DIR)/$@ $^ $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJ_DIR)/*.o && rm $(BIN_DIR)/$(EXE)
