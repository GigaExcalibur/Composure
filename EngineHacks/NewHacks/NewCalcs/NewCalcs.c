#include "gbafe.h"

void NewComputeBattleUnitHitRate(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	bunitA->battleHitRate = GetItemHit(bunitA->weapon) + (bunitA->unit.skl * 5 / 2) + bunitA->wTriangleHitBonus;
}

void NewComputeBattleUnitAvoidRate(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	bunitA->battleAvoidRate = (bunitA->unit.lck * 5 / 2) + bunitA->terrainAvoid;
}

void NewComputeBattleUnitCritRate(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	bunitA->battleCritRate = 0;
}

void NewComputeBattleUnitSpeed(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	int base = bunitA->unit.spd;
	
	base -= GetItemCrit(bunitA->weapon);
	
	if(base < 0) {
		base = 0;
	}
	
	bunitA->battleSpeed = base;
}

void NewComputeBattleUnitDodgeRate(struct BattleUnit* bunitA, struct BattleUnit* bunitB) {
	int base = bunitA->unit.mag;
	
	base -= GetItemWeight(bunitA->weapon);
	
	if(base < 0) {
		base = 0;
	}
	
	bunitA->battleDodgeRate = base;
}

long long ZeroAid(u8 stat, struct Unit* unit) {

	stat = 0;

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
