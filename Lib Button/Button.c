/*
 *	Button.c
 *
 *      Author: Ngoc Tuan
*/
#include "Button.h"

__weak void button_pressing_callback(uint8_t button_id)
{
	
}
__weak void button_release_callback(uint8_t button_id)
{
	
}
__weak void button_press_short_callback(uint8_t button_id)
{
	
}
__weak void button_pressing_timeout_callback(uint8_t button_id)
{
	
}
__weak void button_pressing_accel_callback(uint8_t button_id)
{
	
}
void handle_button(Button *btn)
{
	btn->current_status = HAL_GPIO_ReadPin(btn->gpio_port,btn->gpio_pin); 
	if(btn->button_active == BUTTON_ACTIVE_HIGH)
	{
		 btn->current_status = !btn->current_status;
	}
	switch(btn->button_state)
	{
		case BUTTON_READ:
		{
			if((btn->current_status == 0 && btn->last_status == 1) )
			{
						btn->time_debounce = HAL_GetTick();	
						btn->button_state = BUTTON_WAIT_DEBOUND;
			}
		}
		break;
		case BUTTON_WAIT_DEBOUND:
		{
			if(HAL_GetTick() - btn->time_debounce>= TIME_DEBOUND_BUTTON)
			{
				if(btn->current_status ==0 && btn->last_status ==1)//nhan xuong
				{
					button_pressing_callback(btn->button_id);
					btn->t_long_press = HAL_GetTick();
					btn->last_status = 0;
					btn->t_accel_press = HAL_GetTick();
					btn->t_accel_call = TIME_ACCEL_MAX;
					btn->button_state = BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS;
				}
				else if(btn->current_status ==1 && btn->last_status ==0)//nha ra
				{
					btn->t_long_press = HAL_GetTick() - btn->t_long_press;
					if(btn->t_long_press <= TIME_SHORT_PRESS)
					{
						button_press_short_callback(btn->button_id);
					}
					button_release_callback(btn->button_id);
					btn->last_status = 1;
					btn->button_state = BUTTON_READ;
				}
				else //khong dung
				{
					btn->last_status = 1;
					btn->button_state = BUTTON_READ;
				}
			}
		}
		break;
		case BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS:
		{
				if(btn->current_status == 1 && btn->last_status == 0)
				{
					btn->button_state = BUTTON_WAIT_DEBOUND;
					btn->time_debounce = HAL_GetTick();	
				}
				else if(HAL_GetTick() - btn->t_long_press >= TIME_LONG_PRESS)
				{
					button_pressing_timeout_callback(btn->button_id);
					btn->button_state = BUTTON_WAIT_RELEASE;
				}		
				else if(HAL_GetTick() -  btn->t_accel_press >= btn->t_accel_call)
				{
					btn->t_accel_call -=TIME_ACCEL_DELTA;
					if(btn->t_accel_call <= TIME_ACCEL_MIN)
					{
						btn->t_accel_call = TIME_ACCEL_MIN;
					}
					button_pressing_accel_callback(btn->button_id);
					btn->t_accel_press = HAL_GetTick();
				}
		}
		break;
		case BUTTON_WAIT_RELEASE:
		{
			if(btn->current_status == 1 && btn->last_status == 0)
			{
				btn->button_state = BUTTON_WAIT_DEBOUND;
				btn->time_debounce = HAL_GetTick();	
			}
			else if(HAL_GetTick() -  btn->t_accel_press >= btn->t_accel_call)
			{
				btn->t_accel_call -=TIME_ACCEL_DELTA;
				if(btn->t_accel_call <= TIME_ACCEL_MIN)
				{
					btn->t_accel_call = TIME_ACCEL_MIN;
				}
				button_pressing_accel_callback(btn->button_id);
				btn->t_accel_press = HAL_GetTick();
			}
		}
		break;
		default:
			
			break;
	}
}
static void button_config_gpio(Button *btn)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(btn->gpio_port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
  else if(btn->gpio_port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(btn->gpio_port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(btn->gpio_port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = btn->gpio_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = btn->button_active == BUTTON_ACTIVE_LOW? GPIO_PULLUP: GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
//button_active: BUTTON_ACTIVE_LOW, BUTTON_ACTIVE_HIGH
void button_init(Button *btn,GPIO_TypeDef *port,uint16_t pin,uint8_t button_active,uint8_t button_id)
{
	btn->gpio_port = port;
	btn->gpio_pin = pin;
	btn->button_active = button_active;
	btn->button_state = BUTTON_READ;
	btn->button_id = button_id;
	btn->current_status = 1;
	btn->last_status = 1;
	button_config_gpio(btn);
}
