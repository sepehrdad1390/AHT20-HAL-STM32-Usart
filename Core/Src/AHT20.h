
#include "main.h"


#define AHT20_i2c hi2c1
extern I2C_HandleTypeDef AHT20_i2c;


void AHT20_init();
HAL_StatusTypeDef AHT20_read(float *temp,float *hum);
