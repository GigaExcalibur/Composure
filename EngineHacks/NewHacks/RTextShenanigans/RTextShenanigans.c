#include "gbafe.h"

extern u16 GetFullName(u8 unitid);
extern u16 FullNameName_Link;
extern u16 FullNameDescList[][2];

extern u16 ItName_Link;
extern u16 RngName_Link;

bool DoesHelpBoxHaveFullName(int textid) {
	int i = 0;
	while(FullNameDescList[i][0] != 0) {
		if(FullNameDescList[i][0] == textid) {
			return true;
		}
		i++;
	}
	return false;
}

int DrawHelpBoxFullName() {
	u16 name = GetFullName(gStatScreen.unit->pCharacterData->number);
	if(name) {
		Text_InsertDrawString(&gHelpBoxSt.text[0], 0, 8, GetStringFromIndex(FullNameName_Link));
		Text_InsertDrawString(&gHelpBoxSt.text[0], 44, 7, GetStringFromIndex(name));
	}
	return 1;
}

void ApplyHelpBoxContentSize(struct HelpBoxProc* proc, int w, int h) {
    w = 0xF0 & (w + 15);

    switch (GetHelpBoxItemInfoKind(proc->item)) {
        case 1:
            w = 0x90;
            h = h + 0x20;

            break;

        case 2:
            if (w < 0x90) {
                w = 0x90;
            }

            h = h + 0x10;

            break;
    }

	int textID = proc->mid;
	if(DoesHelpBoxHaveFullName(textID)) {
		if(w < 0x90) {
			w = 0x90;
		}
		h = h + 0x10;
	}

    proc->wBoxFinal = w;
    proc->hBoxFinal = h;

    return;
}

void HelpBoxSetupstringLines(struct ProcHelpBoxIntro* proc) {
    int item = proc->item;

    SetTextFont(&gHelpBoxSt.font);
    SetTextFontGlyphs(0);

    switch (GetHelpBoxItemInfoKind(item)) {
        case HB_EXTINFO_NONE:
            proc->pretext_lines = 0;
            break;

        case HB_EXTINFO_WEAPON:
            DrawHelpBoxWeaponLabels(item);
            proc->pretext_lines = 2;
            break;

        case HB_EXTINFO_STAFF:
            DrawHelpBoxStaffLabels(item);
            proc->pretext_lines = 1;
            break;

        case HB_EXTINFO_SAVEINFO:
            DrawHelpBoxSaveMenuLabels();
            proc->pretext_lines = 1;
            break;
    }

	int textID = proc->msg;
	if(DoesHelpBoxHaveFullName(textID)) {
		DrawHelpBoxFullName();
		proc->pretext_lines = 1;
	}
	
    SetTextFont(0);

    Proc_Break(proc);
}

int DrawHelpBoxWeaponLabels(int item)
{
    Text_InsertDrawString(&gHelpBoxSt.text[0], 0, 8, GetWeaponTypeDisplayString(GetItemType(item)));
    Text_InsertDrawString(&gHelpBoxSt.text[0], 47, 8, GetStringFromIndex(RngName_Link)); // TODO: msg id "Rng[.]"
	Text_InsertDrawString(&gHelpBoxSt.text[0], 97, 8, GetStringFromIndex(ItName_Link)); // TODO: msg id "Crit"

    Text_InsertDrawString(&gHelpBoxSt.text[1], 0, 8, GetStringFromIndex(0x503)); // TODO: msg id "Mt"
    Text_InsertDrawString(&gHelpBoxSt.text[1], 47, 8, GetStringFromIndex(0x4F4)); // TODO: msg id "Hit[.]}"
    Text_InsertDrawString(&gHelpBoxSt.text[1], 97, 8, GetStringFromIndex(0x502)); // TODO: msg id "Wt"

    return 2;
}

//! FE8U = 0x08089CD4
void DrawHelpBoxWeaponStats(int item)
{
    Text_InsertDrawString(&gHelpBoxSt.text[0], 32, 7, GetItemDisplayRankString(item));
    Text_InsertDrawString(&gHelpBoxSt.text[0], 67, 7, GetItemDisplayRangeString(item));
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 129, 7, GetItemCrit(item));

    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 32, 7, GetItemMight(item));
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 81, 7, GetItemHit(item));
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 129, 7, GetItemWeight(item));
}

int DrawHelpBoxStaffLabels(int item)
{
    Text_InsertDrawString(&gHelpBoxSt.text[0], 0, 8, GetStringFromIndex(0x509)); // TODO: msg id "Staff[.]"
    Text_InsertDrawString(&gHelpBoxSt.text[0], 30, 7, GetItemDisplayRankString(item));
    Text_InsertDrawString(&gHelpBoxSt.text[0], 48, 8, GetStringFromIndex(RngName_Link)); // TODO: msg id "Rng[.]"
    Text_InsertDrawString(&gHelpBoxSt.text[0], 70, 7, GetItemDisplayRangeString(item));
    return 1;
}