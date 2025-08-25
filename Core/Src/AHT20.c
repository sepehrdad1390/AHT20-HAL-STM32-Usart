
#include "AHT20.h"

#define AHT20_address 0x70

void AHT20_init()
{
   HAL_Delay(200);
   uint8_t datain[3] = {0xBE,0x08,0x00};
   HAL_I2C_Master_Transmit(&AHT20_i2c,AHT20_address,datain,3,100);
   HAL_Delay(50);
}

HAL_StatusTypeDef AHT20_read(float *temp,float *hum)
{
  uint8_t datag[3] = {0xAC,0x33,0x00};
  uint8_t datar[7] = {0,0,0,0,0,0,0};
  HAL_I2C_Master_Transmit(&AHT20_i2c,AHT20_address,datag,3,100);
  HAL_Delay(82);
  HAL_I2C_Master_Receive(&AHT20_i2c,AHT20_address,datar,7,200); 
  
  if((datar[0] & 0x80)) return HAL_ERROR;
  
  uint8_t crc = 0xFF;
  for(int f = 0;f < 6;f ++)
  {
    crc ^= datar[f];
    for(int a =0 ;a < 8;a ++)
    {
       if(crc & 0x80) crc = (crc << 1) ^ 0x31;
       else crc <<= 1;
    }
  }
  
  if(crc != datar[6]) return HAL_ERROR;
 
  uint32_t tmp = 0,hmd = 0;
  hmd = datar[1];
  hmd <<= 8;
  hmd += datar[2];
  hmd <<= 4;
  hmd += (datar[3] >> 4);
  
  tmp = (datar[3] & 0x0F);
  tmp <<= 8;
  tmp += datar[4];
  tmp <<= 8;
  tmp += datar[5];
  
  *temp = (float)(tmp / (float)1048576) * 200 - 50; 
  *hum = (float)(hmd / (float)1048576) * 100;
  
  return HAL_OK;
}
