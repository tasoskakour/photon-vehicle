
#ifndef MAIN_H
#define MAIN_H

#ifndef F_CPU
#warning "F_CPU undefined, set to 16MHz"
#define F_CPU 16000000UL
#endif

#include <inttypes.h>

/* Define Photoresistor pins as connected to board */
#define PHOTOS_NUM 3 // Define how many photoresistors
//#define PHOTO_THRESHOLD				600 // Uncomment this for activating threshold
#define PHOTO_F_ddr DDRC
#define PHOTO_F_pin PINC
#define PHOTO_F_bit PINC3
#define PHOTO_R_ddr DDRC
#define PHOTO_R_pin PINC
#define PHOTO_R_bit PINC4
#define PHOTO_L_ddr DDRC
#define PHOTO_L_pin PINC
#define PHOTO_L_bit PINC5

#define BUTTON_ddr DDRD
#define BUTTON_pin PIND
#define BUTTON_bit PIND3

#define STARTING_SPEED 30 // same as SpeedTable[0]
#define SPEEDS_NUM 3      // how many speeds available

volatile uint8_t speedTable[3] = {30, 60, 90};

/* Main Sub routines declarations */
void button_init(void);
void ADC_init(void);
uint16_t ADC_collect(uint8_t ADC_channel);
void photoMax(uint16_t *max_val, uint8_t *max_index, uint16_t val1, uint16_t val2, uint16_t val3);
void vehicleControlMovement(void);
void moveForward(void);
void moveBackward(void);
void turnRight(void);
void turnLeft(void);
void setSpeed(uint8_t speedSelect);
void vehicleStop(void);

#endif