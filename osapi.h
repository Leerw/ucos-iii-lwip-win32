#ifndef __OS_API_H
#define __OS_API_H

#include "os.h"
#include "app_cfg.h"

#define OSAPISemNew             OSSemCreate
#define OSAPISemWait            OSSemPend
#define OSAPISemSignal          OSSemPost

#define OSAPIMboxNew            OSQCreate
#define OSAPIMboxFree           OSQDel
#define OSAPIMboxTryPost        OSQPost
#define OSAPIMboxFetch          OSQPend

#define OSAPISysThreadNew       OSTaskCreate

#endif // !__OS_API_H

