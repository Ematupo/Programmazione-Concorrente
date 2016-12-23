all: prog prog1

prog: buffer.o msg.o test_buffer.o
	gcc -o prog -g buffer.o msg.o test_buffer.o -lpthread -lcunit
CFLAGS=-g
prog1: buffer.o msg.o test_msg.o
	gcc -o prog1 -g buffer.o msg.o test_msg.o -lpthread -lcunit
clean:
	rm *.o
