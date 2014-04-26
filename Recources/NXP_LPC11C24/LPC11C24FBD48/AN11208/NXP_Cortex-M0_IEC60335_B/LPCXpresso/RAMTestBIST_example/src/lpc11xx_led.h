#ifndef __LED_H 
#define __LED_H

#define LED_GPIO    LPC_GPIO0

#define SET_BIT(x) (1<<x)

#define setLed(x)    (LED_GPIO->DATA |= SET_BIT(x))
#define clearLed(x)  (LED_GPIO->DATA &= (~(SET_BIT(x))))


extern void initLed	(uint8_t led);
extern void toggleLed	(uint8_t led);

#endif /* end __LED_H */
