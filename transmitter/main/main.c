#include "./include/light.h"
#include "./include/mytimer.h"
// #include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"
// #include "./include/sensor.h"
#include "./include/decoder.h"
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


void app_main(void)
{
    light_ook_config(GPIO_NUM_17);

    // motor_config();

    // servo_config();

    // turn_forward();


    while (1)
    {
        gpio_set_level(GPIO_NUM_17, 1);
        udelay(1000*2);
        gpio_set_level(GPIO_NUM_17, 0);
        udelay(1000*2);   
    }


}
