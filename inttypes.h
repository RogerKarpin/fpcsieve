// This file is only included in Windows solutions
#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <stdint.h>

#define strtoull(X,Y,Z) _strtoui64(X, Y, Z)
#define atoll  _atoi64

#define PRIu64 "I64u"
#define PRId64 "I64d"
#define PRIx64 "I64x"
#define SCNu64 "I64u"
#define SCNd64 "I64d"
#define SCNx64 "I64x"

#endif
