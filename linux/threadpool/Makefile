EXEC = threadpool
OBJS = main.o threadpool.o
SRC  = main.c threadpool.c

CC = gcc
CFLAGS += -O2 -Wall 
LDFLAGS +=

all:$(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -pthread

%.o:%.c uwsts.h
	$(CC) $(CFLAGS) -c $< -o $@ -pthread

clean:
	@rm -vf $(EXEC) *.o *~
