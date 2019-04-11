# _*_ Makefile _*_

CC=arm-linux-gcc

OBJS = server.o 
OBJC = client.o

all:server client

server: $(OBJS)
	$(CC) -o server $(OBJS)

server.o: server.c
	$(CC) -c server.c

client:$(OBJC)
	$(CC) -o client $(OBJC)

client.o:client.c
	$(CC) -c client.c

clean:
	rm *.o server client
	rm om.txt
