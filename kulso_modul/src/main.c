/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "main.h"

extern uint8_t data[2];

// function definitions
void led_init();
void led_on();
void led_off();

static void SystemClock_Config(void);
/*-------------- main --------------*/
int main(void)
{
	uint8_t btle_config = 0;

	HAL_Init();
	SystemClock_Config();

	led_init();

	/*----------------- hõmérõ inicializálása --------------------*/
	//Temp_Init();	//i2c initet õ hívja
	HAL_StatusTypeDef result = Temp_Init();

	/*------ hõmérséklet lekérdezése -------------*/
	double t;
	t = Temp_GetTemperature();
	HAL_Delay(10);


	/*----------- bluetooth inicializálása  --------------*/
	btle_init();

	while(1)
	{

		/* bluetooth config */

		//ha még nem sikerült a config
		if (btle_config == 0) {
			if (btle_config_as_client() != HAL_OK) {
				//factory reset
				btle_factory_reset();
				wake_sw(reset);

				// konfigurálás újra próbálása
			}

			else {
				//config sikerült
				btle_config = 1;

			}
		}
		else{
			t = Temp_GetTemperature();

			HAL_Delay(1000);

			btle_temp_set_char((char*)data);
			led_on();

			HAL_Delay(1000);

		}

	}


} //main






/*---------------------- led functions -----------------------*/
void led_init(){
//pa6
GPIO_InitTypeDef GPIO_led;

GPIO_led.Alternate =
GPIO_led.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_led.Pin = GPIO_PIN_6;
GPIO_led.Pull =GPIO_PULLDOWN;
GPIO_led.Speed =GPIO_SPEED_LOW;

__GPIOA_CLK_ENABLE();
HAL_GPIO_Init(GPIOA,&GPIO_led);
}

void led_on(){

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
}

void led_off(){

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
}




static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}


