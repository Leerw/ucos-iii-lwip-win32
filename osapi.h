#ifndef __OS_API_H
#define __OS_API_H

#include "os.h"

#define OSAPISemNew				OSSemCreate
#define OSAPISemWait            OSSemPend
#define OSAPISemSend            OSSemPost

#define OSAPIMboxNew            OSQCreate
#define OSAPIMboxFree           OSQDel
#define OSAPIMboxPost           OSQPost

#endif // !__OS_API_H

