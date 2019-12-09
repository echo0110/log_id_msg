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


#include <string.h>


//void LOG_ID_MSG(const char *fmt, ...)

void LOG_ID_MSG(long tick, char *func, char *fmt, ...) //后面的char *fmt  全是%d 按顺序给存下来就行
{
    char buf[1024];
	va_list ap;              //定义一个可变 参数的（字符指针）
    va_start(ap, fmt);
    char c;
    char * pstr,*logstr,*pos;
	char log[50]={0};
	int i=0,j=0,val = 0;
	logstr=log;
	int log_array[20];

#ifdef LINUX_PC_TEST
	test_log(10,2,"print_trace %s -- %s",strValOne,strValTwo);
	return ;
#endif

	printf("###strValOne=%s\n",strValOne);
	printf("###strValTwo=%s\n",strValTwo);


	LTE_GCC_UNUSED_PARAM(globalTick);	 
	LTE_GCC_UNUSED_PARAM(floatValOne);
	LTE_GCC_UNUSED_PARAM(floatValTwo);
	UInt32 size = 0;
	log_info_t *p_shm_seg = NULL;

	UInt32 write_offset;
	//LogParamValues *pParamValues; 

	Log_id_ParamValues *pParamValues;
	LogShmHeader *pLogHdr;
		
#ifdef __GNUC__
	p_shm_seg = TLS_Key;
#else
	p_shm_seg = pthread_getspecific(TLS_Key);
#endif
	
	
    while ((c=*fmt++)!=NULL)  
    {
        switch (c)
        {
           case '%':
           c = *fmt++;
           switch (c)
           {
              i++;
              case 'd':
			  {
			    log_array[i]=va_arg(ap,int);				  
			  }
              break;
              case 'x':
              break;
              case 's':
			  strncpy(pParamValues->params.strValues[0], va_arg(ap,char*), MAX_LOG_STR_LEN);
			  pParamValues->params.strValues[0][MAX_LOG_STR_LEN - 1] = '\0';	
              break;
			  case 'f':
			  if(search(log, "floatValOne")!=NULL)
			  {
			    floatValOne=va_arg(ap,int);				  
			  }
			  if(search(log, "floatValTwo")!=NULL)
			  {
			    floatValTwo=va_arg(ap,int);				  
			  }
			  logstr=log;
			  memset(log,0,50);
			  
              break;
              case 'c':
			  
              break;
			}
         break;
         default:
		 *logstr++=c;
       }
    }
	
	pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

	printf("log_id_msg_trace\n");
	if (pLogHdr->writeOffset + sizeof(Log_id_ParamValues) == pLogHdr->readOffset)
	{
		pLogHdr->lostLogCount++;
		return;
	}
	else if (pLogHdr->lostLogCount != 0)
	{
		/* write lost log count */
		pParamValues = (Log_id_ParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset + size);
	    pParamValues->logId = L2_LOG_LOST;
		pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
		pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
		pParamValues->globalTick = logMetaData.currentTick;
		pParamValues->intValues[0] = pLogHdr->lostLogCount;
		strncpy(pParamValues->params.strValues[0], __func__, MAX_LOG_STR_LEN);
		if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(Log_id_ParamValues))) < MAX_LOG_LENGTH)
		{
			pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
			pLogHdr->rotatedFlag = 1;
		}
		else
		{
			pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(Log_id_ParamValues);;
		}

		/* set lost log count to 0 */
		pLogHdr->lostLogCount = 0;

		/* again check if we can write the log */
		if (pLogHdr->writeOffset + sizeof(Log_id_ParamValues) == pLogHdr->readOffset)
		{
			pLogHdr->lostLogCount++;
			return;
		}
	}
	/*Initialize ptr to Optional arguments list in variadic function*/
	write_offset = pLogHdr->writeOffset;

	pParamValues = (Log_id_ParamValues *)(p_shm_seg->p_shmptr + write_offset + size);

	pParamValues->replaceid=log_array[0];
	pParamValues->logId = log_array[1];
	
	strNCpy(&pParamValues->tm, &logMetaData.tm, sizeof(logMetaData.tm));
	/*pParamValues->logLevel = logLevel;
	  pParamValues->logCategory = logCategory;*/
	pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
	pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
	pParamValues->globalTick = logMetaData.currentTick;
	pParamValues->intValues[0] = log_array[0];
	pParamValues->intValues[1] = log_array[0];
	pParamValues->intValues[2] = log_array[0];
	pParamValues->intValues[3] = log_array[0];
	pParamValues->intValues[4] = log_array[0];
	/*pParamValues->flValues[0]  = floatValOne;
	  pParamValues->flValues[1] = floatValTwo;*/

	size += sizeof(Log_id_ParamValues);

//				strncpy(pParamValues->params.strValues[0], strValOne, MAX_LOG_STR_LEN);
//				pParamValues->params.strValues[0][MAX_LOG_STR_LEN - 1] = '\0';

  


//				  strncpy(pParamValues->params.strValues[1], strValTwo, MAX_LOG_STR_LEN);
//				  pParamValues->params.strValues[1][MAX_LOG_STR_LEN - 1] = '\0';

	if((l2Input.shmSize - (write_offset + size)) < MAX_LOG_LENGTH)
	{
		pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
		pLogHdr->rotatedFlag = 1;
	}
	else
	{
		pLogHdr->writeOffset = write_offset + size;
	}

#ifdef PERSIST_FATAL_LOG
	if (LOGFATAL == logCommValues[logId].logLevel)
	{
		write_critical_log(pParamValues);
	}
#endif
}



#if 1
int main()
{
	// LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
						  // relayGtpuCurrentTick_g, __LINE__, incr,0,0,0,0,0,
						  // __func__, "MemExtendFail");

	//myprint("这是整数%d,这是字符串%s,这是16进制%x,这是字符%c",100,"hello world",100,'a');
	//print_msg("log_id:%d,globalTick:%d,intValOne:%d,floatValOne:%f\n",\
	//1,2,3,4.3);
	// LOG_ID_MSG("replaceid is:%d,logId is:%d,intValOne is:%d,strValOne is %s\n",10,897,1,"niuben");
	// //system("pause");		
	LOG_ID_MSG(10,_func_,"replaceid is:%d,logId is %d, logLev is %d,logCat is %d,\
						intValOne is %d, intValTwo is %d,intValThree is %d, intValFour is %d,\
						intValFive is %d,strValTwo is %s\n",
						  6,897, LOGWARNING, L2_SYS_FAIL, 
						  relayGtpuCurrentTick_g,  __LINE__, p_size->curr_size,
						  0,0,0,0,0,__func__,"MemFailed");
	return 0;
}
#endif
