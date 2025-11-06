#ifndef __D_UTIL_H__
#define __D_UTIL_H__

#include <stdlib.h>

#define ASSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

#endif // !__D_UTIL_H__
