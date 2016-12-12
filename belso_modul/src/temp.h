/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef temp_h
#define temp_h


/*------------------------ hõmérõ inicilaizálása ----------------------*/
HAL_StatusTypeDef Temp_Init();

/*----------------- I2C-s hõmérõ értékének lekérdezése ----------------*/
double Temp_GetTemperature();

//double konvertálása stringé a kiíratáshoz
char* tempToString(double temp, char *tempString);



#endif
