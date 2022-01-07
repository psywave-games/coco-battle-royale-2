//this example shows how to set up a palette and use 8x8 HW sprites
//also shows how fast (or slow) C code is

#include "neslib.h"

#define PRESSING(g,b)	((g&b)==b)
#define SPR_PLAYER	0x42

static unsigned char i,j;
static unsigned char spr;

static unsigned char gamepad[] = {0, 0};
static unsigned char player_x = 128;
static unsigned char player_y = 128;

const unsigned char palSprites[]={
	0x0f,0x30,0x27,0x16,
};

void main(void)
{
	pal_spr(palSprites);

	ppu_on_all();

	for (;;)
	{
		ppu_wait_frame();
		// joystick inputs
		for (i = 0; i < 2; gamepad[i] = pad_poll(i), i++);


		player_x += (PRESSING(gamepad[0], PAD_RIGHT) - PRESSING(gamepad[0], PAD_LEFT)) * 3;
		player_y += (PRESSING(gamepad[0], PAD_DOWN) - PRESSING(gamepad[0], PAD_UP)) * 3;
		
		spr = 0;
		spr = oam_spr(player_x - 16, player_y, SPR_PLAYER + player_x%2, 0, spr);
	}
}