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
#define ERROR_INTSIZE               0x008F 
#define ERROR_IO_FILETOOBIG		    0x0090 
#define ERROR_OBJECT_MISSING_PARENT 0x0091 
#define ERROR_OBJECT_SELF_PARENT    0x0092 
#define ERROR_OBJECT_NULL_OBJECT    0x0093 
#define ERROR_OBJECT_NAMETOBIG	    0x0094 
#define ERROR_STRING_INVALIDSIZE    0x0095 
#define ERROR_STRING_DSTTOOSMALL    0x0096	


#ifdef ENGINE_DEBUG
// Log to the standard output an error message associated with the error code given. Does nothing if error code is 0.
#define Engine_log_errorcode(errorcode) internal_Engine_log_errorcode((ecode)errorcode)
void internal_Engine_log_errorcode(ecode errorcode);
#else
// Build configuration has debug disabled. All calls to this function will be removed. 
#define Engine_log_errorcode(errorcode) do {} while(0)
#endif
