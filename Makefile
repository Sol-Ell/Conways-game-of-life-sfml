run:
	g++ -c main.cpp -o main.o
	g++ -o main.exe main.o -lsfml-graphics -lsfml-window -lsfml-system
	./main.exe