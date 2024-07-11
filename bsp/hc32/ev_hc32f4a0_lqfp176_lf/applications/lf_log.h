#define LF_DEBUG

#ifndef LOG_TAG
#define DBG_TAG "lf"
#else
#define DBG_TAG LOG_TAG
#endif /* LOG_TAG */

#ifdef LF_DEBUG
#define DBG_LVL DBG_LOG
#else
#define DBG_LVL DBG_INFO
#endif /* LF_DEBUG */
#include "ulog.h"
