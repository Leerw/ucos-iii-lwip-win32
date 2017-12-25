#ifndef __OS_API_H
#define __OS_API_H

#include "os.h"
#include "app_cfg.h"

#define OSAPISemNew             OSSemCreate
#define OSAPISemWait            OSSemPend
#define OSAPISemSend            OSSemPost

#define OSAPIMboxNew            OSQCreate
#define OSAPIMboxFree           OSQDel
#define OSAPIMboxTryPost        OSQPost
#define OSAPIMboxFetch          OSQPend

#endif // !__OS_API_H

