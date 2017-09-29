//
//  routines.h
//  
//
//  Created by Leo Li on 9/26/17.
//

#ifndef routines_h
#define routines_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include "msg.h"
#include "helpers.h"
#include "msg_stack.h"

extern struct msg * mailbox;
extern sem_t * send_sem;
extern sem_t * rec_sem;
extern FILE *fp;
extern int nb;

void m_thread(void * NUM_THREADS);
void adder(void *threadid);
void InitMailBox();


#endif /* routines_h */
