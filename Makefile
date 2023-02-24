all: main

%: %.c list.o 
	gcc -o $@ $^

%.o: %.c %.h
	gcc -c $<

clean:
	rm main *.o