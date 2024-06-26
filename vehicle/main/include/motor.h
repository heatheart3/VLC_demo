#ifndef __MOTOR__
#define __MOTOR__

/* A (Right wheel):
    AIN1(NEG)   AIN2(POS)     Direction
        0       1       forward
        1       0       backward
        0       0       stop
*/
/*
    B (Left wheel):
    BIN1(POS)   BIN2(NEG)     Direction
        1       0       forward
        0       1       backward
        0       0       stop
*/

#include "./driver/gpio.h"
#include "./driver/ledc.h"
#include <string.h>

#define VEHICLE_RMOTOR_PWM_CHANNEL LEDC_CHANNEL_0
#define VEHICLE_LMOTOR_PWM_CHANNEL LEDC_CHANNEL_1


#define GPIO_RIGHT_POS    18 //AIN2 PA15 
#define GPIO_RIGHT_NEG    11  //AIN1 PB5

#define GPIO_LEFT_POS    10 //BIN1 PB4
#define GPIO_LEFT_NEG    17 //BIN2 PA12

#define GPIO_RIGHT_PWM   15 //PWMA PA11
#define GPIO_LEFT_PWM   16 //PWMB PA8
#define DUTY_RESOLUTION LEDC_TIMER_13_BIT

#define FORWARD "forward"
#define BACKWARD "backward"

typedef struct Vehicle_state
{
    int speed;
    const char* direction;
};


// #define STOP "110"
// #define FORWARD "101"
// #define BACKWARD "010"
// #define LEFT   "001"
// #define RIGHT  "100"

void motor_config();
int perDuty2Duty(int perDuty);
void sudden_stop();
void step_stop();
void move_control(const char* command, int speed);
void step_startup(const char* command, int speed );
void update_vehicle_state(int speed, const char* direction);


#endif