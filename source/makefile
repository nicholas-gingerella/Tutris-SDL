CXX = g++
CXXFLAGS = -Wall -std=c++11 -I$(SDL_INCLUDE) -I$(INCLUDE_DIR)
SDL_LIB = -L/usr/lib -lSDL2 -lSDL2_image -Wl,-rpath=/usr/lib
LDFLAGS = $(SDL_LIB)
SDL_INCLUDE = /usr/SDL_INCLUDE
INCLUDE_DIR = ../include
OBJ_DIR = obj
BIN_DIR = ../bin
EXE = Tutris

_INCLUDES = field.h tetromino.h
INCLUDES = $(patsubst %,$(INCLUDE_DIR)/%,$(_INCLUDES))

_OBJS = main.o field.o tetromino.o 
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

$(OBJ_DIR)/%.o: %.cpp $(INCLUDES)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJS)
	$(CXX) -o $(BIN_DIR)/$@ $^ $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(OBJ_DIR)/*.o && rm $(BIN_DIR)/$(EXE)
