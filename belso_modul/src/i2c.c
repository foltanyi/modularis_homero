#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"

//#include "bsp_globalfunctions.h"
#include "i2c.h"

/* ----------------- Belsõ, globális változók ------------------- */

// A tényleges I2C periféria beállítására és hivatkozására használt struct.
I2C_HandleTypeDef hi2c;

const uint32_t i2c_timeout = 5000;

/* ----------------- Publikus függvények ------------------------ */

/** I2C kommunikáció inicializálása. */
HAL_StatusTypeDef I2C_Init(uint32_t ownAddress)
{

	//i2c gpio portok inicializálása
	__GPIOB_CLK_ENABLE();
	__I2C1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Alternate=GPIO_AF4_I2C1;
	GPIO_init.Mode=GPIO_MODE_AF_OD;
	GPIO_init.Pin=GPIO_PIN_7 | GPIO_PIN_6;
	GPIO_init.Pull=GPIO_PULLUP;
	GPIO_init.Speed=GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOB,&GPIO_init);


	// A hi2c handler struct Instance és Init mezõit kitöltve, majd a HAL_I2C_Init() függvényt
	// meghívva inicializálja az I2C1 perifériát
	// 7 bites címzés, clock speed 100000, nincs kettõs címzés, duty cycle 2,
	//	general call és nostretch letiltva.

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

/** Regiszter olvasása I2C-n keresztül. */
HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{

	// A kapott paraméterek és a HAL_I2C_Mem_Read() függvény segítségével beolvassuk a  "deviceAddress" eszköz
	//	"registerAddress" regiszterébõl (cím hossza 1 byte) "dataSize" bájtot a "pData" által mutatott bufferbe.

	//HAL_I2C_Mem_Read(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);

	return HAL_I2C_Mem_Read(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);;
}

/** Regiszter írása I2C-n keresztül. */
HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{

	// A paraméterek és a HAL_I2C_Mem_Write() függvény segítségével elküldjük az adatokat.
	// A memória cím méretét byteban kell megadni (1 byte).

	//HAL_I2C_Mem_Write(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);

	return HAL_I2C_Mem_Write(&hi2c,deviceAddress,registerAddress,1,pData,dataSize,i2c_timeout);;
}

