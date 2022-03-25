INCLUDES = -I.. -I$(DEP_DIR)/include -Isrc -I. -I$(DEP_DIR)/include -Iinclude -I.
INC = $(INCLUDES)
CC := g++
raytracer: raytracer.o Vector3.o
	$(CC) $(INC) raytracer.o Vector3.o -o raytracer

%.o: %.cpp
	$(CC) $(INC) -c -o $(@) $(<)

clean:
	rm *.o *.ppm raytracer