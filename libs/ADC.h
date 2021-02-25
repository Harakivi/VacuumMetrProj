#ifndef __ADC_H_
#define __ADC_H_

#define ADC_BUFFSIZE    8 //Размер буфера для какждого из каналов АЦП
#define CHAN_COUNT      5 //Количество каналов АЦП для считывания

//-----------------Структура содержащая усредненные значения напряжения с АЦП---------------------
typedef struct {
  int16_t PDE1_VDC;
  int16_t PDE2_VDC;
  int16_t PDE3_VDC;
  int16_t PDE4_VDC;
  uint16_t BATT_VDC;
}VDC_Struct;
//------------------------------------------------
void ADC1Init(void);
void DMA_ADCInit(void);
void ADCTIMinit(void);
//------------------------------------------------
#endif /* __ADC_H_ */
