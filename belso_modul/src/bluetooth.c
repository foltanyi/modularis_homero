/*
 * RN4020 bluetooth modul
 *
 *
 */

#include "main.h"

//global variables
 UART_HandleTypeDef uart_4_handle;
 char data_Rx[100];
 char Rx_data_4[20];

uint8_t Rx_index_4 = 0, Rx_Buffer_4[100];	//uart data buffers
 volatile uint8_t uart_4_string_arrived = 0;			//uart 4 flag

void init_uart()
{

	__GPIOC_CLK_ENABLE();
	__UART4_CLK_ENABLE();



	/*----------- initialize GPIO for uart4 ------------*/
	GPIO_InitTypeDef gpio_init;

	gpio_init.Alternate = GPIO_AF8_UART4;


	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOC,&gpio_init);

/*------------- initialize uart 4 -----------------------*/

	uart_4_handle.Instance = UART4; //UART4;USART3

	uart_4_handle.Init.BaudRate = 115200;
	uart_4_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_4_handle.Init.Mode = UART_MODE_TX_RX;
	uart_4_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart_4_handle.Init.Parity = UART_PARITY_NONE;
	uart_4_handle.Init.StopBits = UART_STOPBITS_1 ;
	uart_4_handle.Init.WordLength = UART_WORDLENGTH_8B;

	if (HAL_UART_Init(&uart_4_handle) != HAL_OK) //;
	{
	   led_sajat_on();
	}


	//uart receive it
	  HAL_NVIC_SetPriority(UART4_IRQn, 0, 1);
	  HAL_NVIC_EnableIRQ(UART4_IRQn);

	  HAL_UART_Receive_IT(&uart_4_handle,(uint8_t*)Rx_data_4,1);	//Rx_data_4 data_Rx

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
 /* Prevent unused argument(s) compilation warning */
 UNUSED(huart);

 HAL_UART_Receive_IT(&uart_4_handle,(uint8_t*)data_Rx,1);
}


/*----------------------------------------*/
void btle_pin_init()
{

	__GPIOE_CLK_ENABLE();

	// PE4	WAKE_SW
	// PE2	CMD/MLDP

	GPIO_InitTypeDef gpio_init;


	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pin = GPIO_PIN_2 | GPIO_PIN_4;
	gpio_init.Pull = GPIO_PULLDOWN;
	gpio_init.Speed = GPIO_SPEED_LOW;
	//gpio_init.Alternate =;



	HAL_GPIO_Init(GPIOE,&gpio_init);
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


/*-----------------------------------------*/
void wake_sw(int _set)
{
	if(_set)
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
}

/*------------------------------------*/
void btle_init()
{
	init_uart();
	btle_pin_init();

	//wake_sw(set);
}


/*-
 * config the btle modul
 */

char *data_Tx  = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //48 elemû

HAL_StatusTypeDef uart_return = -1;

HAL_StatusTypeDef btle_config_as_central()
{
	wake_sw(set);
//	HAL_Delay(10);
	//CMD

/*
	data_Tx = "SF,2\n\r";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,6,1000);

	data_Tx = "R,1\n\r";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,5,10000);*/


	/*data_Tx = "+\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,3,10000);*/

	//HAL_UART_Receive(&uart_4_handle,(uint8_t*)data_Rx,5,2000);


	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

		//flag 0
		uart_4_string_arrived = 0;


	data_Tx = "SF,1\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,6,1000);

	//ERR	v.	AOK

	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;


	data_Tx = "SS,00000000\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,13,1000);

	//AOK

	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;

	//flag 0
	uart_4_string_arrived = 0;


	/*data_Tx = "SR,00000000\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,13,1000);*/

	data_Tx = "SR,92000000\r\n";		//central: 92000000
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,13,1000);

	//AOK

	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;

	HAL_Delay(10);
	data_Tx = "R,1\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,5,1000);
	HAL_Delay(10);

	//todo:
	//	rebbot és cmd is érkezik, mind2 után kell tovább menni

	if (btle_wait_until_timeout(2,3000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;

	//Reboot
	//CMD
	led_sajat_on();
	HAL_Delay(10);

	return HAL_OK;

}



/*
 *
 * connect two rn4020 moduls
 */
HAL_StatusTypeDef btle_connect()
{
	//*data_Rx = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	/*data_Tx = "F\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,3,1000);

	if (btle_wait_until_timeout(2,1000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;

	//AOK
	//001EC029296F,0,-27

	HAL_Delay(10);

	data_Tx = "X\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,3,1000);
	HAL_Delay(10);
	//AOK

	if (btle_wait_until_timeout(1,1000) != HAL_OK)
		return HAL_TIMEOUT;*/

	//flag 0
	uart_4_string_arrived = 0;

	data_Tx = "E,0,001EC029296F\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,18,1000);

	//AOK
	//Connected

	if (btle_wait_until_timeout(2,10000) != HAL_OK)
		return HAL_TIMEOUT;


	//flag 0
	uart_4_string_arrived = 0;

	//CURV,2A19 read battery



	/**data_Rx = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	HAL_UART_Receive(&uart_4_handle,(uint8_t*)data_Rx,5,2000);
	LCD_disp_data_at_pos(data_Rx,Line2_Column1);*/



	/*HAL_Delay(10000);
	data_Tx = "LC\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,4,1000);
	HAL_Delay(10);


	while(uart_4_string_arrived != 1);


	//flag 0
	uart_4_string_arrived = 0;*/

	return HAL_OK;
}



HAL_StatusTypeDef btle_get_temp(double *temp_kulso)
{

	/*HAL_Delay(100);
	data_Tx = "LC\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,4,1000);
	HAL_Delay(10);*/


//	while(uart_4_string_arrived != 1);


	//flag 0
	uart_4_string_arrived = 0;


	HAL_Delay(100);
	data_Tx = "CURV,12345678901234567890123456789011\r\n";
	uart_return = HAL_UART_Transmit(&uart_4_handle, (uint8_t*) data_Tx, 39,1000);
	HAL_Delay(10);
//R,00.
//AOK

	if (btle_wait_until_timeout(2,1000) != HAL_OK)
		return HAL_TIMEOUT;

	uint8_t temp,tortResz;
//char R, vessz;

	sscanf(Rx_Buffer_4, "R,%d.%d", &temp, &tortResz);

//flag 0
	uart_4_string_arrived = 0;

	*temp_kulso=0;

	*temp_kulso = temp + tortResz/10;



	return HAL_OK;
}


void btle_factory_reset()
{

	char *data_Tx  = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; //16 elemû

	data_Tx = "SF,2\r\n";
	HAL_UART_Transmit(&uart_4_handle,(uint8_t*)data_Tx,6,1000);
}












