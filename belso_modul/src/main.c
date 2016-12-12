//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------



#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"


#include "main.h"



static void SystemClock_Config(void);

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"




int main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.


	HAL_Init();


	  /* Configure the system clock to 168 MHz */
	SystemClock_Config();




	/*----------------- led inicializálása ----------------------*/
	led_sajat_init();

	/*----------------- timer init ---------------------------*/
	init_timer();

	/*----------------- LCD inicializálása ----------------------*/
	LCD_init();

	/*----------------- hõmérõ inicializálása --------------------*/
	//Temp_Init();	//i2c initet õ hívja
	I2C_Init(0xA0);

	btle_init();


	/*-----------Az elsõ sorba kiírjuk, hogy "benti temp:"
	  			 A második sorba kiírjuk, hogy "kinti temp:"---------*/
	//LCD_temp_szoveg_kiirasa();

	//LCD_disp_data_at_pos("benti temp:",Line1_Column1 );


	//LCD_BD_write_string("Bence\0");


	LCD_temp_szoveg_kiirasa();


	//read_lcd();

	//aktuális hõmérséklet tárolására szolgáló változó
	volatile double belso_temp;
	volatile double kulso_temp;

	//btle allapot gep
	uint8_t btle_config = 0;
	uint8_t	btle_connected = 0;
	//aktuális hõmérséklet stringként tárolása a kiíratáshoz
	char tempBelsoString[9];
	char tempKulsoString[9];

	//hõmérséklet lekérdezése
	belso_temp = Temp_GetTemperature();
	//konvertálás
	tempToString(belso_temp, tempBelsoString);
	//LCD frissítése
	LCD_disp_data_at_pos(tempBelsoString, Line1_Column9);




  // Infinite loop
  while (1)
    {

		if (btle_config == 0) {
			if (btle_config_as_central() != HAL_OK) {
				//factory reset
				btle_factory_reset();

				// konfigurálás újra próbálása
			}

			else {
				//config sikerült
				btle_config = 1;
			}
		}//btle_config if
		else{

			if(btle_connected == 0)
			{
				if(btle_connect() == HAL_OK)
					btle_connected = 1;

			}

			else
			{
				//btle temp kiolvasas
				btle_get_temp(&kulso_temp);

				tempToString(kulso_temp,tempKulsoString);
				//LCD frissítése
				LCD_disp_data_at_pos(tempKulsoString,Line2_Column10);

			}

		}//btle_config else


	  //hõmérséklet lekérdezése
		belso_temp=Temp_GetTemperature();
	  //konvertálás
	  tempToString(belso_temp,tempBelsoString);
	  //LCD frissítése
	  LCD_disp_data_at_pos(tempBelsoString,Line1_Column9);

	  HAL_Delay(2000);
	  HAL_Delay(2000);
	  HAL_Delay(2000);

    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
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

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
