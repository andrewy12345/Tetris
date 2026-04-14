/*
 * volume_presets.c
 */

#include "xil_types.h"

/* Volume preset levels in 0-255 scale */
const u8 volume_presets[] = {
    0,      // 0%
    26,     // 10%
    51,     // 20%
    128,    // 50%
    192,    // 75%
    255     // 100%
};
