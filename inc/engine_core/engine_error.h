#pragma once

#include "engine_core/configuation.h"
#include "engine_core/engine_types.h"
#include "errno.h"

// All error codes are 16 bit, signed integers. errors are an extension of the errno.h header.
// Engine errors start at 141.
// Generic failure is still -1.

#define ERROR_GENERIC				-1
#define ERROR_BADPOINTER			0x008D
#define ERROR_BADVALUE				0x008E
#define ERROR_IO_FILETOBIG			0x008F
#define ERROR_OBJECT_MISSING_PARENT 0x0090
#define ERROR_OBJECT_SELF_PARENT    0x0091
#define ERROR_OBJECT_NULL_OBJECT    0x0092


#ifdef ENGINE_DEBUG
#define Engine_log_errorcode(errorcode) internal_Engine_log_errorcode((ecode)errorcode)
void internal_Engine_log_errorcode(ecode errorcode);
#else
#define Engine_log_errorcode(errorcode) do {} while(0)
#endif