.thumb
@draws the stat screen
.include "mss_defs.s"

.global MSS_page1
.type MSS_page1, %function


MSS_page1:

page_start

@load the growth getters onto the stack, if needed
ldr r0,=Growth_Getter_Table
str		r0,[sp,#0xC]

ldr r0,=Display_Growth_Options_Link
ldr r0,[r0]
mov		r1,#0x10		@set if stat name color should reflect growth
and		r0,r1
mov		r1,r8
ldrb	r1,[r1,#0xB]
mov		r2,#0xC0
tst		r1,r2
beq		IsPlayerUnit
mov		r0,#0
IsPlayerUnit:
str		r0,[sp,#0x14]

draw_textID_at 13, 3, textID=0x4fe, growth_func=2 @str (str)
@draw_number_at 19, 3, StrGetter, 2

draw_textID_at 13, 5, textID=0x4ef, growth_func=7 @def (def)
@draw_number_at 27, 3, DefGetter, 2

draw_textID_at 13, 7, textID=0x4EC, growth_func=4 @skl (dex)
@draw_number_at 19, 5, SklGetter, 2

draw_textID_at 13, 9, textID=0x4ED, growth_func=5 @spd (ini)
@draw_number_at 19, 7, SpdGetter, 2


draw_textID_at 21, 3, textID=0x4f6, 			   @move
@draw_number_at 19, 9, MovGetter, 2
draw_affinity_icon_at 27, 3

draw_textID_at 21, 5, textID=0x4F0, growth_func=8 @res (res)
@draw_number_at 27, 7, ResGetter, 2

draw_textID_at 21, 7, textID=0x4ee, growth_func=6 @luck (eva)
@draw_number_at 27, 5, LuckGetter, 2

draw_textID_at 21, 9, textID=0x4FF, growth_func=3 @mag (poi)
@draw_number_at 27, 7, MagGetter, 2



ldr r0,=TalkTextIDLink
ldrh r0,[r0]
draw_talk_text_at 13, 13

/* vanilla
draw_textID_at 13, 3, textID=0x4fe, growth_func=2 @str
draw_textID_at 13, 5, textID=0x4ff, growth_func=3 @mag
draw_textID_at 13, 7, textID=0x4EC, growth_func=4 @skl
draw_textID_at 13, 9, textID=0x4ED, growth_func=5 @spd
draw_textID_at 13, 11, textID=0x4ee, growth_func=6 @luck
draw_textID_at 13, 13, textID=0x4ef, growth_func=7 @def
draw_textID_at 13, 15, textID=0x4f0, growth_func=8 @res
*/
b 	NoRescue
.ltorg 
NoRescue:

ldr		r0,=StatScreenStruct
sub		r0,#1
mov		r1,r8
ldrb	r1,[r1,#0xB]
mov		r2,#0xC0
tst		r1,r2
beq		Label2
ldrb	r1,[r0]
mov		r2,#0xFE
and		r1,r2
strb	r1,[r0]			@don't display enemy growths
Label2:
ldrb	r0,[r0]
mov		r1,#1
tst		r0,r1
beq		ShowStats
b		ShowGrowths

ShowStats:
b		ShowStats2


ShowGrowths:
/*
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#4]		@str growth getter
draw_growth_at 17, 3
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#8]		@mag growth getter
draw_growth_at 17, 5
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#12]		@skl growth getter
draw_growth_at 17, 7
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#16]		@spd growth getter
draw_growth_at 17 9
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#20]		@luk growth getter
draw_growth_at 17, 11
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#24]		@def growth getter
draw_growth_at 17, 13
ldr		r0,[sp,#0xC]
ldr		r0,[r0,#28]		@res growth getter
draw_growth_at 17, 15
draw_textID_at 13, 17, textID=0x4E9, growth_func=1 @hp name
ldr		r0,[sp,#0xC]
ldr		r0,[r0]			@hp growth getter (not displaying because there's no room atm)
draw_growth_at 17, 17
*/
b		NextColumn
.ltorg

ShowStats2:
b		ShowStats3

NextColumn:


/*
draw_textID_at 21, 3, textID=0x4f7 @con
draw_con_bar_with_getter_at 24, 3


draw_textID_at 21, 5, textID=0x4f8 @aid
draw_number_at 25, 5, 0x80189B8, 2 @aid getter
draw_aid_icon_at 26, 5

draw_status_text_at 21, 7

draw_textID_at 21, 9, textID=0x4f1 @affin






ldr r0,=SkillsTextIDLink
ldrh r0, [r0]
draw_textID_at 21, 13, colour=White @skills
*/
Nexty:

b skipliterals
.ltorg

ShowStats3:

draw_str_bar_at 16, 3
draw_def_bar_at 16, 5
draw_skl_bar_at 16, 7
draw_spd_bar_at 16, 9
draw_move_bar_with_getter_at 24, 3
draw_res_bar_at 24, 5
draw_luck_bar_at 24, 7
draw_mag_bar_at 24, 9





b		NextColumn
.ltorg

skipliterals:

.set NoAltIconDraw, 1 @this is the piece that makes them use a separate sheet
/*
mov r0, r8
ldr r1, =Skill_Getter
mov lr, r1
.short 0xf800 @skills now stored in the skills buffer

mov r6, r0
ldrb r0, [r6] 
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 21, 15

ldrb r0, [r6,#1]
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 24, 15

ldrb r0, [r6, #2]
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 27, 15

ldrb r0, [r6, #3]
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 21, 17

ldrb r0, [r6, #4]
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 24, 17

ldrb r0, [r6, #5]
cmp r0, #0
beq SkillEnd
draw_skill_icon_at 27, 17
*/

SkillEnd:

@ draw_textID_at 13, 15, textID=0x4f6 @move
@ draw_move_bar_at 16, 15

@blh DrawBWLNumbers
/*


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
  /*
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
  */
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
  @*/
  @i think this loop just clears a gfx buffer
  /*
  loc_0x8087660:
  add     r0, r4, r5
  strh    r0, [r2]
  add     r0, r4, r3
  strh    r0, [r1]
  add     r2, #0x2
  add     r1, #0x2
  add     r4, #0x1
  cmp     r4, #0x7
  ble     loc_0x8087660


ldr		r0,=StatScreenStruct
sub		r0,#0x2
ldrb	r0,[r0]
cmp		r0,#0x0
beq		DoNotUpdate
ldr		r0,=BgBitfield
ldrb	r1,[r0]
mov		r2,#0x5
orr		r1,r2
strb	r1,[r0]
ldr		r0,=BgMapFillRect
mov		r14,r0
ldr		r0,=Const_2003D2C
ldr		r1,=Const_2022D40
mov		r2,#0x12
mov		r3,#0x12
.short	0xF800
ldr		r0,=BgMapFillRect
mov		r14,r0
ldr		r0,=Const_200472C
ldr		r1,=Const_2023D40
mov		r2,#0x12
mov		r3,#0x12
.short	0xF800
ldr		r0,=StatScreenStruct
sub		r0,#0x2
mov		r1,#0x0
strb	r1,[r0]
*/
b DoNotUpdate
.ltorg

DoNotUpdate:
page_end

.ltorg

Restore_Palette:
@r0=thing to store back, r1=0 if we can skip this
cmp		r1,#0
beq		RestoreDone
cmp		r0,#0
beq		RestoreDone
ldr		r1,Const2_2028E70
ldr		r1,[r1]
strh	r0,[r1,#0x10]
RestoreDone:
bx		r14

.align
Const2_2028E70:
.long 0x02028E70

.include "GetTalkee.s"

.ltorg
