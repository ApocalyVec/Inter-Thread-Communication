////
//  routines.c
//  
//
//  Created by Leo Li on 9/26/17.
//

#include "routines.h"

#define line_buffer_size 64


//routine for MAIN thread
void m_thread(void * NUM_THREADS) {
    /*
    sem_t test;
    sem_init(&test, 1, -1);
    int t;
    sem_getvalue(&test, &t);
    printf("Before TRY: %d.\n", t);
    sem_trywait(&test);
    sem_getvalue(&test, &t);
    printf("After TRY: %d.\n", t);
    */
    
    //variable initialization
    long i;
    int rc;
    struct StackNode* root = NULL; //wait to be sent queue
    
    char * line = malloc(sizeof(char *) * line_buffer_size);
    char ** line_arguments =  malloc(sizeof(char **) * line_buffer_size); //for tokenized input arguments from file test.txt
    struct msg curr_msg;
    struct msg ter_msg; //termination msg
    void *status;
    
    //creating CHILD threads
    pthread_t threads[(long)NUM_THREADS]; //array of child threads
    for(i=0; i<(long)NUM_THREADS; i++){
        printf("In main: creating thread %ld\n", i+1);
        rc = pthread_create(&threads[i], NULL, adder, (void *)(i+1));
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    //initialize global semaphores
    send_sem = malloc(sizeof(sem_t) * (long)NUM_THREADS);
    rec_sem = malloc(sizeof(sem_t) * (long)NUM_THREADS);
    for(i = 0; i < (long)NUM_THREADS; i ++) {
        sem_init(&send_sem[i], 1, 1);
    }
    for(i = 0; i < (long)NUM_THREADS; i ++) {
        sem_init(&rec_sem[i], 1, 0);
    }


    //handling message
    i = 0; //line counter
    while(fgets(line, line_buffer_size, fp) != NULL) {
        i ++;
        line_arguments = parse_input(line); //tokenize arguments with space as diliminator
        
        //conditions to terminate CHILD threads
        if(line_arguments[0] == NULL) {
            printf("Error reading detected in line %ld of test.txt, no argument found. Terminating all CHILD threads. \n", i);
            break; //not enough arguments
        }
        else if(line_arguments[1] == NULL) {
            printf("Error reading detected in line %ld of test.txt, only one argument found. Terminating all CHILD threads. \n", i);
            break; //not enough arguments
        }
        if(atoi(line_arguments[1]) < 0) {
            printf("Error reading detected in line %ld of test.txt: negative thread index. Terminating all CHILD threads. \n", i);
            break; //argument illegal
        }
        if(atoi(line_arguments[0]) == 0) {
            printf("Error reading detected in line %ld of test.txt: 0 as input. Terminating all CHILD threads. \n", i);
            break; //argument illegal
        }
        if(atoi(line_arguments[1]) > (long)NUM_THREADS) {
            printf("Error reading detected in line %ld of test.txt: thread index out of bound. Terminating all CHILD threads. \n", i);
            break; //argument illegal
        } //end of conditions to terminate CHILD threads
        
        //creating msg based on inputs
        set_msg(&curr_msg, 0, atoi(line_arguments[0]), 0, 0);
        //sending msg

        //printf("sending message to %d waiting. \n", atoi(line_arguments[1]));//test
        if(nb) {
            if(NBSendMsg(atoi(line_arguments[1]) - 1, &curr_msg) == -1) {
                push(&root, atoi(line_arguments[1]) - 1 ,curr_msg.value);
            }
        }
        else SendMsg(atoi(line_arguments[1]) - 1, &curr_msg);//index number minus one to match the real index number
        //printf("Value after sending is %d. \n", mailbox[atoi(line_arguments[1])].value);
        //printf("sending message to %d finished. \n", atoi(line_arguments[1]));//test
        //printf("Indexing number in main thread is: %ld.\n", atoi(line_arguments[1]));
    }
    
    //NB ENABLED: resent buffered msg
    if(nb) {
        while(!isEmpty(root)) {
            //printf("isEmpty(root): %d\n", isEmpty(root));
            set_msg(&curr_msg, 0, peek_value(root), 0, 0);
            if(NBSendMsg(peek_iTo(root), &curr_msg) != -1) {
                //printf("Msg resent successful; Sent to #%d, Sent value is %d\n", peek_iTo(root)+1, curr_msg.value);
                pop(&root);
                //printf("After popping, ~isEmpty(root): %d\n", !isEmpty(root));
            }
        }
    }
    
    //send termination msg
    set_msg(&ter_msg, 0, -1, 0, 0);
    for(i=0; i<(long)NUM_THREADS; i++) {

        //printf("Sending termination_msg to %ld waiting.\n", i+1);
        
        SendMsg(i, &ter_msg);
        
        //printf("Sending termination_msg to %ld finished.\n", i+1);
    }
    
    
    //pthread join
    for(i=0; i<(long)NUM_THREADS; i++) {
        rc = pthread_join(threads[i], &status);
        
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            printf("Main: completed join with thread %ld having a status of %ld\n",i,(long)status);
            exit(-1);
        }
    }
    
    for(i = 0; i < (long)NUM_THREADS; i++) {
        printf("The result from thread %ld is %d from %d operations during %d secs. \n", i + 1, mailbox[i].value, mailbox[i].cnt, mailbox[i].tot);
    }
    
    //clearing used sems
    for(i = 0; i < (long)NUM_THREADS; i ++) {
        sem_destroy(&send_sem[i]);
    }
    for(i = 0; i <(long)NUM_THREADS; i ++) {
        sem_destroy(&rec_sem[i]);
    }

    pthread_exit(NULL); //exit
}

//routine for CHILD thread
void adder(void * threadid) {
    
    //timer variables
    time_t ini;
    time_t fin;
    ini = time(NULL);
    
    struct msg ret_msg;
    printf("Children thread #%ld created.\n\n", (long)threadid);
    //receiving msg
    while(mailbox[(long)threadid-1].value >= 0) {
        if(mailbox[(long)threadid-1].value < 0) {
            //printf("Thread #%ld quits receiving msg on TERMINATION.\n", (long)threadid);
            break;
        }//break on termination
        
        RecvMsg((long)threadid-1, &ret_msg);//fill in the return msg
        
    }

    //printf("Receiving termination_msg, thread index %ld. \n", (long)threadid);
    fin = time(NULL);
    ret_msg.tot = (long)(fin - ini);
    
    sem_post(&send_sem[(long)threadid-1]);
    SendMsg((long)threadid-1, &ret_msg); //making sending back to main possible after termination
    //printf("Receiving termination_msg, thread index %ld finished. \n", (long)threadid);

    pthread_exit(NULL);
}

void InitMailBox() {
    int i;
    for(i = 0; i < (sizeof(mailbox)/sizeof(struct msg)); i ++) {
        set_msg(&mailbox[i], -1, 0, 0, 0);
    }
    pthread_exit(NULL); //exit
}



