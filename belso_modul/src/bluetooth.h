

//
#define set 1
#define reset 0


void btle_init();
HAL_StatusTypeDef btle_config_as_central();
HAL_StatusTypeDef btle_connect();
HAL_StatusTypeDef btle_get_temp();
void btle_factory_reset();

void bence_RxCpltCallback(UART_HandleTypeDef *huart);
