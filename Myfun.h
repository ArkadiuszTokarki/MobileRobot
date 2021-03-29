#ifndef _MYFUN
#define _MYFUN
#include <stm32f4xx.h>
#include <stm32f407xx.h>
#include <stdlib.h>
#include <stdio.h>
void USART_Conf(void);
int ComReceive(USART_TypeDef* USARTx);
void Pwm_Conf(void);
void Motor_Conf(void);
void delay_ms(uint32_t ms);
void StepMotor_Conf(void);
void HalfStepRotation(char direction,uint16_t  angle);
void HalfStepMotors_2( uint8_t num);
void DirectionVal (int val);
void SuspensionControl(void);
void BluetoothRecive(void);
void speed (void);


#define MOT_PWM1			TIM3->CCR1 

#define MOTLON  GPIOD->ODR |= GPIO_ODR_ODR_3
#define MOTLOFF GPIOD->ODR &= ~GPIO_ODR_ODR_3

#define MOTRON  GPIOD->ODR |= GPIO_ODR_ODR_2
#define MOTROFF GPIOD->ODR &= ~GPIO_ODR_ODR_2

#endif
