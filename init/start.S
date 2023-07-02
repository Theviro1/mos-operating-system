#include <asm/asm.h>
#include <mmu.h>

.text
EXPORT(_start)
.set at
.set reorder
	/* disable interrupts */
	mtc0    zero, CP0_STATUS

	/*!!!changed*/
	li t0,0xc0000000
	mtc0 t0, CP0_CONTEXT

	/* hint: you can reference the memory layout in include/mmu.h */
	/* set up the kernel stack */
	/* Exercise 1.3: Your code here. (1/2) */
	li sp,0x80400000
	jal mips_init
	/* jump to mips_init */
	/* Exercise 1.3: Your code here. (2/2) */
