/**
 * CONSTANTS
 */

#define MAX_ENIMIES                     (20)
#define MAX_PLAYERS                     (2)
#define	PI						        (3)
#define SPEED                           (3)
#define MIN_ARENA_X                     (6)
#define MAX_ARENA_X                     (242)
#define MIN_ARENA_Y                     (14)
#define MAX_ARENA_Y                     (200)

/**
 * FUNCTIONS
 */
#define DISTANCE(x1,y1,x2,y2)	        ((abs(x1-x2)+abs(y1-y2))/(PI*2))
#define PRESSING(g,b)			        ((g&b)==b)
#define CLAMP(v,min,max)                (MIN(max,MAX(min,v)))

/**
 *  SPRITES
 */
#define SPR_PLAYER				        0x05
#define SPR_EDGE                        0x10

/**
 * TYPES
 */
struct coco_s {
    unsigned char x;
    unsigned char y;
    signed char sign;
};

/** GLOBAL VARIABLES **/

static struct coco_s players[MAX_ENIMIES];
static unsigned char gamepad[MAX_PLAYERS];


/** GENERAL VARIABLES **/
static signed char s;
static unsigned char i,j;
static unsigned char spr;

/*
 * UTILS
 */
void put_str(unsigned int adr,const char *str)
{
    vram_adr(adr);
	for (;*str != NULL; vram_put(*str), str++);
}

void put_arena()
{
    for (i = 1; i < 31; vram_adr(NTADR_A(i,26)), vram_put(SPR_EDGE + 4), vram_adr(NTADR_A(i,1)), vram_put(SPR_EDGE + 4), i++);
    for (i = 2; i < 26; vram_adr(NTADR_A(0,i)), vram_put(SPR_EDGE + 5), vram_adr(NTADR_A(31,i)), vram_put(SPR_EDGE + 5), i++);
    vram_adr(NTADR_A(0, 1));
    vram_put(SPR_EDGE + 2);
    vram_adr(NTADR_A(31,1));
    vram_put(SPR_EDGE + 3);    
    vram_adr(NTADR_A(0,26));
    vram_put(SPR_EDGE);
    vram_adr(NTADR_A(31,26));
    vram_put(SPR_EDGE + 1);
}