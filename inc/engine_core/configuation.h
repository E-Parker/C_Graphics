#pragma once

// Enables shader debugging and logging error codes with a description of the error.
#define ENGINE_DEBUG

// Prefer to use stdint.h and stdbool.h over engine's definitions.  
//#define ENGINE_USE_STDDEF

// Strings add an extra trailing null character, only needed by printf to be displayed properly. 
#define STRING_USE_CSTR_REDUNDANCY

