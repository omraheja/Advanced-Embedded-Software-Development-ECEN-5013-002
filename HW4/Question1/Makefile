# _*_ Makefile _*_

OBJ = mythread.o

mythread: $(OBJ)
	gcc -o mythread $(OBJ) -pthread -lrt

mythread.o: mythread.c
	gcc -c mythread.c

clean:
	rm *.o mythread
	rm om.txt
