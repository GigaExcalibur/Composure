#include "Staves.h"

void ExecAggravation(struct Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetAggravationBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
	
	return;
}

void AddUnitToTargetListIfNotAggravated(struct Unit* unit) {
	if (IsAggravationBitSet(unit)) {
		return;
	}
	
	AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	return;
}

void MakeTargetListForAggravation(struct Unit* unit) {
	int x = unit->xPos;
    int y = unit->yPos;

	gSubjectUnit = unit;

	InitTargets(x, y);

	Item_TURange(unit, AddUnitToTargetListIfNotAggravated, AggravationID_Link);
	
	//BmMapFill(gBmMapRange, 0);
	//MapAddInBoundedRange(x, y, GetItemMinRange(AggravationID_Link), GetItemMaxRange(AggravationID_Link));
	ForEachUnitInRange(AddUnitToTargetListIfNotAggravated);
}

void AggravationUsabilityWrapper() {
	asm("mov r0,r4;	\
		 mov r1,r5; \
		 bl AggravationUsability; \
		 pop {r4,r5}; \
		 pop {r1}; \
         bx r1; \
	");	
}

bool AggravationUsability(struct Unit* unit) {
	MakeTargetListForAggravation(unit);
	return GetSelectTargetCount() != 0;
}

void ExecAggravationWrapper() {
	asm(" 	mov r0,r6; \
			bl ExecAggravation; \
			ldr r0,=#0x802FF77; \
			bx r0; \
	");

}

void AggravationTargeting(struct Unit* unit) {

	MakeTargetListForAggravation(unit);
	
	BmMapFill(gBmMapMovement, -1);
	
	StartSubtitleHelp(
		NewTargetSelection(&AggravationSelectInfo),
		GetStringFromIndex(AggravationSubtitleText_Link));
}

void AggravationTargetingWrapper() {
	asm("	mov r0, r5; \
			mov r2, r4; \
			bl AggravationTargeting; \
			pop {r4-r5}; \
			pop {r0}; \
			bx r0; \
	");
}

void RefreshUnitAggravationInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 10);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 10, 2); // last parameter is lines
	
    DrawUnitAgiChangeText(proc->lines + 0, unit, AggravationBuffAmount_Link);
    PutText(proc->lines, gBG0TilemapBuffer + TILEMAP_INDEX(x + 1, y + 3));
	
    DrawUnitPoiChangeText(proc->lines + 1, unit, -1 * AggravationDebuffAmount_Link);
    PutText(proc->lines + 1, gBG0TilemapBuffer + TILEMAP_INDEX(x + 1, y + 5));
}

u8 AggravationTargetChange(ProcPtr proc, struct SelectTarget* target)
{
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitAggravationInfoWindow(GetUnit(target->uid));
}