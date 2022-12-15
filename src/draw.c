void put_all(const char c)
{
    vram_adr(NTADR_A(0,1));
    vram_fill(c, 32*28);
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
        vram_put(0x5c + l);
        vram_put('P');
        vram_put(digit_lockup[1][s]);
        vram_put(digit_lockup[0][s]);
        
    }
}

void put_score()
{
    for (l = 0; l < joysticks; ++l) {
        s = player_score[l];
        vram_put(0x5c + l);
        vram_put('P');
        if (s) {
            vram_put(digit_lockup[1][s]);
            vram_put(digit_lockup[0][s]);
        } else {
            vram_write("00", 2);
        }
    }
}

void draw_menu()
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
    vram_put(BR_BL_TR_TL(1,1,2,2));
    /** colorize link */
    vram_adr(ATADR_A(1,26));
    vram_fill(BR_BL_TR_TL(1,1,2,2), 30);
    /** game title */
    put_ret(5, 3, 25, 9);
    put_logo();
    /** put menu options */
    vram_adr(NTADR_A(11,16));
    vram_write(jp? I18N_JP_1_PLAYERS: I18N_EN_1_PLAYERS, 9);
    vram_adr(NTADR_A(11,17));
    vram_write(jp? I18N_JP_2_PLAYERS: I18N_EN_2_PLAYERS, 9);
    vram_adr(NTADR_A(11,18));
    vram_write(jp? I18N_JP_3_PLAYERS: I18N_EN_3_PLAYERS, 9);
    vram_adr(NTADR_A(11,19));
    vram_write(jp? I18N_JP_4_PLAYERS: I18N_EN_4_PLAYERS, 9);
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

void draw_arena()
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
    put_str(NTADR_A(26,1), "\x10  /20");
    gamestate = FSM_GAMEPLAY;
    ppu_on_all();
}

void draw_celebration()
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
    if (player_rank[i] == DIGIT_WINNER) {
        put_str(NTADR_A(12, 4), I18N_EN_WINNER);
    }

    gamestate = FSM_CELEBRATION;
    ppu_on_all();
}