#ifndef __ASSERT_H_
#define __ASSERT_H_

#include <stdio.h>
#include <stdlib.h>

#define CLR_REG "\e[1;1H\e[2J"

#define assert(x)\ 
 if((!(x))){\
    printf("\e[1;1H\e[2J assertion error!\n error at %s() function( at line: %d ).\n",__func__, __LINE__);\ 
    exit(1);\
}\

#define assertf(x, ...)\ 
 if((!(x))){\
    printf("assertion error!\n error at %s() function( at line: %d ).\n last log was: ",__func__, __LINE__);\ 
    printf(__VA_ARGS__);\
    exit(1);\
}\



#endif