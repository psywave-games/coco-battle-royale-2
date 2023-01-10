#define     FAMICON_VERSION     (0)
#include    "src/anim.h"
#include    "src/draw.h"
#include    "src/ia.h"
#include    "src/screen.h"
#include    "src/spawn.h"
#include    "lib/micromages.h"
#include    "lib/neslib.h"
#include    "lib/globals.h"
#include    "src/i18n.c"
#include    "src/ia.c"
#include    "src/draw.c"
#include    "src/screen.c"
#include    "src/game.c"
#include    "src/anim.c"
#include    "src/spawn.c"

void main(void)
{
	game_setup();

	for (;;) {
        game_loop();
    }
}