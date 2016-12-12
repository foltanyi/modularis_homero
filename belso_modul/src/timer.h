/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef timer_h
#define timer_h


extern TIM_HandleTypeDef htim4;


// timer init
void init_timer();


// wait us
void wait_10us(uint8_t ten_us);

//hal delay ms fv-hez
void SysTick_Handler ();

#endif
