all:
	g++ main.cpp -o set -O3 -march=native -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio