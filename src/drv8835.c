/*
 * DRV8835 Motor Driver.c
 *
 * Created: 20/10/2016 4:31:05 μμ
 * Author : Tasos Kakouris
 */

/**
  * A library to control your motors with the DVR8835 driver.
  * Mode: Phase/Enable
  * Velocity of motors is the same and is set by Pulse-width modulation at PORTB1 output. 
  */

#include <avr/io.h>
#include "drv8835.h"

/**
 * Function: motorDriver_init
 * --------------------------
 * Sets some initial settings of Timer1 for PWM generation:
 * 		Mode: Phase Correct PWM
 * 		PWM Frequency: ~20kHZ = Fclc/(2*N*TOP)
 * 		Prescaler = 1 (no-prescaling)
 * 		TOP = ICR1 = 400
 * 
 */
void motorDriver_init(void)
{
	APHASE_ddr |= (1 << APHASE_bit);
	BPHASE_ddr |= (1 << BPHASE_bit);

	DDRB |= (1 << PORTB1); //set PWM port as output

	TCCR1A = 0b10000010;
	TCCR1B = 0b00010001;
	ICR1H = HI(PWM_TOP);
	ICR1L = LO(PWM_TOP);

	OCR1AH = 0;
	OCR1AL = 0;
	TCNT1H = 0;
	TCNT1L = 0;

	TIMSK1 = 0;

	return;
}

/**
 * Function: motorA_setPhase
 * -------------------------
 * Set movement direction of DC Motor A by controlling A Phase pin
 * 
 * direction: 0 -> forward
 * 			  1 -> backwards
 */
void motorA_setPhase(uint8_t direction)
{
	if (direction)
	{
		APHASE_port |= (1 << APHASE_bit);
	}
	else
	{
		APHASE_port &= ~(1 << APHASE_bit);
	}
}

/**
 * Function: motorB_setPhase
 * -------------------------
 * Set movement direction of DC Motor B by controlling B Phase pin
 * 
 * direction: 0 -> forward
 * 			  1 -> backwards
 */
void motorB_setPhase(uint8_t direction)
{
	if (direction)
	{
		BPHASE_port |= (1 << BPHASE_bit);
	}
	else
	{
		BPHASE_port &= ~(1 << BPHASE_bit);
	}
}

/**
 * Function: motors_setDutyCycle
 * -----------------------------
 * Sets speed of motors by changing duty cycle (changing OCR1)
 * 
 * dutyCycle: is given as integer percentage (e.g 50, 70, 90 % etc)
 */
void motors_setDutyCycle(uint8_t dutyCycle)
{
	uint16_t temp = (uint16_t)(((float)dutyCycle / 100) * PWM_TOP);
	TCNT1H = 0;
	TCNT1L = 0;
	OCR1AH = HI(temp);
	OCR1AL = LO(temp);
	return;
}