#pragma once

#include "engine_core/list.h"

typedef struct SparseList {
	List list;
	List inactiveIndicies;
} SparseList;