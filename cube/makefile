
GAMES = Connect4 Debug Effects PerfectFit PuzzleCube Reversi Snake TicTacToe

reqOutFiles = out/core/cubeCore.o out/core/visualRenderer.o out/core/color.o out/core/perlin.o out/core/geometry.o
CXXFLAGS =
ifneq ($(filter arm%,$(shell uname -p)),)
CXXFLAGS += -lwiringPi -pthread
else
CXXFLAGS += `pkg-config --cflags --libs sdl2`
endif

.PHONY: all clean games $(GAMES) $(GAMESgame)

all: menu games

$(GAMES):
	@make bin/$@

games: $(addprefix bin/, $(GAMES))

bin/%: src/games/%.cpp $(reqOutFiles) | bin/
	@g++ -o $@ src/games/$(patsubst bin/%,%,$@).cpp $(reqOutFiles) $(CXXFLAGS)
	@echo Made $(patsubst bin/%,%,$@)

menu: bin/menu
	@ln -sf bin/menu menu

bin/menu: out/menu.o $(reqOutFiles) | bin/
	@g++ -o bin/menu $^ $(CXXFLAGS)
	@echo Made menu

out/menu.o: src/menu.cpp | out/
	@g++ -std=c++11 -o out/menu.o -c src/menu.cpp

out/core/cubeCore.o: src/core/cubeCore.cpp src/core/cubeCore.hpp src/core/visualRenderer.hpp | out/core/
	@g++ -std=c++11 -o out/core/cubeCore.o -c src/core/cubeCore.cpp

out/core/visualRenderer.o: src/core/visualRenderer.cpp src/core/visualRenderer.hpp src/core/cubeCore.hpp | out/core/
	@g++ -std=c++11 -o out/core/visualRenderer.o -c src/core/visualRenderer.cpp

out/core/color.o: src/core/color.cpp src/core/color.hpp src/core/cubeCore.hpp | out/core/
	@g++ -std=c++11 -o out/core/color.o -c src/core/color.cpp

out/core/perlin.o: src/core/perlin.cpp src/core/perlin.hpp | out/core/
	@g++ -std=c++11 -o out/core/perlin.o -c src/core/perlin.cpp

out/core/geometry.o: src/core/geometry.cpp src/core/geometry.hpp | out/core/
	@g++ -std=c++11 -o out/core/geometry.o -c src/core/geometry.cpp

out/core/:
	@mkdir -p out/core/

out/:
	@mkdir -p out/

bin/:
	@mkdir -p bin/

clean:
	@rm -rf out/ bin/ menu