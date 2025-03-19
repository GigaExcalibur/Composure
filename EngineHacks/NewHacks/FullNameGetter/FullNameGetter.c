#include "gbafe.h"

extern u16 FullNameTable[];

u16 GetFullName(u8 unitid) {
	return FullNameTable[unitid];
}
