//
//  EXEDefs.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 19.10.2020.
//

#ifndef EXEDefs_h
#define EXEDefs_h

#include <cstdio>

#include "InstrExecutors.hpp"
#include "SPUDtypes.hpp"
#include "SPUCore.hpp"
#include "SPUCoreHelpers.hpp"
#include "ParseHelpers.hpp"

#define HASBYTES(bytes) ( ( bytes ) + *SPI <= binary->currentSize + binary->code)

#define STACKRESULT switch (result) {\
case STACK_OP_OK:\
    break;\
case STACK_OP_UNDERFLOW:\
    return SPU_EXE_STACK_UNDERFLOW;\
    break;\
case STACK_OP_OVERFLOW:\
    return SPU_EXE_STACK_OVERFLOW;\
    break;\
case STACK_OP_NOMEMORY:\
    return SPU_EXE_NOMEMORY;\
    break;\
case STACK_OP_INTEGRITYERR:\
    return SPU_EXE_CORRUPTED;\
    break;\
case STACK_OP_NULL:\
    return SPU_EXE_CORRUPTED;\
    break;\
case STACK_OP_PTRINVALID:\
    return SPU_EXE_CORRUPTED;\
    break;\
}
#ifdef ADDSPI
#undef ADDSPI
#endif
#define ADDSPI(n) (*SPI) += (n)

#ifdef MOVSPI
#undef MOVSPI
#endif
#define MOVSPI(n) (*SPI) = (n)
#define INCSPI (*SPI)++

#define PUSH(data) StackPush(&(core->stack), (data))
#define callPUSH(data) StackPush(&(core->callStack), (data))

#define POP(data) StackPop(&(core->stack), (data))
#define callPOP(data) StackPop(&(core->callStack), (data))

#undef COMPLEXVALOK
#define COMPLEXVALOK switch(val) { \
case SPU_CV_WRONGSTRUCT: \
return SPU_EXE_CORRUPTED;\
case SPU_CV_WRONGREG: \
return SPU_EXE_CORRUPTED; \
case SPU_CV_WRONGNUM: \
return SPU_EXE_CORRUPTED; \
case SPU_CV_WRONGOP: \
return SPU_EXE_CORRUPTED; \
case SPU_CV_NOARG:\
return SPU_EXE_CORRUPTED; \
case SPU_CV_OK: \
break;\
} \


#endif /* EXEDefs_h */
