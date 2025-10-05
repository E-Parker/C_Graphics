#pragma once
// include for forward declarations and common definitions used throughout the engine.

// TODO: FIX THIS MORONIC BS! Fucking horrible idea to include this here but it should fix issues with missing malloc and free definitions.
#include "stdlib.h"
#include "engine_core/configuation.h"

#ifdef ENGINE_USE_STDDEF
#include "stdint.h"
#include "stdbool.h"
typedef uint8_t				i8;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef uint64_t			u64;
typedef uint8_t				u8;
typedef int16_t				i16;
typedef int32_t				i32;
typedef int64_t				i64;
#else
typedef signed char			i8;
typedef short				i16;
typedef long				i32;
typedef long long			i64;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;
#endif

typedef i16 ecode;

#ifndef _STDBOOL
#define bool _Bool
#define true 1
#define false 0
#define _STDBOOL
#endif

typedef struct List List;
typedef struct HashTable HashTable;
typedef struct Object Object;

extern const u8 Object_TypeNone;
extern const u8 Object_TypeStaticMesh;
extern const u8 Object_TypeSkinnedMesh;
extern const u8 Object_TypeText;
extern const u8 Object_TypeCamera;

typedef ecode	(*Function_Errorcode_NoParam)	();
typedef u8		(*Function_Tick)				(void*, const double);
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

// Similar to assert(value) function. Forces the engine to exit with an error code. 
// See errno.h for generic codes, and engine_error.h for engine specific errors.
#define Engine_validate(check, errorcode) internal_Engine_validate((!(check))? true : false, errorcode);
extern void internal_Engine_validate(bool check, ecode errorcode);

// Attempt graceful shutdown of the engine.
extern void Engine_exit(ecode errorcode);

// Forcefully exit the program. use in cases where memory allocation fails, or where a safe exit isn't possible.
extern void Engine_exit_forced(ecode errorcode);