#include "./include/motor.h"
#include "./include/servo.h"
#include "./include/mytimer.h"

struct Vehicle_state vehicle_state = {0, "stop"};

void motor_config()
{
    ledc_timer_config_t ledc_timer = { 
	.duty_resolution = DUTY_RESOLUTION,	// PWM占空比分辨率
	.freq_hz = 5000,						// PWM信号频率
	.speed_mode = LEDC_LOW_SPEED_MODE,				// 定时器模式
	.timer_num = LEDC_TIMER_0				// 定时器序号;使用哪个定时器0-3;
    };
    ledc_timer_config(&ledc_timer);
    //configure PWMA
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .gpio_num = GPIO_RIGHT_PWM,
        .channel = LEDC_CHANNEL_0,
        .duty = 3095,
        .intr_type = LEDC_INTR_DISABLE,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
    //configure PWMB
    ledc_channel.gpio_num = GPIO_LEFT_PWM;
    ledc_channel_config(&ledc_channel);
    //configure AIN1,AIN2,BIN1,BIN2
    gpio_config_t io_config;
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = ( (1 << GPIO_RIGHT_NEG) | (1 << GPIO_RIGHT_POS) | (1 << GPIO_LEFT_POS) 
                                | (1 << GPIO_LEFT_NEG));
    gpio_config(&io_config);

    
}

int perDuty2Duty(int perDuty)
{
    int duty = 0;
    int total_duty = (1 << DUTY_RESOLUTION) - 1;
    duty = perDuty * total_duty * 0.01;
    return duty; 
}

void move_control(const char* command, int speed)
{
    sudden_stop();    

    //speed refers to percentage of the duty of PWM so it should less than 100 
    if(speed > 100)
    {
        speed = 100;
    }
    else if(speed < 0)
    {
        speed = 0;        
    }
    
    if(strcmp(command, "forward") == 0)
    {
        gpio_set_level(GPIO_RIGHT_POS, 1);
        gpio_set_level(GPIO_RIGHT_NEG, 0);
        gpio_set_level(GPIO_LEFT_POS, 1);
        gpio_set_level(GPIO_LEFT_NEG, 0);
    }
    else if(strcmp(command, "backward") == 0)
    {
        gpio_set_level(GPIO_RIGHT_POS, 0);
        gpio_set_level(GPIO_RIGHT_NEG, 1);
        gpio_set_level(GPIO_LEFT_POS, 0);
        gpio_set_level(GPIO_LEFT_NEG, 1);
    }
   
    //update speed
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(speed));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    update_vehicle_state(speed, command);
}


void sudden_stop()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, perDuty2Duty(0));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    gpio_set_level(GPIO_RIGHT_NEG, 0);
    gpio_set_level(GPIO_RIGHT_POS, 0);
    gpio_set_level(GPIO_LEFT_NEG, 0);
    gpio_set_level(GPIO_LEFT_POS, 0);
    update_vehicle_state(0, "stop");
}


void step_stop()
{
    uint8_t decelerate_rate = 5;
    uint16_t decelerate_period = 70;
    uint8_t decelerate_round = (uint8_t) vehicle_state.speed / decelerate_rate;
    uint8_t i = 0;
    for (i = 0; i < decelerate_round; i++)
    {
        move_control(vehicle_state.direction, vehicle_state.speed - decelerate_rate);
        udelay(1000 * decelerate_period);
    }
    sudden_stop();
}

void step_startup(const char* command, int speed)
{
    uint8_t accelerate_rate = 5;
    uint16_t accelerate_period = 70;
    uint8_t accelerate_round = speed / accelerate_rate;
    vehicle_state.speed  = 0;
    
    for(int i = 0; i < accelerate_round; i++)
    {
        move_control(vehicle_state.direction, vehicle_state.speed + accelerate_rate);
        udelay(1000 * accelerate_period);
    }
    move_control(vehicle_state.direction, vehicle_state.speed + speed%accelerate_rate);
}

void update_vehicle_state(int speed, const char* direction)
{
    vehicle_state.speed = speed;
    vehicle_state.direction = direction;
}


