FLAGS = -Iinclude -Wall

OBJECTS = obj/list.o obj/file_hash.o obj/work_file.o obj/commit_handler.o obj/reference_handler.o

all: bin/main

bin/%: src/%.c $(OBJECTS)
	rm -f tmp*
	rm -Rf ??
	gcc -g -o $@ $^ $(FLAGS)
	mkdir -p ../projet_tests
	cp -r * ../projet_tests/

obj/%.o: src/%.c include/%.h
	gcc -c -g -o $@ $< $(FLAGS)

clean:
	rm -Rf bin/main obj/*.o tmp* ??
