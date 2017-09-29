//
//  helpers.c
//  
//
//  Created by Leo Li on 9/26/17.
//

#include "helpers.h"

#define deliminator " \a\n\r\t"

char **parse_input(char *input) {
    int i = 0;
    char *arguement = malloc(128 * sizeof(char));
    arguement = strtok(input, deliminator);
    char **arguements = malloc(128 * sizeof(char*));
    
    if(!arguements) {
        printf("parse_input: initial: fail to allocate for arguements\n");
        exit(1);
    }
    
    //put arguements in
    while(arguement != NULL) {
        arguements[i] = arguement;
        i ++;
        arguement = strtok(NULL, deliminator);
    }
    
    //put NULL arguement in the end
    arguements[i] = arguement;
    return arguements;
}
