FLAGS = -Iinclude -Wall

OBJECTS = obj/list.o obj/file_hash.o obj/work_file.o

all: bin/main

bin/%: src/%.c $(OBJECTS)
	rm -f tmp*
	rm -Rf ??
	gcc -o $@ $^ $(FLAGS)

obj/%.o: src/%.c include/%.h
	gcc -c -o $@ $< $(FLAGS)

clean:
	rm -Rf bin/main obj/*.o tmp* ??
