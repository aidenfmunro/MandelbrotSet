all:
	g++ main.cpp -o set -Ofast -march=native -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

clean:
	rm ./set