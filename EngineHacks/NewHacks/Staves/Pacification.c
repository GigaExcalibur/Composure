#include "Staves.h"

void ExecPacification(struct Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetPacificationBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
	
	return;
}

void AddUnitToTargetListIfNotPacified(struct Unit* unit) {
	if (IsPacificationBitSet(unit)) {
		return;
	}
	
	AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	return;
}

void MakeTargetListForPacification(struct Unit* unit) {
	int x = unit->xPos;
    int y = unit->yPos;

	gSubjectUnit = unit;

	InitTargets(x, y);

	Item_TURange(unit, AddUnitToTargetListIfNotPacified, PacificationID_Link);
	
	//BmMapFill(gBmMapRange, 0);
	//MapAddInBoundedRange(x, y, GetItemMinRange(PacificationID_Link), GetItemMaxRange(PacificationID_Link));
	ForEachUnitInRange(AddUnitToTargetListIfNotPacified);
}

void PacificationUsabilityWrapper() {
	asm("mov r0,r4;	\
		 mov r1,r5; \
		 bl PacificationUsability; \
		 pop {r4,r5}; \
		 pop {r1}; \
         bx r1; \
	");	
}

bool PacificationUsability(struct Unit* unit) {
	MakeTargetListForPacification(unit);
	return GetSelectTargetCount() != 0;
}

void ExecPacificationWrapper() {
	asm(" 	mov r0,r6; \
			bl ExecPacification; \
			ldr r0,=#0x802FF77; \
			bx r0; \
	");

}

void PacificationTargeting(struct Unit* unit) {

	MakeTargetListForPacification(unit);
	
	BmMapFill(gBmMapMovement, -1);
	
	StartSubtitleHelp(
		NewTargetSelection(&PacificationSelectInfo),
		GetStringFromIndex(PacificationSubtitleText_Link));
}

void PacificationTargetingWrapper() {
	asm("	mov r0, r5; \
			mov r2, r4; \
			bl PacificationTargeting; \
			pop {r4-r5}; \
			pop {r0}; \
			bx r0; \
	");
}

void RefreshUnitPacificationInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 10);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 10, 2); // last parameter is lines
	
    DrawUnitAgiChangeText(proc->lines + 0, unit, -1 * PacificationDebuffAmount_Link);
    PutText(proc->lines, gBG0TilemapBuffer + TILEMAP_INDEX(x + 1, y + 3));
	
    DrawUnitPoiChangeText(proc->lines + 1, unit, PacificationBuffAmount_Link);
    PutText(proc->lines + 1, gBG0TilemapBuffer + TILEMAP_INDEX(x + 1, y + 5));
}

u8 PacificationTargetChange(ProcPtr proc, struct SelectTarget* target)
{
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitPacificationInfoWindow(GetUnit(target->uid));
}