/*
 * Timer.h - Configures the Timer1 for Input Capture to measure the SDV impulses
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIMER_H
#define TIMER_H

extern volatile unsigned int prevImpulseCount;
extern volatile unsigned int odometerCount;
void initTimerInputCapture();

#endif

#ifdef __cplusplus
}
#endif