#pragma once

#include <assert.h>
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <time.h>


#define RASTER_WIDTH 256
#define RASTER_HEIGHT 256
#define NUM_PIXELS ((RASTER_WIDTH * RASTER_HEIGHT))

const int scale = 16; // 1/2^16
const int FractionMask = 0xFFFFFFFF >> (32 - scale);
const int WholeMask = -1 ^ FractionMask;

#define DOUBLETOFIXED(x) ((x) * (double)(1 << scale))
#define FIXEDTODOUBLE(x) ((double)(x) / (double)(1<<scale))
#define INTTOFIXED(x) ((x) << scale)
#define FIXEDTOINT(x) ((x) >> scale)
#define FRACTIONALPART(x) ((x) & FractionMak)
#define WHOLEPART(x) ((x) & WholeMask)
#define MUL(x, y) ((((x)>>8) * ((y)>>8)) >> 0)
#define DIV(x, y) (((x)<<7)/(y)<<9) //numbers must equal 16

#define PI		3.14159f
#define EPSILON 0.00001f

#define Degree_to_Radian(deg_r) (((deg_r) * PI) / 180.0f))
#define NDC_to_Screen_X(ndcX) (((ndcX) + 1.0f) * ((RASTER_WIDTH) >> 1))
#define NDC_to_Screen_Y(ndcY) ((1.0f - (ndcY)) * ((RASTER_HEIGHT) >> 1))

#define AspectRatio (((float)RASTER_WIDTH) / (RASTER_HEIGHT))

#define Red 0xFFFF0000
#define Green 0xFF00FF00
#define Blue 0xFF0000FF

#define Black 0x00000000
#define White 0xFFFFFFFF

#define Cyan 0xFF00FFFF
#define Magenta 0xFFFF00FF
#define Yellow 0xFFFFFF00

#define Limt 0XFF00FF00
#define Fuchia 0XFFFF00FF
#define Silver 0XFFC0C0C0
#define Gray 0XFF808080
#define Maroon 0XFF800000
#define Olive 0XFF808000
#define Purple 0XFF800080
#define Teal 0XFF008080
#define NavyBlue 0XFF000080