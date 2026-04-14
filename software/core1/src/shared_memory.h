/*
 * shared_memory.h
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include "xil_types.h"

/* ---------------------------------------------------------------------------- *
 * 						Shared Memory Base Address								*
 * ---------------------------------------------------------------------------- */
#define SHARED_MEM_BASE     0xFFFF0000  // OCM

/* ---------------------------------------------------------------------------- *
 * 						Shared Memory Map										*
 * ---------------------------------------------------------------------------- */

// Button and switch state (shared from Core 0 to Core 1)
#define ROW_CLEARED (*(volatile u8 *)(SHARED_MEM_BASE + 0x0004))
#define SHARED_BUTTON_CHANGED (*(volatile u8 *)(SHARED_MEM_BASE + 0x0005))
#define SHARED_SPEED_UP (*(volatile u8 *)(SHARED_MEM_BASE + 0x0006))
#define RESTART_BGM (*(volatile u8 *)(SHARED_MEM_BASE + 0x0007))


/* ---------------------------------------------------------------------------- *
 * 						Music Control (Core 0 -> Core 1)						*
 * ---------------------------------------------------------------------------- */
#define SHARED_SHOULD_PLAY_MUSIC (*(volatile u8 *)(SHARED_MEM_BASE + 0x0000))

#define SHARED_VOLUME_LEVEL (*(volatile u8 *)(SHARED_MEM_BASE + 0x0002))
#define SHARED_VOLUME_INDEX (*(volatile u8 *)(SHARED_MEM_BASE + 0x0003))

/* Volume preset levels in 0-255 scale */
extern const u8 volume_presets[];

#define NUM_VOLUME_PRESETS 6

#define SHARED_CURRENT_SCORE (*(volatile u32 *)(SHARED_MEM_BASE + 0x0010))
#define SHARED_HIGH_SCORE (*(volatile u32 *)(SHARED_MEM_BASE + 0x0014))

/* ---------------------------------------------------------------------------- *
 * 							Button Bit Masks									*
 * ---------------------------------------------------------------------------- */
#define BTNC                0x01  // Center button
#define BTND                0x02  // Down button
#define BTNL                0x04  // Left button
#define BTNR                0x08  // Right button
#define BTNU                0x10  // Up button

/* ---------------------------------------------------------------------------- *
 * 							Switch Bit Masks									*
 * ---------------------------------------------------------------------------- */
#define SW0                 0x01  // Switch 0
#define SW1                 0x02  // Switch 1
#define SW2                 0x04  // Switch 2

/* ---------------------------------------------------------------------------- *
 * 							Helper Macros										*
 * ---------------------------------------------------------------------------- */
// Check if a specific button is pressed
#define IS_BUTTON_PRESSED(btn)  ((SHARED_BUTTON_STATE & (btn)) != 0)

// Check if a specific switch is ON
#define IS_SWITCH_ON(sw)        ((SHARED_SWITCH_STATE & (sw)) != 0)

// Check if ANY button is pressed
#define ANY_BUTTON_PRESSED()    (SHARED_BUTTON_STATE != 0)

// Check if NO buttons are pressed
#define NO_BUTTON_PRESSED()     (SHARED_BUTTON_STATE == 0)


#endif /* SHARED_MEMORY_H_ */
