/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef temp_h
#define temp_h


/*------------------------ h�m�r� inicilaiz�l�sa ----------------------*/
HAL_StatusTypeDef Temp_Init();

/*----------------- I2C-s h�m�r� �rt�k�nek lek�rdez�se ----------------*/
double Temp_GetTemperature();

//double konvert�l�sa string� a ki�rat�shoz
char* tempToString(double temp, char *tempString);



#endif
