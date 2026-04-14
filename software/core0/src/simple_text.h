/*
 * simple_text.h
 *
 * Simple text rendering for VGA display
 */

#ifndef SIMPLE_TEXT_H_
#define SIMPLE_TEXT_H_

#include "xil_types.h"

/**
 * Draw a single character at position (x, y)
 * @param buffer: VGA buffer pointer
 * @param x: X position
 * @param y: Y position
 * @param character: Character to draw
 * @param color: Text color (0xRRGGBB)
 * @param scale: Size scale (1 = 8x8, 2 = 16x16, etc.)
 */
void draw_char(int* buffer, int x, int y,
               char character, uint32_t color, int scale);

/**
 * Draw a string at position (x, y)
 * @param buffer: VGA buffer pointer
 * @param x: X position
 * @param y: Y position
 * @param text: String to draw (max 100 chars)
 * @param color: Text color
 * @param scale: Size scale
 */
void draw_string(int* buffer, int x, int y,
                 const char* text, uint32_t color, int scale);

/**
 * Draw integer at position (x, y)
 * @param buffer: VGA buffer pointer
 * @param x: X position
 * @param y: Y position
 * @param number: Number to draw
 * @param color: Text color
 * @param scale: Size scale
 */
void draw_number(int* buffer, int x, int y,
                 u32 number, uint32_t color, int scale);

#endif /* SIMPLE_TEXT_H_ */
