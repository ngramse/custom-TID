#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int impulseCount;
volatile unsigned int prevImpulseCount;
volatile unsigned int odometerCount;

void initTimerInputCapture()
{
    cli(); //block interrupts while writing the timer registers
    TCCR1A = 0;
    TCCR1B |= (1<<ICNC1) | (1<<ICES1) | (1<<WGM12) | (1<<CS11) | (1<<CS10); //set up input capture trigger for rising edge and set up the prescaler to 64
    TCNT1 = 0; //preassign the counter value so that the timer overflow is every 0.1926s
    OCR1A = 53136; //set up the output compare register with a value so that the counter will be reset after 0.1926s (OCR1A = clock / (frequency until reset)  * 2 * prescaler)
    TIMSK1 |= (1<<OCIE1A) | (1<<ICIE1); //activate interrupts for clear timer on compare match and input capture (pin D8)
    sei(); //allow interrupts
}

ISR (TIMER1_COMPA_vect) //will be executeted every 0.1926s
{
    prevImpulseCount = impulseCount;
    odometerCount += impulseCount;
    impulseCount = 0;
}

ISR (TIMER1_CAPT_vect) //will be executed on a rising edge
{
    impulseCount += 1;
}