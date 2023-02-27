FLAGS = -Iinclude -Wall

OBJECTS = obj/list.o obj/file_hash.o

all: bin/main

bin/%: src/%.c $(OBJECTS)
	gcc -o $@ $^ $(FLAGS)

obj/%.o: src/%.c include/%.h
	gcc -c -o $@ $< $(FLAGS)

clean:
	rm bin/main obj/*.o
