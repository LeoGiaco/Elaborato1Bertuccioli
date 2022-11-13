SHELL=/bin/bash
FLAGS=-g -I ./core -I ./shapes -lglut -lGLU -lGL -lGLEW

all: main.out

main.out: ./bin/main.o ./bin/Window.o ./bin/GLProgram.o ./bin/Scene.o ./bin/Shape2D.o ./bin/ComplexShape2D.o ./bin/Bullet.o
	g++ -o main.out ./bin/*.o ${FLAGS}

./bin/main.o: main.cpp
	g++ -c -o ./bin/main.o main.cpp ${FLAGS}

./bin/Window.o: ./core/Window.cpp ./core/Window.h
	g++ -c -o ./bin/Window.o ./core/Window.cpp ${FLAGS}

./bin/GLProgram.o: ./core/GLProgram.cpp ./core/GLProgram.h
	g++ -c -o ./bin/GLProgram.o ./core/GLProgram.cpp ${FLAGS}

./bin/Scene.o: ./core/Scene.cpp ./core/Scene.h
	g++ -c -o ./bin/Scene.o ./core/Scene.cpp ${FLAGS}

./bin/Shape2D.o: ./shapes/Shape2D.cpp ./shapes/Shape2D.h
	g++ -c -o ./bin/Shape2D.o ./shapes/Shape2D.cpp ${FLAGS}

./bin/ComplexShape2D.o: ./shapes/ComplexShape2D.cpp ./shapes/ComplexShape2D.h
	g++ -c -o ./bin/ComplexShape2D.o ./shapes/ComplexShape2D.cpp ${FLAGS}

./bin/Bullet.o: ./entities/Bullet.cpp ./entities/Bullet.h
	g++ -c -o ./bin/Bullet.o ./entities/Bullet.cpp ${FLAGS}

.PHONY: clean

clean:
	-rm ./bin/*.o *.out