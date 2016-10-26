#ifndef _PRINT_MACROS_H
#define _PRINT_MACROS_H

#include <iostream>
#include "pin.H"

extern PIN_LOCK lock;

#define ERROR(str) std::cerr << "[ERROR (" << __FILE__ << ":" << __LINE__ << ")]: " << str << std::endl;
#define PRINT_SCN(str) std::cerr << str << std::endl;
#define PRINT(str) std::cout << str << std::endl;

#ifdef DEBUG_BUILD
    #define DEBUG(str) std::cerr << str << std::endl;
    #define DEBUGN(str) std::cerr << str;
#else
    #define DEBUG(str);
    #define DEBUGN(str);
#endif

#ifdef LOG_APPROX
    #define MT_AX_LOG(threadid, log, str) { PIN_GetLock(&lock, threadid+1); log << str << std::endl; PIN_ReleaseLock(&lock); }
#else
    #define MT_AX_LOG(threadid, log, str) { }
#endif

#ifdef LOG_ASSEM
    #define AX_LOG_ASSEM(log, str) { log << str << std::endl; }
    #define AX_LOGN_ASSEM(log, str) { log << str; }
#else
    #define AX_LOG_ASSEM(log, str);
    #define AX_LOGN_ASSEM(log, str);
#endif

#ifdef LOG_ACCESS
    #define AX_LOG_ACCESS(log, str) { log << str << std::endl; }
    #define AX_LOGN_ACCESS(log, str) { log << str; }
#else
    #define AX_LOG_ACCESS(log, str);
    #define AX_LOGN_ACCESS(log, str);
#endif

#endif /*_PRINT_MACROS_H*/
