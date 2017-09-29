CFLAGS = -g

main: main.o msg.o routines.o helpers.o msg_stack.o
	gcc $(CFLAGS) -Wall msg.o main.o routines.o helpers.o msg_stack.o -lm -o main -pthread
msg.o:msg.c
	gcc $(CFLAGS) -Wall -c msg.c
routines.o:routines.c
	gcc $(CFLAGS) -Wall -c routines.c
helpers.o:helpers.c
	gcc $(CFLAGS) -Wall -c helpers.c
main.o:main.c
	gcc $(CFLAGS) -Wall -c main.c
msg_stack.o:msg_stack.c
	gcc $(CFLAGS) -Wall -c msg_stack.c
clean:
	rm -f main.o msg.o routines.o helpers.o msg_stack.o

all: main
