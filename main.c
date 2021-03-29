#include "Myfun.h"
extern int CONTROL;
extern uint16_t StepL;
int main(void)
{
	SysTick_Config(16000000/1000);
	StepMotor_Conf();
	Pwm_Conf();
	USART_Conf();
	Motor_Conf();
	
	while(1)
	{

	
		//GPIOD->ODR |= GPIO_ODR_ODR_5 Lewe przod; <=> Pink
		//GPIOD->ODR |= GPIO_ODR_ODR_4 Lewe tyl <=>  Blue
		//GPIOD->ODR |= GPIO_ODR_ODR_6 prawe tyl <=> yellow
		//GPIOD->ODR |= GPIO_ODR_ODR_7; prawe przod <=> orange
		
		
		//GPIOA->ODR |= GPIO_ODR_ODR_9 Lewe przod; <=> Pink
		//GPIOA->ODR |= GPIO_OD	R_ODR_10 Lewe tyl <=>  Blue
		//GPIOA->ODR |= GPIO_ODR_ODR_12 prawe tyl <=> yellow
		//GPIOA->ODR |= GPIO_ODR_ODR_11; prawe przod <=> orange
		
		speed ();
		BluetoothRecive();
		SuspensionControl();
	
}
	}

