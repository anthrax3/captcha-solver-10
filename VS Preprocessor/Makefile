full: main.o image.o
	g++ -o split main.o image.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

main.o: main.cpp
	g++ -c main.cpp

image.o: image.cpp image.h
	g++ -c image.cpp

