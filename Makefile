CXX := g++
OUTPUT := sfmlgame

# SFML_DIR := /opt/homebrew/Cellar/sfml/2.6.1 # macOS m1
SFML_DIR := .

CXX_FLAGS := -O3 -std=c++17
# CXX_FLAGS := -g -O3 -std=c++17 # for debugging
INCLUDES := -I./src -I$(SFML_DIR)/include
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L$(SFML_DIR)/lib

SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(patsubst src/%.cpp,bin/%.o,$(SRC_FILES))

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@
	cp -r ./assets ./bin/

bin/%.o: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $< -c -o $@

clean:
	rm -f $(OBJ_FILES)
	rm -f ./bin/$(OUTPUT)
	rm -rf ./bin/assets

run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..