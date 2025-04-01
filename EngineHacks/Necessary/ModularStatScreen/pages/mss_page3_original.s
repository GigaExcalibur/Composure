.thumb
.include "mss_defs.s"

.global MSS_page3
.type MSS_page3, %function


MSS_page3:

page_start

mov r0, r8
draw_weapon_rank_at 1, 1, Sword, 0
draw_weapon_rank_at 1, 3, Lance, 1
draw_weapon_rank_at 1, 5, Axe, 2
draw_weapon_rank_at 9, 3, Bow, 3
draw_weapon_rank_at 9, 5, Anima, 4 @general magic replaces anima
draw_weapon_rank_at 9, 1, Light, 5 @dagger replaces knife
@draw_weapon_rank_at 9, 5, Dark, 6
draw_weapon_rank_at 5, 7, Staff, 7

blh DrawBWLNumbers

page_end
