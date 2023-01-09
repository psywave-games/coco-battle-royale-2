void oam_edge(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    spr = oam_spr(x1 - 6, y1 - 7, SPR_EDGE, 0xC0, spr);
    spr = oam_spr(x2 + 6, y1 - 7, SPR_EDGE, 0x80, spr);
    spr = oam_spr(x1 - 6, y2 + 7, SPR_EDGE, 0x40, spr);
    spr = oam_spr(x2 + 6, y2 + 7, SPR_EDGE, 0x00, spr);
}

void put_all(const char c)
{
    vram_adr(NTADR_A(0,0));
    vram_fill(c, 32*30);
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
        vram_put(SPR_POINTER + l);
        vram_put('P');
        if (s && s <= 3) {
            s += DIGIT_NOOB;
        }
        else if (s == MAX_ENIMIES) {
            s = DIGIT_NOOB;
        }
        vram_put(digit_lockup[1][s]);
        vram_put(digit_lockup[0][s]);
    }
}

void put_score()
{
    for (l = 0; l < joysticks; ++l) {
        s = player_score[l];
        vram_put(SPR_POINTER + l);
        vram_put('P');
        if (s) {
            vram_put(digit_lockup[1][s]);
            vram_put(digit_lockup[0][s]);
        } else {
            vram_write("00", 2);
        }
    }
}
