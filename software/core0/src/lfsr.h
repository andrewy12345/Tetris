/*
 * lfsr.h
 *
 * Hardware interface for the LFSR random number generator IP core
 * Base Address: 0x43C00000
 */

#ifndef LFSR_H
#define LFSR_H

#include "xil_types.h"
#include "xil_io.h"

/* ============================================================================
 * LFSR IP CORE BASE ADDRESS
 * ============================================================================ */
#define LFSR_BASE_ADDR      0x43C10000

/* ============================================================================
 * REGISTER OFFSETS
 * ============================================================================ */
#define LFSR_CONTROL_REG    0x00  // Control register
#define LFSR_SEED_REG       0x04  // Seed value register
#define LFSR_RANDOM_REG     0x08  // Random value output (read-only)
#define LFSR_TETRIS_REG     0x0C  // Tetris piece number (read-only)

/* ============================================================================
 * CONTROL REGISTER BIT DEFINITIONS
 * ============================================================================ */
#define LFSR_CTRL_ENABLE    0x00000001  // Bit 0: Enable LFSR
#define LFSR_CTRL_RESET     0x00000002  // Bit 1: Reset to seed

/* ============================================================================
 * DEFAULT VALUES
 * ============================================================================ */
#define LFSR_DEFAULT_SEED   0xACE1

/* ============================================================================
 * TETRIS PIECE DEFINITIONS
 * ============================================================================ */
#define TETRIS_I_PIECE      0  // Straight piece
#define TETRIS_O_PIECE      1  // Square piece
#define TETRIS_T_PIECE      2  // T-shaped piece
#define TETRIS_S_PIECE      3  // S-shaped piece
#define TETRIS_Z_PIECE      4  // Z-shaped piece
#define TETRIS_J_PIECE      5  // J-shaped piece
#define TETRIS_L_PIECE      6  // L-shaped piece

/* ============================================================================
 * INLINE FUNCTIONS FOR HARDWARE ACCESS
 * ============================================================================ */

/**
 * Initialize the LFSR with a seed value
 * @param seed: 16-bit seed value (0x0000 is invalid, will be set to 0x0001)
 */
static inline void lfsr_init(u16 seed)
{
    // Write seed value
    Xil_Out32(LFSR_BASE_ADDR + LFSR_SEED_REG, (u32)seed);

    // Reset LFSR to seed and enable
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, LFSR_CTRL_RESET | LFSR_CTRL_ENABLE);

    // Clear reset bit, keep enabled
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, LFSR_CTRL_ENABLE);
}

/**
 * Enable the LFSR (starts generating random numbers)
 */
static inline void lfsr_enable(void)
{
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, LFSR_CTRL_ENABLE);
}

/**
 * Disable the LFSR (stops generating random numbers)
 */
static inline void lfsr_disable(void)
{
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, 0x00000000);
}

/**
 * Reset the LFSR to its seed value
 */
static inline void lfsr_reset(void)
{
    u32 ctrl = Xil_In32(LFSR_BASE_ADDR + LFSR_CONTROL_REG);
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, ctrl | LFSR_CTRL_RESET);
    Xil_Out32(LFSR_BASE_ADDR + LFSR_CONTROL_REG, ctrl & ~LFSR_CTRL_RESET);
}

/**
 * Get the full 16-bit random value from the LFSR
 * @return 16-bit random number
 */
static inline u16 lfsr_get_random(void)
{
    return (u16)(Xil_In32(LFSR_BASE_ADDR + LFSR_RANDOM_REG) & 0xFFFF);
}

/**
 * Get a random Tetris piece number (0-6)
 * @return Tetris piece number
 */
static inline u8 lfsr_get_tetris_piece(void)
{
    return (u8)(Xil_In32(LFSR_BASE_ADDR + LFSR_TETRIS_REG) & 0x07);
}

/**
 * Get a random number in a specific range [0, max-1]
 * @param max: Maximum value (exclusive)
 * @return Random number between 0 and max-1
 */
static inline u16 lfsr_get_random_range(u16 max)
{
    return lfsr_get_random() % max;
}

/**
 * Get the piece name as a string (for debugging)
 * @param piece: Piece number (0-6)
 * @return Pointer to piece name string
 */
static inline const char* lfsr_get_piece_name(u8 piece)
{
    static const char* piece_names[] = {
        "I-piece (Straight)",
        "O-piece (Square)",
        "T-piece",
        "S-piece",
        "Z-piece",
        "J-piece",
        "L-piece",
        "Invalid"
    };

    if (piece > 6) {
        return piece_names[7];
    }
    return piece_names[piece];
}

#endif // LFSR_H
