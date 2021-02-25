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
}Config_Struct;

uint8_t SaveCongig(Config_Struct* _Config);
Config_Struct biasCalibrate(void);

#endif /* _CONFIGURATION_H_ */