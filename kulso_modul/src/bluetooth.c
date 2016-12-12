#include "main.h"
#include <string.h>


//global variables
 UART_HandleTypeDef uart_1_handle;

 //char data_Rx[100];
 char Rx_data_4[20];

 uint8_t Rx_index_4 = 0, Rx_Buffer_4[100];	//uart data buffers
 volatile uint8_t uart_4_string_arrived = 0;			//uart 4 flag

void init_uart()
{

	__GPIOC_CLK_ENABLE();
	__LPUART1_CLK_ENABLE();



	/*----------- initialize GPIO for uart1 ------------*/
	GPIO_InitTypeDef gpio_init;

	gpio_init.Alternate = GPIO_AF0_LPUART1;


	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOC,&gpio_init);


	/*---------- initialize uart 1 -----------------------*/

	uart_1_handle.Instance = LPUART1;

	uart_1_handle.Init.BaudRate = 115200;
	uart_1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_1_handle.Init.Mode = UART_MODE_TX_RX;
	uart_1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart_1_handle.Init.Parity = UART_PARITY_NONE;
	uart_1_handle.Init.StopBits = UART_STOPBITS_1 ;
	uart_1_handle.Init.WordLength = UART_WORDLENGTH_8B;

	HAL_UART_Init(&uart_1_handle);

	//uart receive it
	  HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 1);
	  HAL_NVIC_EnableIRQ(LPUART1_IRQn);

	 HAL_StatusTypeDef return_uart = HAL_UART_Receive_IT(&uart_1_handle,(uint8_t*)Rx_data_4,1);	//Rx_data_4 data_Rx

}


void btle_pin_init()
{

	__GPIOA_CLK_ENABLE();

	// PA11	WAKE_SW
	// PA10	CMD/MLDP

	GPIO_InitTypeDef gpio_init;


	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pin = GPIO_PIN_11 | GPIO_PIN_10;
	gpio_init.Pull = GPIO_PULLDOWN;
	gpio_init.Speed = GPIO_SPEED_LOW;
	//gpio_init.Alternate =;



	HAL_GPIO_Init(GPIOA,&gpio_init);
}


void wake_sw(int _set)
{
	if(_set)
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}


void btle_init()
{


	init_uart();
	btle_pin_init();

	//wake_sw(set);

}


/*
 *
 * bluetooth wait until answer or timeout
 *
 * answer_num: válasz üzenet sorainak száma (hány db \n érkezik)
 */

HAL_StatusTypeDef btle_wait_until_timeout(uint8_t answer_num, uint32_t timeout)
{
	 uint32_t tickstart = HAL_GetTick();


	 while(1)
	 {
		 if(uart_4_string_arrived == answer_num)
			 return HAL_OK;

		 if((HAL_GetTick()-tickstart) > timeout)
			 return HAL_TIMEOUT;
	 }


}

/*--------------------------------------------------- */



void btle_factory_reset()
{

	char *data_Tx  = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //16 elemû

	data_Tx = "SF,2\r\n";
	HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,6,1000);
}


HAL_StatusTypeDef btle_config_as_client()
{


	HAL_StatusTypeDef uart_return = -1;

	uint32_t i;

	char *data_Tx  = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //48elemû

	wake_sw(set);
//	HAL_Delay(10);

	/*if (btle_wait_until_timeout(1,3000) != HAL_OK)
		return HAL_TIMEOUT;*/

	while(uart_4_string_arrived !=1 );

	uart_4_string_arrived = 0;

	/*data_Tx = "+\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,3,10000);
*/
	data_Tx = "SF,2\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,6,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;

	data_Tx = "SS,00000001\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,13,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;

	data_Tx = "SR,32000000\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,13,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;


	data_Tx = "R,1\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,5,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(2,3000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;


	// clear private services
	data_Tx = "PZ\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,4,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;



	data_Tx = "PS,123456789012345678901234567890FF\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,37,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;


	data_Tx = "PC,12345678901234567890123456789011,02,03\r\n";// Set private characteristic to be readable, notifiable and 3 bytes in length
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,43,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;


	data_Tx = "R,1\r\n";
	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,5,1000);
	if(uart_return != HAL_OK)
		return uart_return;

	//HAL_Delay(10);
	//wait until flag
	if (btle_wait_until_timeout(2,3000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;



	return HAL_OK;
}


/* 53 55 57 2C 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32
33 34 35 36 37 38 39 30 31 31 2C 19 0D 0A //rossz 	2C= ,
 *
 *
 *53 55 57 2C 31 32 33 34 35 36
37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30
31 31 2C 32 35 0D
 *
 */

char data_Tx [50];
//char *data_Tx  = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //48elemû

HAL_StatusTypeDef btle_temp_set_char(char *data)
{
	HAL_StatusTypeDef uart_return = -1;

	uint8_t tort = (data[1] == 128) ?  5 : 0;

	int n = sprintf(data_Tx, "SUW,12345678901234567890123456789011,%d.%d\r\n", data[0], tort);	//data[0], data[1]);

	//int n = sprintf(data_Tx, "test%c", x);	//data[0], data[1]);

	//data_Tx = "SUW,12345678901234567890123456789011,5\r\n";

	uart_return = HAL_UART_Transmit(&uart_1_handle,(uint8_t*)data_Tx,n,1000);
		if(uart_return != HAL_OK)
			return ;

		//HAL_Delay(10);
		//wait until flag
		if (btle_wait_until_timeout(1,1000) != HAL_OK)
			return HAL_TIMEOUT;


		//flag 0
		uart_4_string_arrived = 0;

}


/*
 *  uart receive callback fuction
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//void bence_RxCpltCallback(UART_HandleTypeDef *huart)
{

	//int i;

	// UART4
	if(huart->Instance == LPUART1)
	{
		/*if (Rx_index_4 == 0)
		{
			for (i = 0; i < 100; i++)
				Rx_Buffer_4[i] = 0;

		}	//clear Rx_Buffer_4 before receiving new data*/

		//led_sajat_on();
		// karakter vizsgálata
		switch (Rx_data_4[0]) {
		// \n
		case 13: {
			Rx_Buffer_4[Rx_index_4++] = '\0';
			Rx_index_4 = 0;

			//flag 1 be
			uart_4_string_arrived += 1;

			//for(int i=0;i<100000;i++);

			//led_sajat_off();
			break;
		}

			// \r
		case 10: {
			break;
		}

		default: {
			Rx_Buffer_4[Rx_index_4++] = Rx_data_4[0];//add data to Rx_Buffer_4
		}
		}


		HAL_UART_Receive_IT(&uart_1_handle, (uint8_t*)Rx_data_4, 1);	//activate UART receive interrupt every time

	}


}


