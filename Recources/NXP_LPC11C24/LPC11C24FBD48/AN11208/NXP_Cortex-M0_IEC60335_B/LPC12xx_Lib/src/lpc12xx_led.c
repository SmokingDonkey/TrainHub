#include "IEC60335.h"

void initLed (uint8_t led)
{
	/* reuse P0.7 led on LPC1200 LpcXpresso stick */
	LED_GPIO->DIR |= SET_BIT(led);
}

void toggleLed (uint8_t led)
{
	if(LED_GPIO->OUT & SET_BIT(led)) { clearLed(led); }
		else setLed(led);
}


