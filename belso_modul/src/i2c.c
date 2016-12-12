#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"

//#include "bsp_globalfunctions.h"
#include "i2c.h"

/* ----------------- Bels�, glob�lis v�ltoz�k ------------------- */

// A t�nyleges I2C perif�ria be�ll�t�s�ra �s hivatkoz�s�ra haszn�lt struct.
I2C_HandleTypeDef hi2c;

const uint32_t i2c_timeout = 5000;

/* ----------------- Publikus f�ggv�nyek ------------------------ */

/** I2C kommunik�ci� inicializ�l�sa. */
HAL_StatusTypeDef I2C_Init(uint32_t ownAddress)
{

	//i2c gpio portok inicializ�l�sa
	__GPIOB_CLK_ENABLE();
	__I2C1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Alternate=GPIO_AF4_I2C1;
	GPIO_init.Mode=GPIO_MODE_AF_OD;
	GPIO_init.Pin=GPIO_PIN_7 | GPIO_PIN_6;
	GPIO_init.Pull=GPIO_PULLUP;
	GPIO_init.Speed=GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOB,&GPIO_init);


	// A hi2c handler struct Instance �s Init mez�it kit�ltve, majd a HAL_I2C_Init() f�ggv�nyt
	// megh�vva inicializ�lja az I2C1 perif�ri�t
	// 7 bites c�mz�s, clock speed 100000, nincs kett�s c�mz�s, duty cycle 2,
	//	general call �s nostretch letiltva.

	hi2c.Instance=I2C1;

	hi2c.Init.OwnAddress1=ownAddress;
	hi2c.Init.ClockSpeed=100000;
	hi2c.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode=I2C_DUALADDRESS_DISABLED;
	hi2c.Init.DutyCycle=I2C_DUTYCYCLE_2;
	hi2c.Init.NoStretchMode=I2C_NOSTRETCH_DISABLED;
	hi2c.Init.GeneralCallMode=I2C_GENERALCALL_DISABLED;

	HAL_I2C_MspInit(&hi2c);

	HAL_I2C_Init(&hi2c);

	return HAL_OK;
}

/** Regiszter olvas�sa I2C-n kereszt�l. */
HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{

	// A kapott param�terek �s a HAL_I2C_Mem_Read() f�ggv�ny seg�ts�g�vel beolvassuk a  "deviceAddress" eszk�z
	//	"registerAddress" regiszter�b�l (c�m hossza 1 byte) "dataSize" b�jtot a "pData" �ltal mutatott bufferbe.

	//HAL_I2C_Mem_Read(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);

	return HAL_I2C_Mem_Read(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);;
}

/** Regiszter �r�sa I2C-n kereszt�l. */
HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{

	// A param�terek �s a HAL_I2C_Mem_Write() f�ggv�ny seg�ts�g�vel elk�ldj�k az adatokat.
	// A mem�ria c�m m�ret�t byteban kell megadni (1 byte).

	//HAL_I2C_Mem_Write(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);

	return HAL_I2C_Mem_Write(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);;
}

