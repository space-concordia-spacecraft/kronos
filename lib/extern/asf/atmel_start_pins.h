/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME70 has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define PA21 GPIO(GPIO_PORTA, 21)
#define PA25 GPIO(GPIO_PORTA, 25)
#define PA26 GPIO(GPIO_PORTA, 26)
#define PA27 GPIO(GPIO_PORTA, 27)
#define PA28 GPIO(GPIO_PORTA, 28)
#define PA30 GPIO(GPIO_PORTA, 30)
#define PA31 GPIO(GPIO_PORTA, 31)
#define PB4 GPIO(GPIO_PORTB, 4)
#define LED0 GPIO(GPIO_PORTC, 8)
#define WRITE_PROTECT_0 GPIO(GPIO_PORTC, 11)
#define CARD_DETECT_0 GPIO(GPIO_PORTC, 16)
#define PD25 GPIO(GPIO_PORTD, 25)
#define PD26 GPIO(GPIO_PORTD, 26)

#endif // ATMEL_START_PINS_H_INCLUDED
