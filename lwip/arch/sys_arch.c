#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <stdio.h>

#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"

#include "arch/sys_arch.h"
#include "arch/queue.h"

#include "osapi.h"

static struct sys_timeouts lwip_timeouts[LWIP_TASK_MAX + 1];
CPU_STK T_LWIP_THREAD_STK[LWIP_TASK_MAX][LWIP_STK_SIZE];
OS_SEM sempool[100];
CPU_CHAR* name[100] = { "sem1",  "sem2",  "sem3",  "sem4",  "sem5",  "sem6",  "sem7",  "sem8",  "sem9",  "sem10",
                        "sem11", "sem12", "sem13", "sem14", "sem15", "sem16", "sem17", "sem18", "sem19", "sem20" ,
                        "sem21", "sem22", "sem23", "sem24", "sem25", "sem26", "sem27", "sem28", "sem29", "sem30" ,
                        "sem31", "sem32", "sem33", "sem34", "sem35", "sem36", "sem37", "sem38", "sem39", "sem40" ,
                        "sem41", "sem42", "sem43", "sem44", "sem45", "sem46", "sem47", "sem48", "sem49", "sem50" ,
                        "sem51", "sem52", "sem53", "sem54", "sem55", "sem56", "sem57", "sem58", "sem59", "sem60" ,
                        "sem61", "sem62", "sem63", "sem64", "sem65", "sem66", "sem67", "sem68", "sem69", "sem70" ,
                        "sem71", "sem72", "sem73", "sem74", "sem75", "sem76", "sem77", "sem78", "sem79", "sem80" ,
                        "sem81", "sem82", "sem83", "sem84", "sem85", "sem86", "sem87", "sem88", "sem89", "sem90" ,
                        "sem91", "sem92", "sem93", "sem94", "sem95", "sem96", "sem97", "sem98", "sem99", "sem100" };
static CPU_CHAR sem_ptr = 0;

/*
***********************************************************************************************************
*                                    as the create a thread function                                      *
* sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio) *
* has no argument sys_thread_t, so we cannot return a sys_thread_t in the function because if we create   *
* a temporary variable thread with type sys_thread_t, we cannoot return a sys_thread_t pointer outside    *
*                    So I decided to create a thread pool to avoid this problem                           *
*                                          date: 2017-12-27                                               *
*                                             by: leerw                                                   *
***********************************************************************************************************
*/
OS_TCB lwip_tcbs[100];
CPU_CHAR* name[100] = 
{
  "lwip_tcb1",  "lwip_tcb2",  "lwip_tcb3",  "lwip_tcb4",  "lwip_tcb5",  "lwip_tcb6",  "lwip_tcb7",  "lwip_tcb8",  "lwip_tcb9",  "lwip_tcb10",
  "lwip_tcb11", "lwip_tcb12", "lwip_tcb13", "lwip_tcb14", "lwip_tcb15", "lwip_tcb16", "lwip_tcb17", "lwip_tcb18", "lwip_tcb19", "lwip_tcb20",
  "lwip_tcb21", "lwip_tcb22", "lwip_tcb23", "lwip_tcb24", "lwip_tcb25", "lwip_tcb26", "lwip_tcb27", "lwip_tcb28", "lwip_tcb29", "lwip_tcb30",
  "lwip_tcb31", "lwip_tcb32", "lwip_tcb33", "lwip_tcb34", "lwip_tcb35", "lwip_tcb36", "lwip_tcb37", "lwip_tcb38", "lwip_tcb39", "lwip_tcb40",
  "lwip_tcb41", "lwip_tcb42", "lwip_tcb43", "lwip_tcb44", "lwip_tcb45", "lwip_tcb46", "lwip_tcb47", "lwip_tcb48", "lwip_tcb49", "lwip_tcb50",
  "lwip_tcb51", "lwip_tcb52", "lwip_tcb53", "lwip_tcb54", "lwip_tcb55", "lwip_tcb56", "lwip_tcb57", "lwip_tcb58", "lwip_tcb59", "lwip_tcb60",
  "lwip_tcb61", "lwip_tcb62", "lwip_tcb63", "lwip_tcb64", "lwip_tcb65", "lwip_tcb66", "lwip_tcb67", "lwip_tcb68", "lwip_tcb69", "lwip_tcb70",
  "lwip_tcb71", "lwip_tcb72", "lwip_tcb73", "lwip_tcb74", "lwip_tcb75", "lwip_tcb76", "lwip_tcb77", "lwip_tcb78", "lwip_tcb79", "lwip_tcb80",
  "lwip_tcb81", "lwip_tcb82", "lwip_tcb83", "lwip_tcb84", "lwip_tcb85", "lwip_tcb86", "lwip_tcb87", "lwip_tcb88", "lwip_tcb89", "lwip_tcb90",
  "lwip_tcb91", "lwip_tcb92", "lwip_tcb93", "lwip_tcb94", "lwip_tcb95", "lwip_tcb96", "lwip_tcb97", "lwip_tcb98", "lwip_tcb99", "lwip_tcb100",
};
static CPU_CHAR tcb_ptr = 0;

void sys_init(void)
{
  u8_t i;
  for(i = 0; i < sizeof(lwip_timeouts) / sizeof(struct sys_timeouts); i++)
    lwip_timeouts[i].next = NULL;
}

/*----------------------------------------------------------------------*/
sys_sem_t sys_sem_new(u8_t count)
{
  /**********************************************************************
  sys_sem_t sem = CreateSemaphore(NULL, count, MAXLONG, NULL);
  ***********************************************************************/
  /********************************************************************
  if (sem == INVALID_HANDLE_VALUE) 
  {
    fprintf(stderr, "CreateSemaphore error: %d\n", GetLastError());
    return SYS_SEM_NULL;
  }
  return sem;
  *********************************************************************/

  /*********************************************************************
  *							modified sys_sem_new
  *                           to adapt ucos-iii
  *                           date: 2017-12-25
  *                               by: leerw
  **********************************************************************/
  

  OS_ERR err;
  CPU_CHAR* sem_name = name[sem_ptr];
  OSAPISemNew(&(sempool[sem_ptr]), sem_name, (OS_SEM_CTR) count, &err);
  if (err != OS_ERR_NONE)
  {
	  fprintf(stderr, "CreateSemaphore error: %d\n", err);
	  return SYS_SEM_NULL;
  }
  return &(sempool[sem_ptr++]);
}

/*----------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t sem)
{
  /*********************************************************************
  CloseHandle(sem);
  **********************************************************************/
  /*********************************************************************
  *							modified sys_sem_free
  *                           to adapt ucos-iii
  *                           date: 2017-12-25
  *                              by: leerw
  **********************************************************************/
 
  
  OS_ERR err;
  OS_ERR time_err;
  while (0 != OSSemDel(sem, OS_OPT_DEL_ALWAYS, &err))
  {
	  OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_DLY, &time_err);
  }
  
}

/*----------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t sem)
{
  /*********************************************************************
  if (!ReleaseSemaphore(sem, 1, NULL)) 
  {
    fprintf(stderr, "ReleaseSemaphore error: %d\n", GetLastError());
  }
  **********************************************************************/
  /*********************************************************************
  *							modified sys_sem_signal
  *                           to adapt ucos-iii
  *                           date: 2017-12-25
  *                               by: leerw
  **********************************************************************/

  
  OS_ERR err;
  OSAPISemSignal(sem, OS_OPT_POST_1, &err);
  if (err != OS_ERR_NONE)
  {
	  fprintf(stderr, "ReleaseSemaphore error: %d\n", err);
	  printf("signal filed!\n");
  }

  
}


/*----------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
  /**********************************************************************
  DWORD dwWaitResult = WaitForSingleObject(sem, (timeout != 0) ? timeout : INFINITE);
  switch (dwWaitResult) 
  {
  case WAIT_OBJECT_0: 
    return ERR_OK;
  case WAIT_ABANDONED:
  case WAIT_TIMEOUT: 
  default:
    return SYS_ARCH_TIMEOUT;
  }
  ***********************************************************************/


  /**********************************************************************
  *                        modified sys_arch_sem_wait
  *						    with opt none blocking
  *                               by: leerw
  *                           date: 2017-12-25
  ***********************************************************************/

  
  OS_ERR err;
  OSAPISemWait(sem, timeout, OS_OPT_PEND_NON_BLOCKING, (CPU_TS *)0, &err);
  switch (err)
  {
	case OS_ERR_NONE:
	  return ERR_OK;
	default:
	  return SYS_ARCH_TIMEOUT;
  }
  
  
  
  
}


/*----------------------------------------------------------------------*/
sys_mbox_t sys_mbox_new(int size)
{
  /**********************************************************************
  return queue_create();
  ***********************************************************************/

  /**********************************************************************
  *                        modified sys_mbox_new
  *						    with opt none blocking
  *                               by: leerw
  *                           date: 2017-12-25
  ***********************************************************************/
  
  OS_Q p_q;
  CPU_CHAR *name = NULL;
  size = (OS_MSG_QTY)(size);
  OS_ERR err;
  OSAPIMboxNew(&p_q, name, size, &err);
  if (err != OS_ERR_NONE)
  {
	  sprintf(stderr, "sys_mbox was created failed! The error is %d\n", err);
  }
  return &p_q;
  
}

/*----------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t mbox)
{
  /**********************************************************************
  queue_free(mbox);
  ***********************************************************************/
  /**********************************************************************
  *                        modified sys_mbox_free
  *						    with opt none blocking
  *                               by: leerw
  *                           date: 2017-12-25
  ***********************************************************************/

  
  OS_ERR err;
  OSAPIMboxFree(mbox, OS_OPT_DEL_ALWAYS, &err);
  if (err != OS_ERR_NONE)
  {
	  sprintf(stderr, "sys_mbox was freed failed! The error is %d\n", err);
  }
  
}

/*----------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
  sys_mbox_trypost(mbox, msg); 
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg) 
{
  /**********************************************************************
  queue_push(mbox, msg);
  return ERR_OK;
  ***********************************************************************/
  /**********************************************************************
  *                        modified sys_mbox_trypost
  *						    with opt none blocking
  *                               by: leerw
  *                           date: 2017-12-25
  ***********************************************************************/

  
  OS_ERR err;
  OSAPIMboxTryPost(mbox, msg, sizeof msg, OS_OPT_POST_FIFO, &err);
  if (err != OS_ERR_NONE)
  {
	  sprintf(stderr, "msg was tried to post failed! The error is %d\n", err);
	  return err;
  }
  return ERR_OK;
  
}

/*----------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
  /**********************************************************************
  *msg = queue_pop(mbox, timeout);
  if (*msg == NULL)
    return SYS_ARCH_TIMEOUT;
  return 0;
  ***********************************************************************/
  /**********************************************************************
  *                        modified sys_mbox_trypost
  *						    with opt none blocking
  *                               by: leerw
  *                           date: 2017-12-25
  ***********************************************************************/
  
  OS_ERR err;
  *msg = OSAPIMboxFetch(mbox, timeout, OS_OPT_PEND_BLOCKING, sizeof(*msg), (CPU_TS *)0, &err);
  if (err == OS_ERR_TIMEOUT || *msg == (void *)0)
  {
	  return SYS_ARCH_TIMEOUT;
  }
  return 0;
  

}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
  return sys_arch_mbox_fetch(mbox, msg, 0);
}

/*----------------------------------------------------------------------*/
struct sys_timeouts * sys_arch_timeouts(void)
{
  /**********************************************************************
  u8_t prio = GetThreadPriority(GetCurrentThread());
  u8_t offset = prio - LWIP_START_PRIO;
  if (prio == THREAD_PRIORITY_ERROR_RETURN) 
  {
    fprintf(stderr, "CreateThread failed with %d.\n", GetLastError()); 
    return &lwip_timeouts[LWIP_TASK_MAX];
  }
  if (offset >= 0 && offset < LWIP_TASK_MAX)
    return &lwip_timeouts[offset];
  return &lwip_timeouts[LWIP_TASK_MAX];
  **********************************************************************/

  OS_PRIO prio;
  prio = OSTCBCurPtr->Prio - LWIP_START_PRIO;
  if (prio >= 0 && prio < LWIP_TASK_MAX)
  {
	  return &lwip_timeouts[prio];
  }
  return &lwip_timeouts[LWIP_TASK_MAX];
}


/*------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{ 
  /**********************************************************************
  sys_thread_t t = (sys_thread_t)_beginthreadex( 
    NULL,                            // security attributes
    stacksize,                       // stack size  
    (LPTHREAD_START_ROUTINE)thread,  // thread function name
    arg,                             // argument to thread function 
    0,                               // creation flags 
    NULL);                           // returns the thread identifier 
  
  if (t == NULL) 
  {
    fprintf(stderr, "CreateThread failed with %d.\n", GetLastError()); 
    ExitProcess(3);
  }
  if (!SetThreadPriority(t, prio))
    fprintf(stderr, "SetThreadPriority failed with %d\n", GetLastError());
  
  return t;
  **************************************************************************/

  /**********************************************************************
  *                        modified _beginthreadex
  *						   to ucos-iii OSTaskCreate
  *                               by: leerw
  *                           date: 2017-12-26
  ***********************************************************************/

  OS_TCB t;
  OS_ERR err;
  if (prio > 0 && prio <= LWIP_TASK_MAX)
  {
	  OSAPISysThreadNew(&t,
		  name,
		  thread,
		  arg,
		  LWIP_START_PRIO + (prio - 1),
		  &T_LWIP_THREAD_STK[prio - 1][LWIP_STK_SIZE - 1],
		  0,
		  stacksize + 1,
		  (OS_MSG_QTY)0u,
		  (OS_TICK)0u,
		  (void       *)0,
		  (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
		  (OS_ERR     *)&err);
	  if (err != OS_ERR_NONE)
	  {
		  fprintf(stderr, "CreateThread failed with %d.\n", err);
		  ExitProcess(3);
	  }
  }
  return &t;
  
}
