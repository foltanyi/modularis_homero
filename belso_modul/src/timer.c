#include "main.h"

/*
 	 idõzítéshez szükséges fv.-ek

 */


/*





void systickInit (uint16_t frequency)
{
   //RCC_ClocksTypeDef RCC_Clocks;
   //RCC_GetClocksFreq (&RCC_Clocks);
   SysTick_Config (168000000/ frequency);
}
*/


TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

void init_timer()
{

	/*---------------- TIM4 init -----------------
	 * 	wait 0.1ms									*/
	__HAL_RCC_TIM4_CLK_ENABLE();

	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim4.Init.Period = 840;
	htim4.Init.Prescaler = 20;
	//htim4.Init.RepetitionCounter = 0;

	//htim4.Instance->ARR

	htim4.Instance = TIM4;

	// enable it and set priority
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	//init timer
	HAL_TIM_Base_Init(&htim4);     // Init timer
	HAL_TIM_Base_Start_IT(&htim4); // start timer interrupts



	/*---------------- TIM5 init -----------------
	 *  wait 1us									*/
	__HAL_RCC_TIM5_CLK_ENABLE();

	//wait 1 ms

	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim5.Init.Period = 168;
	htim5.Init.Prescaler = 10;
	//htim5.Init.RepetitionCounter = 0;

	//htim5.Instance->ARR

	htim5.Instance = TIM5;

	// enable it and set priority
	/*HAL_NVIC_SetPriority(TIM5_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);*/

	//init timer
	HAL_TIM_Base_Init(&htim5);     // Init timer
	//HAL_TIM_Base_Start_IT(&htim5); // start timer interrupts


}


/*--------------------------- interrupt kezeles---------------------------*/
uint8_t vilagossag;
// timer it callback fv
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	// tim4 a PWM-et vezérli
	if(htim->Instance == TIM4)
	{
		switch (vilagossag % 3)
		{
		case 0:{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
			break;
		}
		case 1:{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
			break;
		}
		case 2:{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
			break;
		}
		}//switch

	}//if
	vilagossag++;



}


/*--------------------------- wait us ----------------------------------*/
void wait_10us(uint8_t ten_us)
{
	HAL_TIM_Base_Start(&htim5);

	for(int i=0;i<ten_us;i++)
		while(__HAL_TIM_GET_COUNTER(&htim5) != 168);

	HAL_TIM_Base_Stop(&htim5);

}



/*-----------------------------------------------------------------------*/
extern uint32_t uwTick;

//volatile uint8_t ms;
void SysTick_Handler ()
 {
	uwTick++;
 }


/*
void wait_ms()
{
	int k;
	int i;

	for(i=0;i<168;i++)
		{
			ticks=0;
			while(ticks!=1)
				k++;
		}


}
*/




