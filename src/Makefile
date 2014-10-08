CC=g++


all: Sp2


Sp2: input.o Sp2.cpp
	$(CC) input.o Sp2.cpp -o Sp2 -lxdo -lXi -lX11 -lpthread


input.o: input.cpp

	$(CC) -c input.cpp


clean:
	rm Sp2 input.o


install:
	cp Sp2 /usr/bin

uninstall:
	rm /usr/bin/Sp2
