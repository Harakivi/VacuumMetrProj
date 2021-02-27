#ifndef _DISP_BUFF_H_
#define _DISP_BUFF_H_
/*---------------------DISPLAY_BUFF Defines------------------*/
#define DISP_X_SIZE             83 //Количество пикселей по горизонтали
#define DISP_Y_SIZE             47 //Количество пикселей по вертикали      

void DISP_Update();

void VBUF_Clear(void);

void VBUF_Draw_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length);
void VBUF_Clear_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length);
void VBUF_Invert_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length);
void VBUF_Draw_Image(uint8_t xBegin, uint8_t yBegin, uint8_t length, uint8_t Height, uint8_t *Image);
void VBUF_Write_Char(uint8_t xBegin, uint8_t yBegin, char characters);
void VBUF_Write_String(uint8_t xBegin, uint8_t yBegin, char *characters);

#endif /* _DISP_BUFF_H_ */