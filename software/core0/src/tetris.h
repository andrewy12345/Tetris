#pragma once

#include "xil_types.h"

// defines for vga
#define H_RESOLUTION 		1280
#define V_RESOLUTION 		1024
#define PIXELS_PER_FRAME 	H_RESOLUTION*V_RESOLUTION
#define BYTES_PER_FRAME 	PIXELS_PER_FRAME*4

// defines for drawing squares
#define STARTING_V_POS 12
#define STARTING_H_POS 390
#define SQUARE_LENGTH 50

// colors
#define WHITE 0x00FFFFFF
#define GREY 0x00888888
#define BLACK 0x00000000
#define BLUE 0x00FF0000
#define GREEN 0x0000FF00
#define RED 0x000000FF
#define CYAN 0x00FFFF00

#define BG_COLOR_MAIN_MENU 0X00FF8300
#define BG_COLOR_PAUSE 0X00FF8300


