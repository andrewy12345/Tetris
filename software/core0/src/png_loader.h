
#ifndef PNG_LOADER_OPTIMIZED_H_
#define PNG_LOADER_OPTIMIZED_H_

#include "xil_types.h"
#include <string.h>

/* ---------------------------------------------------------------------------- *
 * 						Screen Rendering with Background Color				*
 * ---------------------------------------------------------------------------- */

/**
 * Fill entire screen with solid background color (fast!)
 *
 * @param vga_buffer    Pointer to VGA buffer or working buffer
 * @param color         BGR0 color value (0x00BBGGRR format)
 * @param pixel_count   Number of pixels (usually 1280*1024)
 *
 * Example:
 *   int* screen_buffer = (int*)0x10200000;
 *   fill_screen(screen_buffer, 0x00000000, 1280*1024);  // Black background
 *
 * Time: ~10-50 ms for 1280x1024 (fast memset)
 */
static inline void fill_screen(int* vga_buffer, uint32_t color, unsigned int pixel_count)
{
    for (unsigned int i = 0; i < pixel_count; i++) {
        vga_buffer[i] = color;
    }
}

/* ---------------------------------------------------------------------------- *
 * 					Blit BGR0 Object onto Screen (Compositing)				*
 * ---------------------------------------------------------------------------- *
 * Copy a small BGR0 image (like text or button) onto screen at position
 *
 * Typical use:
 *   - Text: 400x60 pixels = 24 KB
 *   - Button: 200x50 pixels = 40 KB
 *   - Logo: 300x200 pixels = 240 KB
 *
 * Much smaller than full 5.24 MB screen!
 */

/**
 * Blit BGR0 object onto screen at specific position
 *
 * @param vga_buffer    Pointer to screen buffer
 * @param object        Pointer to BGR0 object data
 * @param screen_width  Screen width in pixels (1280)
 * @param obj_width     Object width in pixels
 * @param obj_height    Object height in pixels
 * @param x_pos         X position on screen
 * @param y_pos         Y position on screen
 *
 * Example:
 *   // Blit button at screen position (640, 400)
 *   blit_object(screen_buffer, button_bgr0,
 *               1280, 200, 50, 640, 400);
 *
 * This copies only the small button object, not the full screen!
 */
static inline void blit_object(int* vga_buffer,
                              const uint32_t* object,
                              unsigned int screen_width,
                              unsigned int obj_width,
                              unsigned int obj_height,
                              unsigned int x_pos,
                              unsigned int y_pos)
{
    for (unsigned int y = 0; y < obj_height; y++) {
        // Calculate screen position: (y + y_pos) * screen_width + x_pos
        unsigned int screen_offset = (y + y_pos) * screen_width + x_pos;

        // Calculate object position: y * obj_width
        unsigned int obj_offset = y * obj_width;

        // Copy one row of the object
        memcpy(&vga_buffer[screen_offset],
                       &object[obj_offset],
                       obj_width * sizeof(uint32_t));
    }
}

#endif
