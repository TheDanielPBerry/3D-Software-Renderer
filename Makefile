cc = g++

flags = -g `sdl2-config --cflags --libs`

output = raster


run: build
	./$(output)

debug: build
	gdb ./$(output)

build:
	$(cc) -o $(output) main.cpp Plane.cpp rasterize.cpp $(flags)

build_floor:
	$(cc) -o floor main.cpp render.cpp camera.cpp $(flags)


