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
	$(cc) -o $(output) main.cpp $(dependencies) $(flags) -O4


server_dependencies = obj/Plane.o obj/scene.o obj/model.o obj/Vec.o obj/Physics.o obj/player.o obj/game.o
client_dependencies = $(server_dependencies)
net_flags = -Llib/ -lboost_serialization



server: $(dependencies) server.cpp
	$(cc) server.cpp $(dependencies) $(flags) $(net_flags) -o server

client: $(dependencies) client.cpp
	$(cc) client.cpp $(dependencies) $(flags) $(net_flags) -o client

obj/%.o: %.cpp
	$(cc) -c $< -o $@ $(flags) -O4

clean:
	rm obj/*



