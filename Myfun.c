#include "Myfun.h"
volatile uint32_t tick = 0;
volatile uint8_t button = 0;
int Step=0;
uint16_t StepR=0;
uint16_t StepL=0;
int actangle;
uint8_t time=35;
volatile int CONTROL=0;
volatile int position[2];	
volatile uint32_t PWM = 0;
volatile uint32_t basePWM = 650 ;
	
void delay_ms(uint32_t ms)
{
	tick = 0;
	while(tick < ms);
}

void SysTick_Handler(void)
{
	tick++;
}


void USART_Conf(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	GPIOC->MODER &= ~GPIO_MODER_MODER10 & ~GPIO_MODER_MODER11;
	GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0;
	GPIOC->AFR[1] |= 0x00007700;
	
	USART3->BRR = 	16000000/9600;
  USART3->CR1 |= 	USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

int ComReceive(USART_TypeDef* USARTx)
{
	uint32_t znak;
	if((USARTx->SR & USART_SR_RXNE) != RESET)
	{
		znak = USARTx->DR;
		return znak;
	}
	return 0;
}


void StepMotor_Conf(void)
{
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0;
	
	
}
void Motor_Conf(void)
{
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 ;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0 ;


}

void HalfStepRotation(char direction,uint16_t  angle)
{	
	
 actangle = (float)(angle * (4095.0f/360.0f));
	
			switch(direction)
	{
		case 'L': 
		StepR=0;
		for(int i =0; i<8; i++)
	{
		HalfStepMotors_2(i);
	
		if (time>1) 
			{	
				time--;
				delay_ms(time);
			}
		else
			{
				delay_ms(1);
			}
			
		StepL++;
			
		if(StepL >= actangle)
			{	
				time=35;
				CONTROL=0;
				position[0]=0;
				break;
			}
	}
	break;
	case 'R':
		StepL=0;
		for(int i = 7; i>=0; i--)
	{
		HalfStepMotors_2(i);
	
		if (time>1) 
			{	
				time--;
				delay_ms(time);
			}
		else
			{
				delay_ms(1);
			}
			
		StepR++;
		if(StepR  >= actangle)
			{	
				time=35;
				CONTROL=0;
				position[0]=0;
				break;
				
			}

	}
	break;
}
}
void HalfStepMotors_2( uint8_t num)
{
		switch(num)
	{
		case 0: 
			
			GPIOD->ODR |= GPIO_ODR_ODR_7;//orange
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;//Blue
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;//Pink
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;//yellow
		
			GPIOA->ODR |= GPIO_ODR_ODR_11;//orange
			GPIOA->ODR &= ~GPIO_ODR_ODR_12;//yellow
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;//Blue
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;//Pink
			break;
		case 1: 
			GPIOD->ODR |= GPIO_ODR_ODR_7;
			GPIOD->ODR |= GPIO_ODR_ODR_6;
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;
		
			GPIOA->ODR |= GPIO_ODR_ODR_11;
			GPIOA->ODR |= GPIO_ODR_ODR_12;
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;
		
		break;
		case 2: 
			GPIOD->ODR |= GPIO_ODR_ODR_6;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;
		
			GPIOA->ODR |= GPIO_ODR_ODR_12;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;
		break;
		case 3: 
			GPIOD->ODR |= GPIO_ODR_ODR_6;
			GPIOD->ODR |= GPIO_ODR_ODR_5;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;
		
		
			GPIOA->ODR |= GPIO_ODR_ODR_12;
			GPIOA->ODR |= GPIO_ODR_ODR_9;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;
			break;
			case 4: 
			GPIOD->ODR |= GPIO_ODR_ODR_5;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;
			
			GPIOA->ODR |= GPIO_ODR_ODR_9;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;
			GPIOA->ODR &= ~GPIO_ODR_ODR_12;
			break;
			case 5: 
			GPIOD->ODR |= GPIO_ODR_ODR_4;
			GPIOD->ODR |= GPIO_ODR_ODR_5;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;
			
			GPIOA->ODR |= GPIO_ODR_ODR_10;
			GPIOA->ODR |= GPIO_ODR_ODR_9;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_12;
			break;
			case 6: 
			GPIOD->ODR |= GPIO_ODR_ODR_4;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;
			
			GPIOA->ODR |= GPIO_ODR_ODR_10;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;
			GPIOA->ODR &= ~GPIO_ODR_ODR_12;
			break;
			case 7: 
			GPIOD->ODR |= GPIO_ODR_ODR_4;
			GPIOD->ODR |= GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;
			
			GPIOA->ODR |= GPIO_ODR_ODR_10;
			GPIOA->ODR |= GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;
			break;
			default: 
			GPIOD->ODR &= ~GPIO_ODR_ODR_4;
			GPIOD->ODR &= ~GPIO_ODR_ODR_6;
			GPIOD->ODR &= ~GPIO_ODR_ODR_7;
			GPIOD->ODR &= ~GPIO_ODR_ODR_5;
			
			GPIOA->ODR &= ~GPIO_ODR_ODR_10;
			GPIOA->ODR &= ~GPIO_ODR_ODR_11;
			GPIOA->ODR &= ~GPIO_ODR_ODR_9;
			GPIOA->ODR &= ~GPIO_ODR_ODR_12;
			break;
	}

}



void Pwm_Conf(void)
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	GPIOA->MODER	|= GPIO_MODER_MODER6_1 |  GPIO_MODER_MODER0_1;
	GPIOA->PUPDR	|= GPIO_PUPDR_PUPDR6_0 |	GPIO_MODER_MODER0_0;
	GPIOA->AFR[0]	|= 0x22000002;
	
	//timer 3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ; //PWM Mode 1 CH1
	TIM3->CCER	|= TIM_CCER_CC1E | TIM_CCER_CC2E;	
	TIM3->PSC	= 16-1;	//Prescaler
	TIM3->ARR	= 20000-1;	//Period
	TIM3->CR1	|= TIM_CR1_CEN;	//Enable TIM3
	
	// timer 5
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ;	//PWM Mode 1 CH1
	TIM5->CCER	|= TIM_CCER_CC1E;
	TIM5->PSC	= 16-1;	//Prescaler
	TIM5->ARR	= 1000-1;	//Period
	TIM5->CR1	|= TIM_CR1_CEN;	//Enable TIM5
}

void BluetoothRecive(void)
{

			switch ( ComReceive(USART3) )
	{
		case 50 :
		DirectionVal (0);
		break;
		
		case 1 :
		DirectionVal (0);
		DirectionVal (1);
		break;
		
		case 2:
		DirectionVal (0);
		DirectionVal (2);
		break;
		
		case 3:
			TIM3->CCR1= 350;
			PWM = 0;
		break;
		
		case 4:
			TIM3->CCR1 = 385;
			PWM = 25;
		break;
		
		case 5:
			TIM3->CCR1 = 420;
			PWM = 50;
		break;
		
		case 6:
			TIM3->CCR1= 455;
			PWM = 75;
		break;
		
		case 7:
			TIM3->CCR1= 490;
			PWM = 100;
		break;
		
		case 8:
			TIM3->CCR1 = 525;
			PWM = 125;
		break;
		
		case 9:
			TIM3->CCR1 = 560;
			PWM = 150;
		break;
	
		case 10:
			TIM3->CCR1=595;
			PWM = 175;
		break;
		
		case 11:
			TIM3->CCR1=630;
			PWM = 200;
		break;
		
		case 12:
			TIM3->CCR1= 665;
			PWM = 225;
		break;	
		
		case 13 :
			TIM3->CCR1 = 700;
			PWM = 250;
		break;
		
		case 14:
			TIM3->CCR1= 735;
			PWM = 225;
		break;
		
		case 15:
			TIM3->CCR1 = 770;
			PWM = 200;
		break;
		
		
		case 16:
			TIM3->CCR1 = 805;
			PWM = 175;
		break;
		
		case 17:
			TIM3->CCR1= 840;
			PWM = 150;
		break;
		
		case 18:
			TIM3->CCR1= 875;
			PWM = 125;
		break;
		
		case 19:
			TIM3->CCR1 = 910;
			PWM = 100;
		break;
		
	case 20:
			TIM3->CCR1 = 945;
			PWM = 75;
			break;
	
	case 21:
			TIM3->CCR1= 980;
			PWM = 50;
		break;
	
	case 22:
			TIM3->CCR1= 1015;
			PWM = 25;
		break;
	
		case 23:
			TIM3->CCR1= 1050;
			PWM = 0;
		break;

		
		case 24:
			CONTROL=1;
		break;
				
		case 25:
			CONTROL=2;
		break;
						
		case 26:
			CONTROL=3;
		break;
	}

}
void DirectionVal (int val)
{
	if(val ==1 && basePWM<750)
	{
		MOTRON;

		while(basePWM<750)
		{
			basePWM+=10;
			delay_ms(10);
		}
		
	}
		if(val ==2 && basePWM<750)
	{
		MOTLON;

		while(basePWM<750)
		{
			basePWM+=10;
			delay_ms(10);
		}
		
	}
	
	
			if(val ==0 && basePWM!=650 && basePWM!=0 )
	{
	
		while(basePWM!=600)
		{
			basePWM-=10;
			delay_ms(50);
		}
		
		delay_ms(1000);
		
		MOTLOFF;
		MOTROFF;
		
	}

}

void speed (void)
{
	if(basePWM>740 && PWM!=0 )
	{
		TIM5->CCR1 =  basePWM + PWM;
	}
	else if(PWM==0)
	{
		TIM5->CCR1 =  basePWM + 200;
	} 
		else TIM5->CCR1 =  basePWM;
}


	void SuspensionControl(void)
	{
				switch ( CONTROL )
	{
				case 1: 
					if(position[0]==2) 
					{
						HalfStepRotation('R',90);
						position[1]=1;
					} 
					else
					{
						HalfStepRotation('L',90);
						position[0]=1;
					}
					
					break;
				
				case 2:
					HalfStepRotation('L',180);
					position[0]=2;
					break;
				
				case 3:
					if(position[0]==1) 
						{
						HalfStepRotation('R',90);
						
						}
						if(position[1]==1) 
						{
						HalfStepRotation('R',90);
						
						}
					if(position[0]==2) 			
						{
						HalfStepRotation('R',180);
		
						}
					break;
				

	}
}