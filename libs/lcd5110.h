#ifndef _LCD5110_
#define _LCD5110_
//------------------------------------------------
#define DC_HIGH LCD_DC_PORT->ODR |= LCD_DC_PIN /// DATA/COMMAND SET
#define DC_LOW LCD_DC_PORT->ODR &= ~LCD_DC_PIN /// DATA/COMMAND RESET
#define RST_HIGH LCD_RST_PORT->ODR |= GPIO_ODR_ODR2 /// RST SET
#define RST_LOW LCD_RST_PORT->ODR &= ~GPIO_ODR_ODR2 /// RST RESET
#define CS_HIGH LCD_CS_PORT->ODR |= GPIO_ODR_ODR4 /// CHIP SELECT SET
#define CS_LOW LCD_CS_PORT->ODR &= ~GPIO_ODR_ODR4 /// CHIP SELECT RESET
//------------------------------------------------
#define BRIGHT TIM3->CCR3///< Brightness
//------------------------------------------------
#define BLACK 1 ///< Black pixel
#define WHITE 0 ///< White pixel
//------------------------------------------------
#define LCDWIDTH 84   ///< LCD is 84 pixels wide
#define LCDHEIGHT 48  ///< 48 pixels high
//------------------------------------------------
#define PCD8544_POWERDOWN 0x04            ///< Function set, Power down mode
#define PCD8544_ENTRYMODE 0x02            ///< Function set, Entry mode
#define PCD8544_EXTENDEDINSTRUCTION 0x01  ///< Function set, Extended instruction set control
//------------------------------------------------
#define PCD8544_DISPLAYBLANK 0x0      ///< Display control, blank
#define PCD8544_DISPLAYNORMAL 0x4     ///< Display control, normal mode
#define PCD8544_DISPLAYALLON 0x1      ///< Display control, all segments on
#define PCD8544_DISPLAYINVERTED 0x5   ///< Display control, inverse mode
//------------------------------------------------
#define PCD8544_FUNCTIONSET 0x20      ///< Basic instruction set
#define PCD8544_DISPLAYCONTROL 0x08   ///< Basic instruction set - Set display configuration
#define PCD8544_SETYADDR 0x40         ///< Basic instruction set - Set Y address of RAM, 0 <= Y <= 5
#define PCD8544_SETXADDR 0x80         ///< Basic instruction set - Set X address of RAM, 0 <= X <= 83
//------------------------------------------------
#define PCD8544_SETTEMP 0x04          ///< Extended instruction set - Set temperature coefficient
#define PCD8544_SETBIAS 0x10          ///< Extended instruction set - Set bias system
#define PCD8544_SETVOP 0x80           ///< Extended instruction set - Write Vop to register
//------------------------------------------------
void DISP_Init();
void DISP_command(int data);
void DISP_data(uint8_t data);
void DISP_Send_DMA(uint8_t* _VBUF);
void DISP_setBias(int val);
void DISP_setContrast(int val);
void DISP_Send(uint8_t* _VBUF);
void DISP_SetPos(uint8_t xPos, uint8_t yPos);
void DISP_Clear(void);
void DISP_Clear_DMA(void);
void gpioInit(void);
void SPI2Init(void);
void bltInit(void);
//------------------------------------------------
#endif /* _LCD5110_ */