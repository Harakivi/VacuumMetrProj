#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
/*---------------------DISPLAY_BUFF Defines------------------*/

#define CONFIG_ADDRESS 0x800f800 //BANK31

#pragma pack(push, 1)
typedef struct {
  int16_t BIAS_PDE1;
  uint16_t AMPF_PDE1;
  int16_t BIAS_PDE2;
  uint16_t AMPF_PDE2;
  int16_t BIAS_PDE3;
  uint16_t AMPF_PDE3;
  int16_t BIAS_PDE4;
  uint16_t AMPF_PDE4;
  uint16_t Bright;
  uint16_t CalibComplete;
  uint32_t WorkingTime9V;
  uint32_t WorkingTime8V;
  uint32_t WorkingTime7V;
  uint32_t WorkingTime6V;
  uint32_t WorkingTime5V;
  uint32_t WorkingTime4V;
  uint32_t WorkingTime3_5V;
}Config_Struct;
#pragma pack(pop)

uint8_t SaveCongig(Config_Struct* _Config);
uint8_t SaveBright(uint16_t _Bright);
uint8_t SaveWorkingTime(uint32_t workingTime, uint16_t voltage);
Config_Struct biasCalibrate(void);
Config_Struct ampfCalibrate(void);

#endif /* _CONFIGURATION_H_ */