/**
 * @todo: refact this cheat.
 */
#define MAX_ENIMIES                     (20)
#define MAX_PLAYERS                     (4)
#define SPEED                           (3)
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
#define SPR_POINTER                     0xBC

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
