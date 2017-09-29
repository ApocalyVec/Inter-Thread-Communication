//
//  msg_stack.h
//  
//
//  Created by Leo Li on 9/28/17.
//

#ifndef msg_stack_h
#define msg_stack_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "msg.h"

struct StackNode
{
    int iTo;
    int value;
    struct StackNode* next;
};
struct StackNode* newNode(int iTo, int value);
int isEmpty(struct StackNode *root);
void push(struct StackNode** root, int iTo, int value);
int pop(struct StackNode** root);
int peek_iTo(struct StackNode* root);
int peek_value(struct StackNode* root);

#endif /* msg_stack_h */
