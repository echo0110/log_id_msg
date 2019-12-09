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


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdarg.h>
using namespace std;
void print_msg(const char *format, ...)
{
    char buf[1024];
    va_list my_format;
    va_start(my_format, format);
    char c;
    char * pstr;
    while ((c=*format++)!=NULL)
    {
        switch (c)
        {
           case '%':
           c = *format++;
           switch (c)
           {
              case 'd':
              _itoa(va_arg(my_format, int), buf, 10);
              fputs(buf, stdout);
              break;
              case 'x':
              _itoa(va_arg(my_format, int), buf, 16);
              fputs(buf, stdout);
              break;
              case 's':
              pstr =va_arg(my_format, char*);
              fputs(pstr, stdout);
              break;
              case 'c':

            putchar(va_arg(my_format, char));
              break;
}
         break;
         default:
         putchar(c);
       }
    }
}

#if 1
int main()
{
	//myprint("这是整数%d,这是字符串%s,这是16进制%x,这是字符%c",100,"hello world",100,'a');
	print_msg("log id is %d, log level is %d\n", id, level, int);
	system("pause");
	return 0;
}
#endif
