cc = g++

flags = -g -rdynamic `sdl2-config --cflags --libs` -lSDL2_image

output = render

all: build

run: build
	./$(output)

debug: build
	gdb ./$(output)

dependencies = obj/Plane.o obj/rasterize.o obj/Light.o obj/scene.o obj/model.o obj/Vec.o obj/Physics.o obj/game.o obj/player.o obj/lines.o obj/boxedit.o

build: $(dependencies)
	$(cc) -o $(output) main.cpp $(dependencies) $(flags)


obj/%.o: %.cpp
	$(cc) -c $< -o $@ $(flags) -O4

clean:
	rm obj/*


