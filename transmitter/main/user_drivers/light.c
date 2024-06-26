#include "./include/light.h"
#include  "./include/mytimer.h"

void light_pwm_config()
{
    ledc_timer_config_t timer_con = {};
    timer_con.duty_resolution = LEDC_TIMER_2_BIT; //resolution =  (100 / 8192) %
    timer_con.freq_hz = 20*1000;
    timer_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_con.timer_num = LEDC_TIMER_1;
    ledc_timer_config(&timer_con);
    
    ledc_channel_config_t channel_con = {};
    channel_con.channel = LEDC_CHANNEL_1;
    channel_con.gpio_num = GPIO_LEFT_LIGHT;
    channel_con.duty = 2;
    channel_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    channel_con.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&channel_con);
}

void light_ook_config(const int gpio_num)
{
    gpio_config_t io_config={
        .pin_bit_mask = (1<<gpio_num),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en=0
    };

    gpio_config(&io_config);
}

void transmit_ook(const char *data,int GPIO_OUTPUT_LIGHT)
{
    // printf("transmit_ook: %s\n",data);
    for(int i=0;i<strlen(data);i++)
    {
        if(data[i]=='1')
        gpio_set_level(GPIO_OUTPUT_LIGHT,1);
        else
        gpio_set_level(GPIO_OUTPUT_LIGHT,0);
        udelay(1000);
    }
}


void transmit_ascii(const char* data,int GPIO_OUTPUT_LIGHT)
{   
    volatile char bit;
    transmit_ook(MES_HEADER,1);
    for(int i=0;i<strlen(data);i++)
    {
        for(int j=7;j>=0;j--)
        {
            bit = (data[i]>>j)&0x01;
            if(bit == 1)
            {
                gpio_set_level(GPIO_OUTPUT_LIGHT,0);
                udelay(1000);
                gpio_set_level(GPIO_OUTPUT_LIGHT,1);
                udelay(1000);
            }
            else
            {
                gpio_set_level(GPIO_OUTPUT_LIGHT,1);
                udelay(1000);
                gpio_set_level(GPIO_OUTPUT_LIGHT,0);
                udelay(1000);
            }
        }
    }

}