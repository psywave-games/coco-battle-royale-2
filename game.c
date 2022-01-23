/**
 * Coco Battle Royale is a nintendo entertainment system game.
 * Copyright (C) 2022  RodrigoDornelles	

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "neslib.h"

/**
 * CONSTANTS
 */

#define MAX_ENIMIES                     (20)
#define MAX_PLAYERS                     (2)
#define	PI						        (3)
#define SPEED                           (3)
#define MIN_ARENA_X                     (6)
#define MAX_ARENA_X                     (242)
#define MID_ARENA_X                     ((MAX_ARENA_X/2)+MIN_ARENA_X)
#define MIN_ARENA_Y                     (14)
#define MAX_ARENA_Y                     (200)
#define MID_ARENA_Y                     ((MAX_ARENA_Y/2)+MIN_ARENA_Y)
#define RANGE_ARENA                     (40)
#define LOOK_RIGHT                      (0)
#define LOOK_LEFT                       (1)

/**
 * FUNCTIONS
 */
#define SEED_PACK(v)                    ((unsigned char)(v-256))
#define SEED_UNPACK(v)                  ((unsigned int)(v+256))
#define ABS(v)                          (v<0?-v:v)
#define POW(v)                          (v*v)
#define DISTANCE(x1,y1,x2,y2)	        ((char)(ABS((int)x1-(int)x2)+ABS((int)y1-(int)y2)))
#define PRESSING(g,b)			        ((g&b)==b)
#define CLAMP(v,min,max)                (MIN(max,MAX(min,v)))
#define vram_adr_put(x,y,s)             {vram_adr(NTADR_A(x,y));vram_put(s);}

/**
 *  SPRITES
 */
#define SPR_PLAYER_DEAD                 0x00
#define SPR_PLAYER				        0x01
#define SPR_EDGE                        0x6C
#define SPR_LOGO                        0x60
#define SPR_BONE                        0x6D

/**
 * TYPES
 */

enum fsm_game_e {
	FSM_DRAW_MENU,
	FSM_DRAW_ARENA,
	FSM_MENU,
	FSM_GAMEPLAY
};

struct coco_s {
    unsigned char x;
    unsigned char y;
    union {
        signed char sprite;
        struct {
            unsigned char attacking: 1;
            unsigned char flipped: 1;
            unsigned char walking: 1;
        } status;
    } info;
};

/** GLOBAL CONSTANTS **/
static const unsigned char good_seeds[] = {
    SEED_PACK(283), SEED_PACK(285), SEED_PACK(499),
    SEED_PACK(301), SEED_PACK(305), SEED_PACK(274),
    SEED_PACK(332), SEED_PACK(336), SEED_PACK(353),
    SEED_PACK(368), SEED_PACK(378), SEED_PACK(395), 
    SEED_PACK(451), SEED_PACK(507)
};

const unsigned char palSprites[]={
	0x0f,0x30,0x27,0x16,
	0x0f,0x27,0x30,0x16,
	0x0f,0x2D,0x27,0x16,
};

/** GLOBAL VARIABLES **/
static struct coco_s players[MAX_ENIMIES];		/** all cocks entitys **/
static unsigned char gamepad[MAX_PLAYERS];		/** joystick inputs **/
static enum fsm_game_e gamestate;	            /** finite state machine **/
static unsigned char two_players;				/** local multiplayer mode **/
static unsigned char seed;						/** randomness control **/

/** GENERAL VARIABLES **/
static signed char s;
static unsigned char i,j;
static unsigned char spr;

/*
 * UTILS
 */
void put_all(const char c)
{
    vram_adr(NTADR_A(0,1));
    vram_fill(c, 32*28);
}

void put_str(unsigned int adr,const char *str)
{
    vram_adr(adr);
	for (;*str != NULL; vram_put(*str), str++);
}

void put_ret(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    /** adjust size **/
    x2 += 1; y2 += 1;

    /** draw border lines **/
    for (i = x1; i < x2; vram_adr(NTADR_A(i,y1)), vram_put(SPR_EDGE + 0x40), vram_adr(NTADR_A(i,y2)), vram_put(SPR_EDGE + 0x40), i++);
    for (i = y1; i < y2; vram_adr(NTADR_A(x1,i)), vram_put(SPR_EDGE + 0x50), vram_adr(NTADR_A(x2,i)), vram_put(SPR_EDGE + 0x50), i++);
    
    /** draw border edges **/
    vram_adr(NTADR_A(x1, y1));
    vram_put(SPR_EDGE);    
    vram_adr(NTADR_A(x1,y2));
    vram_put(SPR_EDGE + 0x10);
    vram_adr(NTADR_A(x2,y1));
    vram_put(SPR_EDGE + 0x20);
    vram_adr(NTADR_A(x2,y2));
    vram_put(SPR_EDGE + 0x30);
}

void put_logo()
{
    /** adjust position on screen **/
    const unsigned char pivot_x = 7;
    const unsigned char pivot_y = 5;

    /** draw text **/
    for (i = 0; i < 7; i++) for (j = 0; j < 12; j++) {
        vram_adr(NTADR_A(pivot_x + j, pivot_y + i));
        vram_put(SPR_LOGO + (i * 0x10) + j);
    }

    /** draw bones **/
    for (i = 0; i < 2; i++) {
        vram_adr_put(pivot_x + 11 + (i*3), pivot_y, SPR_BONE);
        vram_adr_put(pivot_x + 12 + (i*3), pivot_y, SPR_BONE + 1);
        vram_adr_put(pivot_x + 13 + (i*3), pivot_y, SPR_BONE + 2);
        vram_adr_put(pivot_x + 11 + (i*3), pivot_y + 1, SPR_BONE + 0x10);
        vram_adr_put(pivot_x + 12 + (i*3), pivot_y + 1, SPR_BONE + 0x11);
        vram_adr_put(pivot_x + 13 + (i*3), pivot_y + 1, SPR_BONE + 0x12);
        vram_adr_put(pivot_x + 12 + (i*3), pivot_y + 5, SPR_BONE + 0x31);
        vram_adr_put(pivot_x + 13 + (i*3), pivot_y + 5, SPR_BONE + 0x32);
        for (j = 2; j < 5; j++) {
            vram_adr_put(pivot_x + 12 + (i*3), pivot_y + j, SPR_BONE + 0x21);
            vram_adr_put(pivot_x + 13 + (i*3), pivot_y + j, SPR_BONE + 0x22);
        }
    }

    /** draw border **/
    put_ret(pivot_x -1, pivot_y - 1, pivot_x + 16, pivot_y + 5);
}

void spawn_cocks()
{
    set_rand(SEED_UNPACK(good_seeds[seed]));

	for (i = 0; i < MAX_ENIMIES; i++)
	{ 
        if (i <= two_players) {
            // random player positions
            players[i].x = MID_ARENA_X + (two_players? (i? RANGE_ARENA/2: RANGE_ARENA/(-2)): 0);
            players[i].y = MID_ARENA_Y;
        }
        else do {
            // random npc positions
            players[i].x = rand8();
            players[i].y = rand8();
        }
        while (
            // uncenter npc positions
            players[i].x > MID_ARENA_X - RANGE_ARENA &&  players[i].x < MID_ARENA_X + RANGE_ARENA
            && players[i].y > MID_ARENA_Y - RANGE_ARENA &&  players[i].y < MID_ARENA_Y + RANGE_ARENA
        );
        // look to center
        players[i].info.status.flipped = players[i].x > MID_ARENA_X? LOOK_LEFT: LOOK_RIGHT;
	}
}

void main(void)
{
	pal_spr(palSprites);
	pal_col(1,0x30);

	/** game loop **/
	for (;;)
	{
		/** reset sprite count **/
		spr = 0;

		/** wait for next frame**/
		ppu_wait_frame();

		/** joystick inputs **/
		for (i = 0; i <= two_players; gamepad[i] = pad_poll(i), i++);
		
		switch (gamestate) {
            case FSM_DRAW_MENU:
                ppu_off();
				put_all(NULL);
                put_logo();
                put_str(NTADR_A(11,15), "1 PLAYERS");
                put_str(NTADR_A(11,16), "2 PLAYERS");
				ppu_on_all();
				gamestate = FSM_MENU;
                break;

			case FSM_DRAW_ARENA:
				ppu_off();
				put_all(NULL);
                put_ret(MIN_ARENA_X/8, MIN_ARENA_Y/8, MAX_ARENA_X/8, MAX_ARENA_Y/8);
				put_str(NTADR_A(1,27),"COCKS:           (SELECT) MENU");
				put_str(NTADR_A(1,28),"PLAYERS:         (START) PAUSE");
				gamestate = FSM_GAMEPLAY;
				ppu_on_all();
				break;

            case FSM_MENU:
                /** select best seed by frame **/
                seed = (seed + 1) % sizeof(good_seeds);

                /** switch between multiplayer and singleplayer **/
                if (gamepad[0] & (PAD_UP | PAD_DOWN) && s == 0) {
                    two_players = two_players? 0: 1;
                }

                /** begin start the game **/
                if (gamepad[0] & (PAD_A | PAD_START)) {
				    gamestate = FSM_DRAW_ARENA;
                    spawn_cocks();
                }

                /** draw option **/
                oam_spr((10 * 8), (15 * 8) + (two_players << 3), '>', 0, 0);

                /** old input **/
                s = gamepad[0];
                break;

            case FSM_GAMEPLAY:
                /** player moves **/
                for (i = 0; i < MAX_PLAYERS; i++) {
                    s = PRESSING(gamepad[i], PAD_RIGHT) - PRESSING(gamepad[i], PAD_LEFT);
                    players[i].x += s * SPEED;
                    players[i].y += (PRESSING(gamepad[i], PAD_DOWN) - PRESSING(gamepad[i], PAD_UP)) * SPEED;
                    players[i].info.status.flipped = (s != 0)? (s == -1): !!players[i].info.status.flipped;
                    players[i].info.status.walking = (players[i].x ^ players[i].y)>>3;
                }
                
                /** arena colision **/ 
                for (i = 0; i < MAX_ENIMIES; i++) {
                    players[i].x = CLAMP(players[i].x, MIN_ARENA_X, MAX_ARENA_X);
                    players[i].y = CLAMP(players[i].y, MIN_ARENA_Y, MAX_ARENA_Y);
                }

                /** draw cocks **/
                for (i = 0; i < MAX_ENIMIES; i++) {
                    j = i <= two_players? i: 2; /** set color **/
                    spr = oam_spr(players[i].x, players[i].y, SPR_PLAYER + players[i].info.sprite, j, spr);
                }
                break;
		}
	}
}