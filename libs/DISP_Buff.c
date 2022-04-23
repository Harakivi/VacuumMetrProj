#include "main.h"
#include <string.h>

const uint8_t ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};

const uint8_t bender[84 * 48 / 8] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x10, 0xF0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x40, 0x40, 0x00, 0x40, 0x50, 0x51, 0x50, 0x50, 0x5C, 0x50,
  0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x84, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFC, 0x00, 0x01, 0x00,
  0x00, 0x88, 0x25, 0x21, 0x01, 0x01, 0x01, 0x01, 0x01, 0x05, 0x8D, 0x05, 0x00, 0x00, 0x20, 0x00,
  0x00, 0x04, 0x9C, 0xFD, 0xFD, 0xF8, 0xF2, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x80,
  0x20, 0x41, 0x52, 0xF2, 0x16, 0x14, 0x15, 0x14, 0x14, 0xE4, 0x84, 0x84, 0x84, 0x85, 0x05, 0xE4,
  0x04, 0x04, 0xE4, 0x14, 0x14, 0x05, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0x1F, 0x1D, 0x33, 0x24, 0x00, 0x0F, 0x0A, 0x4A, 0x4A, 0x40, 0x40, 0x4F, 0x40, 0x40,
  0x40, 0x40, 0x40, 0x0F, 0x0A, 0x0A, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

uint8_t VBUF[84 * 48 / 8] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

//width 6 / height 32
const uint8_t chel_1 [] = {
0x00, 0x00, 0x00, 0x7c, 0x82, 0x01, 0x01, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x20,
0xf9, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x78, 0x8f, 0x88, 0x73, 0x06, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x1f, 0x70, 0x80, 0x00, 0x00   // 
};

//size: 11x32
const uint8_t chel_2[] = {
0x00, 0x00, 0x00, 0x7c, 0x82, 0x01, 0x01, 0x82, 0x7c, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x08, 0x08, 
0xc5, 0x3f, 0x04, 0x04, 0x08, 0xf0, 0x06, 0x81, 0x70, 0x08, 0x08, 0x07, 0x08, 0x08, 0xf0, 0x00,
0x03, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00   // 
};

//size: 23x32
const uint8_t chel_smile[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x84, 0x62, 0x42, 0x59, 0x41, 0x41, 0x59, 0x42, 0x62, 
0x84, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x09,
0x09, 0x0a, 0xca, 0x3e, 0x0a, 0x09, 0x09, 0x08, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x70, 0x08, 0x08, 0x07, 0x08, 0x08, 0xf0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // 
};

//size: 16x24
const uint8_t H_symb [] = {
0x00, 0x00, 0x00, 0xc0, 0xfc, 0xff, 0x7f, 0x03, 0x00, 0x00, 0x00, 0xc0, 0xfc, 0xff, 0x7f, 0x03, 
0x00, 0x00, 0xf8, 0xff, 0xff, 0x3f, 0x38, 0x38, 0x70, 0x70, 0xf8, 0xff, 0x7f, 0x07, 0x00, 0x00,
0x70, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00   // 
};

//size: 16x24
const uint8_t A_symb [] = {
0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xf8, 0x7c, 0x1e, 0x0f, 0x07, 0x07, 0x0f, 0x1f, 0xfe, 0xfc,
0xc0, 0xc0, 0xe0, 0xfc, 0xff, 0xff, 0xc3, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xf8, 0xff, 0xff, 0x87,
0x61, 0xff, 0xff, 0x1f, 0x03, 0x01, 0x01, 0x01, 0x03, 0x03, 0xfb, 0xff, 0xff, 0x07, 0x03, 0x03   // 
};

//size: 16x24
const uint8_t R_symb [] = {
0x00, 0x00, 0x00, 0xe0, 0xfe, 0xff, 0x1f, 0x07, 0x07, 0x0f, 0x0e, 0x1e, 0x7c, 0xf8, 0xf0, 0xc0,
0x00, 0x80, 0xfc, 0xff, 0xff, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xf0, 0x78, 0x3f, 0x1f, 0x0f,
0xe0, 0xff, 0xff, 0x1f, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3d, 0xf8, 0xf0, 0xe0, 0xc0, 0x00, 0x00   // 
};

//size: 16x24
const uint8_t K_symb [] = {
0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x80, 0xc0, 0xf0, 0xf8, 0x7e, 0x1f,
0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0xfc, 0xfe, 0xef, 0xc7, 0x07, 0x03, 0x01, 0x00, 0x00,
0xc0, 0xf0, 0xfe, 0x3f, 0x0f, 0x01, 0x00, 0x00, 0x01, 0x03, 0x07, 0x1f, 0x7e, 0xf8, 0xe0, 0xc0   // 
};

//size: 16x24
const uint8_t I_symb [] =  {
0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x07, 0x07, 0x87, 0xff, 0xff, 0x7f, 0x0f, 0x0e, 0x0e, 0x0e,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xfc, 0xff, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x60, 0x70, 0x70, 0x70, 0x70, 0xfe, 0xff, 0xff, 0xe3, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00   // 
};

//size: 16x24
const uint8_t V_symb [] = {
0x1c, 0x7c, 0xf8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf8, 0xff, 0x7f, 0x07,
0x00, 0x00, 0x07, 0x7f, 0xff, 0xfc, 0x80, 0x00, 0xc0, 0xf0, 0xfc, 0x7f, 0x1f, 0x07, 0x00, 0x00,
0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x7f, 0x1f, 0x0f, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00   // 
};

//size: 12x19
const uint8_t b_symb [] = {
0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFE, 0xFF, 0x3F, 0x06, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFC, 0xFF,
0x3F, 0x07, 0x07, 0x07, 0x8F, 0xFE, 0xFE, 0x78, 0x06, 0x07, 0x07, 0x07, 
};

//size: 12x19
const uint8_t y_symb [] = {
0x00, 0x1E, 0x7F, 0xFE, 0xF0, 0x80, 0x00, 0x00, 0xFE, 0xFF, 0xFE, 0x00, 0x00, 0x80, 0xC1, 0xEF,
0xFF, 0x7E, 0x3E, 0x1F, 0x0F, 0x01, 0x0E, 0x0F, 0x0F, 0x03, 0x01, 
};

//size: 5x8
const uint8_t ArrowUP[] = {
0x0c, 0x03, 0xff, 0x03, 0x0c   // Стрелка вверх
};

//size: 5x8
const uint8_t ArrowDOWN[] = {
0x30, 0xc0, 0xff, 0xc0, 0x30   // Стрелка вниз
};

//size: 10x8
const uint8_t fullBATT [] = {
0x1C, 0x77, 0x41, 0x5D, 0x41, 0x5D, 0x41, 0x5D, 0x41, 0x7F,   // Символ полной батареи
};

//size: 10x8
const uint8_t midBATT [] = {
0x1C, 0x77, 0x41, 0x41, 0x41, 0x5D, 0x41, 0x5D, 0x41, 0x7F, // Символ средней батареи
};

//size: 10x8
const uint8_t lowBATT [] = {
0x1C, 0x77, 0x41, 0x41, 0x41, 0x41, 0x41, 0x5D, 0x41, 0x7F, // Символ низкой батареи
};

//size: 10x8
const uint8_t emptyBATT [] = {
0x1C, 0x77, 0x41, 0x63, 0x55, 0x49, 0x55, 0x63, 0x41, 0x7F, // Символ разряженной батареи
};

//Передать видео буфер в дисплей
void DISP_Update()
{
  DISP_Send_DMA(VBUF);
}

//Очистить весь буфер
void VBUF_Clear()
{
  memset(VBUF, 0 ,84 * 48 / 8); 
//  for(uint8_t yCurr = 0; yCurr < DISP_Y_SIZE; yCurr ++)
//  {
//    for(uint8_t xCurr = 0; xCurr < DISP_X_SIZE; xCurr++)
//    {
//      VBUF[((yCurr/8) * 84) + xCurr] &= ~(1 << (yCurr % 8));
//    }
//  }
}



//Нарисовать линию с определенной высотой и длинной
void VBUF_Draw_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(xBegin > DISP_X_SIZE || yBegin > DISP_Y_SIZE)return;
  
  for(uint8_t yCurr = height > 0 ? yBegin - height : yBegin  ; yCurr < (height > 0 ? yBegin  : yBegin - height)  ; yCurr ++)
  {
    if(yCurr > DISP_Y_SIZE)break;
    for(uint8_t xCurr = xBegin; xCurr < xBegin + length; xCurr++)
    {
      if(xCurr > DISP_X_SIZE)break;
      VBUF[((yCurr/8) * 84) + xCurr] |= 1 << (yCurr & 8 - 1);
    }
  }
}

void VBUF_Draw_Pixel(uint8_t x, uint8_t y, uint8_t pixel)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(x > DISP_X_SIZE || y > DISP_Y_SIZE)return;
  if(!pixel)
  {
    VBUF[((y/8) * 84) + x] &= ~(1 << (y & 8 - 1));
  }
  else
  {
    VBUF[((y/8) * 84) + x] |= 1 << (y & 8 - 1);
  }
}

//Очистить линию с определенной высотой и длинной
void VBUF_Clear_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(xBegin > DISP_X_SIZE || yBegin > DISP_Y_SIZE)return;
  
  for(uint8_t yCurr = height > 0 ? yBegin - height : yBegin  ; yCurr < (height > 0 ? yBegin  : yBegin - height)  ; yCurr ++)
  {
    if(yCurr > DISP_Y_SIZE)break;
    for(uint8_t xCurr = xBegin; xCurr < xBegin + length; xCurr++)
    {
      if(xCurr > DISP_X_SIZE)break;
      VBUF[((yCurr/8) * 84) + xCurr] &= ~(1 << (yCurr & 8 - 1));
    }
  }
}

//Инвертировать линию с определенной высотой и длинной
void VBUF_Invert_Line(uint8_t xBegin, uint8_t yBegin, int8_t height, uint8_t length)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(xBegin > DISP_X_SIZE || yBegin > DISP_Y_SIZE)return;
  
  for(uint8_t yCurr = height > 0 ? yBegin - height : yBegin  ; yCurr < (height > 0 ? yBegin  : yBegin - height)  ; yCurr ++)
  {
    if(yCurr > DISP_Y_SIZE)break;
    for(uint8_t xCurr = xBegin; xCurr < xBegin + length; xCurr++)
    {
      if(xCurr > DISP_X_SIZE)break;
      if(VBUF[((yCurr/8) * 84) + xCurr] & (1 << (yCurr & 8 - 1)))
      {
        VBUF[((yCurr/8) * 84) + xCurr] &= ~(1 << (yCurr & 8 - 1));
      }
      else
      {
        VBUF[((yCurr/8) * 84) + xCurr] |= 1 << (yCurr & 8 - 1);
      }
    }
  }
}

//Отрисовка изображения по определенным координатам дисплея
void VBUF_Draw_Image(int16_t xBegin, int16_t yBegin, int16_t length, int16_t Height, uint8_t *Image)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(xBegin > DISP_X_SIZE || yBegin > DISP_Y_SIZE)return;
  
  for(int16_t yCurr = yBegin < 0 ? 0 : yBegin; yCurr < yBegin + Height; yCurr ++)
  {
    if(yBegin < 0)
    {
      yCurr = 0;
    }
    if(yCurr > DISP_Y_SIZE)break;
    for(int16_t xCurr = xBegin < 0 ? 0 : xBegin ; xCurr < xBegin + length; xCurr++)
    {
      if(xCurr > DISP_X_SIZE)break;
      if(Image[(((yCurr - yBegin)/8) * length) + xCurr - xBegin] & (1 << ((yCurr - yBegin) & 8 - 1)))
      {
        VBUF[((yCurr/8) * 84) + xCurr] |= 1 << (yCurr & 8 - 1);
      }
      else
      {
        VBUF[((yCurr/8) * 84) + xCurr] &= ~(1 << (yCurr & 8 - 1));
      }
    }
  }
}

//Запись Char по определенным координатам дисплея
void VBUF_Write_Char(uint8_t xBegin, uint8_t yBegin, char characters)
{
  //Проверяем входные данные, чтобы не выйти за границы дисплея
  if(xBegin > DISP_X_SIZE || yBegin > DISP_Y_SIZE)return;
  
  for(uint8_t yCurr = yBegin; yCurr < yBegin + 8; yCurr ++)
  {
    if(yCurr > DISP_Y_SIZE)break;
    for(uint8_t xCurr = xBegin; xCurr < xBegin + 5; xCurr++)
    {
      if(xCurr > DISP_X_SIZE)break;
      if(ASCII[characters - 0x20][xCurr - xBegin] & (1 << ((yCurr - yBegin) & 8 - 1)))
      {
        VBUF[((yCurr/8) * 84) + xCurr] |= 1 << (yCurr & 8 - 1);
      }
      else
      {
        VBUF[((yCurr/8) * 84) + xCurr] &= ~(1 << (yCurr & 8 - 1));
      }
      if(xCurr == xBegin + 4 && xCurr + 1 <= DISP_X_SIZE)
      {
        VBUF[((yCurr/8) * 84) + xCurr + 1] &= ~(1 << (yCurr & 8 - 1));
      }
    }
  }
}

//Запись строки по определенным координатам дисплея слева на право
void VBUF_Write_String(uint8_t xBegin, uint8_t yBegin, char *characters)
{
  while(*characters)
  {
    VBUF_Write_Char(xBegin, yBegin,*characters++);
    xBegin += 6;
  }
}

//Запись строки по определенным координатам дисплея справа на лево
void VBUF_Write_String_Right(uint8_t xBegin, uint8_t yBegin, char *characters)
{
  uint8_t smb_cnt = 0;
  while(*characters)
  {
    smb_cnt++;
    characters++;
  }
  while(smb_cnt--)
  {
    VBUF_Write_Char(xBegin, yBegin,*--characters);
    xBegin -= 6;
  }
}