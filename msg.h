//
//  msg.h
//  
//
//  Created by Leo Li on 9/23/17.
//

#ifndef msg_h
#define msg_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for sleep() call
#include <errno.h> //for trywait
#include "routines.h"

extern struct msg * mailbox;
extern sem_t * send_sem;
extern sem_t * rec_sem;

struct msg {
    int iFrom; //sender
    int value; //message value
    int cnt; //count of operations (not needed by all msgs)
    int tot; //total time (not needed by all msgs)
};

void set_msg(struct msg *m, int iFrom, int value, int cnt, int tot);
void SendMsg(int iTo, struct msg *pMsg);
/* iTo - mailbox to send to */
/* pMsg - message to be sent */
int NBSendMsg(int iTo, struct msg *pMsg);
void RecvMsg(int iFrom, struct msg *pMsg);
/* iFrom - mailbox to receive from */
/* pMsg - message structure to fill in with received message */

#endif /* msg_h */
