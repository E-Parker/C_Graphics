#pragma once
// include for forward declarations and common definitions used throughout the engine.

// TODO: FIX THIS MORONIC BS! Fucking horrible idea to include this here but it should fix issues with missing malloc and free definitions.
#include "stdlib.h"
#include "engine_core/configuation.h"

// Using standard int:
#ifdef ENGINE_USE_STDDEF
#include "stdint.h"
#include "stdbool.h"
typedef uint8_t				i8;
typedef int16_t				ecode;
typedef int16_t				i16;
typedef int32_t				i32;
typedef int64_t				i64;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef uint64_t			u64;
typedef uint8_t				u8;
#else

#define bool _Bool
#define true 1
#define false 0

// Windows 32 but or 64 bit:
#ifdef _WIN32
typedef signed char			i8;
typedef short				ecode;
typedef short				i16;
typedef long				i32;
typedef long long			i64;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;
#else

// Linux / Unix / Mac / BSD
typedef signed char			i8;
typedef short				ecode;
typedef short				i16;
typedef int				    i32;
typedef long 			    i64;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long 	    u64;
#endif

#endif

typedef struct List List;
typedef struct HashTable HashTable;
typedef struct Object Object;

extern const u8 Object_TypeNone;
extern const u8 Object_TypeCamera;
extern const u8 Object_TypeDrawable;	// All objects which have a draw function have type greater than this.
extern const u8 Object_TypeStaticMesh;
extern const u8 Object_TypeSkinnedMesh;
extern const u8 Object_TypeText;

typedef ecode	(*Function_Errorcode_NoParam)	();
typedef ecode	(*Function_Tick)				(void*, const double);
typedef i32		(*Function_i32_NoParam)			();
typedef i64		(*Function_i64_NoParam)			();
typedef u32		(*Function_u32_NoParam)			();
typedef u64		(*Function_u64_NoParam)			();
typedef void*	(*Function_VoidPtr_NoParam)		();
typedef void	(*Function_Void_NoParam)		();
typedef void	(*Function_Void_OneParam)		(void*);
typedef void	(*Function_Void_TwoParam)		(void*, void*);
typedef void	(*Function_Void_ThreeParam)		(void*, void*, void*);
typedef void	(*Function_Void_FourParam)		(void*, void*, void*, void*);

#ifdef USE_MEMCPY
#define Engine_copy(dst, src, size) memcpy(dst, str, size)
#else
#define Engine_copy(dst, src, size) for (u64 enigne_copy_iterator = 0; enigne_copy_iterator < size; ++enigne_copy_iterator) { (u8*)(dst)[enigne_copy_iterator] = (u8*)(src)[enigne_copy_iterator]; } 
#endif

#ifdef ENGINE_LOG
#include "stdio.h"
#define Engine_log(...) printf(__VA_ARGS__)
#else
#define Engine_log(...) 
#endif


#ifdef ENGINE_DEBUG
// Similar to assert(value) function. Forces the engine to exit with an error code. 
#define Engine_validate(check, errorcode) internal_Engine_validate((!(check))? true : false, errorcode);
extern void internal_Engine_validate(bool check, ecode errorcode);
#else
// Similar to assert(value) function. Forces the engine to exit with an error code. 
// Build configuration has debug disabled. See engine_error.h for engine specific errors.
#define Engine_validate(check, errorcode) do { if (!(check)) { Engine_exit_forced(errorcode); } } while(0)
#endif

// Attempt graceful shutdown of the engine.
extern void Engine_exit(ecode errorcode);

// Forcefully exit the program. use in cases where memory allocation fails, or where a safe exit isn't possible.
extern void Engine_exit_forced(ecode errorcode);
