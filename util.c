/* util.c -- (C) Geoffrey Reynolds, March 2009.


   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
*/

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"


void *xmalloc(size_t size)
{
  void *ret;

  if ((ret = malloc(size)) == NULL)
  {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return ret;
}

void *xrealloc(void *mem, size_t size)
{
  void *ret;

  if ((ret = realloc(mem,size)) == NULL)
  {
    perror("realloc");
    exit(EXIT_FAILURE);
  }

  return ret;
}

char *xstrdup(const char *str)
{
  if (str == NULL)
    return NULL;
  else
    return strcpy(xmalloc(strlen(str)+1),str);
}

/* Returns 0 if successful, -1 if cannot parse, -2 if out of range.
 */
int parse_uint(unsigned int *result, const char *str,
               unsigned int lo, unsigned int hi)
{
  uint64_t result64;
  int status;

  status = parse_uint64(&result64,str,lo,hi);

  if (status == 0)
    *result = result64;

  return status;
}

/* Returns 0 if successful, -1 if cannot parse, -2 if out of range.
 */
int parse_uint64(uint64_t *result, const char *str,
                 uint64_t lo, uint64_t hi)
{
  uint64_t num;
  unsigned int expt;
  char *tail;

  expt = 0;
  errno = 0;
  num = strtoull(str,&tail,0);

  if (errno != 0 || num > hi)
    return -2;

  switch (*tail)
  {
    case 'P': expt += 3;
    case 'T': expt += 3;
    case 'G': expt += 3;
    case 'M': expt += 3;
    case 'K': expt += 3;
      if (tail[1] != '\0')
        return -1;
      for ( ; expt > 0; expt -= 3)
        if (num > hi/1000)
          return -2;
        else
          num *= 1000;
      break;

    case 'e':
    case 'E':
      expt = strtoul(tail+1,&tail,0);
      if (errno != 0)
        return -2;
      if (*tail != '\0')
        return -1;
      while (expt-- > 0)
        if (num > hi/10)
          return -2;
        else
          num *= 10;
      break;

    case 'p': expt += 10;
    case 't': expt += 10;
    case 'g': expt += 10;
    case 'm': expt += 10;
    case 'k': expt += 10;
      if (tail[1] != '\0')
        return -1;
      if (num > (hi>>expt))
        return -2;
      num <<= expt;
      break;

    case 'b':
    case 'B':
      expt = strtoul(tail+1,&tail,0);
      if (errno != 0)
        return -2;
      if (*tail != '\0')
        return -1;
      while (expt-- > 0)
        if (num > (hi>>1))
          return -2;
        else
          num <<= 1;
      break;

    case '\0':
      break;

    default:
      return -1;
  }

  if (num < lo)
    return -2;

  *result = num;
  return 0;
}
