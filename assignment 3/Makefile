raytracer: raytracer.o Vector3.o
	g++ raytracer.o Vector3.o -o raytracer -lpthread

raytracer.o: raytracer.cpp
	g++ -c raytracer.cpp

Vector3.o: Vector3.cpp
	g++ -c Vector3.cpp

clean:
	rm *.o *.ppm raytracer