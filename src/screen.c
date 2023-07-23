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
    vram_adr(NTADR_A(11,16));
    vram_write(jp? I18N_JP_1_PLAYERS: I18N_EN_1_PLAYERS, 9);
#if MAX_PLAYERS >= 2
    vram_adr(NTADR_A(11,17));
    vram_write(jp? I18N_JP_2_PLAYERS: I18N_EN_2_PLAYERS, 9);
#endif
#if MAX_PLAYERS > 3
    vram_adr(NTADR_A(11,18));
    vram_write(jp? I18N_JP_3_PLAYERS: I18N_EN_3_PLAYERS, 9);
#endif
#if MAX_PLAYERS > 4
    vram_adr(NTADR_A(11,19));
    vram_write(jp? I18N_JP_4_PLAYERS: I18N_EN_4_PLAYERS, 9);
#endif
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