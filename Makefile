FLAGS = -Iinclude -Wall

OBJECTS = obj/list.o obj/file_hash.o obj/work_file.o obj/commit_handler.o obj/reference_handler.o obj/branch_handler.o obj/merge_handler.o obj/utilities.o

all: bin/main_test bin/main

#rm -Rf ??
bin/%: src/%.c $(OBJECTS)
	rm -f tmp*
	gcc -g -o $@ $^ $(FLAGS)
	mkdir -p ../projet_tests
	cp -r * ../projet_tests/

obj/%.o: src/%.c include/%.h
	gcc -c -g -o $@ $< $(FLAGS)

clean:
	rm -Rf bin/main obj/*.o tmp* ?? .refs .add .current_branch
