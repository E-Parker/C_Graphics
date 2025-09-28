#pragma once
// include for forward declarations and common definitions used throughout the engine.

// TODO: FIX THIS MORONIC BS! Fucking horrible idea to include this here but it should fix issues with missing malloc and free definitions.
#include "stdlib.h"

#ifdef ENGINE_USE_STDDEF
#include "stdint.h"
#include "stdbool.h"
typedef uint64_t			u64;
typedef uint32_t			u32;
typedef uint16_t			u16;
typedef uint8_t				u8;
typedef int64_t				i64;
typedef int32_t				i32;
typedef int16_t				i16;
typedef uint8_t				i8;
#else
typedef signed char			i8;
typedef short				i16;
typedef int					i32;
typedef long int			i64;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long int	u64;
#endif

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

typedef u8		(*Function_Tick)			(void*, const double);
typedef void*	(*Function_VoidPtr_NoParam) ();
typedef void	(*Function_Void_NoParam)	();
typedef void	(*Function_Void_OneParam)	(void*);
typedef void	(*Function_Void_TwoParam)	(void*, void*);
typedef void	(*Function_Void_ThreeParam) (void*, void*, void*);
typedef void	(*Function_Void_FourParam)  (void*, void*, void*, void*);

