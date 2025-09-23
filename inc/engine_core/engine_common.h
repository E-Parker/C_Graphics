#pragma once
// include for forward declarations used in the engine.

#include "stdbool.h"
#include "stdint.h"

typedef struct List List;
typedef struct HashTable HashTable;

typedef struct Object Object;

typedef uint8_t (*Function_Tick)			(void*, const double);
typedef void*	(*Function_VoidPtr_NoParam) ();
typedef void	(*Function_Void_NoParam)	();
typedef void	(*Function_Void_OneParam)	(void*);
typedef void	(*Function_Void_TwoParam)	(void*, void*);
typedef void	(*Function_Void_ThreeParam) (void*, void*, void*);
