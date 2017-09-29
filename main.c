//
//  main.c
//  
//
//  Created by Leo Li on 9/23/17.
//

//CS_3013 proj3
//zli12@wpi.edu
//Ziheng(Leo) Li

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "msg.h"
#include "routines.h"
#include "helpers.h"
#include "msg_stack.h" //queue for NBsendMsg

#define MAXTHREAD = 10;

struct msg * mailbox; //global variable mailbox
//global semaphores used in main and child threads' routines
//initializing pointers to sems
sem_t * send_sem;
sem_t * rec_sem;
//global file pointer
FILE *fp;
int nb;//switch for np option


int main(int argc, char *argv[]) {
    long NUM_THREADS;
    int rc;
    pthread_t m_threads;
    
    if(argc != 3 && argc != 4) {
        printf("Need at least two arguement, first is the number of threads, second is the file wish to be read in, third is whether to turn on NB (OPTIONAL) You have %d.\n", argc);
        exit(-1);
    }//checking for if the number of command line argument is valid
    
    NUM_THREADS = atoi(argv[1]);
    if(NUM_THREADS > 10) {
        printf("Number of thread must be lease or equal to 10, given: %ld\n", NUM_THREADS);
        printf("Thread number set to maximum thread number = 10\n");
        NUM_THREADS = 10;
    } // set thread number to be 10 if given number is greater than 10
    
    //allocate memory for global mailbox
    mailbox = malloc(sizeof(struct msg) * NUM_THREADS);
    
    //input file initializing
    fp = fopen(argv[2], "r");
    if(fp == NULL)
    {
        perror("Error opening file");
        exit(-1);
    }
    //setting nb option
    if(argc == 4) {
        nb = 1;
        printf("NB option ON.\n");
    }
    else {
        nb = 0;
        printf("NB option OFF.\n");
    }
    //inialize mailbox
    rc = pthread_create(&m_threads, NULL, InitMailBox, NULL);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    //creating main thread
    rc = pthread_create(&m_threads, NULL, m_thread, (void *)NUM_THREADS);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    } //pthread_create returns non-zero on failure
    
    pthread_exit(NULL);
    return 1;
}


