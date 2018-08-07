/*
 * Light Controlled Vehicle.c
 *
 * Created: 12/11/2016 5:20:53 μμ
 * Author : Tasos Kakouris

 * A vehicle that moves autonomously, following the path towards the maximum source of light.
 * It has 3 photo-resistors, the values of the resistors are chanigng according the light that hits them
 *
 * The button (INT1-PD3) acts as a switch:
 * - If pressed instantly one time, then the vehicle's movement is deactivated. If pressed again, the it's activated.
 * - If pressed continuously for at least 1 sec, then it changes the speed of the vehicle.
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include "main.h"
#include "drv8835.h"
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t initDutyCycle = STARTING_SPEED;
volatile uint8_t vehicleEnableMovementFlag = 0, speedTableIndex = 0;

/**
 * Iterrupt Service Routin for Button
 * ---------------------------------
 * 1) Stops/Starts vehicle movement
 * 2) Changes speed of vehicle
 *
*/
ISR(INT1_vect)
{
	/* Change state of vehicle, save new state */
	if (vehicleEnableMovementFlag)
	{
		vehicleStop();
		vehicleEnableMovementFlag = 0;
	}
	else
	{
		vehicleEnableMovementFlag = 1;
	}

	/* Check if button is pressed longer than 1sec */
	_delay_ms(1200);

	if (!((BUTTON_pin >> BUTTON_bit) & 1))
	{
		/* Change speed */
		speedTableIndex++;
		if (speedTableIndex >= SPEEDS_NUM)
			speedTableIndex = 0;
		initDutyCycle = speedTable[speedTableIndex];
	}
}

int main(void)
{
	button_init();
	ADC_init();
	motorDriver_init();
	while (1)
	{
		vehicleControlMovement();
	}
}

/**
 * Function: button_init
 * ---------------------
 * Sets up button at PD3 (INT1)
 * 		Interrupt at falling edge
 *
 */
void button_init(void)
{
	BUTTON_ddr &= ~(1 << BUTTON_bit);
	EICRA = 0b00001000; // falling edge of INT1
	EIMSK = 2;			// interrupt enabled
	sei();
	return;
}

/**
 * Function: ADC_init
 * ------------------
 * Sets up Analog to Digital Converter for Photoresistors:
 * 		- ARef = 5V
 * 		- Single Running Mode
 * 		- Prescaler = 64 
 * 		- F_ADC = 250KHz => Fs = 19230 samples/sec
 *
 */
void ADC_init(void)
{
	/* Set pins as analog input */
	PHOTO_F_ddr &= ~(1 << PHOTO_F_bit);
	PHOTO_R_ddr &= ~(1 << PHOTO_R_bit);
	PHOTO_L_ddr &= ~(1 << PHOTO_L_bit);
	/* Set ADC  settings */
	ADMUX = 0b01000000 | PHOTO_F_bit;
	ADCSRA = 0b10000110;
	return;
}
/**
 * Function: ADC_collect
 * ---------------------
 * Collects analog reading from ADC_channel given
 * 
 * ADC_channel: the channel to collect
 * 
 * returns the 16bit digital value
 */
uint16_t ADC_collect(uint8_t ADC_channel)
{
	uint16_t adcVal;
	/* Change channel and start conversion */
	ADMUX = ((ADMUX & 0b11111000) | ADC_channel);
	ADCSRA |= (1 << ADSC);
	while (!((ADCSRA >> ADIF) & 1))
	{
	} // wait ADC conversion

	adcVal = ADCL;
	adcVal |= (ADCH << 8);

	return adcVal;
}

/**
 * Function: photoMax
 * ------------------
 * Changes maximum value and maximum holder of max value
 * 
 * max_val: pointer of maximum value
 * max_index: pointer of maximum index
 * val1, val2, val3: the 3 values of photoresistors to be compared
 * 
 */
void photoMax(uint16_t *max_val, uint8_t *max_index, uint16_t val1, uint16_t val2, uint16_t val3)
{

	*max_index = 1;
	*max_val = val1;

	if (val2 > *max_val)
	{
		*max_index = 2;
		*max_val = val2;
	}
	if (val3 > *max_val)
	{
		*max_index = 3;
		*max_val = val3;
	}

	return;
}

/**
 * Function: vehicleControlMovement
 * --------------------------------
 * Controls the vehicle according to photo resistors readings
 * 
 */
void vehicleControlMovement(void)
{
	uint8_t maxPhotoIndex = 0;
	uint16_t maxVal = 0, front, right, left;

	/* Collect ADC readings and find the maximum */
	front = ADC_collect(PHOTO_F_bit);
	right = ADC_collect(PHOTO_R_bit);
	left = ADC_collect(PHOTO_L_bit);
	photoMax(&maxVal, &maxPhotoIndex, front, right, left);

/* If a PHOTO_THRESHOLD is defined then the vehicle remains still till it passes that threshold */
#ifdef PHOTO_THESHOLD
	if (maxVal < PHOTO_THRESHOLD)
	{
		vehicleStop();
		return;
	}
#endif

	/*Move vehicle according to maxPhotoIndex */
	if (vehicleEnableMovementFlag)
	{
		switch (maxPhotoIndex)
		{
		case 1:
			moveForward();
			break;
		case 2:
			turnRight();
			break;
		case 3:
			turnLeft();
			break;
		}
	}

	if (vehicleEnableMovementFlag == 0)
		vehicleStop(); // safety

	return;
}

/**
 * Function: moveForward
 * ---------------------
 * Moves vehicle forwards
 * 
 */
void moveForward(void)
{
	motorA_setPhase(0);
	motorB_setPhase(1);
	motors_setDutyCycle(initDutyCycle);
	return;
}

/**
 * Function: moveBackward
 * ---------------------
 * Moves vehicle backwards
 * 
 */
void moveBackward(void)
{
	motorA_setPhase(1);
	motorB_setPhase(0);
	motors_setDutyCycle(initDutyCycle);
	return;
}

/**
 * Function: turnRight
 * ---------------------
 * Turns vehicle right
 * 
 */
void turnRight(void)
{
	motorA_setPhase(0);
	motorB_setPhase(0);
	motors_setDutyCycle(initDutyCycle);
	return;
}

/**
 * Function: turnLeft
 * ---------------------
 * Turns vehicle left
 * 
 */
void turnLeft(void)
{
	motorA_setPhase(1);
	motorB_setPhase(1);
	motors_setDutyCycle(initDutyCycle);
	return;
}

/**
 * Function setSpeed
 * -----------------
 * Changes speed accordingly (Duty cycle of timer1 PWM is changed)
 * 
 * speedSelect: 1-3
 */
void setSpeed(uint8_t speedSelect)
{
	switch (speedSelect)
	{
	case 1:
		initDutyCycle = speedTable[0];
		break;
	case 2:
		initDutyCycle = speedTable[1];
		break;
	case 3:
		initDutyCycle = speedTable[2];
		break;
	}

	// Check if vehicle is stopped
	uint16_t ocr_val = ((OCR1AH << 8) || OCR1AL);
	if (ocr_val != 0)
		motors_setDutyCycle(initDutyCycle);

	return;
}

/**
 * Function: vehicleStop
 * ---------------------
 * Stop vehicle
 */
void vehicleStop(void)
{
	motors_setDutyCycle(0); //0% duty cycle => stops generating PWM => stops vehicle
	return;
}