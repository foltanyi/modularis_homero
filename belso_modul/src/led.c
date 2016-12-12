#include "stm32f4xx.h"
#include "led.h"

/*
  LED: PC5 láb



  */




GPIO_InitTypeDef led_init;

void led_sajat_init()
{

	//led pi inicializálása

	led_init.Pin=GPIO_PIN_5;
	led_init.Mode=GPIO_MODE_OUTPUT_PP;
	led_init.Pull=GPIO_PULLDOWN;
	led_init.Speed=GPIO_SPEED_FREQ_LOW;
	led_init.Alternate=GPIO_MODE_AF_PP;

	__GPIOC_CLK_ENABLE();
	HAL_GPIO_Init(GPIOC,&led_init);

}


//led bekapcsolása
void led_sajat_on()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);

}


//led kikapcsolása
void led_sajat_off()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);

}


