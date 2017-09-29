//
//  msg.c
//  
//
//  Created by Leo Li on 9/23/17.
//

#include "msg.h"

void set_msg(struct msg *m, int iFrom, int value, int cnt, int tot) {
    m->iFrom = iFrom;
    m->value = value;
    m->cnt = cnt;
    m->tot = tot;
}

void SendMsg(int iTo, struct msg *pMsg) {
    /*
    int s;
    int r;
    sem_getvalue(&send_sem, &s);
    sem_getvalue(&rec_sem, &r);
    printf("INIT main: send_sem is %d.\n", s);
    printf("INIT main: rec_sem is %d.\n", r);
     */
    sem_wait(&send_sem[iTo]);
    mailbox[iTo].iFrom = pMsg->iFrom;
    mailbox[iTo].value = pMsg->value;
    mailbox[iTo].cnt = pMsg->cnt;
    mailbox[iTo].tot = pMsg->tot;
    sem_post(&rec_sem[iTo]);
    
}

int NBSendMsg(int iTo, struct msg *pMsg) {
    /*
     int s;
     int r;
     sem_getvalue(&send_sem, &s);
     sem_getvalue(&rec_sem, &r);
     printf("INIT main: send_sem is %d.\n", s);
     printf("INIT main: rec_sem is %d.\n", r);
     */
    int s;
    sem_getvalue(&send_sem[iTo], &s);

    sem_trywait(&send_sem[iTo]);
    if(s == 0) { //there's already a message
        //printf("TRYWAIT exit for thread #%d.\n", iTo +1);
        return -1;
    }
    mailbox[iTo].iFrom = pMsg->iFrom;
    mailbox[iTo].value = pMsg->value;
    mailbox[iTo].cnt = pMsg->cnt;
    mailbox[iTo].tot = pMsg->tot;
    sem_post(&rec_sem[iTo]);
    
    //printf("sending message to %d finished. \n", iTo+1);
    return 1;
}

void RecvMsg(int iFrom, struct msg *pMsg) { //iFrom is index number
    /*
    int s;
    int r;
    sem_getvalue(&send_sem, &s);
    sem_getvalue(&rec_sem, &r);
    printf("INIT main: send_sem is %d.\n", s);
    printf("INIT main: rec_sem is %d.\n", r);
     */
    sem_wait(&rec_sem[iFrom]);
    //printf("Thread #%d is receiving message. Received value is %d\n", iFrom+1, mailbox[iFrom].value);//test
    if(mailbox[iFrom].value < 0) { //this is a termination msg
        //printf("Thread #%int: RecvMsg BREAKS on TERMINATION.\n", iFrom+1);
        return;
    }
    //handling msg value
    if(mailbox[iFrom].value > 0) {
        pMsg->cnt ++;
    } //number of operations only counts when received value is non-negative
    //printf("In receiving, pMsg->value was %d.\n", pMsg->value);
    //printf("In receiving, mailbox[iFrom].value was %d.\n", mailbox[iFrom].value);
    pMsg->value += mailbox[iFrom].value;
    //clear mail box
    set_msg(&mailbox[iFrom], -1, 0, 0, 0);
    //printf("Thread #%d is  receiving message finished, posting send_sem. \n", iFrom+1);//test
    sem_post(&send_sem[iFrom]);
    sleep(1);
}
