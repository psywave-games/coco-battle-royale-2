//this example shows how to set up a palette and use 8x8 HW sprites
//also shows how fast (or slow) C code is

#include "neslib.h"
#include "game.h"

const unsigned char palSprites[]={
	0x0f,0x30,0x27,0x16,
};

void main(void)
{
	pal_spr(palSprites);
	pal_col(1,0x30);
	put_str(NTADR_A(1,27),"COCKS:   26");
	put_str(NTADR_A(1,28),"PLAYERS: 2");
	put_str(NTADR_A(18,27),"(SELECT) MENU");
	put_str(NTADR_A(18,28),"(START) PAUSE");
	put_arena();

	ppu_on_all();

	for (;;)
	{
		spr = 0;
		ppu_wait_frame();

		// joystick inputs
		for (i = 0; i < MAX_PLAYERS; gamepad[i] = pad_poll(i), i++);

		// player moves
		for (i = 0; i < MAX_PLAYERS; i++) {
			s = PRESSING(gamepad[i], PAD_RIGHT) - PRESSING(gamepad[i], PAD_LEFT);
			players[i].sign = s != 0? s: players[i].sign;
			players[i].x += s * SPEED;
			players[i].y += (PRESSING(gamepad[i], PAD_DOWN) - PRESSING(gamepad[i], PAD_UP)) * SPEED;
		}

		// arena colision 
		for (i = 0; i < MAX_ENIMIES; i++) {
			players[i].x = CLAMP(players[i].x, MIN_ARENA_X, MAX_ARENA_X);
			players[i].y = CLAMP(players[i].y, MIN_ARENA_Y, MAX_ARENA_Y);
		}

		// draw cocks
		for (i = 0; i < MAX_ENIMIES; i++) {
			s = SPR_PLAYER + (players[i].sign * ((((players[i].x ^ players[i].y)>>3)%2) + 1));
			spr = oam_spr(players[i].x, players[i].y, s, 0, spr);
		}
	}
}