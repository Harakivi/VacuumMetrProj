#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
/*---------------------DISPLAY_BUFF Defines------------------*/

#define CONFIG_ADDRESS 0x800f800 //BANK31

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
  uint32_t dimmerOff;
  uint32_t deviceOff;
  uint16_t CalibComplete;
}Config_Struct;

uint8_t SaveCongig(Config_Struct* _Config);
uint8_t SaveDispSets(DISP_SETS_Struct* dispSets);
Config_Struct biasCalibrate(void);
Config_Struct ampfCalibrate(void);

#endif /* _CONFIGURATION_H_ */