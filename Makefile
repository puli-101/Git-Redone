FLAGS = -Iinclude -Wall

all: bin/main

bin/%: src/%.c obj/list.o 
	gcc -o $@ $^ $(FLAGS)

obj/%.o: src/%.c include/%.h
	gcc -c -o $@ $< $(FLAGS)

clean:
	rm bin/main obj/*.o
