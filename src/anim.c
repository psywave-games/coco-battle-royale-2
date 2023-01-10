#include "anim.h"
#include "game.h"

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
    if (++step_1 > second) {
        sfx_play(2, 1);
        step_1 = 0;
        ++step_2;
        return false;
    }
    if (step_2 >= end) {
        sfx_play(3, 1);
        music_pause(0);
        return true;
    }
    spr = 0;
    spr = oam_spr(100 + (8 * 3), (10 * 8), ('0' + end - step_2), 0, spr);
    spr = oam_spr((27 * 8), (1 * 8) -1, '?', 3, spr);
    spr = oam_spr((28 * 8), (1 * 8) -1, '?', 3, spr);
    for (i = 0; i < 8; ++i) {
        spr = oam_spr(100 + (i << 3), (9 * 8), I18N_EN_COUNT[i], 0, spr); 
    }
    for (i = 0; i < 4; ++i) {
        spr = oam_spr(100 + (i << 3), (10 * 8), I18N_EN_COUNT[i + 8], 0, spr); 
    }
    for (i = 0; i < joysticks; ++i) {
        spr = oam_spr(players[i].x, players[i].y - 8, SPR_POINTER + i, 4, spr);
        spr = oam_spr(players[i].x, players[i].y, players[i].info.sprite, (i >> 1), spr);
    }
    for (i = 0; i < 3; ++i) {
        spr = oam_spr(100 + (8 * 4) + (i << 3), (10 * 8), ((step_1 >> 3) > i)? '.': ' ', 0, spr);
    }
    oam_edge(MIN_ARENA_X, MIN_ARENA_Y, MAX_ARENA_X, MAX_ARENA_Y);
    return false;
}