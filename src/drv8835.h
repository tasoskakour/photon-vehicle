/**
 * Below implementation is for commin pins at AENABLE and BENABLE (same velocity for motors)
 * Phase-Enable Mode (MODE = HIGH)
 *
 */
#ifndef DRV8835_H
#define DRV8835_H

#include <inttypes.h>

/**
 * DRV8835 Pin Definitions. 
 *  User must change that according to his settings 
 * 
 * Reserved Pins: PORTB1 is used for TIMER1 generating PWM
 */
#define APHASE_ddr DDRB
#define APHASE_port PORTB
#define APHASE_bit PORTB2
#define BPHASE_ddr DDRB
#define BPHASE_port PORTB
#define BPHASE_bit PORTB3

// Dont change anything below here
#define LO(x) ((x)&0xFF)
#define HI(x) (((x) >> 8) & 0xFF)
#define PWM_TOP 400

// Functions
void motorDriver_init(void);
void motorA_setPhase(uint8_t direction);
void motorB_setPhase(uint8_t direction);
void motors_setDutyCycle(uint8_t dutyCycle);

#endif