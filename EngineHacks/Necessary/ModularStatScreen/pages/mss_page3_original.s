.thumb
.include "mss_defs.s"

.global MSS_page3
.type MSS_page3, %function


MSS_page3:

page_start

mov r0, r8
draw_weapon_rank_at 1, 1, Sword, 0
draw_weapon_rank_at 1, 3, Lance, 1
draw_weapon_rank_at 9, 1, Axe, 2
draw_weapon_rank_at 9, 3, Bow, 3
draw_weapon_rank_at 1, 5, Anima, 4
draw_weapon_rank_at 1, 7, Light, 5
draw_weapon_rank_at 9, 5, Dark, 6
draw_weapon_rank_at 9, 7, Staff, 7

  ldr     r0, =SSS_Flag
  ldr     r0, [r0]
  cmp     r0, #0x0
  beq     DefaultBox
    ldr     r0, =SSS_StatsBoxTSA
    b       DecompressBoxTSA
  DefaultBox:
    ldr     r0, =#0x8A02204   @box TSA
  DecompressBoxTSA:
  ldr     r4, =gGenericBuffer
  mov     r1, r4
  blh     Decompress
  ldr     r0, =#0x20049EE     @somewhere on the bgmap
  mov     r2, #0xC1
  lsl     r2, r2, #0x6
  mov     r1, r4
  blh     BgMap_ApplyTsa
  ldr     r0, =#0x8205A24     @map of text labels and positions
  blh     DrawStatscreenTextMap
  
	draw_textID_at 14, 13, textID=0x04F3, width=5 @atk
	draw_textID_at 14, 15, textID=0x04F4, width=5 @hit
	draw_textID_at 14, 17, textID=0x04F7, width=5 @as

	draw_textID_at 21, 13, textID=0x0500, width=5 @rng
	draw_textID_at 21, 15, textID=0x04F5, width=5 @avo
	draw_textID_at 21, 17, textID=0x04F8, width=5 @ds
	
  ldr     r6, =StatScreenStruct
  ldr     r0, [r6, #0xC]
  ldr     r0, [r0, #0x4]
  ldrb    r0, [r0, #0x4]
  cmp     r0, #Deny_Statscreen_Class_Hi
    beq     SS_DoneEquipHighlightBar
  cmp     r0, #Deny_Statscreen_Class_Lo
    beq     SS_DoneEquipHighlightBar
  
    @.if \showBallista

        ldr     r2, [r6, #0xC]
        ldr     r0, [r2, #0xC]
        mov     r1, #0x80        
        lsl     r1, r1, #0x4        @Check "in ballista" bit
        and     r0, r1
        cmp     r0, #0x0
        beq     NoBallistaEquipped_Box
        
        @get a non-empty ballista at unit position
        mov     r0, #0x10
        mov     r1, #0x11
        ldsb    r0, [r2, r0]
        ldsb    r1, [r2, r1]
        blh     GetBallistaItemAt
        cmp     r0, #0x0
        beq     NoBallistaEquipped_Box
        mov     r5, r0
        mov     r4, #0x0             @slot id
        b       SS_DrawEquippedItemHighlight
        
    @.endif
  
  NoBallistaEquipped_Box:
  ldr     r0, [r6, #0xC] 
  blh     EquippedItemSlotGetter
  mov     r4, r0
  mov     r5, #0x0
  cmp     r4, #0x0             @no equipped item will be -1
  blt     SS_DoneEquipHighlightBar
  
  SS_DrawEquippedItemHighlight:
  /*
  lsl     r4, r4, #0x1
  add     r0, r4, #1
  lsl     r0, r0, #0x6
  ldr     r1, =#0x2003D4C
  add     r0, r0, r1
  mov     r1, #0x0
  mov     r2, #0x35            @the equip 'E'
  blh     DrawSpecialUiChar
  add     r0, r4, #2
  lsl     r0, r0, #0x6
  ldr     r1, =#0x200472E
  add     r0, r0, r1
  ldr     r1, =#0x8A02250     @TSA for highlight bar
  mov     r2, #0xC1
  lsl     r2, r2, #0x6
  blh     BgMap_ApplyTsa
  
  cmp     r5, #0x0
  */
  bne     SS_DoneEquipHighlightBar
  
  SS_ItemBox_GetID:
  ldr     r0, [r6, #0xC]
  add     r0, #0x1E
  add     r0, r0, r4
  ldrh    r5, [r0]
  
  SS_DoneEquipHighlightBar:
  ldr     r0, =StatScreenStruct
  ldr     r0, [r0, #0xC]
  ldr     r0, [r0, #0x4]
  ldrb    r0, [r0, #0x4]
  cmp     r0, #Deny_Statscreen_Class_Hi
  beq     SS_DrawItemBox_Unarmed
  cmp     r0, #Deny_Statscreen_Class_Lo
  beq     SS_DrawItemBox_Unarmed
  
  ldr     r4, =#0x200407C     @bgmap offset
  ldr     r6, =gActiveBattleUnit
  mov     r0, r6

  @atk
  mov     r0, #0x5A
  ldsh    r0, [r6, r0]
  draw_number_at 19, 13
  
  @hit
  mov     r0, #0x60
  ldsh    r0, [r6, r0]
  draw_number_at 19, 15
  
  @AS
  mov     r0, #0x5E
  ldsh    r0, [r6, r0]
  draw_number_at 19, 17
  
  @avo
  mov     r0, #0x62
  ldsh    r0, [r6, r0]
  draw_number_at 27, 15
  
  @crit avo ("defense speed")
  mov     r0, #0x68
  ldsh    r0, [r6, r0]
  draw_number_at 27, 17
/*
  ldr     r4, =#0x200407C     @bgmap offset
  ldr     r6, =gActiveBattleUnit
  mov     r0, r6
  add     r0, #0x5A         @load battle atk
  mov     r1, #0x0
  ldsh    r2, [r0, r1]
  mov     r0, r4
  mov     r1, #0x2
  blh     DrawDecNumber
  mov     r0, r4
  add     r0, #0x80
  mov     r1, r6
  add     r1, #0x60         @load battle hit
  mov     r3, #0x0
  ldsh    r2, [r1, r3]
  mov     r1, #0x2
  blh     DrawDecNumber
  mov     r0, r4
  add     r0, #0xE
  mov     r1, r6
  add     r1, #0x66         @load battle crit
  mov     r3, #0x0
  ldsh    r2, [r1, r3]
  mov     r1, #0x2
  blh     DrawDecNumber
  add     r4, #0x8E
  mov     r0, r6
  add     r0, #0x62         @load battle avoid
  mov     r6, #0x0
  ldsh    r2, [r0, r6]
  mov     r0, r4
  mov     r1, #0x2
  blh     DrawDecNumber
  b       SS_DrawItemBox_RangeText
*/
  SS_DrawItemBox_Unarmed:
/*
  ldr     r4, =#0x200407C
  mov     r0, r4
  mov     r1, #0x2
  mov     r2, #0xFF
  blh     DrawDecNumber
  mov     r0, r4
  add     r0, #0x80
  mov     r1, #0x2
  mov     r2, #0xFF
  blh     DrawDecNumber
  mov     r0, r4
  add     r0, #0xE
  mov     r1, #0x2
  mov     r2, #0xFF
  blh     DrawDecNumber
  add     r4, #0x8E
  ldr     r0, =gActiveBattleUnit
  add     r0, #0x62         @load battle avoid
  mov     r1, #0x0
  ldsh    r2, [r0, r1]
  mov     r0, r4
  mov     r1, #0x2
  blh     DrawDecNumber
  mov     r5, #0x0            @set item as blank
*/
  SS_DrawItemBox_RangeText:
  mov     r0, r5
  blh     GetItemRangeString
  mov     r5, r0
  ldr     r4, =#0x2003CB4
  blh     Text_GetStringTextWidth
  mov     r1, #0x37
  sub     r1, r1, r0
  mov     r0, r4
  mov     r2, #0x2
  mov     r3, r5
  blh     Text_InsertString, r4
  mov     r4, #0x0
  ldr     r0, =gpStatScreenPageBg0Map
  ldr     r3, =#0x7060
  mov     r5, r3
  ldr     r6, =#0x2C2
  add     r2, r0, r6
  ldr     r1, =#0x7068
  mov     r3, r1
  add     r6, #0x40
  add     r1, r0, r6


page_end
