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


void spawn_cocks()
{
    set_rand(SEED_UNPACK(good_seeds[seed]));
    /** reset scores**/
    for (i = 0; i < MAX_PLAYERS; i++) {
        player_score[i] = 0;
        player_rank[i] = 0;
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

void main(void)
{
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
                draw_menu();
                break;

			case FSM_DRAW_ARENA:
				draw_arena();
				break;

            case FSM_DRAW_CELEBRATION:
				draw_celebration();
                gamestate = FSM_CELEBRATION;
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
                spr = oam_spr((5 * 8), (3 * 8) - 1, SPR_EDGE, 0xC0, spr);
                spr = oam_spr((26 * 8), (3 * 8) - 1, SPR_EDGE, 0x80, spr);
                spr = oam_spr((5 * 8), (10 * 8) - 1, SPR_EDGE, 0x40, spr);
                spr = oam_spr((26 * 8), (10 * 8) - 1, SPR_EDGE, 0x00, spr);
                /** animate logo*/
                if (roosters_count == 0) {
                    if (step_1 < 60) {
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
                break;

            case FSM_CELEBRATION:
                if (gamepad_old[PLAYER_1] == 0) {
                    /** restart game **/
                    if ((gamepad[PLAYER_1] & PAD_START)) {
                        gamestate = FSM_RESTART;
                    }
                }
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
                                /** points */
                                if (r == i) {
                                    ++player_score[i];
                                    /** winner */
                                    if (roosters_total <= 2) {
                                        player_rank[r] = DIGIT_WINNER;
                                    }
                                }
                                /** looser */
                                else if (j == r) {
                                    /** set ranking  **/
                                    player_rank[r] = roosters_total;
                                    /** noob */
                                    if (roosters_total == MAX_ENIMIES) {
                                        player_rank[r] = DIGIT_NOOB;
                                    }
                                    /** podium */
                                    if (roosters_total <= 3) {
                                        player_rank[r] = DIGIT_NOOB + roosters_total;
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
                /** game over */
                if (roosters_total == 1) {
                    gamestate = FSM_DRAW_CELEBRATION;
                }

                /** draw number of coocks **/
                roosters_total = roosters_count;
                spr = oam_spr((27 * 8), (1 * 8) -1, digit_lockup[1][roosters_count], 3, spr);
                spr = oam_spr((28 * 8), (1 * 8) -1, digit_lockup[0][roosters_count], 3, spr);

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