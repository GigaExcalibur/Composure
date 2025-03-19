#include "Staves.h"

bool IsAggravationBitSet(struct Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), AggravationBitOffset_Link);
}

void SetAggravationBit(struct Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), AggravationBitOffset_Link);
}

void UnsetAggravationBit(struct Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), AggravationBitOffset_Link);
}

bool IsPacificationBitSet(struct Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), PacificationBitOffset_Link);
}

void SetPacificationBit(struct Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), PacificationBitOffset_Link);
}

void UnsetPacificationBit(struct Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), PacificationBitOffset_Link);
}

// goes in start of turn calc loop
// on player phase, ALL units lose aggravation and pacification buffs
void ClearSpeedBits() {
	if(gPlaySt.faction == FACTION_BLUE) {
		int unitID = 1;
		while(unitID < 132) {
			struct Unit* curUnit = GetUnit(unitID);
			UnsetAggravationBit(curUnit);
			UnsetPacificationBit(curUnit);
			unitID++;
		}
	}
}

long long AggravationAgiBonus(u8 stat, struct Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), AggravationBitOffset_Link)) stat += AggravationBuffAmount_Link;
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			struct Unit* unit;
		};
	} result;
	result.stat = stat;
	result.unit = unit;
	return result.asLongLong;
}

long long AggravationPoiPenalty(u8 stat, struct Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), AggravationBitOffset_Link)) stat -= AggravationDebuffAmount_Link;
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			struct Unit* unit;
		};
	} result;
	result.stat = stat;
	result.unit = unit;
	return result.asLongLong;
}

long long PacificationPoiBonus(u8 stat, struct Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), PacificationBitOffset_Link)) stat += PacificationBuffAmount_Link;
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			struct Unit* unit;
		};
	} result;
	result.stat = stat;
	result.unit = unit;
	return result.asLongLong;
}

long long PacificationAgiPenalty(u8 stat, struct Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), PacificationBitOffset_Link)) stat -= PacificationDebuffAmount_Link;
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			struct Unit* unit;
		};
	} result;
	result.stat = stat;
	result.unit = unit;
	return result.asLongLong;
}

void DrawUnitAgiChangeText(struct Text* text, struct Unit* unit, s8 bonus) {
    ClearText(text);

    Text_InsertDrawString(text, 0, 3, GetStringFromIndex(0x4ED)); // Agi[.]
    Text_InsertDrawString(text, 40, 3, GetStringFromIndex(0x53A)); // --

	Text_InsertDrawNumberOrBlank(text, 32, 2, GetUnitSpeed(unit));
    Text_InsertDrawNumberOrBlank(text, 56, 2, MAX(GetUnitSpeed(unit) + bonus, 0));

    return;
}

void DrawUnitPoiChangeText(struct Text* text, struct Unit* unit, s8 bonus) {
    ClearText(text);

    Text_InsertDrawString(text, 0, 3, GetStringFromIndex(0x4FF)); // Poi[.]
    Text_InsertDrawString(text, 40, 3, GetStringFromIndex(0x53A)); // --

	Text_InsertDrawNumberOrBlank(text, 32, 2, prMagGetter(unit));
    Text_InsertDrawNumberOrBlank(text, 56, 2, MAX(prMagGetter(unit) + bonus, 0));

    return;
}

void StartSpeedStaffInfoWindow(ProcPtr parent) {
	struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);
	InitTextDb(proc->lines, 8);
	InitTextDb(proc->lines+1, 8);
}

int SpeedStaffInitSelect(ProcPtr proc)
{
    StartSpeedStaffInfoWindow(proc);
}

void DrawBattleForecastContentsExtended(struct BattleForecastProc * proc)
{
	int damage;
    int critRate;
	
    CallARM_FillTileRect(gUiTmScratchB, gTSA_BattleForecastExtended, 0x1000);

    TileMap_FillRect(gUiTmScratchA, 10, 19, 0);

    PutBattleForecastUnitName(gUiTmScratchA + 0x23, &proc->unitNameTextA, &gBattleActor.unit);
    PutBattleForecastUnitName(gUiTmScratchA + 0x1E1, &proc->unitNameTextA, &gBattleTarget.unit);

    PutBattleForecastItemName(gUiTmScratchA + 0x221, &proc->itemNameText, gBattleTarget.weaponBefore);

    if ((gBattleTarget.weapon == 0) && (gBattleTarget.weaponBroke == 0)) {
        damage = -1;

        gBattleTarget.battleEffectiveHitRate = 0xFF;
        gBattleTarget.battleEffectiveCritRate = 0xFF;
    } else {
        damage = gBattleTarget.battleAttack - gBattleActor.battleDefense;

        if (damage < 0) {
            damage = 0;
        }
    }

    if (gBattleTarget.hpInitial > 99) {
        PutNumberTwoChr(gUiTmScratchA + 0x62, 2, 0xFF);
    } else {
        PutNumberTwoChr(gUiTmScratchA + 0x62, 2, gBattleTarget.hpInitial);
    }

	PutNumberTwoChr(gUiTmScratchA + 0xA2, 2, damage);
    PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0x40, 2, gBattleTarget.battleEffectiveHitRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0x80, 2, gBattleTarget.battleEffectiveCritRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0xC0, 2, gBattleTarget.battleSpeed);
	PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0x100, 2, gBattleTarget.battleDodgeRate);
	
    damage = gBattleActor.battleAttack - gBattleTarget.battleDefense;

    if (GetItemIndex(gBattleActor.weapon) == ITEM_MONSTER_STONE) {
        damage = 0xFF;
    }

    if (damage < 0) {
        damage = 0;
    }

    if (gBattleActor.hpInitial > 99) {
        PutNumberTwoChr(gUiTmScratchA + 0xA2 - 0x3A, 2, 0xFF);
    } else {
        PutNumberTwoChr(gUiTmScratchA + 0xA2 - 0x3A, 2, gBattleActor.hpInitial);
    }

	PutNumberTwoChr(gUiTmScratchA + 0xA8, 2, damage);
    PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0x40, 2, gBattleActor.battleEffectiveHitRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0x80, 2, gBattleActor.battleEffectiveCritRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0xC0, 2, gBattleActor.battleSpeed);
	PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0x100, 2, gBattleActor.battleDodgeRate);

    PutTwoSpecialChar(gUiTmScratchA + 0xA8 - 0x44, TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_HP_A, TEXT_SPECIAL_HP_B);

    PutText(gaBattleForecastTextStructs, gUiTmScratchA + 0xA8 - 5);
    PutText(gaBattleForecastTextStructs + 1, gUiTmScratchA + 0xA8 + 0x3B);
    PutText(gaBattleForecastTextStructs + 2, gUiTmScratchA + 0xA8 + 0x7B);
    PutText(gaBattleForecastTextStructs + 5, gUiTmScratchA + 0xA8 + 0xBB);
    PutText(gaBattleForecastTextStructs + 4, gUiTmScratchA + 0xA8 + 0xFB);

    DrawIcon(gUiTmScratchA + 0xA8 + 0x13F, GetItemIconId(gBattleTarget.weaponBefore), 0x4000);

    DrawIcon(gUiTmScratchA + 0xA8 - 0x87, GetItemIconId(gBattleActor.weaponBefore), 0x3000);
}

void BattleForecast_LoopDisplay(struct BattleForecastProc * proc)
{
    proc->unk_2C++;

    if (proc->needContentUpdate) {

        int side = GetBattleForecastPanelSide();

        if ((side != 0) && (side != proc->side)) {
            Proc_Break(proc);
            return;
        }

        DrawBattleForecastContents(proc);
        PutBattleForecastTilemaps(proc);
        InitBattleForecastFramePalettes();
    }
	
    PutBattleForecastWeaponTriangleArrows(proc);
    PutBattleForecastMultipliers(proc);
    UpdateBattleForecastEffectivenessPalettes(proc);
}

void PutBattleForecastWeaponTriangleArrows(struct BattleForecastProc * proc)
{
    int wtArrowA = 0;
    int wtArrowB = 0;

    if (gBattleActor.wTriangleHitBonus > 0) {
        wtArrowA = 1;
    }

    if (gBattleActor.wTriangleHitBonus < 0) {
        wtArrowA = 2;
    }

    if (gBattleTarget.wTriangleHitBonus > 0) {
        wtArrowB = 1;
    }

    if (gBattleTarget.wTriangleHitBonus < 0) {
        wtArrowB = 2;
    }

    if (wtArrowB != 0) {
		if(proc->frameKind == 1) {
			UpdateStatArrowSprites((proc->x + 8) * 8 + 3, (proc->y + 11) * 8, wtArrowB == 2 ? 1 : 0);
		}			
        else if(proc->frameKind == 2) {
			UpdateStatArrowSprites((proc->x + 8) * 8 + 3, (proc->y + 15) * 8, wtArrowB == 2 ? 1 : 0);
		}
    }

    if (wtArrowA != 0) {
        UpdateStatArrowSprites((proc->x + 2) * 8 + 3, (proc->y + 1) * 8, wtArrowA == 2 ? 1 : 0);
    }
}