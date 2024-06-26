#include "./include/light.h"
#include "./include/mytimer.h"
// #include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"
#include "./include/sensor.h"
// #include "./include/decoder.h"
#include "./include/test.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define DECODED_MES_LEN 3
#define VALID_MES_LEN 6
char decode_data[DECODED_MES_LEN] = {0};
char mes_buffer[20] = {0};

// insert experiment function
void vehicle_insert(void *ptParam)
{
    turn_forward();
    while (1)
    {
        step_startup(FORWARD,50);
        move_control(FORWARD,50);
        udelay(1000 * 1300);
        step_stop();
        udelay(1000 * 500);
        step_startup(BACKWARD, 50);
        move_control(BACKWARD,50);
        udelay(1000 * 1400);
        step_stop();
        udelay(1000 * 500);
    }
    
    
}

// turning experiment function

void vehicle_turning(void *ptParam)
{
    turn_right90();
    while (1)
    {
        step_startup(FORWARD,10);
        move_control(FORWARD,10);
        udelay(1000 * 2200);
        step_stop();
        udelay(1000 * 500);
        step_startup(BACKWARD,10);
        move_control(BACKWARD,10);
        udelay(1000 * 2200);
        step_stop();
        udelay(1000 * 500);
    }
    

}

void light_gpio_conifg()
{
    gpio_config_t io_config={
        .pin_bit_mask = (1ULL<<19 | 1ULL<< 39),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en=0
    };

    gpio_config(&io_config); 
}

//vehicle light control
void light()
{
    while (1)
    {
        // vehicle_turning();
        // printf("hello light\n");
        gpio_set_level(GPIO_NUM_19, 1);
        gpio_set_level(GPIO_NUM_39, 1);
        udelay(1000*2);  
        gpio_set_level(GPIO_NUM_19, 0);
        gpio_set_level(GPIO_NUM_39, 0);
        udelay(1000*2);   

    }
}

void app_main(void)
{
   
    // configure vehicle light
    light_gpio_conifg();
    
    //configure motor
    motor_config();

    // configure servo 
    servo_config();

    // xTaskCreatePinnedToCore(vehicle_turning, "vehicle_turning", 8192, NULL, 1, NULL, 1); 
    xTaskCreatePinnedToCore(vehicle_insert, "vehicle_insert", 8192, NULL, 1, NULL, 1); 



}
