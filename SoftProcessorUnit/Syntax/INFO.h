//
//  INFO.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 19.10.2020.
//

#include "INFODefs.h"

// CMD_VALUETYPE 0 - non-assignable value
// CMD_VALUETYPE 1 - assignable value

COMMAND_INFO(
             push,
             CMD_CODE 0,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 1
             )

COMMAND_INFO(
             pop,
             CMD_CODE 1,
             CMD_ARGRULES "?",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             in,
             CMD_CODE 2,
             CMD_ARGRULES "?",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             dump,
             CMD_CODE 3,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             clear,
             CMD_CODE 4,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             add,
             CMD_CODE 5,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             sub,
             CMD_CODE 6,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             mul,
             CMD_CODE 7,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             div,
             CMD_CODE 8,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             sin,
             CMD_CODE 9,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             cos,
             CMD_CODE 10,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             sqrt,
             CMD_CODE 11,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             pow,
             CMD_CODE 12,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             hlt,
             CMD_CODE 13,
             NOARGS,
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             out,
             CMD_CODE 14,
             CMD_ARGRULES "?",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 1
             )

COMMAND_INFO(
             jmp,
             CMD_CODE 15,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             jb,
             CMD_CODE 16,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             jbe,
             CMD_CODE 17,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             je,
             CMD_CODE 18,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             jne,
             CMD_CODE 19,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             ja,
             CMD_CODE 20,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             jae,
             CMD_CODE 21,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             jm,
             CMD_CODE 22,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             inc,
             CMD_CODE 23,
             CMD_ARGRULES "?",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             dec,
             CMD_CODE 24,
             CMD_ARGRULES "?",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             call,
             CMD_CODE 25,
             CMD_ARGRULES "*",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             ret,
             CMD_CODE 26,
             CMD_ARGRULES "",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             mov,
             CMD_CODE 27,
             CMD_ARGRULES "**",
             CMD_FLAGBYTES 2,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             rend,
             CMD_CODE 28,
             CMD_ARGRULES "",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             slp,
             CMD_CODE 29,
             CMD_ARGRULES "**",
             CMD_FLAGBYTES 1,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             abs,
             CMD_CODE 30,
             CMD_ARGRULES "",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             pixset,
             CMD_CODE 31,
             CMD_ARGRULES "***",
             CMD_FLAGBYTES 3,
             CMD_VALUETYPE 0
             )

COMMAND_INFO(
             clrscr,
             CMD_CODE 33,
             CMD_ARGRULES "",
             CMD_FLAGBYTES 0,
             CMD_VALUETYPE 0
             )


COMMAND_INFO(
        gr,
CMD_CODE 34,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)

COMMAND_INFO(
        geq,
CMD_CODE 35,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)

COMMAND_INFO(
        le,
CMD_CODE 36,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)

COMMAND_INFO(
        leq,
CMD_CODE 37,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)

COMMAND_INFO(
        neq,
CMD_CODE 38,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)

COMMAND_INFO(
        eq,
CMD_CODE 39,
CMD_ARGRULES "",
CMD_FLAGBYTES 0,
CMD_VALUETYPE 0
)
