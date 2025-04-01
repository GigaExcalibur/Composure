#include "gbafe.h"

extern u8 Img_PhaseChangeUnk[];
extern u8 Img_PhaseChangePlayer[];
extern u16 Pal_PhaseChangePlayer[];
extern u8 Img_PhaseChangeEnemy[];
extern u16 Pal_PhaseChangeEnemy[];
extern u8 Img_PhaseChangeOther[];
extern u16 Pal_PhaseChangeOther[];
extern u16 Pal_085A06B8[];
extern u8 Img_PhaseChangeSquares[];

extern const u16 DefaultFactionMapSpritePalettes[4][16];
extern const u16 MenuPlayerMapSpritePalettes[4][16];
extern const u16 MenuUIPalettes[5][16];
extern u16 BattleAnimFramePalettes[5][16];
extern u16 PhaseChangePalettes[5][16];

void ApplyUnitSpritePalettes(void)
{
    ApplyPalettes(DefaultFactionMapSpritePalettes, 0x1C, 4);

    if (gBmSt.gameStateBits & BM_FLAG_LINKARENA)
        ApplyPalette(gPal_MapSpriteArena, 0x10 + OBJPAL_UNITSPRITE_PURPLE);
    else
        ApplyPalette(gPal_NotMapSprite, 0x10 + OBJPAL_UNITSPRITE_PURPLE);
	
	ApplyPalette(&MenuPlayerMapSpritePalettes[gPlaySt.config.windowColor], 0x1C);
}

void ApplyUnitMapUiFramePal(int faction, int palId) {
	ApplyPalette(&MenuUIPalettes[gPlaySt.config.windowColor], palId);
}

void UnpackUiFramePalette(int palId)
{
    if (palId < 0)
        palId = BGPAL_WINDOW_FRAME;

    ApplyPalette(&MenuUIPalettes[gPlaySt.config.windowColor], palId);
}

const u16 * GetFactionBattleForecastFramePalette(int faction)
{

    switch (faction) {
        case FACTION_BLUE:
            return MenuUIPalettes[gPlaySt.config.windowColor];

        case FACTION_RED:
            return MenuUIPalettes[4];

        case FACTION_GREEN:
            return DefaultFactionMapSpritePalettes[2];

        case FACTION_PURPLE:
            return DefaultFactionMapSpritePalettes[3];
    }
}

void PhaseIntro_InitGraphics(ProcPtr proc)
{
    Decompress(Img_PhaseChangeUnk, BG_CHR_ADDR(0xA00));
    Decompress(Img_PhaseChangeSquares, BG_CHR_ADDR(BGCHR_PHASE_CHANGE_SQUARES));

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

    switch (gPlaySt.faction)
    {

    case FACTION_BLUE:
        Decompress(Img_PhaseChangePlayer, BG_CHR_ADDR(BGCHR_PHASE_CHANGE_NAME));
        ApplyPalette(&PhaseChangePalettes[gPlaySt.config.windowColor], BGPAL_PHASE_CHANGE);
        ApplyPalette(&PhaseChangePalettes[gPlaySt.config.windowColor], 18);
        break;

    case FACTION_RED:
        Decompress(Img_PhaseChangeEnemy, BG_CHR_ADDR(BGCHR_PHASE_CHANGE_NAME));
        ApplyPalette(&PhaseChangePalettes[4], BGPAL_PHASE_CHANGE);
        ApplyPalette(&PhaseChangePalettes[4], 18);
        break;

    case FACTION_GREEN:
        Decompress(Img_PhaseChangeOther, BG_CHR_ADDR(BGCHR_PHASE_CHANGE_NAME));
        ApplyPalette(&PhaseChangePalettes[4], BGPAL_PHASE_CHANGE);
        ApplyPalette(&PhaseChangePalettes[4], 18);
        break;

    }
}
