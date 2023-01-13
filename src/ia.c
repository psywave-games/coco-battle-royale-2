#include "ia.h"

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

static struct npc_ia_s npcs[MAX_ENIMIES];

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