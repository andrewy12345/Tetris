/*
 * simple_text.c
 *
 * Simple text rendering for VGA display
 * FIXED: Font data with all bytes bit-reversed for correct display
 */

#include "simple_text.h"
#include "xil_types.h"
#include <string.h>
#include <stdio.h>

#define H_RESOLUTION 1280
#define V_RESOLUTION 1024

/* ============================================================================ *
 *                        SIMPLE 8x8 FONT DATA
 * ============================================================================ */

static u8 font_data[256][8];

static void init_font(void)
{
    memset(font_data, 0, sizeof(font_data));

    // Space (32)
    u8 char_space[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    memcpy(font_data[32], char_space, 8);

    // % (37) - Percent symbol
    u8 char_percent[] = {0xC6, 0xCC, 0x18, 0x30, 0x60, 0xC6, 0x86, 0x00};
    memcpy(font_data[37], char_percent, 8);

    // 0 (48) - Bit-reversed from 0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00
    u8 char_0[] = {0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00};
    memcpy(font_data[48], char_0, 8);

    // 1 (49) - Bit-reversed from 0x0C, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F
    u8 char_1[] = {0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00};
    memcpy(font_data[49], char_1, 8);

    // 2 (50)
    u8 char_2[] = {0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0xFE, 0x00};
    memcpy(font_data[50], char_2, 8);

    // 3 (51)
    u8 char_3[] = {0xFE, 0x0C, 0x18, 0x7C, 0x06, 0xC6, 0x7C, 0x00};
    memcpy(font_data[51], char_3, 8);

    // 4 (52)
    u8 char_4[] = {0x0C, 0x1C, 0x3C, 0x6C, 0xFE, 0x0C, 0x1E, 0x00};
    memcpy(font_data[52], char_4, 8);

    // 5 (53)
    u8 char_5[] = {0xFE, 0xC0, 0xF8, 0x06, 0x06, 0xC6, 0x7C, 0x00};
    memcpy(font_data[53], char_5, 8);

    // 6 (54)
    u8 char_6[] = {0x3C, 0x60, 0xC0, 0xF8, 0xC6, 0xC6, 0x7C, 0x00};
    memcpy(font_data[54], char_6, 8);

    // 7 (55)
    u8 char_7[] = {0xFE, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00};
    memcpy(font_data[55], char_7, 8);

    // 8 (56)
    u8 char_8[] = {0x7C, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0x7C, 0x00};
    memcpy(font_data[56], char_8, 8);

    // 9 (57)
    u8 char_9[] = {0x7C, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0x78, 0x00};
    memcpy(font_data[57], char_9, 8);

    // : (58)
    u8 char_colon[] = {0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00};
    memcpy(font_data[58], char_colon, 8);

    // - (45)
    u8 char_minus[] = {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00};
    memcpy(font_data[45], char_minus, 8);

    // A (65)
    u8 char_A[] = {0x10, 0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00};
    memcpy(font_data[65], char_A, 8);

    // B (66)
    u8 char_B[] = {0xF8, 0x6C, 0x6C, 0x78, 0x6C, 0x6C, 0xF8, 0x00};
    memcpy(font_data[66], char_B, 8);

    // C (67)
    u8 char_C[] = {0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00};
    memcpy(font_data[67], char_C, 8);

    // D (68)
    u8 char_D[] = {0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00};
    memcpy(font_data[68], char_D, 8);

    // E (69)
    u8 char_E[] = {0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00};
    memcpy(font_data[69], char_E, 8);

    // F (70)
    u8 char_F[] = {0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00};
    memcpy(font_data[70], char_F, 8);

    // G (71)
    u8 char_G[] = {0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00};
    memcpy(font_data[71], char_G, 8);

    // H (72)
    u8 char_H[] = {0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00};
    memcpy(font_data[72], char_H, 8);

    // I (73)
    u8 char_I[] = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00};
    memcpy(font_data[73], char_I, 8);

    // J (74)
    u8 char_J[] = {0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00};
    memcpy(font_data[74], char_J, 8);

    // K (75)
    u8 char_K[] = {0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00};
    memcpy(font_data[75], char_K, 8);

    // L (76)
    u8 char_L[] = {0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00};
    memcpy(font_data[76], char_L, 8);

    // M (77)
    u8 char_M[] = {0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0x00};
    memcpy(font_data[77], char_M, 8);

    // N (78)
    u8 char_N[] = {0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00};
    memcpy(font_data[78], char_N, 8);

    // O (79)
    u8 char_O[] = {0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00};
    memcpy(font_data[79], char_O, 8);

    // P (80)
    u8 char_P[] = {0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00};
    memcpy(font_data[80], char_P, 8);

    // Q (81)
    u8 char_Q[] = {0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x7E, 0x00};
    memcpy(font_data[81], char_Q, 8);

    // R (82)
    u8 char_R[] = {0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00};
    memcpy(font_data[82], char_R, 8);

    // S (83)
    u8 char_S[] = {0x7C, 0xC6, 0xE0, 0x78, 0x1C, 0xC6, 0x7C, 0x00};
    memcpy(font_data[83], char_S, 8);

    // T (84)
    u8 char_T[] = {0xFE, 0xB2, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00};
    memcpy(font_data[84], char_T, 8);

    // U (85)
    u8 char_U[] = {0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00};
    memcpy(font_data[85], char_U, 8);

    // V (86)
    u8 char_V[] = {0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00};
    memcpy(font_data[86], char_V, 8);

    // W (87)
    u8 char_W[] = {0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00};
    memcpy(font_data[87], char_W, 8);

    // X (88)
    u8 char_X[] = {0xC6, 0x6C, 0x38, 0x10, 0x38, 0x6C, 0xC6, 0x00};
    memcpy(font_data[88], char_X, 8);

    // Y (89)
    u8 char_Y[] = {0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00};
    memcpy(font_data[89], char_Y, 8);

    // Z (90)
    u8 char_Z[] = {0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00};
    memcpy(font_data[90], char_Z, 8);

    // | (124) - Pipe symbol
    u8 char_pipe[] = {0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00};
    memcpy(font_data[124], char_pipe, 8);
}

static volatile u8 font_initialized = 0;

/* ============================================================================ *
 *                        RENDERING FUNCTIONS
 * ============================================================================ */

void draw_char(int* buffer, int x, int y,
               char character, uint32_t color, int scale)
{
    if (!font_initialized) {
        init_font();
        font_initialized = 1;
    }

    if (character < 0 || character > 127) {
        character = ' ';
    }

    const u8* char_data = font_data[(u8)character];

    for (int row = 0; row < 8; row++) {
        u8 bitmap = char_data[row];

        // Read bits from MSB to LSB (left to right)
        for (int col = 0; col < 8; col++) {
            if (bitmap & (0x80 >> col)) {
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        int px = x + col * scale + sx;
                        int py = y + row * scale + sy;

                        if (px >= 0 && px < H_RESOLUTION &&
                            py >= 0 && py < V_RESOLUTION) {
                            buffer[py * H_RESOLUTION + px] = color;
                        }
                    }
                }
            }
        }
    }
}

void draw_string(int* buffer, int x, int y,
                 const char* text, uint32_t color, int scale)
{
    if (text == NULL) {
        return;
    }

    int current_x = x;
    int char_width = 8 * scale;
    int max_chars = (H_RESOLUTION - x) / char_width;

    for (int i = 0; i < max_chars && text[i] != '\0'; i++) {
        draw_char(buffer, current_x, y, text[i], color, scale);
        current_x += char_width;
    }
}

void draw_number(int* buffer, int x, int y,
                 u32 number, uint32_t color, int scale)
{
    char num_str[20];
    sprintf(num_str, "%u", number);
    draw_string(buffer, x, y, num_str, color, scale);
}
