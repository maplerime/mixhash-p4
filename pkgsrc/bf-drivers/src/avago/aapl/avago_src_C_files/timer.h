/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* Copyright (c) 2014-2017 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** */
/* AAPL Revision: 2.5.0                                        */
/** Doxygen File Header */
/** @file */
/** @brief Declarations for timer functions. */

#ifndef AVAGO_HIGH_RES_TIMER_H_
#define AVAGO_HIGH_RES_TIMER_H_

/* OVERVIEW of High Resolution Timer functions. */
/* Use timer to accumulate elapsed time in microseconds. */
/*  Controls are like a stop watch: start, stop, get, and reset. */
/* Usage: */
/* Note: Must call one of the reset functions first: */
/*  aapl_timer_reset_start - Initialize and start timer.  Call this, */
/*  aapl_timer_reset       - Initialize timer.            or this, first */
/*  aapl_timer_start       - Start the timer */
/*  aapl_timer_stop        - Stop  the timer */
/*  aapl_timer_get         - Get the current elapsed time in microseconds */
/*  aapl_timer_switchto    - Synchronously stop one timer and start another */

/** @cond INTERNAL */

typedef struct high_res_timer
{
#ifdef WIN32
    __int64 m_start;        /* accumulation start time */
    __int64 m_timer;        /* accumulated time */
#else
    struct timeval m_start; /* accumulation start time */
    unsigned long m_timer;           /* accumulated time */
#endif
} Aapl_microtimer_t;

EXT unsigned long aapl_timer_get(Aapl_microtimer_t *timer); /* Returns timer value in microseconds */
EXT void aapl_timer_reset_start(Aapl_microtimer_t *timer);  /* Resets and starts timer */
EXT void aapl_timer_reset(Aapl_microtimer_t *timer);        /* Stops and clears timer */
EXT void aapl_timer_start(Aapl_microtimer_t *timer);        /* Starts or resumes timer */
EXT void aapl_timer_stop(Aapl_microtimer_t *timer);         /* Stops timer */
EXT const char *aapl_timer_gets(Aapl_microtimer_t *timer);  /* Returns formatted time string */

/* Stop counting, start new timer: */
EXT void aapl_timer_switchto(Aapl_microtimer_t *oldtimer, Aapl_microtimer_t *newtimer);


/** @endcond */

#endif /* AVAGO_HIGH_RES_TIMER_H_ */
