/*

	?? kasztolás char -> uint8_t
*/
#include "main.h"



/*---------------LCD pins:-----------------------------------

DB7:	D port GPIO_PIN_11
DB6:	D port GPIO_PIN_10
DB5:	B port GPIO_PIN_15
DB4:	D port GPIO_PIN_8
DB3:	B port GPIO_PIN_11
DB2:	E port GPIO_PIN_15
DB1:	E port GPIO_PIN_14
DB0:	E port GPIO_PIN_13

RS		E port GPIO_PIN_10	//data or instruction
R/W		E port GPIO_PIN_11
E		E port GPIO_PIN_12

PWM:
PC2		lcd_pwm

meghajtó IC enable és dir:
dir PB13	0: B->A
/OE PD9



*/


/*------------------
 * 					  LCD incializálás
 	 	 	 	 	 	 	 	 	    -----------------------*/

/*--------------------LCD pins init---------------------------*/
void LCD_pins_init()
{
		__GPIOD_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
		__GPIOE_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();

		GPIO_InitTypeDef lcd_DB_init_Dport;
		GPIO_InitTypeDef lcd_DB_init_Bport;
		GPIO_InitTypeDef lcd_DB_init_Eport;
		GPIO_InitTypeDef lcd_DB_init_Cport;

		lcd_DB_init_Dport.Pin= GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_9;
		lcd_DB_init_Bport.Pin= GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_13;
		lcd_DB_init_Eport.Pin= GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		lcd_DB_init_Cport.Pin= GPIO_PIN_2;

		lcd_DB_init_Dport.Mode=GPIO_MODE_OUTPUT_PP;
		lcd_DB_init_Bport.Mode=GPIO_MODE_OUTPUT_PP;
		lcd_DB_init_Eport.Mode=GPIO_MODE_OUTPUT_PP;
		lcd_DB_init_Cport.Mode=GPIO_MODE_OUTPUT_PP;

		lcd_DB_init_Dport.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
		lcd_DB_init_Bport.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
		lcd_DB_init_Eport.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
		lcd_DB_init_Cport.Speed=GPIO_SPEED_FREQ_VERY_HIGH;

		lcd_DB_init_Dport.Pull=GPIO_PULLDOWN;
		lcd_DB_init_Bport.Pull=GPIO_PULLDOWN;
		lcd_DB_init_Eport.Pull=GPIO_PULLDOWN;
		lcd_DB_init_Cport.Pull=GPIO_PULLDOWN;


		HAL_GPIO_Init(GPIOD, &lcd_DB_init_Dport);
		HAL_GPIO_Init(GPIOB, &lcd_DB_init_Bport);
		HAL_GPIO_Init(GPIOE, &lcd_DB_init_Eport);
		HAL_GPIO_Init(GPIOC, &lcd_DB_init_Cport);

	return;
}


/*------------------LCD istruction code küldés-----------------------*/
void LCD_IC_write(uint8_t data)
{
	//74HCT244 meghajtó IC engedélyezése
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);

	//lcd vezérlõ jelei
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);	// RS=0 instruction
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	// R/W
	wait_10us(1);	//wait for 10us
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);		// E->1


	HAL_Delay(1);

	uint8_t adat;
	uint8_t mask=0b00000001;

	adat=data;

	//DB0 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB1 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB2 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB3 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB4 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB5 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB6 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB7 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);


		wait_10us(1);	//wait for 10us		//data setup time 40ns; most 10us


		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);	// E->0

		wait_10us(1);	//wait for 10us		//data hold time 10ns; most 10us

		//led_sajat_on();

}

/*----------------- LCD inicializálása ----------------------*/
void LCD_init()
{

	// LCD pins inicializálása
	LCD_pins_init();

	//lcd: wait 40ms after VDD rises
	HAL_Delay(45);


	//chip enable -> L
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
	wait_10us(1);	//wait for 10us

	//lcd_pwm
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);


	//function set
	/*
	 * DB2: F: font type: 5x11 dots or 5x8 dots
	 * DB3: N: 2 line or 1 line
	 * DB4: DL: data buth length: 8 bit or 4 bit
	 */
	LCD_IC_write(0b00111000);
	wait_10us(4);	//wait for 40us

	LCD_IC_write(0b00111000);
	wait_10us(4);	//wait for 40us


	//display ON
	/* on/off control bits
	 * DB0: B: cursor blinking
	 * DB1: C: cursor
	 * DB2: D: set desplay
	 */
	LCD_IC_write(0b00001100);
	wait_10us(4);	//wait for 40us


	//display clear
	LCD_IC_write(0b00000001);
	wait_10us(170);	//wait for 1.7ms


	//entry mode set
	/* on/off control bits
	 * DB0: SH: shift desplay
	 * DB1: I/D: cursor moving direction
	 */
	LCD_IC_write(0b00000110);
	wait_10us(4);	//wait for 40us


}
/*----------------------------------------------------------*/



/*-------------------
 * 						LCD kiírás
 	 	 	 	 	 	 				-----------------------*/

/*------------------LCD DB adat küldés-----------------------*/
void LCD_DB_write(uint8_t data)
{
	//74HCT244 meghajtó IC engedélyezése
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);

	//lcd vezérlõ jelei
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);		// RS
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	// R/W
	wait_10us(1);	//wait for 10us
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);		// E->1


	HAL_Delay(1);

	uint8_t adat;
	uint8_t mask=0b00000001;

	adat=data;

	//DB0 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB1 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB2 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB3 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB4 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB5 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB6 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);

		adat=data;
		mask=mask<<1;

	//DB7 küldése
		if((adat & mask) == mask) HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);


		wait_10us(1);	//wait for 10us		//data setup time 40ns; most 10us

		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);	// E->0

		wait_10us(1);	//wait for 10us		//data hold time 10ns; most 10us

		//led_sajat_on();

}
/*-----------------------------------------------------------*/


/*--------------LCD olvass-------------------------------------*/

/*
uint8_t data_read[8];

void read_lcd()
{
	//74HCT244 meghajtó IC engedélyezése
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);

	//lcd vezérlõ jelei
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);		// RS
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);		// R/W
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);		// E->1

	HAL_Delay(1);

	data_read[0] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13);
	data_read[1] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14);
	data_read[2] = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15);
	data_read[3] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11);
	data_read[4] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8);
	data_read[5] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
	data_read[6] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10);
	data_read[7] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11);

}*/




/*------------------LCD sztring kiírása----------------------*/
void LCD_BD_write_string(char* data)
{
	int i=0;
	while(data[i] != '\0')
		LCD_DB_write(data[i++]);


}
/*-----------------------------------------------------------*/





/*-----------LCD sztring kiírása a mgeadott pozíciótól kezdve-------*/
void LCD_disp_data_at_pos(char* data,uint8_t pos)
{
	//if((pos>>4 != 0) || (pos>>4 != 4))
		//return ;

	//set position:
		//RS és R/W -> L
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);

		// DB7=1 DB6-DB0= pos
		uint8_t pos_data=(pos | 0x80);
		LCD_IC_write(pos_data);

	HAL_Delay(1);

	//sztring kiírása
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		LCD_BD_write_string(data);



}
/*---------------------------------------------------------------*/


/*-----------Az elsõ sorba kiírjuk, hogy "benti temp:"
  			 A második sorba kiírjuk, hogy "kinti temp:"
 														---------*/
void LCD_temp_szoveg_kiirasa()
{

	LCD_disp_data_at_pos("in temp:",Line1_Column1 );

	LCD_disp_data_at_pos("out temp:",Line2_Column1);

}
/*-------------------------------------------------------------*/













