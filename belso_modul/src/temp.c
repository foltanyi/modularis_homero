#include "main.h"

/* ------------------- Belsõ, globális változókk --------------------- */

//temp sensor slave address
const uint8_t TempAddr = (uint8_t)0b1001000<<1;
//temp sensor data register select
const uint8_t TempPtrRegRead = (uint8_t)0x00;
//temp sensor config register select
uint8_t TempPtrRegConfig = (uint8_t)0x01;
//temp sensor config data
uint8_t TempConfig = (uint8_t)0x04;


/*---------------------------- hõmérõ inicializálása ----------------------*/
HAL_StatusTypeDef Temp_Init()
{
	// 7 bit slave address: 1001 A2 A1 A0 = 1001000 = 0xA0
	HAL_StatusTypeDef result = I2C_Init(0xA0);
	if (result != HAL_OK)
	{
		return result;
	}

	// Init

	result = I2C_WriteRegister(TempAddr, TempPtrRegConfig, &TempConfig, 1);
	if (result != HAL_OK)
	{
		return result;
	}

	return HAL_OK;
}

uint8_t data[2];




/*------------------- I2C-s hhõmérséklet lekérdezése-----------------*/
double Temp_GetTemperature()
{
	double temperature;


	if (I2C_ReadRegister(TempAddr, TempPtrRegRead, data, 2) != HAL_OK)
	{
		return 0xFF;
	}

	//a temp sensor a homersekletet 9 biten kettes komplemens formaba tarolja, LSB=0.5°C
	// -63° es 63° kozott az also 8 bit eleg a homerseklet tarolasahoz ezert csak ezt hasznaljuk



	if((data[0] & 0x80) == 0x80)	//ha negatív
	{
		data[0] =  ~ data[0];
		data[0] += 1;
		temperature = 0 - data[0];

		if(data[1] == 128)		//fél fok
			temperature -= 0.5;
	}
	else							//ha pozitív
	{
		temperature = data[0];

		if(data[1] == 128)		//fél fok
			temperature += 0.5;
	}


	return temperature;
}

//double konvertálása stringé a kiíratáshoz
char* tempToString(double temp, char *tempString)
{

	//negativ szam
	if(temp<0)
	{
		//3 számjegyû a hõmérséklet
		if ((int) (temp / 100) != 0) {
			tempString[0] = 45;		//minusz jel
			tempString[1] = (int) (temp / 100 + 48);
			tempString[2] = (int) (temp - ((int) (temp / 100) * 100)) / 10 + 48;
			tempString[3] = (int) (temp - (int) (temp / 10) * 10) + 48;
			tempString[4] = '.';
			tempString[5] = (temp - (int) temp) * 10 + 48;
			tempString[6] = 178;	//fok jel
			tempString[7] = 'C';
			tempString[8] = '\0';
			return tempString;
		}

		else {
			//2 számjegyû a hõmérséklet
			if ((int) (temp - ((int) (temp / 100) * 100)) / 10 != 0) {
				tempString[0] = 45;		//minusz jel
				tempString[1] = (int) (temp - ((int) (temp / 100) * 100)) / 10+ 48;
				tempString[2] = (int) (temp - (int) (temp / 10) * 10) + 48;
				tempString[3] = '.';
				tempString[4] = (temp - (int) temp) * 10 + 48;
				tempString[5] = 178;	//fok jel
				tempString[6] = 'C';
				tempString[7] = '\0';
				return tempString;
			}
			//1 számjegyû a hõmérséklet
			else {
				tempString[0] = 45;		//minusz jel
				tempString[1] = (int) (temp - (int) (temp / 10) * 10) + 48;
				tempString[2] = '.';
				tempString[3] = (temp - (int) temp) * 10 + 48;
				tempString[4] = 178;	//fok jel
				tempString[5] = 'C';
				tempString[6] = '\0';
				return tempString;
			}

		}	//2 számjegyû a hõmérséklet vege
	}	//negativ szam vege



	//pozitiv szam
	else {
		//3 számjegyû a hõmérséklet
		if ((int) (temp / 100) != 0) {
			tempString[0] = (int) (temp / 100 + 48);
			tempString[1] = (int) (temp - ((int) (temp / 100) * 100)) / 10 + 48;
			tempString[2] = (int) (temp - (int) (temp / 10) * 10) + 48;
			tempString[3] = '.';
			tempString[4] = (temp - (int) temp) * 10 + 48;
			tempString[5] = 178;	//fok jel
			tempString[6] = 'C';
			tempString[7] = '\0';
			return tempString;
		}

		else {
			//2 számjegyû a hõmérséklet
			if ((int) (temp - ((int) (temp / 100) * 100)) / 10 != 0) {

				tempString[0] = (int) (temp - ((int) (temp / 100) * 100)) / 10
						+ 48;
				tempString[1] = (int) (temp - (int) (temp / 10) * 10) + 48;
				tempString[2] = '.';
				tempString[3] = (temp - (int) temp) * 10 + 48;
				tempString[4] = 178;	//fok jel
				tempString[5] = 'C';
				tempString[6] = '\0';
				return tempString;
			}
			//1 számjegyû a hõmérséklet
			else {
				tempString[0] = (int) (temp - (int) (temp / 10) * 10) + 48;
				tempString[1] = '.';
				tempString[2] = (temp - (int) temp) * 10 + 48;
				tempString[3] = 178;	//fok jel
				tempString[4] = 'C';
				tempString[7] = '\0';
				return tempString;
			}

		}//2 számjegyû a hõmérséklet vege
	}//pozitiv szam vege
}//tempToString vege



