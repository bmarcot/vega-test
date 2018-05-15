/* Timer0 and SysTick share the same internal clock source (XTALI), so they
 * must tick at the same frequency.  This test requires to pass the option
 * `-count 1` to qemu-sytstem-arm. */

#include <kernel/kernel.h>
#include <kernel/types.h>

#include "platform.h"
#include "unit.h"

/* Pick a random value, SysTick must not wrap-around */
#define LOAD_VAL 0xc00000

int main()
{
	/* Disable the Timer IRQ */
	NVIC_DisableIRQ(Timer0A_IRQn);

	TIMER0->CTL &= ~GPTM_GPTMCTL_TAEN_Msk;
	TIMER0->CFG = 0;
	TIMER0->TAMR = GPTM_GPTMTAMR_TAMR_OneShot << GPTM_GPTMTAMR_TAMR_Pos;
	TIMER0->TAILR = LOAD_VAL;

	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = 0xffffff;

	/*
	 * Writing to SYST_CVR clears both the register and the COUNTFLAG status
	 * bit to zero. This causes the SysTick logic to reload SYST_CVR from
	 * SYST_RVR on the next timer clock.
	 */
	/* Counter decrementing is effective from now on */
	SysTick->VAL = 0;

	/* Start SysTick and Timer0 ``almost'' simultaneously */
	TIMER0->CTL |= (GPTM_GPTMCTL_TAEN_Enabled << GPTM_GPTMCTL_TAEN_Pos);

	while ((TIMER0->RIS & GPTM_GPTMRIS_TATORIS_Msk) == 0)
		;

	/* Reading CTRL will clear VAL (in Qemu SysTick implementation) */
	u32 val = SysTick->VAL;

	if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
		pr_err("SysTick must not wrap-around!");
		TEST_EXIT(1);
	}


	/*
	 * We could considering masking the result because we cannot start the two
	 * counters at the exact same time. As such, it is acceptable to have a
	 * difference of a few ticks (mask = 0xff?).
	 */
	if ((0xffffff - val) != LOAD_VAL) {
		pr_err("SysTick and Timer0 must tick at the same frequency");
		TEST_EXIT(1);
	}

	TEST_EXIT(0);
}
