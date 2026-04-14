/*
 * background_music_player.c
 */

#include "bg_music.h"
#include "shared_memory.h"
#include "xil_cache.h"
#include "move.h"
#include "start.h"
#include "tetris.h"

/* ---------------------------------------------------------------------------- *
 * 						background_music_player()								*
 * ---------------------------------------------------------------------------- *
 * Plays pre-recorded background music in a loop.
 *
 * Controls:
 * - BTNU (Up) - Restart from beginning
 * - BTND (Down) - Speed up
 * - BTNL (Left) - Move sound effect
 * - BTNR (Right) - Tetris sound effect
 * ---------------------------------------------------------------------------- */
void background_music_player() {
	// Initialize volume
	SHARED_VOLUME_INDEX = 3;  // Start at 50%
	SHARED_VOLUME_LEVEL = volume_presets[SHARED_VOLUME_INDEX];

	// Initial control value for audios
    u32 playback_index = 0;
    u8 is_paused = 0;
    float PLAYBACK_SPEED = 15.0;
    float speed_counter = 0.0;

    u32 move_playback_index = 0;
    u8 move_is_playing = 0;
    float move_speed = 20.0;
    float move_speed_counter = 0.0;

    u32 tetris_playback_index = 0;
	u8 tetris_is_playing = 0;
	float tetris_speed = 15.0;
	float tetris_speed_counter = 0.0;

	// Stop playing when on menus except gamplay
    while (1) {
    	if (!SHARED_SHOULD_PLAY_MUSIC) {
			Xil_Out32(I2S_DATA_TX_L_REG, 0);
			Xil_Out32(I2S_DATA_TX_R_REG, 0);
			is_paused = 1;
			move_is_playing = 0;
			continue;
		} else {
			if (is_paused == 1) {
				is_paused = 0;
			}
		}

            // Restart from beginning
            if (RESTART_BGM) {
                playback_index = 0;
                is_paused = 0;
                PLAYBACK_SPEED = 15.0;
            }

            //speed up
            if (SHARED_SPEED_UP) {
            	PLAYBACK_SPEED -= 0.5;
            }

            //Moving sound
            if (SHARED_BUTTON_CHANGED) {
				move_playback_index = 0;
				move_is_playing = 1;
				move_speed_counter = 0.0;
			}

            //Tetris sound
            if (ROW_CLEARED) {
                tetris_playback_index = 0;
                tetris_is_playing = 1;
                tetris_speed_counter = 0.0;
            }

            SHARED_BUTTON_CHANGED = 0;
            ROW_CLEARED = 0;
            RESTART_BGM = 0;
        	SHARED_SPEED_UP = 0;

        u32 output_sample = 0;

        /* -------------------------------------------------------------------- *
         * Audio Playback
         * -------------------------------------------------------------------- */
        if (!is_paused) {
            // Speed control: only advance playback index every PLAYBACK_SPEED samples
            speed_counter += 1;

            if (speed_counter >= PLAYBACK_SPEED) {
                speed_counter = 0;  // Reset counter

                // Move to next sample
                playback_index++;

                // Loop back to beginning when we reach the end
                if (playback_index >= bg_music_length) {
                    playback_index = 0;
                }
            }

        } if (move_is_playing) { // Play move sound effect
            move_speed_counter += 1;

            if (move_speed_counter >= move_speed) {
                move_speed_counter = 0;
                move_playback_index++;

                // Stop sound effect when it reaches the end
                if (move_playback_index >= move_length) {
                    move_is_playing = 0;
                    move_playback_index = 0;
                }
            }
        }

        if (tetris_is_playing) { // Play row clear sound effect
            tetris_speed_counter += 1;
            if (tetris_speed_counter >= tetris_speed) {
                tetris_speed_counter = 0;
                tetris_playback_index++;
                if (tetris_playback_index >= tetris_length) {
                    tetris_is_playing = 0;
                    tetris_playback_index = 0;
                }
            }
        }

        //Choose which sound to play
        if (SHARED_SHOULD_PLAY_MUSIC) {
        	if (move_is_playing) {
        		output_sample = move_left[move_playback_index];
        	} else if (tetris_is_playing) {
        		output_sample = tetris_left[tetris_playback_index];
        	} else {
        		output_sample = bg_music_left[playback_index];
        	}
        	u32 scaled = (output_sample * SHARED_VOLUME_LEVEL) >> 8;

            Xil_Out32(I2S_DATA_TX_L_REG, scaled);
            Xil_Out32(I2S_DATA_TX_R_REG, scaled);
        } else {
            // Output silence when paused
            Xil_Out32(I2S_DATA_TX_L_REG, 0);
            Xil_Out32(I2S_DATA_TX_R_REG, 0);
        }
    }
}
