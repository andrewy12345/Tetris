/*
 * png_loader.cc
 */

#include "png_loader.h"
#include <string.h>
#include "xil_printf.h"
#include <stdlib.h>

#include "lodepng.h"

/* ============================================================================ *
 * 					Load PNG Image from Pre-Decoded Array					*
 * ============================================================================ *
 * Pattern: Same as background_music_player()
 *   1. Define image array in header: unsigned char menu_main_png[]
 *   2. Call load_png_image() to copy to target memory
 *   3. Use memcpy for fast bulk copy (no runtime processing)
 * ============================================================================ */
int load_png_image(const unsigned char* image_data,
                   unsigned int image_size,
                   void* target_addr)
{
    // Validate input
    if (!image_data || !target_addr) {
        xil_printf("  ERROR: NULL pointer passed to load_png_image()\r\n");
        return -1;
    }

    // Expected size for 1280x1024 image in BGR0 format
    // 1280 * 1024 * 4 bytes per pixel = 5,242,880 bytes
    const unsigned int EXPECTED_SIZE = 1280 * 1024 * 4;

    if (image_size != EXPECTED_SIZE) {
        xil_printf("  ERROR: Image size mismatch\r\n");
        xil_printf("         Expected: %u bytes (1280x1024x4)\r\n", EXPECTED_SIZE);
        xil_printf("         Got:      %u bytes\r\n", image_size);
        return -1;
    }

    xil_printf("  Loading image (%u bytes)...\r\n", image_size);

    memcpy(target_addr, image_data, image_size);

    xil_printf("  Image loaded successfully\r\n");

    return 0;  // Success
}

/* ============================================================================ *
 * 				Load PNG Image using int* pointer (Alternative)				*
 * ============================================================================ *
 * Same as load_png_image() but uses int* pointer casting.
 * Makes it explicit that we're copying 32-bit pixel data.
 * ============================================================================ */
int load_png_image_ptr(const int* image_data,
                       unsigned int image_size,
                       int* target_addr)
{
    // Validate input
    if (!image_data || !target_addr) {
        xil_printf("  ERROR: NULL pointer passed to load_png_image_ptr()\r\n");
        return -1;
    }

    // Expected size for 1280x1024 image in BGR0 format
    const unsigned int EXPECTED_SIZE = 1280 * 1024 * 4;

    if (image_size != EXPECTED_SIZE) {
        xil_printf("  ERROR: Image size mismatch\r\n");
        xil_printf("         Expected: %u bytes (1280x1024x4)\r\n", EXPECTED_SIZE);
        xil_printf("         Got:      %u bytes\r\n", image_size);
        return -1;
    }

    xil_printf("  Loading image (%u bytes)...\r\n", image_size);

    memcpy(target_addr, image_data, image_size);

    xil_printf("  Image loaded successfully\r\n");

    return 0;  // Success
}

/* ============================================================================ *
 * 						Verify Image Buffer (Debug)							*
 * ============================================================================ */
void verify_image_buffer(int* buffer, const char* name)
{
    if (!buffer || !name) {
        xil_printf("ERROR: verify_image_buffer() called with NULL pointer\r\n");
        return;
    }

    xil_printf("Verifying %s:\r\n", name);

    // Check first pixel
    xil_printf("  First pixel: 0x%08X\r\n", buffer[0]);

    // Check if entire buffer is same color (possible error)
    int first = buffer[0];
    int all_same = 1;

    for (int i = 1; i < 100; i++) {  // Check first 100 pixels
        if (buffer[i] != first) {
            all_same = 0;
            break;
        }
    }

    if (all_same) {
        xil_printf("  WARNING: First 100 pixels are identical\r\n");
    } else {
        xil_printf("  OK: Buffer contains varying pixel data\r\n");
    }
}
