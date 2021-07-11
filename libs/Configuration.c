#include "main.h"



extern VDC_Struct SensorVoltageStruct;

Config_Struct* Config = (Config_Struct*)CONFIG_ADDRESS;

uint8_t flagCalib = 0;

uint8_t SaveCongig(Config_Struct* _Config)
{
  uint16_t* data = (uint16_t*)_Config;
  //Разблокируем флэш
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
  
  //Проверяем разблокировался ли флэш
  if(FLASH->CR & FLASH_CR_LOCK != 0)
  {
    //Если флэш заблокирован, то выходим с 0
    return 0;
  }
  
  //Присваиваем адресс в памяти переменной
  uint32_t configReg = CONFIG_ADDRESS;
  
  //Перед записью стираем область памяти 
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = CONFIG_ADDRESS;
  FLASH->CR |= FLASH_CR_STRT;
  
  uint16_t timeout = 10000;
  //Ждём коца стирания
  while(FLASH->SR & FLASH_SR_BSY == 0)
  {
    timeout--;
    if(!timeout)
    {
      //Если таймаут, то блокируем флэш и выходим с 0
      FLASH->CR &= ~FLASH_CR_PG;
      FLASH->CR |= FLASH_CR_LOCK;
      return 0;
    }
  }
  
  FLASH->CR &= ~FLASH_CR_PER;
  
  FLASH->CR |= FLASH_CR_PG;
  
  for(int i = 0; i < 9 ;i++)
  {
    //Сама запись
    *(__IO uint16_t*)configReg = *(uint16_t*)data++;
    configReg += 2;
    timeout = 10000;
    //Ждём коца записи
    while(FLASH->SR & FLASH_SR_BSY == 0)
    {
      timeout--;
      if(!timeout)
      {
        //Если таймаут, то блокируем флэш и выходим с 0
        FLASH->CR &= ~FLASH_CR_PG;
        FLASH->CR |= FLASH_CR_LOCK;
        return 0;
      }
    }
  }
  //Записываем флаг выполнения настройки
  *(__IO uint16_t*)configReg = 0x5555;
  //блокируем флэш
  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_LOCK;
  //Всё хорошо, возвращаем единицу
  return 1;
}

uint8_t SaveBright(uint16_t _Bright)
{
  Config_Struct _tempConfig = *Config;
  _tempConfig.Bright = _Bright;
  uint16_t* data = (uint16_t*)&_tempConfig;
  //Разблокируем флэш
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
  
  //Проверяем разблокировался ли флэш
  if(FLASH->CR & FLASH_CR_LOCK != 0)
  {
    //Если флэш заблокирован, то выходим с 0
    return 0;
  }
  
  //Присваиваем адресс в памяти переменной
  uint32_t configReg = CONFIG_ADDRESS;
  
  //Перед записью стираем область памяти 
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = CONFIG_ADDRESS;
  FLASH->CR |= FLASH_CR_STRT;
  
  uint16_t timeout = 10000;
  //Ждём коца стирания
  while(FLASH->SR & FLASH_SR_BSY == 0)
  {
    timeout--;
    if(!timeout)
    {
      //Если таймаут, то блокируем флэш и выходим с 0
      FLASH->CR &= ~FLASH_CR_PG;
      FLASH->CR |= FLASH_CR_LOCK;
      return 0;
    }
  }
  
  FLASH->CR &= ~FLASH_CR_PER;
  
  FLASH->CR |= FLASH_CR_PG;
  
  for(int i = 0; i < 10 ;i++)
  {
    //Сама запись
    *(__IO uint16_t*)configReg = *(uint16_t*)data++;
    configReg += 2;
    timeout = 10000;
    //Ждём коца записи
    while(FLASH->SR & FLASH_SR_BSY == 0)
    {
      timeout--;
      if(!timeout)
      {
        //Если таймаут, то блокируем флэш и выходим с 0
        FLASH->CR &= ~FLASH_CR_PG;
        FLASH->CR |= FLASH_CR_LOCK;
        return 0;
      }
    }
  }
  //блокируем флэш
  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_LOCK;
  //Всё хорошо, возвращаем единицу
  return 1;
}


Config_Struct biasCalibrate(void)
{
  flagCalib = 1;
  vTaskDelay(10);
  Config_Struct _tempConfig;
  //_tempConfig.Bright = Config->Bright;
  _tempConfig = *Config;
  uint8_t countReads = 100;
  int16_t bias1 = 0; 
  int16_t bias2 = 0;
  int16_t bias3 = 0;
  int16_t bias4 = 0;
  for(int i = 0; i < countReads; i++)
  {
    bias1 += SensorVoltageStruct.PDE1_VDC;
    bias2 += SensorVoltageStruct.PDE2_VDC;
    bias3 += SensorVoltageStruct.PDE3_VDC;
    bias4 += SensorVoltageStruct.PDE4_VDC;
    vTaskDelay(1);
  }
  bias1 /= countReads;
  bias2 /= countReads;
  bias3 /= countReads;
  bias4 /= countReads;
  _tempConfig.BIAS_PDE1 = bias1;
  _tempConfig.BIAS_PDE2 = bias2;
  _tempConfig.BIAS_PDE3 = bias3;
  _tempConfig.BIAS_PDE4 = bias4;
  flagCalib = 0;
  return _tempConfig;
}

Config_Struct ampfCalibrate(void)
{
  flagCalib = 1;
  vTaskDelay(10);
  Config_Struct _tempConfig;
  _tempConfig = *Config;
  uint8_t countReads = 100;
  int32_t ampf1 = 0; 
  int32_t ampf2 = 0;
  int32_t ampf3 = 0;
  int32_t ampf4 = 0;
  for(int i = 0; i < countReads; i++)
  {
    ampf1 += SensorVoltageStruct.PDE1_VDC;
    ampf2 += SensorVoltageStruct.PDE2_VDC;
    ampf3 += SensorVoltageStruct.PDE3_VDC;
    ampf4 += SensorVoltageStruct.PDE4_VDC;
    vTaskDelay(1);
  }
  ampf1 /= countReads;
  ampf2 /= countReads;
  ampf3 /= countReads;
  ampf4 /= countReads;
  ampf1 -= _tempConfig.BIAS_PDE1;
  ampf2 -= _tempConfig.BIAS_PDE2;
  ampf3 -= _tempConfig.BIAS_PDE3;
  ampf4 -= _tempConfig.BIAS_PDE4;
  int32_t meanVal = ampf1 + ampf2 + ampf3 + ampf4;
  meanVal /= 4;
  ampf1 *= 10000;
  ampf1 /= meanVal;
  ampf2 *= 10000;
  ampf2 /= meanVal;
  ampf3 *= 10000;
  ampf3 /= meanVal;
  ampf4 *= 10000;
  ampf4 /= meanVal;
  _tempConfig.AMPF_PDE1 = ampf1;
  _tempConfig.AMPF_PDE2 = ampf2;
  _tempConfig.AMPF_PDE3 = ampf3;
  _tempConfig.AMPF_PDE4 = ampf4;
  flagCalib = 0;
  return _tempConfig;
}
               