CXX = g++
CXXFLAGS = -ggdb -Wall -std=c++11 -I$(SDL_INCLUDE) -I$(INCLUDE_DIR)
SDL_LIB = -L/usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -Wl,-rpath=/usr/lib
LDFLAGS = $(SDL_LIB)
SDL_INCLUDE = /usr/SDL_INCLUDE
INCLUDE_DIR = ../include
UTILS_INCLUDE_DIR = $(INCLUDE_DIR)/my_utils
TUTRIS_INCLUDES_DIR = $(INCLUDE_DIR)/tutris
OBJ_DIR = obj
BIN_DIR = ../bin
EXE = Tutris

# Tutris Includes
_TUTRIS_INCLUDES = field.h tetromino.h tutris.h
TUTRIS_INCLUDES = $(patsubst %,$(TUTRIS_INCLUDES_DIR)/%,$(_TUTRIS_INCLUDES))

_UTIL_INCLUDES = SDL_Utils.h
UTILS_INCLUDE = $(patsubst %,$(UTILS_INCLUDE_DIR)/%,$(_UTIL_INCLUDES))

_OBJS = main.o field.o tetromino.o 
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

$(OBJ_DIR)/%.o: %.cpp $(UTILS_INCLUDE) $(TUTRIS_INCLUDES)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJS)
	$(CXX) -o $(BIN_DIR)/$@ $^ $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJ_DIR)/*.o && rm $(BIN_DIR)/$(EXE)
