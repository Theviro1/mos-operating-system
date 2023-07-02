#include <asm/asm.h>
#include <stackframe.h>

.section .text.tlb_miss_entry
.set noreorder
.set noat
tlb_miss_entry:
	mfc0	k0, CP0_CONTEXT
	mfc0	k1, CP0_EPC
	lw	k0, 0(k0)
	nop
	mtc0    k0, CP0_ENTRYLO0
	nop
	tlbwr
	jr      k1
	rfe
	/*注释掉原来的j，添加了上述所有行*/
	# j       exc_gen_entry
	



.section .text.exc_gen_entry
exc_gen_entry:
	SAVE_ALL
/* Exercise 3.9: Your code here. */
	mfc0 t0, CP0_CAUSE
	andi t0, 0x7c
	/*注释掉原来的两行，添加后面的行*/
	# lw t0, exception_handlers(t0)
	# jr t0
	li	t1, 0xc
	li	t2, 0x8
	beq	t1, t0, flag
	beq	t2, t0, flag
	lw	t0, exception_handlers(t0)
	jr	t0

flag:
	mtc0 	k1, CP0_EPC
	lw      t0, exception_handlers(t0)
	jr      t0
