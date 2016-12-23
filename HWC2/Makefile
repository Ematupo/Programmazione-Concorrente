all: prog test_buffer test_msg test_provider test_reader test_accepter test_dispatcher

prog: accepter_buffer.o accepter.o \
	buffer.o dispatcher.o list.o msg.o \
	poison_pill.o provider_buffer.o \
	provider.o reader_buffer.o reader.o \
	reader_list.o main.o
	gcc -o prog -g accepter_buffer.o accepter.o buffer.o dispatcher.o list.o msg.o poison_pill.o provider_buffer.o provider.o reader_buffer.o reader.o reader_list.o main.o -lpthread

test_buffer: msg.o buffer.o test_buffer.o
	gcc -o test_buffer -g msg.o buffer.o test_buffer.o -lpthread -lcunit

test_msg: msg.o buffer.o test_msg.o
	gcc -o test_msg -g msg.o buffer.o test_msg.o -lpthread -lcunit

test_provider: msg.o poison_pill.o buffer.o provider_buffer.o provider.o test_provider.o 
	gcc -o test_provider -g msg.o poison_pill.o buffer.o provider_buffer.o provider.o test_provider.o -lpthread -lcunit

test_reader: msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o test_reader.o
	gcc -o test_reader -g msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o test_reader.o -lpthread -lcunit

test_accepter: msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o accepter_buffer.o accepter.o test_accepter.o
	gcc -o test_accepter -g msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o accepter_buffer.o accepter.o test_accepter.o -lpthread -lcunit

test_dispatcher: msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o provider_buffer.o provider.o dispatcher.o test_dispatcher.o
	gcc -o test_dispatcher -g msg.o poison_pill.o list.o buffer.o reader_buffer.o reader_list.o reader.o provider_buffer.o provider.o dispatcher.o test_dispatcher.o -lpthread -lcunit

CFLAGS=-g

clean:
	rm *.o
