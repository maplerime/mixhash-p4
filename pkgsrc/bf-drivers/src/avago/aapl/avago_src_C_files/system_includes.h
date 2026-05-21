/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */

/* System level header file for AAPL */

/** Doxygen File Header */
/** @file */
/** @brief System level declarations. */

#ifndef AVAGO_SYSTEM_INCLUDES_H_
#define AVAGO_SYSTEM_INCLUDES_H_

#ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200112L
#endif

#ifdef HAVE_CONFIG_H
#   include <config.h>
#else
#   define STDC_HEADERS 1
#endif

#define AAPL_COPYRIGHT "Copyright (c) 2013-2017 Avago Technologies. All rights reserved."
#undef AAPL_VERSION
#ifndef AAPL_VERSION_OVERRIDE
#  define AAPL_VERSION "2.5.0"
#else
#  define AAPL_VERSION AAPL_VERSION_OVERRIDE
#endif

#if !(defined HAVE_READLINE_READLINE_H && defined(HAVE_NCURSES_H))
#   undef AAPL_ENABLE_CONSOLE
#   define AAPL_ENABLE_CONSOLE 0
#endif

#if !(defined HAVE_FCNTL_H && defined(HAVE_SYS_MMAN_H))
#   undef AAPL_ALLOW_GPIO_JTAG
#   define AAPL_ALLOW_GPIO_JTAG 0
#else
#   include <fcntl.h>
#   include <sys/mman.h>
#endif

#ifdef __STDC_VERSION__
#   define AAPL_INLINE inline
#else
#   define AAPL_INLINE
#endif

#ifdef _MSC_VER
/* If Microsoft C: */
#  define __func__ __FUNCTION__
#  define snprintf sprintf_s
#  define fileno(x) _fileno(x)
#endif

/*#ifdef WIN32 */
/* #define DECL "C" __declspec(dllexport) */
/* #include <winsock2.h> */
/* #include <windows.h> */
/* BOOL APIENTRY DllMain2( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpAVAGO_RESERVED) { return TRUE; } */
/* */
#if (defined __MINGW32__ || defined _WIN32) && !(defined WIN32)
#    define WIN32
#endif

#if defined __MINGW32__ || defined WIN32
#    include <winsock2.h>
#    include <windows.h>
#    include <io.h>
#    include <ws2tcpip.h>
#    ifndef sleep
#        define sleep(a) Sleep(a * 1000)
#    endif
#    ifndef isatty
#        define isatty(x) _isatty(x)
#    endif
#else
#   define closesocket(x) close(x)
#   if AAPL_ALLOW_AACS
#        include <sys/socket.h>
#        include <netinet/in.h>
#        include <netdb.h>
#   endif
#endif

#if defined(__cplusplus) && AAPL_ENABLE_C_LINKING
#   define EXT extern "C"
#else
#   define EXT extern
#endif

#if AAPL_ALLOW_THREAD_SUPPORT || AAPL_ALLOW_THREADED_SBUS
#   include <pthread.h>
#   define AAPL_THREAD_STORAGE __thread
#else
#   define AAPL_THREAD_STORAGE
#endif

#ifdef KERNEL_INCLUDES
#   include <linux/mm.h>
#   include <linux/slab.h>
#   include <linux/ctype.h>
#   include <linux/rtc.h>
#   include <linux/delay.h>
#else
#   if STDC_HEADERS
        /* Note: These are all part of the C90 standard: */
#       include <ctype.h>
#       include <stdio.h>
#       include <stdarg.h>
#       include <errno.h>
#       include <time.h>
#       include <math.h>       /* used in serdes.c and eye.c */
#       include <stdlib.h>
#       include <string.h>
#   else
#     if HAVE_STDLIB_H
#       include <stdlib.h>
#     endif
#     if HAVE_STRING_H
#       include <string.h>
#     endif
#   endif
#   if HAVE_UNISTD_H && !defined __MINGW32__
#       include <unistd.h>     /* only required on some systems */
#   endif
#   if defined HAVE_SYS_TYPES_H && HAVE_SYS_TYPES_H
#       include <sys/types.h>
#   endif
#   if defined HAVE_STDTYPES_H && HAVE_STDTYPES_H
#       include <inttypes.h>
#   endif
#   if defined HAVE_STDINT_H && HAVE_STDINT_H
#       include <stdint.h>
#   endif
#endif

#if !(defined WIN32)
#  include <sys/time.h>
#endif

#if !(defined(HAVE_FCNTL_H) && defined(HAVE_LINUX_I2C_DEV_H) && defined(HAVE_SYS_IOCTL_H))
#    undef AAPL_ALLOW_SYSTEM_I2C
#    define AAPL_ALLOW_SYSTEM_I2C 0
#endif

#if AAPL_ALLOW_SYSTEM_I2C
#    include <linux/i2c-dev.h>
#    include <fcntl.h>
#    include <sys/ioctl.h>
#endif

#ifdef HAVE_BCM2835_H
/* Include libraries specific to GPIO initialization */
/* As of March 2014, the bcm2835 library can be found here: */
/*     http://www.airspayce.com/mikem/bcm2835/ */
#    include <bcm2835.h>
#else
#    undef AAPL_ALLOW_GPIO_MDIO
#    define AAPL_ALLOW_GPIO_MDIO 0
#endif

#if AAPL_ENABLE_MAIN
#    include <getopt.h>
#endif


#endif /* AVAGO_SYSTEM_INCLUDES_H_ */
