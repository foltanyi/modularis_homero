/*
	interrupt handler függvények hívása

*/

#include "main.h"

/*-------------------------------------------*/


extern UART_HandleTypeDef uart_4_handle;
extern  char data_Rx[100];


extern uint8_t Rx_index_4;
extern uint8_t Rx_Buffer_4[100];	//uart data buffers
extern volatile uint8_t uart_4_string_arrived;			//uart 4 flag


/*--------------- TIM4 IT handler ----------------*/
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim4);

}



void UART4_IRQHandler(void)
{

	//HAL_UART_IRQHandler(&uart_4_handle);

	if(__HAL_UART_GET_FLAG(&uart_4_handle,UART_IT_RXNE) == 0			/*(__HAL_UART_GET_IT(&uart_4_handle, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&uart_4_handle, UART_IT_RXNE) != RESET)*/)
	 {
		if((uart_4_handle.RxState == HAL_UART_STATE_BUSY_RX) || (uart_4_handle.RxState == HAL_UART_STATE_BUSY_TX_RX)){
		 uint8_t Rx_data = (uint8_t) uart_4_handle.Instance->DR;

		 if((uart_4_string_arrived == 0) & (Rx_Buffer_4[Rx_index_4-1] == '\0'))
			 Rx_index_4 = 0;

			switch (Rx_data) {
			// \n
			case 13: {
				Rx_Buffer_4[Rx_index_4++] = '\0';


				//flag 1 be
				uart_4_string_arrived += 1;

				break;
			}

				// \r
			case 10: {
				break;
			}

			default: {
				Rx_Buffer_4[Rx_index_4++] =  Rx_data;//add data to Rx_Buffer_4
			}
			}}
	 }

	//bence_RxCpltCallback(&uart_4_handle);

	//led_sajat_on();

}
