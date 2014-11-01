#pragma once
#ifdef USE_BDW_GC
#include <gc.h>
#else
#include <string.h>
#include <stdlib.h>
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_REALLOC realloc
#define GC_FREE free
#define GC_strdup strdup
#define GC
#endif


