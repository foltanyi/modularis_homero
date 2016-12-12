/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef i2c_h
#define i2c_h



/** I2C kommunik�ci� inicializ�l�sa. */
HAL_StatusTypeDef I2C_Init(uint32_t ownAddress);


/** Regiszter olvas�sa I2C-n kereszt�l. */
HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize);


/** Regiszter �r�sa I2C-n kereszt�l. */
HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize);




#endif
