cc = g++

flags = -g `sdl2-config --cflags --libs`

output = raster


run: build
	./$(output)

debug: build
	gdb ./$(output)

dependencies = obj/Plane.o obj/rasterize.o

build: $(dependencies)
	$(cc) -o $(output) main.cpp $(dependencies) $(flags)


obj/%.o: %.cpp
	$(cc) -c $< -o $@ $(flags)

	
build_floor:
	$(cc) -o floor main.cpp render.cpp camera.cpp $(flags)


