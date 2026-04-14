#pragma once

#include "tetris.h"

/**
 * must run this function before using tick(), initializes a block
 */
void start_game(void);

/**
 * moves the block up, mainly for debugging and testing
 */
void moveUp(void);

/**
 * moves the block down
 */
void moveDown(void);

/**
 * moves the block to the left
 */
void moveLeft(void);

/**
 * moves the block to the right
 */
void moveRight(void);

/**
 * rotates the block clockwise
 */
void rotate(void);

/**
 * updates the game, runs one cycle and moves the block down, and check for game logics and conditions
 */
u8 tick(void);
