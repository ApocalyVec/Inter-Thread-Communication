Expected at least two arguments:
1. number of threads to creat.
2. input file wish to be read.
3. (optional) enable nb

The input file should consist of two number a line. The first number is the value of a message and the second is the #thread the message will go.

The main thread creates the given number of child thread of "adder" routine and reads from the given filename line by line, and send each line as a message to the target child thread. When receiving a message, a child thread will add the value get into its total value (initially 0) and sleep for 1 second.

After reaching EOF, the main thread will send a termination message to all child threads. And the child threads, on receiving the termination message, send their result back to the mailbox. The main thread then prints out the result from each child and terminates.

For example, in file "test.txt":
7 1
4 2
2 1
1 2
5 3
(EOF)

The first line will let main thread to send 3 to child thread 1 (main thread is 0), the second line sends 4 to thread 2 and so forth.

Command line prompt:
shell> ./main 3 test.txt

The result from this will be:

shell> ./main 3 test.txt
NB option OFF.
In main: creating thread 1
In main: creating thread 2
In main: creating thread 3
Children thread #3 created.

Children thread #1 created.

Children thread #2 created.

The result from thread 1 is 8 from 3 operations during 3 secs. 
The result from thread 2 is 5 from 2 operations during 2 secs. 
The result from thread 3 is 7 from 1 operations during 2 secs. 
shell> 

The time result may vary depending on system scheduler.

NB option: Turn on option NB will let the main thread to use a different send message routine NBsendMsg() instead of sendMsg(). Normally, to achieve synchronization, if the recipient hasn't called on recvMsg() routine to get the current message, sendMsg() will be blocked when trying to send another message to the same mailbox. However, NBsendMsg() will not block, and "skip" that message. The skipped messages are stored in a message stack, and main thread will try to resent them after reaching EOF (before sending termination messages). 
The result should be the same no matter NB is on or off.
