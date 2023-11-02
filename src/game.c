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
#include "micromages.h"
#include <stdbool.h>

#ifndef BRAZUKA
#define BRAZUKA                         (0)
#endif
#ifndef FAMICON_VERSION
#define FAMICON_VERSION                 (0)
#endif
#ifndef MAX_ENIMIES
#define MAX_ENIMIES                     (20)
#endif
#ifndef MAX_PLAYERS
#define MAX_PLAYERS                     (6)
#endif
#ifndef SPEED
#define SPEED                           (3)
#endif
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
#define SPR_EDGE                        0xD7
#define SPR_EDGE_1                      0xB9
#define SPR_EDGE_2                      0xCD
#define SPR_POINTER                     0x18
#define SPR_JP_HITO                     0x06
#define SPR_JP_O                        0x07
#define SPR_JP_N                        0x0E
#define SPR_JP_DO                       0x0F
#define SPR_JP_RI                       0x16
#define SPR_JP_TSU                      0x17
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
    FSM_DRAW_CELEBRATION,
	FSM_MENU,
    FSM_COUNT,
	FSM_GAMEPLAY,
	FSM_CELEBRATION,
    FSM_RESTART
};

struct coco_s {
    unsigned char x;
    unsigned char y;
    unsigned char framedata;
    union {
        signed char sprite;
        struct {
            unsigned char walking: 1;
            unsigned char attacking: 1;
            unsigned char recovering: 1;
            unsigned char colorvar: 2;
            unsigned char death: 1;
            unsigned char flipped: 1;
        } status;
    } info;
};

struct framecount_s {
    unsigned char frames: 2;
    unsigned char hunter_last: 2;
    unsigned char hunter_step: 4;
};

static const unsigned char good_seeds[] = {
    SEED_PACK(283), SEED_PACK(285), SEED_PACK(499),
    SEED_PACK(301), SEED_PACK(305), SEED_PACK(274),
    SEED_PACK(332), SEED_PACK(336), SEED_PACK(353),
    SEED_PACK(368), SEED_PACK(378), SEED_PACK(395), 
    SEED_PACK(451), SEED_PACK(507)
};


const char paletteBackgroundMenu[] = {
    0x0f, 0x30, 0x30, 0x30,
    0x0f, 0x30, 0x0f, 0x30,
    0x0f, 0x30, 0x0f, 0x0f
};

const char paletteBackground[] = {
    0x0f, 0x30, 0x30, 0x30,
    0x0f, 0x30, 0x30, 0x30,
    0x0f, 0x30, 0x30, 0x30,
	0x0f, 0x27, 0x30, 0x0f
};

const char paletteSprite[] = {
	0x0f,0x30,0x27,0x16,
	0x0f,0x2C,0x24,0x29,
	0x0f,0x26,0x21,0x2a,
	0x0f,0x15,0x14,0x38,
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

struct npc_ia_s {
    unsigned char target;
    unsigned char input;   
    enum fsm_ia_e state;
};

const char digit_lockup[2][] = {
    {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0x5F, '1', '2', '3'},
    {' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '2', 0x5E, 0xC5, 0xC6, 0xC7}
};

/** GLOBAL VARIABLES **/
static struct framecount_s framecount;          /** IA manager groups **/
static unsigned int framecount_seed;             /** IA Behavior seed**/
static struct coco_s players[MAX_ENIMIES];		/** all cocks entitys **/
static enum fsm_game_e gamestate;	            /** finite state machine **/
static unsigned char seed;						/** randomness control **/
static unsigned char roosters_count;            /** cocks counter **/
static unsigned char roosters_total;            /** total of cocks arrive **/
static struct npc_ia_s npcs[MAX_ENIMIES];

/** micromages 4 players **/
static unsigned char joysticks = 1;				/** local multiplayer mode **/
static unsigned char gamepad_old[MAX_PLAYERS];  /** last frame joysticks inputs **/
static const unsigned char* const gamepad = &joy1;   /** joystick inputs **/

/** score rank 4 players **/
static unsigned char player_score[4] = {0, 1, 11, 21};
static unsigned char player_rank[4] = {186, 186, 202, 105};

/** GENERAL VARIABLES **/
static signed char s;
static unsigned int big1, big2;
static unsigned char i,j,l,r;
static unsigned char spr;


static unsigned char second;                  
static unsigned char speed;
static unsigned char winner_id;
static struct coco_s player;

static signed char earthquake;
static signed char eqx;
static signed char eqy;


/**
 * @short screen menu
 */
const char I18N_CONTINUE[] =
#if FAMICON_VERSION == 0 || BRAZUKA == 1
    "CONTINUE!";
#else
    {' ', ' ', ' ', SPR_JP_TSU, SPR_JP_DZU, SPR_JP_KU, '\0'};
#endif

/**
 * @short screen menu
 */
const char I18N_PLAYERS[] =
#if BRAZUKA == 1
#if MAX_PLAYERS == 1
"JOGAR AGORA"
#else
"1 JOGADOR"
#endif
#if MAX_PLAYERS >= 2
"\r\n2 JOGADORES"
#endif
#if MAX_PLAYERS >= 3
"\r\n3 JOGADORES"
#endif
#if MAX_PLAYERS >= 4
"\r\n4 JOGADORES"
#endif
#if MAX_PLAYERS >= 5
"\r\n5 JOGADORES"
#endif
#if MAX_PLAYERS >= 6
"\r\n6 JOGADORES"
#endif
;
#elif FAMICON_VERSION == 0
#if MAX_PLAYERS == 1
"START GAME"
#else
"1 PLAYER"
#endif
#if MAX_PLAYERS >= 2
"\r\n2 PLAYERS"
#endif
#if MAX_PLAYERS >= 3
"\r\n3 PLAYERS"
#endif
#if MAX_PLAYERS >= 4
"\r\n4 PLAYERS"
#endif
#if MAX_PLAYERS >= 5
"\r\n5 PLAYERS"
#endif
#if MAX_PLAYERS >= 6
"\r\n6 PLAYERS"
#endif
;
#else
{
    ' ', '1',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#if MAX_PLAYERS >= 2
    '\r', '\n', ' ', '2',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#endif
#if MAX_PLAYERS >= 3
    '\r', '\n',  ' ', '3',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#endif
#if MAX_PLAYERS >= 4
    '\r', '\n',  ' ', '4',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#endif
#if MAX_PLAYERS >= 5
    '\r', '\n',  ' ', '5',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#endif
#if MAX_PLAYERS >= 6
    '\r', '\n',  ' ', '6',  SPR_JP_HITO, ' ', SPR_JP_O, SPR_JP_N, SPR_JP_DO, SPR_JP_RI,
#endif
    '\0'
};
#endif
/**
 * @short runtime gameplay
 */
const char I18N_COUNT[] =
#if BRAZUKA == (1)
"BATALHAREM\xdc\xdc";
#else
"STARTINGAT\xdc\xdc";
#endif


const char I18N_EN_SCORE[] = " SCORE: ";
const char I18N_EN_LOOSER[] = "#XX\r\nYOU ARE FRIED.";
const char I18N_EN_WINNER[] = "#1\r\nYOU ARE\r\nULTIMATE\r\nHOT CHICKEN!!";
const char I18N_EN_CREDITS_1[] = "RODRIGO DORNELLES (C) 2022";
const char I18N_EN_CREDITS_2[] = "HTTP://PSYWAVE-GAMES.GITHUB.IO";
const char I18N_EN_RESTART_CNT[] = "STARTING IN   SECONDS...";
const char I18N_EN_RESTART_BTN[] = " HOLD (ATACK) FOR NEW BATTLE!";
const char I18N_EN_RESTART_COIN[] = "INSERT (COIN) FOR NEW BATTLE!!  ";
const char I18N_EN_GAMEPLAY_NAME[] = "      COCO BATTLE ROYALE II      ";

const char I18N_EN_LOGO[] = {
    0x60, 0x61, 0xA2, 0xA3, 0x64, 0x65, 0xA2, 0xA3, 0x20, 0x20, 0x20, 0xC9, 0xCA, 0xCB, 0xC9, 0xCA, 0xCB,
    0x70, 0x71, 0xCC, 0xB3, 0x74, 0x75, 0xCC, 0xB3, 0x20, 0x20, 0x20, 0xD9, 0xDA, 0xDB, 0xD9, 0xDA, 0xDB,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x84, 0x85, 0xA6, 0xA7, 0x62, 0x63, 0xEA, 0xEB, 0x20, 0xEA, 0xEB,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x94, 0x95, 0xB6, 0xB7, 0x72, 0x73, 0xEA, 0xEB, 0x20, 0xEA, 0xEB,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x82, 0x83, 0xA6, 0xA7, 0x62, 0x63, 0xEA, 0xEB, 0x20, 0xEA, 0xEB,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0x92, 0x93, 0xB6, 0xB7, 0x72, 0x73, 0xFA, 0xFB, 0x20, 0xFA, 0xFB
};

const char jp = FAMICON_VERSION;
const char I18N_JP_LOGO[] = {
    0x20, 0xC0, 0xC1, 0xC2, 0xC0, 0xC1, 0xC2, 0x20, 0xC4, 0xF3, 0xD6, 0xF3, 0x20, 0x20, 0xF3, 0xF3, 0x20,
    0x20, 0x20, 0x20, 0xD2, 0x20, 0x20, 0xD2, 0x20, 0xD4, 0xD5, 0x20, 0xEA, 0xE0, 0xE1, 0xEA, 0xEA, 0x20,
    0x20, 0x20, 0x20, 0xE2, 0x20, 0x20, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xEA, 0xE8, 0xE9, 0xE7, 0xEA, 0xC3,
    0x20, 0xF0, 0xF1, 0xF2, 0xF0, 0xF1, 0xF2, 0xF5, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xD0, 0xD1, 0xD3,
    0x20, 0x5B, 0x5C, 0x5D, 'U', 'L', 'T', 'I', 'M', 'A', 'T', 'E', ' ', 'C', 'O', 'C', 'O',
    0x20, 'B', 'A', 'T', 'T', 'L', 'E', ' ', 'R', 'O', 'Y', 'A', 'L', 'E', ' ', 'I', 'I'
};

void oam_edge(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    spr = oam_spr(x1 - 6, y1 - 7, SPR_EDGE, 0xC0, spr);
    spr = oam_spr(x2 + 6, y1 - 7, SPR_EDGE, 0x80, spr);
    spr = oam_spr(x1 - 6, y2 + 7, SPR_EDGE, 0x40, spr);
    spr = oam_spr(x2 + 6, y2 + 7, SPR_EDGE, 0x00, spr);
}

void put_all(const char c)
{
    vram_adr(NTADR_A(0,0));
    vram_fill(c, 32*30);
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

void put_borders()
{
    put_ret(MIN_ARENA_X/8, MIN_ARENA_Y/8, MAX_ARENA_X/8, MAX_ARENA_Y/8);
    vram_adr(ATADR_A(0, 1));
    vram_fill(BR_BL_TR_TL(0,0,3,3), 8);
    vram_adr(ATADR_A(0, 27));
    vram_fill(BR_BL_TR_TL(0,0,1,1), 8);
    for (i = 5; i < 25; i += 2) {
        vram_adr(ATADR_A(0, i));
        vram_put(BR_BL_TR_TL(3,1,3,1));
        vram_adr(ATADR_A(30, i));
        vram_put(BR_BL_TR_TL(2,3,2,3));
    }
}

void put_logo()
{
    for (i = 0; i < 6; i++){
        vram_adr(NTADR_A(7, 4 + i));
        vram_write((jp? I18N_JP_LOGO: I18N_EN_LOGO) + (i*17), 17);
    }
}

void put_rank()
{
    for (l = 0; l < joysticks; ++l) {
        s = player_rank[l];
        vram_put(SPR_POINTER + l);
        vram_put('P');
        if (s && s <= 3) {
            s += DIGIT_NOOB;
        }
        else if (s == MAX_ENIMIES) {
            s = DIGIT_NOOB;
        }
        vram_put(digit_lockup[1][s]);
        vram_put(digit_lockup[0][s]);
    }
}

void put_score()
{
    for (l = 0; l < joysticks; ++l) {
        s = player_score[l];
        vram_put(SPR_POINTER + l);
        vram_put('P');
        if (s) {
            vram_put(digit_lockup[1][s]);
            vram_put(digit_lockup[0][s]);
        } else {
            vram_write("00", 2);
        }
    }
}

void spawn()
{
    unsigned char colorvar = 2;

    /** reset scores**/
    for (i = 0; i < MAX_PLAYERS; i++) {
        player_score[i] = 0;
        player_rank[i] = 0;
    }
    /** reset postions **/
	for (i = 0; i < MAX_ENIMIES; i++)
	{
        if (joysticks == 1 && i == 0){
            // single player center
            players[i].x = MID_ARENA_X;
            players[i].y = MID_ARENA_Y;
        }
        else if (i < joysticks) {
            // multi player positions
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
        players[i].info.status.colorvar = (++colorvar >> 2) % 3;
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
            if (gamestate == FSM_CELEBRATION) {
                npcs[npc].state = FSM_WINNER;
            }
            else if (j < 60) {
                npcs[npc].state = FSM_HUNTER_WAIT;
            }
            else 
            {
                case FSM_HUNTER_WAIT:
                j = rand8();
                if (j < 90) {
                    npcs[npc].input ^= (rand8() + npc + j) & (PAD_LEFT | PAD_RIGHT);
                    // fixed ambiguous input on axis x
                    if (npcs[npc].input & PAD_LEFT && npcs[npc].input & PAD_RIGHT) {
                        npcs[npc].input ^= (PAD_LEFT | PAD_RIGHT);
                    }
                }
                else if (j < 120) {
                    npcs[npc].input ^= (rand8() + npc + j) & (PAD_DOWN | PAD_UP);
                    // fixed ambiguous input on axis x
                    if (npcs[npc].input & PAD_DOWN && npcs[npc].input & PAD_UP) {
                        npcs[npc].input ^= (PAD_DOWN | PAD_UP);
                    }
                }
            }
            break;
    }
}


static unsigned char step_1;
static unsigned char step_2;
static unsigned char step_3;
static unsigned char step_4;

void anim_reset()
{
    step_1 = 0;
    step_2 = 0;
    step_3 = 0;
    step_4 = 0;
}

void anim_menu()
{
    if (step_1 < second) {
        ++step_1;
    } else if (step_2 < 7) {
        step_1 = 1;
        ++step_2;
    } else {
        static const unsigned colors[] = {0x30, 0x29, 0x21, 0x30, 0x29};
        pal_col(1, colors[0 + step_4]);
        pal_col(2, colors[1 + step_4]);
        pal_col(3, colors[2 + step_4]);
        pal_col(17, colors[2 + step_4]);
        step_4 = (step_3>>3) % 3;
        ++step_3;
    }
}

bool anim_count(unsigned char end)
{
    bool finish = false;
    if (++step_1 > second) {
        sfx_play(2, 1);
        step_1 = 0;
        ++step_2;
    }
    if (step_2 >= end) {
        sfx_play(3, 1);
        music_pause(0);
        finish = true;
    }
    spr = oam_spr(100 + (8 * 3), (10 * 8), ('0' + end - step_2), 0, spr);
    spr = oam_spr((27 * 8), (1 * 8) -1, '?', 3, spr);
    spr = oam_spr((28 * 8), (1 * 8) -1, '?', 3, spr);
    for (i = 0; i < 8; ++i) {
        spr = oam_spr(100 + (i << 3), (9 * 8), I18N_COUNT[i], 0, spr); 
    }
    for (i = 0; i < 4; ++i) {
        spr = oam_spr(100 + (i << 3), (10 * 8), I18N_COUNT[i + 8], 0, spr); 
    }
    for (i = 0; i < 3; ++i) {
        spr = oam_spr(100 + (8 * 4) + (i << 3), (10 * 8), ((step_1 >> 3) > i)? '.': ' ', 0, spr);
    }
    oam_edge(MIN_ARENA_X, MIN_ARENA_Y, MAX_ARENA_X, MAX_ARENA_Y);
    return finish;
}

void anim_winner_color()
{
    static unsigned char s0 = 0;
    static unsigned char s1 = 0;
    static unsigned char s2 = 0;
    if (++s0 >= 5) {
        s0 = 0;
        ++s1;
    }
    if (s1 >= 4) {
        s1 = 0;
        ++s2;
    }
    if (s2 >= 3) {
        s2 = 0;
    }
    s = s1 << 2;
    i = s2;
}

void screen_menu()
{
    ppu_off();
    pal_bg(paletteBackgroundMenu);
    oam_clear();
    /** reset nametable */
    put_all(' ');
    /** reset attribute table*/
    vram_adr(ATADR_A(0, 1));
    vram_fill(BR_BL_TR_TL(0,0,0,0), 3*8);
    vram_fill(BR_BL_TR_TL(2,2,2,2), 5*8);
    /** colorize logo */
    vram_adr(ATADR_A(7,7));
    vram_put(BR_BL_TR_TL(2,0,2,0));
    vram_adr(ATADR_A(7,9));
    vram_put(BR_BL_TR_TL(0,0,2,0));
    vram_adr(ATADR_A(8,7));
    vram_fill(BR_BL_TR_TL(2,2,2,2), 4);
    vram_adr(ATADR_A(8,8));
    vram_fill(BR_BL_TR_TL(0,0,2,2), 4);
    vram_adr(ATADR_A(18,6));
    vram_put(BR_BL_TR_TL(1,2,2,2));
    vram_put(BR_BL_TR_TL(2,1,2,2));
    /** colorize link */
    vram_adr(ATADR_A(1,26));
    vram_fill(BR_BL_TR_TL(1,1,2,2), 8);
    /** game title */
    put_ret(5, 3, 25, 9);
    put_logo();
    /** put menu options */
    put_str(NTADR_A(12 - BRAZUKA, 16), I18N_PLAYERS);
    /** put copyright */
    put_str(NTADR_A(3,25), I18N_EN_CREDITS_1);
    put_str(NTADR_A(1,26), I18N_EN_CREDITS_2);
    /** put menu option 'continue' */
    if (roosters_count) {
        put_str(NTADR_A(11,15), I18N_CONTINUE);
    }
    ppu_on_all();
    gamestate = FSM_MENU;
}

void screen_arena()
{
    ppu_off();
    oam_clear();
    pal_bg(paletteBackground);
    pal_spr(paletteSprite);
    /** reset nametable */
    put_all(' ');
    /** reset attribute table*/
    vram_adr(ATADR_A(0, 1));
    vram_fill(BR_BL_TR_TL(3,3,3,3), 8*8);
    /** put borders */
    put_borders();
    /* put footer */
    put_str(NTADR_A(0,28), I18N_EN_GAMEPLAY_NAME);
    /* put header */
    vram_adr(NTADR_A(0, 1));
    put_rank();
    vram_adr(NTADR_A(26,1));
    vram_put(0x00);
    vram_write("  /20", 5);
    ppu_on_all();
}

void screen_celebration()
{
    ppu_off();
    oam_clear();
    pal_bg(paletteBackground);
    pal_spr(paletteSprite);
    put_all(' ');
    put_borders();

    /** print scoreboards **/
    put_str(NTADR_A(1, 1), "RANK: ");
    put_str(NTADR_A(1, 28), "KILLS:");
    vram_adr(NTADR_A(31 - (joysticks * 4), 1));
    put_rank();
    vram_adr(NTADR_A(31 - (joysticks * 4), 28));
    put_score();  

    /** print rank */
    for (l = 0x7f, i = 0; i < joysticks; ++i)
    {
        if (player_rank[i] < l) {
            l = player_rank[i];
        }
    }
    
    if (l <= 1) {
        put_str(NTADR_A(12, 4), I18N_EN_WINNER);
    } else {
        put_str(NTADR_A(12, 4), I18N_EN_LOOSER);
        vram_adr(NTADR_A(13, 4));
        vram_put(digit_lockup[1][l]);
        vram_put(digit_lockup[0][l]);
    }

    gamestate = FSM_CELEBRATION;
    ppu_on_all();
}

void game_move();

void game_setup()
{
    pal_spr(paletteSprite);

    /** NTSC **/
    if (ppu_system()) {
        second = 60;
        speed = 2;
    }
    /** PAL **/
    else {
        second = 50;
        speed = 2;
    }
}

void game_loop(void)
{
	
		/** reset sprite count **/
		spr = 0;
        ++framecount_seed;

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
                music_pause(1);
				gamestate = FSM_RESTART;
                break;

            case FSM_DRAW_MENU:
                screen_menu();
                break;

			case FSM_DRAW_ARENA:
				screen_arena();
                anim_reset();
                gamestate = FSM_COUNT;
				break;

            case FSM_DRAW_CELEBRATION:
				screen_celebration();
                gamestate = FSM_CELEBRATION;
                break;

            case FSM_COUNT:
                spr = 0;
                if (anim_count(5)) {
                    gamestate = FSM_GAMEPLAY;
                }
                for (i = 0; i < joysticks; ++i) {
                    if (players[i].info.status.death) {
                        continue;
                    }
                    j = (players[i].info.sprite &~ 0x40);
                    r = ((i>>1) & 0x3) | (players[i].info.sprite & 0x40);
                    spr = oam_spr(players[i].x, players[i].y - 8, SPR_POINTER + i, 4, spr);
                    spr = oam_spr(players[i].x, players[i].y, j, r, spr);
                }
                break;

            case FSM_MENU:
                /** select best seed by frame **/
                /** !jp randomization adjustment to standardize US/JAP**/
                seed = (seed + 1 + !jp) % sizeof(good_seeds);
                pal_col(7, (16 << (seed >> 3)) + 7); /**< 17, 37 **/

                /** switch between resume, singleplayers and multiplayer **/
                if (gamepad_old[PLAYER_1] == 0 && gamepad[PLAYER_1] & PAD_UP) {
                    s -= 1;
                }
                else if (gamepad_old[PLAYER_1] == 0 && gamepad[PLAYER_1] & PAD_DOWN) {
                    s += 1;
                }

                /** Limit menu options **/
                s = CLAMP(s, roosters_count == 0, MAX_PLAYERS);

                /** begin start the game **/
                i = gamepad[PLAYER_1] & (PAD_START | PAD_A);

                if (i) {
                    if (s == 0) {
                        gamestate = FSM_DRAW_ARENA;
                    }
                    else if (s <= 6) {
                        joysticks = s;
                        gamestate = FSM_MUSIC_ARENA;
                        for(i = 0; i < 4; i++) {
                            playerActive[i] = i >= s;
                        }
                    }
                }

                /** draw option 6, 3, 23, 9 **/
                spr = 0;
                spr = oam_spr((10 * 8), (15 * 8) + (s << 3) - 1, 0xFC, 0, spr);
                spr = oam_spr((22 * 8), (15 * 8) + (s << 3), 0xFC, 0, spr);
                oam_edge((6 * 8) - 2, (4 * 8) - 2, (25 * 8) + 2, (9 * 8));
                /** animate logo*/
                if (roosters_count == 0) {
                    anim_menu();
                }
                break;

            case FSM_CELEBRATION:
                seed = (seed + 1) % sizeof(good_seeds);
                if (gamepad_old[PLAYER_1] == 0) {
                    /** restart game **/
                    if ((gamepad[PLAYER_1] & PAD_START)) {
                        gamestate = FSM_RESTART;
                    }
                }
                s = 0;
                i = winner_id;
                ++framecount.frames;
                player = players[i];
                game_move();
                if (player.framedata) {
                    --player.framedata;
                }
                players[i] = player;
                if (winner_id < joysticks) {
                    spr = oam_spr(player.x, player.y - 8, SPR_POINTER + i, 0, spr);
                }
                anim_winner_color();
                r = (player.info.sprite & 0x40) | 1;
                j = (player.info.sprite &~ (0x40|0x18)) | (i << 3);
                pal_col(21, paletteSprite[s + 1]);
                pal_col(22, paletteSprite[s + 2]);
                pal_col(23, paletteSprite[s + 3]);                
                spr = oam_spr(player.x, player.y, j, r, spr);
                oam_edge(MIN_ARENA_X, MIN_ARENA_Y, MAX_ARENA_X, MAX_ARENA_Y);
                oam_hide_rest(spr);
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

                /** earthquake effect **/
                eqx = earthquake;
                eqy = earthquake;
                if (earthquake) {
                    --earthquake;
                    if (earthquake & 1) {
                        eqx = -earthquake;
                    }
                    if (earthquake & 3) {
                        eqy = -earthquake;
                    }
                }

                /** preapare **/
                roosters_count = 0;
                ia_hunter_cycle();

                /** entitys loop **/
                for (i = 0; i < MAX_ENIMIES; i++) {
                    player = players[i];

                    /** out of game **/
                    if (player.info.status.death) {
                        continue;
                    }

                    /** number of coocks alive **/
                    roosters_count += 1;
                    
                    game_move();

                    /** mediator **/
                    if (player.framedata) {
                        player.framedata -= 1;
                        for (j = 0; j < MAX_ENIMIES; j++) {
                            static struct coco_s victim;
                            /** end of attack **/
                            if (!player.info.status.attacking) {
                                break;
                            }
                            /** not attacking itself **/
                            if (i == j) {
                                continue;
                            }
                            /** optmice access **/
                            victim = players[j];
                            /** blocking attack **/
                            if (victim.framedata) {
                                continue;
                            }
                            /** pidgeot is fainted dude **/
                            if (victim.info.status.death) {
                                continue;
                            }
                            /** far far away **/
                            if (DISTANCE(player.x, victim.x) > 8 || DISTANCE(player.y, victim.y) > 8) {
                                continue;
                            }

                            /** kill store **/
                            players[j].info.status.death = TRUE;

                            /** save score **/
                            for (r = 0; r < joysticks; ++r){
                                /** points */
                                if (r == i) {
                                    earthquake += 3;
                                    ++player_score[i];
                                    /** winner */
                                    if (roosters_total <= 2) {
                                        player_rank[r] = 1;
                                    /** not blink */
                                    } else {
                                        continue;
                                    }
                                }
                                /** looser */
                                else if (j == r) {
                                    /** set ranking  **/
                                    player_rank[r] = roosters_total;
                                /** npc */
                                } else {
                                    continue;
                                }

                                /** flash screen & put score */
                                pal_col(0,0x30);
                                ppu_off();
                                vram_adr(NTADR_A(0, 1));
                                put_rank();
                                ppu_on_all();
                                pal_col(0, paletteBackground[0]);
                            }
                        }
                    }

                    /** draw pointer **/
                    if (i < joysticks) {
                        spr = oam_spr(player.x + eqx, player.y + eqy - 8, SPR_POINTER + i, 4, spr);
                    }

                    /** draw cock **/  
                    spr = oam_spr(player.x + eqx, player.y + eqy, (player.info.sprite &~ 0x40), ((i>>1) & 0x3) | (player.info.sprite & 0x40), spr);
                    players[i] = player;
                }
                /** game over */
                if (roosters_total == 1) {
                    gamestate = FSM_DRAW_CELEBRATION;
                    winner_id = 0;
                    while (players[winner_id].info.status.death) {
                        ++winner_id;
                    }
                }

                /** draw number of coocks **/
                roosters_total = roosters_count;
                spr = oam_spr((27 * 8) - eqy, (1 * 8) -1 - eqx, digit_lockup[1][roosters_count], 3, spr);
                spr = oam_spr((28 * 8) + eqx, (1 * 8) -1 + eqy, digit_lockup[0][roosters_count], 3, spr);

                /* put ret edges **/
                oam_edge(MIN_ARENA_X, MIN_ARENA_Y, MAX_ARENA_X, MAX_ARENA_Y);

                /* end draw */
                oam_hide_rest(spr);
                break;

            case FSM_RESTART:
                /** reset player status **/
                for (i = 0; i < MAX_ENIMIES; players[i].info.sprite = 0, i++);
                seed = (seed + 1) % sizeof(good_seeds);
                gamestate = FSM_DRAW_ARENA;
                roosters_total = 0;
                set_rand(SEED_UNPACK(good_seeds[seed]));
                spawn();
                set_rand((~(framecount_seed << 8) | (framecount_seed >> 8)));
                break;
		}
	
}

void game_move()
{
    /** player input **/
    if (i < joysticks) {
        if(gamepad[i] & PAD_LEFT) {
            player.x -= speed;
            s = -1;
        }
        else if(gamepad[i] & PAD_RIGHT) {
            player.x += speed;
            s = 1;
        } else {
            s = 0;
        }
        if(gamepad[i] & PAD_UP) {
            player.y -= speed;
        }
        else if(gamepad[i] & PAD_DOWN) {
            player.y += speed;
        }
        if ((~gamepad_old[i] & gamepad[i] & PAD_A) && player.framedata == 0) {
            player.framedata = FRAME_PREPARE;
            player.info.status.attacking = 1;
        }
    } 
    /** npc think input **/
    else if (framecount.frames == (i & 0x3)) {
        ia_process(i);
        s = 0;
    }
    /** npc excute input **/
    else {
        if(npcs[i].input & PAD_LEFT) {
            player.x -= speed;
            s = -1;
        }
        else if(npcs[i].input & PAD_RIGHT) {
            player.x += speed;
            s = 1;
        } else {
            s = 0;
        }
        if(npcs[i].input & PAD_UP) {
            player.y -= speed;
        }
        else if(npcs[i].input & PAD_DOWN) {
            player.y += speed;
        }
        if (npcs[i].input & PAD_A) {
            player.framedata = FRAME_PREPARE;
            player.info.status.attacking = 1;
        }
    }

    /** animation sprite **/
    player.info.status.walking = (player.x ^ player.y)>>3;
    player.info.status.attacking = player.framedata < FRAME_ATTACKING && player.framedata > FRAME_RECOVERY;
    player.info.status.recovering = player.framedata && !player.info.status.attacking;
    switch (s) {
        case 1: player.info.status.flipped = 0; break;
        case -1: player.info.status.flipped = 1; break;
    }

    /** arena colision **/
    player.x = CLAMP(player.x, MIN_ARENA_X, MAX_ARENA_X);
    player.y = CLAMP(player.y, MIN_ARENA_Y, MAX_ARENA_Y);
}

void main(void)
{
	game_setup();

	for (;;) {
        game_loop();
    }
}
