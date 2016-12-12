

//
#define set 1
#define reset 0

void wake_sw(int _set);

void btle_init();
void btle_factory_reset();
HAL_StatusTypeDef btle_config_as_client();

HAL_StatusTypeDef btle_temp_set_char(char *data);

void bence_RxCpltCallback(UART_HandleTypeDef *huart);
