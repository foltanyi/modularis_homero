/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32l0xx_it.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}


extern UART_HandleTypeDef uart_1_handle;
extern uint8_t Rx_index_4;
extern uint8_t Rx_Buffer_4[100];	//uart data buffers
extern volatile uint8_t uart_4_string_arrived;			//uart 4 flag


void LPUART1_IRQHandler(void)
{

	//HAL_UART_IRQHandler(&uart_1_handle);


	if((__HAL_UART_GET_IT(&uart_1_handle, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&uart_1_handle, UART_IT_RXNE) != RESET))
	 {
		if((uart_1_handle.State == HAL_UART_STATE_BUSY_RX) || (uart_1_handle.State == HAL_UART_STATE_BUSY_TX_RX)){
		 uint8_t Rx_data = (uint8_t) uart_1_handle.Instance->RDR;

			switch (Rx_data) {
			// \n
			case 13: {
				Rx_Buffer_4[Rx_index_4++] = '\0';
				Rx_index_4 = 0;

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

	 //--uart_1_handle.RxXferCount;
   /*  while(HAL_IS_BIT_SET(uart_1_handle.Instance->ISR, UART_FLAG_RXNE))
     {
     }*/



	//led_sajat_on();

}
