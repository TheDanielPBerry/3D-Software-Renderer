cc = g++

flags = -g `sdl2-config --cflags --libs`

output = 3d


run: build
	./$(output)

debug: build
	gdb ./$(output)
build:
	$(cc) -o $(output) main.cpp render.cpp camera.cpp $(flags)


