# _*_ Makefile _*_

OBJ = filehandle.o

filehandle: $(OBJ)
	gcc -o filehandle $(OBJ)

filehandle.o : filehandle.c
	gcc -c filehandle.c

clean:
	rm *.o filehandle
	rm *.txt
