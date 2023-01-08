#include "spawn.h"

void spawn()
{
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
        players[i].info.status.coloreven = !(i & 1);
        players[i].info.status.death = FALSE;
	}
}
