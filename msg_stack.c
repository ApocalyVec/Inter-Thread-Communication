//
//  msg_stack.c
//  
//
//  Created by Leo Li on 9/28/17.
//

#include "msg_stack.h" //to be more exact, this should be called send_stack

struct StackNode* newNode(int iTo, int value)
{
    struct StackNode* stackNode =
    (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->iTo = iTo;
    stackNode->value = value;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode *root)
{
    return !root;
}

void push(struct StackNode** root, int iTo, int value)
{
    struct StackNode* stackNode = newNode(iTo, value);
    stackNode->next = *root;
    *root = stackNode;
    //printf("msg to %d with value of %d has been push onto stack\n", iTo+1, value);
}

int pop(struct StackNode** root)
{
    if (isEmpty(*root))
        return INT_MIN;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    int popped_iTo = temp->iTo;
    free(temp);
    
    return popped_iTo;
}

int peek_iTo(struct StackNode* root)
{
    if (isEmpty(root)){
        printf("This should never happen. \n");
        return INT_MIN;
    }
    return root->iTo;
}

int peek_value(struct StackNode* root)
{
    if (isEmpty(root)) {
        printf("This should never happen. \n");
        return INT_MIN;
    }
    return root->value;
}
