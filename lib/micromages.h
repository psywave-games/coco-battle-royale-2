/**
 * 4 player support by Morphcat Games
 *
 * https://forums.nesdev.org/viewtopic.php?p=238969#p238969
 * https://github.com/psywave-games/coco-battle-royale-2/issues/11
 */
void __fastcall__ updateInput(void);

extern unsigned char playerActive[4];
extern unsigned char joy1;
extern unsigned char joy2;
extern unsigned char joy3;
extern unsigned char joy4;

#pragma zpsym ("playerActive");
#pragma zpsym ("joy1");
#pragma zpsym ("joy2");
#pragma zpsym ("joy3");
#pragma zpsym ("joy4");
