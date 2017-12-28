#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <os.h>
#include <vld.h>

#define LWIP_STK_SIZE      512

#define LWIP_TASK_MAX    5	//max number of lwip tasks
#define LWIP_START_PRIO  7   //first prio of lwip tasks

/*********************************************************
*                modified by leerw
*                 date: 2017-12-25
**********************************************************/
#define HANDLE OS_SEM
/*********************************************************/
typedef HANDLE* sys_sem_t;
/*********************************************************
typedef struct queue *sys_mbox_t;
**********************************************************/
/*********************************************************
*                modified by leerw
*                 date: 2017-12-25
**********************************************************/
typedef OS_Q*  sys_mbox_t;
/*********************************************************
typedef HANDLE sys_thread_t;
**********************************************************/
/**********************************************************************************
*                          about this sys_thread_t                                *
*                           in the sys_arch.txt                                   *
* Starts a new thread named "name" with priority "prio" that will begin its       *
* execution in the function "thread()". The "arg" argument will be passed as an   *
* argument to the thread() function. The stack size to used for this thread is    *
* the "stacksize" parameter. The id of the new thread is returned. Both the id    *
* and the priority are system dependent.                                          *
*                 I defined the sys_thread_t as the pointer to OS_TCB             *
*                            annotated by leerw                                   *
*                             date: 2017-12-26                                    *
***********************************************************************************
*/
typedef OS_TCB* sys_thread_t;


#define SYS_MBOX_NULL   (sys_mbox_t)0
#define SYS_SEM_NULL    (sys_sem_t)0

/* Global Critical Region Protection */
CRITICAL_SECTION gCriticalSection;

#define SYS_ARCH_DECL_PROTECT(x)
#define SYS_ARCH_PROTECT(x) EnterCriticalSection(&gCriticalSection)
#define SYS_ARCH_UNPROTECT(x) LeaveCriticalSection(&gCriticalSection)

#endif
