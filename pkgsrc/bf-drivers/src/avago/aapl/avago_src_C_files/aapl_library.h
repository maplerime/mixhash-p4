/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/* Internal library header file for AAPL (ASIC and ASSP Programming Layer). */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for library functions. */

#ifndef AAPL_LIBRARY_H_
#define AAPL_LIBRARY_H_

/* Typedefs: */
/* */
#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#if ! HAVE_UINT
typedef unsigned int uint;
#endif

/* Define type for eye gather counts: */
#if defined __STDC_VERSION && __STDC_VERSION__ > 199409L
  typedef long long bigint;
#elif defined __MINGW_EXTENSION
  typedef __int64 bigint;
#elif defined _STDINT_H
  typedef int64_t bigint;
#else
  typedef long long bigint;
#endif

#if defined X86_64
typedef bigint   fn_cast_t;
#else
typedef long int fn_cast_t;
#endif


/* Determine number of elements in a static array: */
#define AAPL_ARRAY_LENGTH(a)   (int)(sizeof(a) / sizeof((a)[0]))
#define AAPL_BITS_SET(var,width,shift,value) var = ((var & ~(((1<<width)-1)<<(shift))) | ((value)<<(shift)))
#define AAPL_BITS_GET(var,width,shift) ((var >> (shift)) & ((1<<width)-1))

/* Specify how to declare a 64 bit constant: */
#ifdef __INT64_C
#   define AAPL_CONST_INT64(x) __INT64_C(x)
#elif defined(__GNUC__) && !defined(__STDC_VERSION__)
    /* Required by older gcc compiler versions. */
#   define AAPL_CONST_INT64(x) ((bigint)x)
#elif defined(__GNUC__) && defined(WIN32)
    /* Required by older gcc compiler versions. */
#   define AAPL_CONST_INT64(x) (x ## LL)
#elif defined(WIN32)
    /* Needed by older Microsoft C compilers: */
#   define AAPL_CONST_INT64(x) (x ## i64)
#else
    /* Works with all new, standard compliant compilers? */
#   define AAPL_CONST_INT64(x) (x)
#endif

#ifdef AAPL_ENABLE_INTERNAL_FUNCTIONS

EXT void  ms_sleep(uint msec);   /* Sleep milliseconds */

/* Converts data to a 32 character ASCII encoded binary str with optional underscores every 8 bits */
EXT char *aapl_hex_2_bin(char *str, uint data, int underscore_en, int bits);

/* Portability utility functions: */
EXT int   aapl_strcasecmp(const char *s1, const char *s2);
EXT int   aapl_strncasecmp(const char *s1, const char *s2, size_t len);
EXT char *aapl_strdup(const char *string);
EXT void  aapl_str_rep(char *str, char search, char replace);
EXT char *aapl_strtok_r(char *str, const char *delim, char **saveptr);
EXT size_t aapl_local_strftime(char *buf, size_t max, const char *format);

#endif /* AAPL_ENABLE_INTERNAL_FUNCTIONS */

#endif /* AAPL_LIBRARY_H_ */

