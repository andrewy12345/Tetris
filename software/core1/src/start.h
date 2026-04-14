/*
 * adventures_with_ip.h
 *
 * Main header file.
 */

#ifndef START_H_
#define START_H_

/* ---------------------------------------------------------------------------- *
 * 								Header Files									*
 * ---------------------------------------------------------------------------- */
#include <stdio.h>
#include <xil_io.h>
#include <sleep.h>
#include "xiicps.h"
#include <xil_printf.h>
#include <xparameters.h>
#include "stdlib.h"


#include <sleep.h>
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xpseudo_asm.h"


/* ---------------------------------------------------------------------------- *
 * 							Custom IP Header Files								*
 * ---------------------------------------------------------------------------- */
#include "audio.h"
#include "shared_memory.h"  // For reading button/switch state from Core 0

/* ---------------------------------------------------------------------------- *
 * 							Prototype Functions									*
 * ---------------------------------------------------------------------------- */

void background_music_player();

/* ---------------------------------------------------------------------------- *
 * 							Global Variables									*
 * ---------------------------------------------------------------------------- */
XIicPs Iic;
// XGpio Gpio; // Not needed - Core 0 handles GPIO
// XScuGic INTCInst; // Not needed - Core 0 handles interrupts

#endif /* START_H_ */
