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

static unsigned char second;                  
static unsigned char speed;

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
                    j = (players[i].info.sprite &~ 0x40);
                    r = (i & 0x3) | (players[i].info.sprite & 0x40);
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
                spr = oam_spr((10 * 8), (15 * 8) + (s << 3), 0xFC, 0, spr);
                spr = oam_spr((20 * 8), (15 * 8) + (s << 3), 0xFC, 0, spr);
                oam_edge((6 * 8) - 2, (4 * 8) - 2, (25 * 8) + 2, (9 * 8));
                /** animate logo*/
                if (roosters_count == 0) {
                    anim_menu();
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
                    static struct coco_s player;
                    player = players[i];

                    /** out of game **/
                    if (player.info.status.death) {
                        continue;
                    }

                    /** number of coocks alive **/
                    roosters_count += 1;
                    
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
                        if ((gamepad[i] & (PAD_B | PAD_A)) && !(gamepad_old[i] & (PAD_B | PAD_A)) && player.framedata == 0) {
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
                                    ++player_score[i];
                                    /** winner */
                                    if (roosters_total <= 2) {
                                        player_rank[r] = 1;
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
                        spr = oam_spr(player.x, player.y - 8, SPR_POINTER + i, 4, spr);
                    }

                    /** draw cock **/  
                    spr = oam_spr(player.x, player.y, (player.info.sprite &~ 0x40), (i & 0x3) | (player.info.sprite & 0x40), spr);
                    players[i] = player;
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