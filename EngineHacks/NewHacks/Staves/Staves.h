#include "gbafe.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

// debuff functions
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);

// debuff externs
extern u32 AggravationBitOffset_Link;
extern u8 AggravationBuffAmount_Link;
extern u8 AggravationDebuffAmount_Link;
extern u32 PacificationBitOffset_Link;
extern u8 PacificationBuffAmount_Link;
extern u8 PacificationDebuffAmount_Link;

// ier stuff
extern void Item_TURange(struct Unit* unit, void* func, int item);
extern s8 prMagGetter(struct Unit* unit);
extern u16 AggravationSubtitleText_Link;
extern u16 PacificationSubtitleText_Link;
extern int AggravationID_Link;
extern int PacificationID_Link;
extern struct SelectInfo CONST_DATA AggravationSelectInfo;
extern struct SelectInfo CONST_DATA PacificationSelectInfo;

// functions functions functions
bool IsAggravationBitSet(struct Unit* unit);
void SetAggravationBit(struct Unit* unit);
bool IsPacificationBitSet(struct Unit* unit);
void SetPacificationBit(struct Unit* unit);
void DrawUnitAgiChangeText(struct Text* text, struct Unit* unit, s8 bonus);
void DrawUnitPoiChangeText(struct Text* text, struct Unit* unit, s8 bonus);
