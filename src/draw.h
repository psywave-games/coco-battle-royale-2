#ifndef H_DRAW_KOKOBATORU
#define H_DRAW_KOKOBATORU

void oam_edge(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void put_all(const char c);
void put_str(unsigned int adr,const char *str);
void put_ret(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void put_borders();
void put_logo();
void put_rank();
void put_score();

#endif
