# _*_ Makefile _*_

CC=arm-linux-gcc

OBJ = mythread.o

mythread: $(OBJ)
	$(CC) -o mythread $(OBJ) -lpthread -lrt

mythread.o: mythread.c
	$(CC) -c mythread.c

clean:
	rm *.o mythread
	rm om.txt
