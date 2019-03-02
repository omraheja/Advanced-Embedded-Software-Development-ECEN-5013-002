# _*_ Makefile _*_

CC=gcc

OBJS = queue_server.o 
OBJC = queue_client.o

all:queue_server queue_client

queue_server: $(OBJS)
	$(CC) -o queue_server $(OBJS) -lrt -g

queue_server.o: queue_server.c
	$(CC) -c queue_server.c

queue_client:$(OBJC)
	$(CC) -o queue_client $(OBJC) -lrt -g

queue_client.o:queue_client.c
	$(CC) -c queue_client.c

clean:
	rm *.o queue_server queue_client
	rm om.txt
