#pragma once
// This file contains all macro definitions for configuration of the engine. All engine files include this.
//
//

// Enables shader debugging and logging error codes with a description of the error.
//#define ENGINE_DEBUG

// Allow logging logs, warnings, and errors to the standard output.
//#define ENGINE_LOG

// Strings add an extra trailing null character, only needed for logging to be displayed properly. 
//#define USE_CSTR_REDUNDANCY

// Prefer to use stdint.h and stdbool.h over engine's definitions.  
//#define ENGINE_USE_STDDEF

// Prefer to use memcpy over in-lined for loop. 
//#define USE_MEMCPY

