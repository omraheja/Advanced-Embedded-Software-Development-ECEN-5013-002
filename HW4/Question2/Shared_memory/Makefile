# _*_ Makefile _*_

CC=gcc

OBJS = shm_server.o 
OBJC = shm_client.o

all:shm_server shm_client

shm_server: $(OBJS)
	$(CC) -o shm_server $(OBJS) -lrt -lpthread

shm_server.o: shm_server.c
	$(CC) -c shm_server.c

shm_client:$(OBJC)
	$(CC) -o shm_client $(OBJC) -lrt -lpthread

shm_client.o:shm_client.c
	$(CC) -c shm_client.c

clean:
	rm *.o shm_server shm_client
	rm om.txt
