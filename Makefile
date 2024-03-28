TARGET=main
CC=g++

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
HEADERS= -I./headers/
CFLAGS= -march=native -mavx2 -O3

all:
	g++ src/main.cpp -c $(CFLAGS) $(HEADERS) 
	g++ src/mandelbrot.cpp -c $(CFLAGS) $(HEADERS) 
	g++ src/perftest.cpp -c $(CFLAGS) $(HEADERS)
	g++ main.o mandelbrot.o perftest.o $(LDFLAGS) -o $(TARGET)

run: all
		./$(TARGET)

clean:
	rm *.o main



# debug : CFLAGS= -march=native -mavx2 -Wno-conversion -Wno-unused-variable -Wno-pointer-arith -g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
