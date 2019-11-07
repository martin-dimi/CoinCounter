run:
	g++ main.cpp Sobel.cpp `pkg-config --libs --cflags opencv4` 
	./a.out coins2.png