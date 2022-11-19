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


/**
 * CONSTANTS
 */

#define MAX_ENIMIES                     (20)
#define MAX_PLAYERS                     (4)
#define SPEED                           (1)
#define MIN_ARENA_X                     (6)
#define MAX_ARENA_X                     (242)
#define MID_ARENA_X                     ((MAX_ARENA_X/2)+MIN_ARENA_X)
#define MIN_ARENA_Y                     (22)
#define MAX_ARENA_Y                     (208)
#define MID_ARENA_Y                     ((MAX_ARENA_Y/2)+MIN_ARENA_Y)
#define RANGE_ARENA                     (40)
#define LOOK_RIGHT                      (0)
#define LOOK_LEFT                       (1)
#define PLAYER_1                        (0)
#define PLAYER_2                        (1)
#define FRAME_PREPARE                   (7)
#define FRAME_ATTACKING                 (6)
#define FRAME_RECOVERY                  (2)
#define DIGIT_WINNER                    (22)
#define DIGIT_NOOB                      (21)
/**
 * FUNCTIONS
 */
#define SEED_PACK(v)                    ((unsigned char)(v-256))
#define SEED_UNPACK(v)                  ((unsigned int)(v+256))
#define ABS(v)                          (v<0?-v:v)
#define POW(v)                          (v*v)
#define PRESSING(g,b)			        ((g&b)==b)
#define CLAMP(v,min,max)                (MIN(max,MAX(min,v)))
#define DISTANCE(a,b)                   (a>b?a-b:b-a)
#define DISTANCE2D(a1,a2,b1,b2)         ((int)DISTANCE(a1,a2)+DISTANCE(b1,b2))

/**
 *  SPRITES
 */
#define SPR_EDGE                        0xBF
#define SPR_EDGE_1                      0xB9
#define SPR_EDGE_2                      0xBC
#define SPR_POINTER                     0x5C
#define SPR_JP_HITO                     0x0C
#define SPR_JP_O                        0x0D
#define SPR_JP_N                        0x0E
#define SPR_JP_DO                       0x0F
#define SPR_JP_RI                       0x1C
#define SPR_JP_TSU                      0x1D
#define SPR_JP_DZU                      0x1E
#define SPR_JP_KU                       0x1F

/**
 * TYPES
 */

enum fsm_game_e {
    FSM_MUSIC_MENU,
    FSM_MUSIC_ARENA,
	FSM_DRAW_MENU,
	FSM_DRAW_ARENA,
	FSM_MENU,
	FSM_GAMEPLAY,
    FSM_RESTART
};

enum fsm_ia_e {
    FSM_DEFAULT,
    FSM_RANDOM,
    FSM_HUNTER_WAIT,
    FSM_HUNTER,
    FSM_SCAPE_WAIT,
    FSM_SCAPE,
    FSM_WINNER
};

struct coco_s {
    unsigned char x;
    unsigned char y;
    unsigned char framedata;
    union {
        signed char sprite;
        struct {
            unsigned char walking: 1;
            unsigned char flipped: 1;
            unsigned char attacking: 1;
            unsigned char recovering: 1;
            unsigned char coloreven: 1;
            unsigned char death: 1;
        } status;
    } info;
};

struct npc_ia_s {
    unsigned char target;
    unsigned char input;   
    enum fsm_ia_e state;
};

struct framecount_s {
    unsigned char frames: 2;
    unsigned char hunter_last: 2;
    unsigned char hunter_step: 4;
};

/** INTERNACIONALIZATION **/
const char I18N_EN_CONTINUE[] = "CONTINUE!";
const char I18N_EN_1_PLAYERS[] = "1 PLAYERS";
const char I18N_EN_2_PLAYERS[] = "2 PLAYERS";
const char I18N_EN_3_PLAYERS[] = "3 PLAYERS";
const char I18N_EN_4_PLAYERS[] = "4 PLAYERS";
const char I18N_EN_LOOSER[] = "#XX\r\nYou were fried.";
const char I18N_EN_WINNER[] = "#1\r\nYOU ARE\r\nULTIMATE\r\nHOT CHICKEN!!";
const char I18N_EN_CREDITS_1[] = "RODRIGO DORNELLES (C) 2022";
const char I18N_EN_CREDITS_2[] = "HTTP://PSYWAVE-GAMES.GITHUB.IO";
const char I18N_EN_RESTART_CNT[] = "STARTING IN   SECONDS...";
const char I18N_EN_RESTART_BTN[] = " HOLD (ATACK) FOR NEW BATTLE!";
const char I18N_EN_RESTART_COIN[] = "INSERT (COIN) FOR NEW BATTLE!!  ";
const char I18N_EN_GAMEPLAY_NAME[] = "      COCO BATTLE ROYALE II      ";

const char I18N_EN_LOGO[] = {
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x62, 0x63, 0x20, 0x20, 0x20, 0x66, 0x67, 0x68, 0x66, 0x67, 0x68,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x72, 0x73, 0x20, 0x20, 0x20, 0x76, 0x77, 0x78, 0x76, 0x77, 0x78,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0xA6, 0xA7, 0xA8, 0x20, 0x20, 0x87, 0x88, 0x20, 0x87, 0x88,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0xB6, 0xB7, 0xB8, 0x20, 0x20, 0x87, 0x88, 0x20, 0x87, 0x88,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x82, 0x83, 0xA6, 0xA6, 0xA7, 0xA8, 0x87, 0x88, 0x20, 0x87, 0x88,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0x92, 0x93, 0xB6, 0xB6, 0xB7, 0xB8, 0x97, 0x98, 0x20, 0x97, 0x98
};

const char jp = FAMICON_VERSION;
const char I18N_JP_CONTINUE[] = {
    ' ', ' ', ' ', SPR_JP_TSU, SPR_JP_DZU, SPR_JP_KU, ' ', ' ', ' '
};
const char I18N_JP_1_PLAYERS[] = {
    ' ', '1',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI, ' '
};
const char I18N_JP_2_PLAYERS[] = {
    ' ', '2', SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI, ' '
};
const char I18N_JP_3_PLAYERS[] = {
    ' ', '3', SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI, ' '
};
const char I18N_JP_4_PLAYERS[] = {
    ' ', '4', SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI, ' '
};

const char I18N_JP_LOGO[] = {
    0xC0, 0xC1, 0xC2, 0xC0, 0xC1, 0xC2, 0x20, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0x20,
    0x20, 0x20, 0xD2, 0x20, 0x20, 0xD2, 0x20, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0x20,
    0x20, 0x20, 0xE2, 0x20, 0x20, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0x20,
    0xF0, 0xF1, 0xF2, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD,
    0x20, 0x20, 0x20, 'C', 'O', 'C', 'O', ' ', 'B', 'A', 'T', 'T', 'L', 'E', 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 'R', 'O', 'Y', 'A', 'L', 'E', ' ', 'I', 'I', 0x20, 0x20, 0x20, 0x20, 
};
 
/** GLOBAL CONSTANTS **/
static const unsigned char npc_groups[] = {
    0, 0, 0, 1, 2,
    3, 0, 1, 2, 3,
    3, 0, 1, 2, 3,
    3, 0, 1, 2, 3,
};

static const unsigned char good_seeds[] = {
    SEED_PACK(283), SEED_PACK(285), SEED_PACK(499),
    SEED_PACK(301), SEED_PACK(305), SEED_PACK(274),
    SEED_PACK(332), SEED_PACK(336), SEED_PACK(353),
    SEED_PACK(368), SEED_PACK(378), SEED_PACK(395), 
    SEED_PACK(451), SEED_PACK(507)
};

const char paletteBackground[] = {
	0x0f,0x30,0x27,0x0f,
	0x0f,0x0f,0x0f,0x0f,
	0x0f,0x27,0x30,0x0f
};

const char paletteSprite[] = {
	0x0f,0x30,0x27,0x16,
	0x0f,0x2C,0x25,0x30,
	0x0f,0x13,0x15,0x25,
	0x0f,0x26,0x2A,0x36,
};

const char digit_lockup[2][] = {
    {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0xD1, '1', '2', '3'},
    {' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '2', 0xD0, 0xFF, 0xEF, 0xDF}
};

/** GLOBAL VARIABLES **/
static struct npc_ia_s npcs[MAX_ENIMIES];       /** IA controll **/
static struct framecount_s framecount;          /** IA manager groups **/
static struct coco_s players[MAX_ENIMIES];		/** all cocks entitys **/
static enum fsm_game_e gamestate;	            /** finite state machine **/
static unsigned char seed;						/** randomness control **/
static unsigned char roosters_count;            /** cocks counter **/
static unsigned char roosters_total;            /** total of cocks arrive **/

/** micromages 4 players **/
static unsigned char joysticks = 1;				/** local multiplayer mode **/
static unsigned char gamepad_old[MAX_PLAYERS];  /** last frame joysticks inputs **/
static const unsigned char* const gamepad = &joy1;   /** joystick inputs **/

/** score rank 4 players **/
static unsigned char player_score[4] = {0, 1, 11, 21};

/** GENERAL VARIABLES **/
static signed char s;
static unsigned int big1, big2;
static unsigned char i,j,l,r;
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
    unsigned char fixed_x = (adr & 0x1f);
    vram_adr(adr);
    for (;*str != '\0'; ++str) {
        /** line feed */
        if (*str == '\n') {
            /** y + 1*/ 
            adr += 32;
            vram_adr(adr);
            continue;
        }
        /** carrier return */
        if (*str == '\r') {
            /** x = fixed_x */  
            adr = fixed_x | (adr & 0xffe0);
            vram_adr(adr);
            continue;
        }
        /** print */
        vram_put(*str);
        ++adr;
    }
}

void put_ret(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    /** adjust size **/
    x2 += 1; 
    y2 += 1;
    j = x2 - x1;

    /** draw horizontal lines **/
    for (j = x1 + 1; j < x2; ++j)
    {
        vram_adr(NTADR_A(j,y1));
        vram_put(SPR_EDGE_1 + (j % 3));
        vram_adr(NTADR_A(j,y2));
        vram_put(SPR_EDGE_1 + (j % 3));
    }

    /** draw vertical lines **/
    for (j = y1 + 1; j < y2; ++j)
    {
        vram_adr(NTADR_A(x1, j));
        vram_put(SPR_EDGE_2 + (j % 3));
        vram_adr(NTADR_A(x2, j));
        vram_put(SPR_EDGE_2 + (j % 3));
    }
}

void put_logo()
{
    for (i = 0; i < 6; i++){
        vram_adr(NTADR_A(7, 4 + i));
        vram_write((unsigned char *) (jp? I18N_JP_LOGO: I18N_EN_LOGO) + (i*17), 17);
    }

    put_ret(6, 3, 23, 9);
}

void put_score()
{
    /** show scores */
    for (l = 0; l < MAX_PLAYERS; ++l) {
        s = player_score[l];
        vram_adr(NTADR_A(l << 2, 1));
        if (l < joysticks) {
            vram_put(0x5c + l);
            vram_put('P');
        } else {
            vram_put(' ');
            vram_put(' ');
        }
        vram_put(digit_lockup[1][s]);
        vram_put(digit_lockup[0][s]);
    }
    
    vram_adr(ATADR_A(0, 1));
    vram_fill(BR_BL_TR_TL(0,0,3,3), 8);
}

void spawn_cocks()
{
    set_rand(SEED_UNPACK(good_seeds[seed]));
    /** reset scores**/
    for (i = 0; i < MAX_PLAYERS; i++) {
        player_score[i] = 0;
    }
    /** reset postions **/
	for (i = 0; i < MAX_ENIMIES; i++)
	{ 
        if (i < joysticks) {
            // random player positions
            players[i].x = MID_ARENA_X + (i&1? RANGE_ARENA/2: RANGE_ARENA/(-2));
            players[i].y = MID_ARENA_Y + (i&2? RANGE_ARENA/2: RANGE_ARENA/(-2));
        }
        else do {
            // random npc positions
            players[i].x = rand8();
            players[i].y = rand8();
            // reset npc behavior
            npcs[i].input = 0;
            npcs[i].state = FSM_DEFAULT;
        }
        while (
            // uncenter npc positions
            players[i].x > MID_ARENA_X - RANGE_ARENA &&  players[i].x < MID_ARENA_X + RANGE_ARENA
            && players[i].y > MID_ARENA_Y - RANGE_ARENA &&  players[i].y < MID_ARENA_Y + RANGE_ARENA
        );
        // look to center
        players[i].info.status.flipped = players[i].x > MID_ARENA_X? LOOK_LEFT: LOOK_RIGHT;
        players[i].info.status.coloreven = !(i & 1);
        players[i].info.status.death = FALSE;
	}
}

void ia_hunter_cycle()
{
    /** verify cycle is completed **/
    if (framecount.hunter_last == framecount.frames++) {
        return;
    }

    /** verify steps to execute **/
    if (++framecount.hunter_step) {
        return;
    }

    /** next group **/
    framecount.hunter_last += 1;

    /** search by hunter **/
    for (i = framecount.hunter_last; i < MAX_ENIMIES; i += 4) {
        /** finded **/
        if (npcs[i].state == FSM_HUNTER_WAIT) {
            big1 = 0xFFFF;
            for (j = 0; j < MAX_ENIMIES; j++) {
                if (i == j) {
                    continue;
                }
                if (players[j].info.status.death) {
                    continue;
                }
                // calc distance aproximy
                big2 = DISTANCE2D(players[i].x, players[j].x, players[i].y, players[j].y);
                if (big2 > big1) {
                    continue;
                }
                npcs[i].state = FSM_HUNTER;
                npcs[i].target = j;
                big1 = big2;
            }
            break;
        }
    }    
}

void ia_process(unsigned char npc)
{
    switch (npcs[npc].state) {
        case FSM_DEFAULT:
            npcs[npc].input = NULL;
            npcs[npc].state = FSM_RANDOM;
            break;

        case FSM_HUNTER:
            j = npcs[npc].target;
            if (players[j].info.status.death) {
                npcs[npc].state = FSM_RANDOM;
                break;
            }
            npcs[npc].input = 0;
            npcs[npc].input |= players[npc].x > players[j].x? PAD_LEFT: PAD_RIGHT;
            npcs[npc].input |= players[npc].y > players[j].y? PAD_UP: PAD_DOWN;
            npcs[npc].input |= DISTANCE2D(players[i].x, players[j].x, players[i].y, players[j].y) < 8? PAD_A: NULL;
            r = rand8();
            if (roosters_total > 2 && r < 10){
                npcs[npc].state = FSM_RANDOM;
            }
            else if (npcs[npc].state == FSM_HUNTER && npcs[j].state == FSM_HUNTER && r < 10) {
                npcs[npc].state = FSM_RANDOM;
            }
            if (r < 25) {
                npcs[npc].input ^= (PAD_LEFT | PAD_RIGHT);
            }
            else if (r < 50) {
                npcs[npc].input ^= (PAD_UP | PAD_DOWN);
            }
            break;

        case FSM_WINNER:
            if (players[npc].y < MID_ARENA_Y - 8) {
                npcs[npc].input = PAD_DOWN;
            }
            else if (players[npc].y > MID_ARENA_Y + 8) {
                npcs[npc].input = PAD_UP;
            }
            else if (players[npc].x > MID_ARENA_X + 32) {
                npcs[npc].input = PAD_LEFT;
            }
            else if (players[npc].x < MID_ARENA_X - 32) {
                npcs[npc].input = PAD_RIGHT;
            }
            else if (rand8() < 64) {
                npcs[npc].input = rand8() % 2? PAD_LEFT: PAD_RIGHT;
            }
            break;
    
        default:
            j = rand8();
            if (roosters_total == 1) {
                npcs[npc].state = FSM_WINNER;
            }
            else if (j < 30) {
                npcs[npc].state = FSM_HUNTER_WAIT;
            }
            else if (j < 60) {
                npcs[npc].input ^= (rand8() + npc + j) & (PAD_LEFT | PAD_RIGHT);
                // fixed ambiguous input on axis x
                if (npcs[npc].input & PAD_LEFT && npcs[npc].input & PAD_RIGHT) {
                    npcs[npc].input ^= (PAD_LEFT | PAD_RIGHT);
                }
            }
            else if (j < 90) {
                npcs[npc].input ^= (rand8() + npc + j) & (PAD_DOWN | PAD_UP);
                // fixed ambiguous input on axis x
                if (npcs[npc].input & PAD_DOWN && npcs[npc].input & PAD_UP) {
                    npcs[npc].input ^= (PAD_DOWN | PAD_UP);
                }
            }
            break;
    }
}

void main(void)
{
    pal_bg(paletteBackground);
	pal_spr(paletteSprite);

	/** game loop **/
	for (;;)
	{
		/** reset sprite count **/
		spr = 0;

		/** wait for next frame**/
		ppu_wait_nmi();

		/** joystick inputs **/
        for (i = 0; i < joysticks; i++) {
            gamepad_old[i] = gamepad[i];
        }
        updateInput();
		
		switch (gamestate) {
            case FSM_MUSIC_MENU:
                music_play(0);
				gamestate = FSM_DRAW_MENU;
                break;

            case FSM_MUSIC_ARENA:
                music_play(1);
				gamestate = FSM_RESTART;
                break;

            case FSM_DRAW_MENU:
                ppu_off();
                oam_clear();
                /** reset nametable */
				put_all(' ');
                /** reset attribute table*/
                vram_adr(ATADR_A(0, 1));
                vram_fill(0, 8*8);
                /** colorize 2 in JAP version*/
                vram_adr(ATADR_A(18,6));
                vram_put(BR_BL_TR_TL(3,0,0,0));
                /** colorize link */
                vram_adr(ATADR_A(1,26));
                vram_fill(BR_BL_TR_TL(3,3,0,0), 30);
                /** game title */
                put_logo();
                /** put menu options */
                vram_adr(NTADR_A(11,16));
                vram_write(jp? I18N_JP_1_PLAYERS: I18N_EN_1_PLAYERS, 9);
                vram_adr(NTADR_A(11,17));
                vram_write(jp? I18N_JP_2_PLAYERS: I18N_EN_2_PLAYERS, 9);
                vram_adr(NTADR_A(11,18));
                vram_write(jp? I18N_JP_3_PLAYERS: I18N_EN_3_PLAYERS, 9);
                vram_adr(NTADR_A(11,19));
                vram_write(jp? I18N_JP_4_PLAYERS: I18N_EN_4_PLAYERS, 9);
                /** put copyright */
                put_str(NTADR_A(3,25), I18N_EN_CREDITS_1);
                put_str(NTADR_A(1,26), I18N_EN_CREDITS_2);
                /** put menu option 'continue' */
                if (roosters_count) {
                    vram_adr(NTADR_A(11,15));
                    vram_write(jp? I18N_JP_CONTINUE: I18N_EN_CONTINUE, 9);
                }
				ppu_on_all();
				gamestate = FSM_MENU;
                break;

			case FSM_DRAW_ARENA:
				ppu_off();
                pal_bg(paletteBackground);
	            pal_spr(paletteSprite);
                oam_clear();
				put_all(' ');
                put_ret(MIN_ARENA_X/8, MIN_ARENA_Y/8, MAX_ARENA_X/8, MAX_ARENA_Y/8);
				put_str(NTADR_A(0,28), I18N_EN_GAMEPLAY_NAME);
				put_str(NTADR_A(26,1), "\x10  /20");
                put_score();
                gamestate = FSM_GAMEPLAY;
				ppu_on_all();
				break;

            case FSM_MENU:
                /** select best seed by frame **/
                /** !jp randomization adjustment to standardize US/JAP**/
                seed = (seed + 1 + !jp) % sizeof(good_seeds);
                pal_col(15, (16 << (seed >> 3)) + 7); /**< 17, 37 **/

                /** switch between resume, singleplayers and multiplayer **/
                if (gamepad_old[PLAYER_1] == 0 && gamepad[PLAYER_1] & PAD_UP) {
                    s -= 1;
                }
                else if (gamepad_old[PLAYER_1] == 0 && gamepad[PLAYER_1] & PAD_DOWN) {
                    s += 1;
                }

                /** Limit menu options **/
                s = CLAMP(s, roosters_count == 0, 4);

                /** begin start the game **/
                i = gamepad[PLAYER_1] & PAD_START; /** start button or insert coin to use all options **/
                i|= (gamepad_old[PLAYER_1] == 0) && (gamepad[PLAYER_1] & PAD_SELECT) && (s == 0); /* use only resume */

                if (i) {
				    switch (s) {
                        case 0:
                            gamestate = FSM_DRAW_ARENA;
                            break;

                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            joysticks = s;
                            gamestate = FSM_MUSIC_ARENA;
                            /** when non-zero is inactive */
                            for(i = 0; i < MAX_PLAYERS; i++) {
                                playerActive[i] = i >= s;
                            }
                            break;
                    }
                }

                /** draw option 6, 3, 23, 9 **/
                spr = 0;
                spr = oam_spr((10 * 8), (15 * 8) + (s << 3), 0xCF, 0, spr);
                spr = oam_spr((20 * 8), (15 * 8) + (s << 3), 0xCF, 0, spr);
                /* put ret edges **/
                spr = oam_spr((6 * 8), (3 * 8) - 1, SPR_EDGE, 0xC0, spr);
                spr = oam_spr((24 * 8), (3 * 8) - 1, SPR_EDGE, 0x80, spr);
                spr = oam_spr((6 * 8), (10 * 8) - 1, SPR_EDGE, 0x40, spr);
                spr = oam_spr((24 * 8), (10 * 8) - 1, SPR_EDGE, 0x00, spr);
                break;

            case FSM_GAMEPLAY:
                /** pause options **/
                if (gamepad_old[PLAYER_1] == 0) {
                    /** restart game **/
                    if ((gamepad[PLAYER_1] & PAD_START)) {
                        gamestate = FSM_RESTART;
                        break;
                    }

                    /** back to main menu **/
                    if ((gamepad[PLAYER_1] & PAD_SELECT)) {
                        gamestate = FSM_DRAW_MENU;
                        break;
                    }
                }

                /** preapare **/
                roosters_count = 0;
                ia_hunter_cycle();

                /** entitys loop **/
                for (i = 0; i < MAX_ENIMIES; i++) {
                    /** out of game **/
                    if (players[i].info.status.death) {
                        continue;
                    }

                    /** number of coocks alive **/
                    roosters_count += 1;
                    
                    /** player input **/
                    if (i < joysticks) {
                        if(gamepad[i] & PAD_LEFT) {
                            players[i].x -= 1 << SPEED;
                            s = -1;
                        }
                        else if(gamepad[i] & PAD_RIGHT) {
                            players[i].x += 1 << SPEED;
                            s = 1;
                        } else {
                            s = 0;
                        }
                        if(gamepad[i] & PAD_UP) {
                            players[i].y -= 1 << SPEED;
                        }
                        else if(gamepad[i] & PAD_DOWN) {
                            players[i].y += 1 << SPEED;
                        }
                        if ((gamepad[i] & (PAD_B | PAD_A)) && !(gamepad_old[i] & (PAD_B | PAD_A)) && players[i].framedata == 0) {
                            players[i].framedata = FRAME_PREPARE;
                            players[i].info.status.attacking = 1;
                        }
                    } 
                    /** npc think input **/
                    else if (framecount.frames == npc_groups[i]) {
                        ia_process(i);
                        s = 0;
                    }
                    /** npc excute input **/
                    else {
                        if(npcs[i].input & PAD_LEFT) {
                            players[i].x -= 1 << SPEED;
                            s = -1;
                        }
                        else if(npcs[i].input & PAD_RIGHT) {
                            players[i].x += 1 << SPEED;
                            s = 1;
                        } else {
                            s = 0;
                        }
                        if(npcs[i].input & PAD_UP) {
                            players[i].y -= 1 << SPEED;
                        }
                        else if(npcs[i].input & PAD_DOWN) {
                            players[i].y += 1 << SPEED;
                        }
                        if (npcs[i].input & PAD_A) {
                            players[i].framedata = FRAME_PREPARE;
                            players[i].info.status.attacking = 1;
                        }
                    }

                    /** animation sprite **/
                    players[i].info.status.walking = (players[i].x ^ players[i].y)>>3;
                    players[i].info.status.attacking = players[i].framedata < FRAME_ATTACKING && players[i].framedata > FRAME_RECOVERY;
                    players[i].info.status.recovering = players[i].framedata && !players[i].info.status.attacking;
                    switch (s) {
                        case 1: players[i].info.status.flipped = 0; break;
                        case -1: players[i].info.status.flipped = 1; break;
                    }

                    /** arena colision **/
                    players[i].x = CLAMP(players[i].x, MIN_ARENA_X, MAX_ARENA_X);
                    players[i].y = CLAMP(players[i].y, MIN_ARENA_Y, MAX_ARENA_Y);

                    /** mediator **/
                    if (players[i].framedata) {
                        players[i].framedata -= 1;
                        for (j = 0; j < MAX_ENIMIES; j++) {
                            /** end of attack **/
                            if (!players[i].info.status.attacking) {
                                break;
                            }
                            /** not attacking itself **/
                            if (i == j) {
                                continue;
                            }
                            /** blocking attack **/
                            if (players[j].framedata) {
                                continue;
                            }
                            /** pidgeot is fainted dude **/
                            if (players[j].info.status.death) {
                                continue;
                            }
                            /** far far away **/
                            if (DISTANCE(players[i].x, players[j].x) > 8 || DISTANCE(players[i].y, players[j].y) > 8) {
                                continue;
                            }

                            /** kill store **/
                            players[j].info.status.death = TRUE;

                            /** save score **/
                            for (r = 0; r < joysticks; ++r){
                                /** winner */
                                if (roosters_total <= 2 && i == r) {
                                    player_score[r] = DIGIT_WINNER;
                                }
                                /** looser */
                                else if (j == r) {
                                    /** set ranking  **/
                                    player_score[r] = roosters_total;
                                    /** noob */
                                    if (roosters_total == MAX_ENIMIES) {
                                        player_score[r] = DIGIT_NOOB;
                                    }
                                    /** podium */
                                    if (roosters_total <= 3) {
                                        player_score[r] = DIGIT_NOOB + roosters_total;
                                    }
                                /** npc */
                                } else {
                                    continue;
                                }

                                /** flash screen & put score */
                                pal_col(0,0x30);
                                ppu_off();
                                put_score();
                                ppu_on_all();
                                pal_col(0, paletteBackground[0]);
                            }
                        }
                    }

                    /** draw pointer **/
                    if (i < joysticks) {
                        spr = oam_spr(players[i].x, players[i].y - 8, SPR_POINTER + i, 4, spr);
                    }

                    /** draw cock **/   
                    spr = oam_spr(players[i].x, players[i].y, players[i].info.sprite, (i >> 1), spr);
                }

                /** draw number of coocks **/
                roosters_total = roosters_count;
                spr = oam_spr((27 * 8), (1 * 8) -1, digit_lockup[1][roosters_count], 0, spr);
                spr = oam_spr((28 * 8), (1 * 8) -1, digit_lockup[0][roosters_count], 0, spr);

                /* put ret edges **/
                spr = oam_spr(MIN_ARENA_X - 6, MIN_ARENA_Y - 7, SPR_EDGE, 0xC0, spr);
                spr = oam_spr(MAX_ARENA_X + 6, MIN_ARENA_Y - 7, SPR_EDGE, 0x80, spr);
                spr = oam_spr(MIN_ARENA_X - 6, MAX_ARENA_Y + 7, SPR_EDGE, 0x40, spr);
                spr = oam_spr(MAX_ARENA_X + 6, MAX_ARENA_Y + 7, SPR_EDGE, 0x00, spr);

                /* end draw */
                oam_hide_rest(spr);
                break;

            case FSM_RESTART:
                /** reset player status **/
                for (i = 0; i < MAX_ENIMIES; players[i].info.sprite = 0, i++);
                seed = (seed + 1) % sizeof(good_seeds);
                gamestate = FSM_DRAW_ARENA;
                roosters_total = 0;
                spawn_cocks();
                break;
		}
	}
}