#ifndef __ASSERT_H_
#define __ASSERT_H_

#include <stdio.h>
#include <stdlib.h>

#define assert(x) if((!(x))){printf("\e[1;1H\e[2J assertion error!\n error at %s() function( at line: %d ).\n",__func__, __LINE__); exit(1);}
#define CLR_REG "\e[1;1H\e[2J"
#define assert_with_log_s(x, log) if((!(x))){printf("assertion error!\n error at %s() function( at line: %d ).\n last log was:%s \n",__func__, __LINE__, log); exit(1);}
#define assert_with_log_d(x, log) if((!(x))){printf("assertion error!\n error at %s() function( at line: %d ).\n last log was:%d \n",__func__, __LINE__, log); exit(1);}



#endif