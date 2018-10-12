#include "msp.h"
#include "LCD.h"


/**
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer


}
