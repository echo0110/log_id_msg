/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: logging.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of logging framework
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: logging.c,v $
 * Initial version
 *
 ****************************************************************************/


#include <stdio.h> 
#include <stdlib.h> 

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


void fun(const char* fmt,...)
{
    va_list ap;
    va_start(ap, fmt);//将ap指向fmt后的第一个参数
    int value = va_arg(ap,int);//前提是我们知道第一个参数是int型；指针指向下一个参数 
    printf("value[%d]\n",value);
    va_end(ap);//将ap置为NULL
}

int main()
{
	fun("%d",1);
	system("pause");
	return 0;
}


