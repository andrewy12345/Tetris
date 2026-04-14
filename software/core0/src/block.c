#include <unistd.h>

#include "xil_io.h"

#include "tetris.h"
#include "block.h"
#include "lfsr.h"

static u16 board[18] = {0};
static u16 boardcpy[18] = {0};

static char block_type; // I, O, S, Z, L, J, T
static char block_list[7] = {'I','O','S','Z','L','J','T'};

static u8 x1, x2, x3, x4;
static u8 y1, y2, y3, y4;

static u8 status = 0;

static u8 orientation = 0;

static u8 row_clear = 0;
static u8 gameover = 0;

// memory locations for different frame buffers
static int* vga_buffer_pointer = (int*)(0x00900000);
static int* frame_buffer_pointer = (int*)(0x01000000);

static void update(void){
	usleep(500);
	status++;
	Xil_Out64(0x43C0004C, status);
	usleep(100000);
	memcpy(vga_buffer_pointer, frame_buffer_pointer, BYTES_PER_FRAME);
}

/**
 * function for GPU control
 * sends the old tetris block position to the GPU erase data registers
 */
static void erase(void){
	Xil_Out64(0x43C00000, 0xFF & x1);
	Xil_Out64(0x43C00004, 0xFF & x2);
	Xil_Out64(0x43C00008, 0xFF & x3);
	Xil_Out64(0x43C0000C, 0xFF & x4);

	Xil_Out64(0x43C00010, 0xFF & y1);
	Xil_Out64(0x43C00014, 0xFF & y2);
	Xil_Out64(0x43C00018, 0xFF & y3);
	Xil_Out64(0x43C0001C, 0xFF & y4);
}

/**
 * function for GPU control
 * sends the old tetris block position to the GPU draw data registers
 */
static void draw_block(void){
	Xil_Out64(0x43C00020, 0xFF & x1);
	Xil_Out64(0x43C00024, 0xFF & x2);
	Xil_Out64(0x43C00028, 0xFF & x3);
	Xil_Out64(0x43C0002C, 0xFF & x4);

	Xil_Out64(0x43C00030, 0xFF & y1);
	Xil_Out64(0x43C00034, 0xFF & y2);
	Xil_Out64(0x43C00038, 0xFF & y3);
	Xil_Out64(0x43C0003C, 0xFF & y4);
}

/**
 * creates a new block by setting its logical (x,y) position
 * also tells the GPU to draw the new block on screen
 *
 * @param type the name of the tetris piece
 */
static void new_block(char type){
	switch(type){
	case 'I':
		block_type = type;
		x1 = 3; x2 = 4; x3 = 5; x4 = 6;
		y1 = 0; y2 = 0; y3 = 0; y4 = 0;
		break;
	case 'O':
		block_type = type;
		x1 = 4; x2 = 5; x3 = 4; x4 = 5;
		y1 = 0; y2 = 0; y3 = 1; y4 = 1;
		break;
	case 'S':
		block_type = type;
		x1 = 4; x2 = 5; x3 = 3; x4 = 4;
		y1 = 0; y2 = 0; y3 = 1; y4 = 1;
		break;
	case 'Z':
		block_type = type;
		x1 = 3; x2 = 4; x3 = 4; x4 = 5;
		y1 = 0; y2 = 0; y3 = 1; y4 = 1;
		break;
	case 'L':
		block_type = type;
		x1 = 3; x2 = 4; x3 = 5; x4 = 3;
		y1 = 0; y2 = 0; y3 = 0; y4 = 1;
		break;
	case 'J':
		block_type = type;
		x1 = 3; x2 = 4; x3 = 5; x4 = 5;
		y1 = 0; y2 = 0; y3 = 0; y4 = 1;
		break;
	case 'T':
		block_type = type;
		x1 = 4; x2 = 3; x3 = 4; x4 = 5;
		y1 = 0; y2 = 1; y3 = 1; y4 = 1;
		break;
	default: return;
	}
	orientation = 0;
	draw_block();
	update();
	return;
}

/**
 * initiates a redraw of the entire board
 * sends board data to the GPU
 */
static void redraw(void){
	Xil_Out64(0x43C00000, 0x03FF & board[0]);
	Xil_Out64(0x43C00004, 0x03FF & board[1]);
	Xil_Out64(0x43C00008, 0x03FF & board[2]);
	Xil_Out64(0x43C0000C, 0x03FF & board[3]);

	Xil_Out64(0x43C00010, 0x03FF & board[4]);
	Xil_Out64(0x43C00014, 0x03FF & board[5]);
	Xil_Out64(0x43C00018, 0x03FF & board[6]);
	Xil_Out64(0x43C0001C, 0x03FF & board[7]);

	Xil_Out64(0x43C00020, 0x03FF & board[8]);
	Xil_Out64(0x43C00024, 0x03FF & board[9]);
	Xil_Out64(0x43C00028, 0x03FF & board[10]);
	Xil_Out64(0x43C0002C, 0x03FF & board[11]);

	Xil_Out64(0x43C00030, 0x03FF & board[12]);
	Xil_Out64(0x43C00034, 0x03FF & board[13]);
	Xil_Out64(0x43C00038, 0x03FF & board[14]);
	Xil_Out64(0x43C0003C, 0x03FF & board[15]);

	Xil_Out64(0x43C00040, 0x03FF & board[16]);
	Xil_Out64(0x43C00044, 0x03FF & board[17]);

	usleep(500);
	Xil_Out64(0x43C0004C, 0x80000000);
	usleep(100000);
}

/**
 * software driver to control the GPU for the game over screen wipe effect
 */
static void gameover_effect(void){

	Xil_Out64(0x43C00000, 0x03FF & board[0]);
	Xil_Out64(0x43C00004, 0x03FF & board[1]);
	Xil_Out64(0x43C00008, 0x03FF & board[2]);
	Xil_Out64(0x43C0000C, 0x03FF & board[3]);

	Xil_Out64(0x43C00010, 0x03FF & board[4]);
	Xil_Out64(0x43C00014, 0x03FF & board[5]);
	Xil_Out64(0x43C00018, 0x03FF & board[6]);
	Xil_Out64(0x43C0001C, 0x03FF & board[7]);

	Xil_Out64(0x43C00020, 0x03FF & board[8]);
	Xil_Out64(0x43C00024, 0x03FF & board[9]);
	Xil_Out64(0x43C00028, 0x03FF & board[10]);
	Xil_Out64(0x43C0002C, 0x03FF & board[11]);

	Xil_Out64(0x43C00030, 0x03FF & board[12]);
	Xil_Out64(0x43C00034, 0x03FF & board[13]);
	Xil_Out64(0x43C00038, 0x03FF & board[14]);
	Xil_Out64(0x43C0003C, 0x03FF & board[15]);

	Xil_Out64(0x43C00040, 0x03FF & board[16]);

	Xil_Out64(0x43C00048, 0x00000000); // send color, black for gameover effect
	for(u8 i = 0; i < 18; i++){
		board[17 - i] = 0x03FF;
		Xil_Out64(0x43C00044 - i*4, 0x03FF & board[17 - i]);

		Xil_Out64(0x43C0004C, 0x40000000 + i);
		usleep(100000);
		memcpy(vga_buffer_pointer, frame_buffer_pointer, BYTES_PER_FRAME);
	}
	usleep(100000);
	return;
}

void moveUp(void){
	if(y1 == 0 || y2 == 0 || y3 == 0 || y4 == 0) return;
	erase();
	y1--; y2--; y3--; y4--;
	draw_block();
	update();
	return;
}

void moveDown(void){
	if(y1 == 17 || y2 == 17 || y3 == 17 || y4 == 17) return;
	if(board[y1 + 1] & (1 << x1) || board[y2 + 1] & (1 << x2) || board[y3 + 1] & (1 << x3) || board[y4 + 1] & (1 << x4)) return;
	erase();
	y1++; y2++; y3++; y4++;
	draw_block();
	update();
	return;
}

void moveLeft(void){
	if(x1 == 0 || x2 == 0 || x3 == 0 || x4 == 0) return;
	if(board[y1] & (1 << (x1 - 1)) || board[y2] & (1 << (x2 - 1)) || board[y3] & (1 << (x3 - 1)) || board[y4] & (1 << (x4 - 1))) return;
	erase();
	x1--; x2--; x3--; x4--;
	draw_block();
	update();
	return;
}

void moveRight(void){
	if(x1 == 9 || x2 == 9 || x3 == 9 || x4 == 9) return;
	if(board[y1] & (1 << (x1 + 1)) || board[y2] & (1 << (x2 + 1)) || board[y3] & (1 << (x3 + 1)) || board[y4] & (1 << (x4 + 1))) return;
	erase();
	x1++; x2++; x3++; x4++;
	draw_block();
	update();
	return;
}

void rotate(void){
	erase();

	// the rotational mapping is different for every block
	// restrictions will be checked first before the block can be rotated
	switch(block_type){
	case 'I':
		switch(orientation){
		case 0:
			if(x1 == 8 || y1 == 0) return;
			if(x2 == 9) return;
			if(y3 == 17) return;
			if(x4 == 0 || y4 == 16) return;
			if(board[y1 - 1] & (1 << (x1 + 2)) || board[y2] & (1 << (x2 + 1)) || board[y3 + 1] & (1 << x3) || board[y4 + 2] & (1 << (x4 - 1))) return;
			x1 += 2; y1--;
			x2++;
			y3++;
			x4--; y4+=2;
			orientation = 1;
			break;
		case 1:
			if(x1 == 1 || y1 == 17) return;
			if(x2 == 0) return;
			if(y3 == 0) return;
			if(x4 == 9 || y4 == 1) return;
			if(board[y1 + 1] & (1 << (x1 - 2)) || board[y2] & (1 << (x2 - 1)) || board[y3 - 1] & (1 << x3) || board[y4 - 2] & (1 << (x4 + 1))) return;
			x1 -= 2; y1++;
			x2--;
			y3--;
			x4++; y4-=2;
			orientation = 0;
			break;
		default: break;
		}
		break;
	case 'O': return;
	case 'S':
		switch(orientation){
		case 0:
			if(x2 == 0 || y1 == 7) return;
			if(x3 == 1) return;
			if(x4 == 0 || y4 == 0) return;
			if(board[y2 + 1] & (1 << (x2 - 1)) || board[y3 - 2] & (1 << x3) || board[y4 - 1] & (1 << (x4 - 1))) return;
			x2--; y2++;
			y3 -= 2;
			x4--; y4--;
			orientation = 1;
			break;
		case 1:
			if(x2 == 9 || y1 == 0) return;
			if(x3 == 8) return;
			if(x4 == 9 || y4 == 17) return;
			if(board[y2 - 1] & (1 << (x2 + 1)) || board[y3 + 2] & (1 << x3) || board[y4 + 1] & (1 << (x4 + 1))) return;
			x2++; y2--;
			y3 += 2;
			x4++; y4++;
			orientation = 0;
			break;
		default: break;
		}
		break;
	case 'Z':
		switch(orientation){
		case 0:
			if(x1 == 9 || y1 == 0) return;
			if(x3 == 0 || y3 == 0) return;
			if(x4 == 1) return;
			if(board[y1 - 1] & (1 << (x1 + 1)) || board[y3 - 1] & (1 << (x3 - 1)) || board[y4 - 2] & (1 << x4)) return;
			x1++; y1--;
			x3--; y3--;
			x4-=2;
			orientation = 1;
			break;
		case 1:
			if(x1 == 0 || y1 == 17) return;
			if(x3 == 9 || y3 == 17) return;
			if(x4 == 8) return;
			if(board[y1 + 1] & (1 << (x1 - 1)) || board[y3 + 1] & (1 << (x3 + 1)) || board[y4 + 2] & (1 << x4)) return;
			x1--; y1++;
			x3++; y3++;
			x4+=2;
			orientation = 0;
			break;
		default: break;
		}
		break;
	case 'L':
		switch(orientation){
		case 0:
			if(x1 == 9 || y1 == 0) return;
			if(x3 == 0 || y3 == 17) return;
			if(y4 == 1) return;
			if(board[y1 - 1] & (1 << (x1 + 1)) || board[y3 + 1] & (1 << (x3 - 1)) || board[y4 - 2] & (1 << x4)) return;
			x1++; y1--;
			x3--; y3++;
			y4 -= 2;
			orientation = 1;
			break;
		case 1:
			if(x1 == 9 || y1 == 17) return;
			if(x3 == 0 || y3 == 0) return;
			if(x4 == 8) return;
			if(board[y1 + 1] & (1 << (x1 + 1)) || board[y3 - 1] & (1 << (x3 - 1)) || board[y4] & (1 << (x4 + 2))) return;
			x1++; y1++;
			x3--; y3--;
			x4 += 2;
			orientation = 2;
			break;
		case 2:
			if(x1 == 0 || y1 == 17) return;
			if(x3 == 9 || y3 == 0) return;
			if(y4 == 16) return;
			if(board[y1 + 1] & (1 << (x1 - 1)) || board[y3 - 1] & (1 << (x3 + 1)) || board[y4 + 2] & (1 << x4)) return;
			x1--; y1++;
			x3++; y3--;
			y4 += 2;
			orientation = 3;
			break;
		case 3:
			if(x1 == 0 || y1 == 0) return;
			if(x3 == 9 || y3 == 17) return;
			if(x4 == 1) return;
			if(board[y1 - 1] & (1 << (x1 - 1)) || board[y3 + 1] & (1 << (x3 + 1)) || board[y4] & (1 << (x4 - 2))) return;
			x1--; y1--;
			x3++; y3++;
			x4 -= 2;
			orientation = 0;
			break;
		default: break;
		}
		break;

	case 'J':
		switch(orientation){
		case 0:
			if(x1 == 9 || y1 == 0) return;
			if(x3 == 0 || y3 == 17) return;
			if(x4 == 1) return;
			if(board[y1 - 1] & (1 << (x1 + 1)) || board[y3 + 1] & (1 << (x3 - 1)) || board[y4] & (1 << (x4 - 2))) return;
			x1++; y1--;
			x3--; y3++;
			x4 -= 2;
			orientation = 1;
			break;
		case 1:
			if(x1 == 9 || y1 == 17) return;
			if(x3 == 0 || y3 == 0) return;
			if(y4 == 1) return;
			if(board[y1 + 1] & (1 << (x1 + 1)) || board[y3 - 1] & (1 << (x3 - 1)) || board[y4 - 2] & (1 << x4)) return;
			x1++; y1++;
			x3--; y3--;
			y4 -= 2;
			orientation = 2;
			break;
		case 2:
			if(x1 == 0 || y1 == 17) return;
			if(x3 == 9 || y3 == 0) return;
			if(x4 == 8) return;
			if(board[y1 + 1] & (1 << (x1 - 1)) || board[y3 - 1] & (1 << (x3 + 1)) || board[y4] & (1 << (x4 + 2))) return;
			x1--; y1++;
			x3++; y3--;
			x4 += 2;
			orientation = 3;
			break;
		case 3:
			if(x1 == 0 || y1 == 0) return;
			if(x3 == 9 || y3 == 17) return;
			if(y4 == 16) return;
			if(board[y1 - 1] & (1 << (x1 - 1)) || board[y3 + 1] & (1 << (x3 + 1)) || board[y4 + 2] & (1 << x4)) return;
			x1--; y1--;
			x3++; y3++;
			y4 += 2;
			orientation = 0;
			break;
		default: break;
		}
		break;
	case 'T':
		switch(orientation){
		case 0:
			if(x1 == 9 || y1 == 17) return;
			if(x2 == 9 || y2 == 00) return;
			if(x4 == 0 || y4 == 17) return;
			if(board[y1 + 1] & (1 << (x1 + 1)) || board[y2 - 1] & (1 << (x2 + 1)) || board[y4 + 1] & (1 << (x4 - 1))) return;
			x1++; y1++;
			x2++; y2--;
			x4--; y4++;
			orientation = 1;
			break;
		case 1:
			if(x1 == 0 || y1 == 17) return;
			if(x2 == 9 || y2 == 17) return;
			if(x4 == 0 || y4 == 0) return;
			if(board[y1 + 1] & (1 << (x1 - 1)) || board[y2 + 1] & (1 << (x2 + 1)) || board[y4 - 1] & (1 << (x4 - 1))) return;
			x1--; y1++;
			x2++; y2++;
			x4--; y4--;
			orientation = 2;
			break;
		case 2:
			if(x1 == 0 || y1 == 0) return;
			if(x2 == 0 || y2 == 17) return;
			if(x1 == 9 || y4 == 0) return;
			if(board[y1 - 1] & (1 << (x1 - 1)) || board[y2 + 1] & (1 << (x2 - 1)) || board[y4 - 1] & (1 << (x4 + 1))) return;
			x1--; y1--;
			x2--; y2++;
			x4++; y4--;
			orientation = 3;
			break;
		case 3:
			if(x1 == 9 || y1 == 0) return;
			if(x2 == 0 || y2 == 0) return;
			if(x4 == 9 || y4 == 17) return;
			if(board[y1 - 1] & (1 << (x1 + 1)) || board[y2 - 1] & (1 << (x2 - 1)) || board[y4 + 1] & (1 << (x4 + 1))) return;
			x1++; y1--;
			x2--; y2--;
			x4++; y4++;
			orientation = 0;
			break;
		default: break;
		}
		break;
	default: return;
	}
	draw_block();
	update();
	return;
}

void start_game(void){
	lfsr_init(LFSR_DEFAULT_SEED);
	memset(board, 0, sizeof(board)); // wipes the board
	redraw();

	// select a random block to start
	u8 random_selection = lfsr_get_tetris_piece();
	new_block(block_list[random_selection]);
}

u8 tick(void){

	// checks if the falling piece can still move downward,
	// if not then it will be set in place, and a new one will spawn at the top
	if(y1 == 17 || y2 == 17 || y3 == 17 || y4 == 17 ||
	   board[y1 + 1] & (1 << x1) || board[y2 + 1] & (1 << x2) || board[y3 + 1] & (1 << x3) || board[y4 + 1] & (1 << x4)){

		// sets the piece in place
		// updates the board data using bit masking
		board[y1] = board[y1] | (1 << x1);
		board[y2] = board[y2] | (1 << x2);
		board[y3] = board[y3] | (1 << x3);
		board[y4] = board[y4] | (1 << x4);

		// game over
		// if the first row has a filled square, the game is over
		if(board[0]){
			gameover_effect();
			memset(board, 0, sizeof(board));
			gameover = 1;
		}

		// setup for row clearing effects
		// make a copy of the current board setup
		memcpy(boardcpy, board, sizeof(board));

		// custom logic to check for filled rows
		// if a row is filled, the logic will clear it and update the board
		// will only check for filled rows if game over state is not reached
		if(!gameover){
			if(board[y1] == 0x03FF){
				row_clear++;
				if(y2 < y1) y2++;
				if(y3 < y1) y3++;
				if(y4 < y1) y4++;
				u8 index = y1;
				do {
					board[index] = board[index - 1];
					index--;
				} while(index > 0);
			}
			if(board[y2] == 0x03FF){
				row_clear++;
				if(y3 < y2) y3++;
				if(y4 < y2) y4++;
				u8 index = y2;
				do {
					board[index] = board[index - 1];
					index--;
				} while(index > 0);
			}
			if(board[y3] == 0x03FF){
				row_clear++;
				if(y4 < y3) y4++;
				u8 index = y3;
				do {
					board[index] = board[index - 1];
					index--;
				} while(index > 0);
			}
			if(board[y4] == 0x03FF){
				row_clear++;
				u8 index = y4;
				do {
					board[index] = board[index - 1];
					index--;
				} while(index > 0);
			}

			// this copies the old board data to the GPU and initiates a row clear effect
			// effect will be shown first then the board will be completely redrawn
			if(row_clear){
				Xil_Out64(0x43C00000, 0x03FF & boardcpy[0]);
				Xil_Out64(0x43C00004, 0x03FF & boardcpy[1]);
				Xil_Out64(0x43C00008, 0x03FF & boardcpy[2]);
				Xil_Out64(0x43C0000C, 0x03FF & boardcpy[3]);

				Xil_Out64(0x43C00010, 0x03FF & boardcpy[4]);
				Xil_Out64(0x43C00014, 0x03FF & boardcpy[5]);
				Xil_Out64(0x43C00018, 0x03FF & boardcpy[6]);
				Xil_Out64(0x43C0001C, 0x03FF & boardcpy[7]);

				Xil_Out64(0x43C00020, 0x03FF & boardcpy[8]);
				Xil_Out64(0x43C00024, 0x03FF & boardcpy[9]);
				Xil_Out64(0x43C00028, 0x03FF & boardcpy[10]);
				Xil_Out64(0x43C0002C, 0x03FF & boardcpy[11]);

				Xil_Out64(0x43C00030, 0x03FF & boardcpy[12]);
				Xil_Out64(0x43C00034, 0x03FF & boardcpy[13]);
				Xil_Out64(0x43C00038, 0x03FF & boardcpy[14]);
				Xil_Out64(0x43C0003C, 0x03FF & boardcpy[15]);

				Xil_Out64(0x43C00040, 0x03FF & boardcpy[16]);
				Xil_Out64(0x43C00044, 0x03FF & boardcpy[17]);


				Xil_Out64(0x43C00048, 0x00008800); // send color
				Xil_Out64(0x43C0004C, 0x40000000); // send a row clear effect command
				usleep(100000);
				memcpy(vga_buffer_pointer, frame_buffer_pointer, BYTES_PER_FRAME); // show the effect
				usleep(500000);
				redraw();
			}
		}

		if(gameover){
			gameover = 0;
			return 10;
		}

		// sets the erase registers with out of bounds data
		// the GPU will skip the erase if the coordinate is out of bounds
		Xil_Out64(0x43C00000, 0xFFFFFFFF);
		Xil_Out64(0x43C00004, 0xFFFFFFFF);
		Xil_Out64(0x43C00008, 0xFFFFFFFF);
		Xil_Out64(0x43C0000C, 0xFFFFFFFF);

		Xil_Out64(0x43C00010, 0xFFFFFFFF);
		Xil_Out64(0x43C00014, 0xFFFFFFFF);
		Xil_Out64(0x43C00018, 0xFFFFFFFF);
		Xil_Out64(0x43C0001C, 0xFFFFFFFF);

		// a piece has been set in place, therefore a new one will be chosen at random
		// and spawned at the top
		u8 random_selection = lfsr_get_tetris_piece();
		new_block(block_list[random_selection]);
		draw_block();
		update();

		// if there was a row clear, return a signal for score increase
		if(row_clear){
			int point = row_clear;
			row_clear = 0;
			return point;
		}

	} else {
		// the block will keep falling if there is still space
		moveDown();
	}

	return 0;

}

