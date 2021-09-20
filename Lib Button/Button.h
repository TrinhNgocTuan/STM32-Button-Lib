/*
 *	Button.h
 *
 *      Author: Ngoc Tuan
*/
#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx_hal.h"
#define TIME_DEBOUND_BUTTON	20
#define TIME_SHORT_PRESS	1000
#define TIME_LONG_PRESS	3000
#define TIME_ACCEL_MIN	100
#define TIME_ACCEL_MAX	1000
#define TIME_ACCEL_DELTA	200

#define BUTTON_ACTIVE_LOW	0
#define BUTTON_ACTIVE_HIGH	1

typedef enum
{
	BUTTON_READ,
	BUTTON_WAIT_DEBOUND,
	BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS,
	BUTTON_WAIT_RELEASE
}Button_State;
typedef struct
{
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
	uint8_t current_status;
	uint8_t last_status;
	uint32_t time_debounce;
	uint32_t t_long_press;
	Button_State button_state;
	uint8_t button_active;
	uint8_t button_id;
	int16_t t_accel_call;
	uint32_t t_accel_press;
}Button;
void handle_button(Button *btn);
void button_init(Button *btn,GPIO_TypeDef *port,uint16_t pin,uint8_t button_active,uint8_t button_id);
#endif
