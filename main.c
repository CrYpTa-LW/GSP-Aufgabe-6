
#include "header.h"

//--- For GPIOs -----------------------------
//Include instead of "stm32f4xx.h" for
//compatibility between Simulation and Board
//#include "TI_memory_map.h"

//--- For Touch Keypad ----------------------
//#include "keypad.h"

//2 Zustände und 5 Ereignisse. 
struct EvReact ERTab[2][5] =
{
	{{1,act0},{0,noact},{0,act2},{0,act3},{0,act4}},	//Lauflicht
	{{1,noact},{0,act1},{1,act2},{1,act3},{1,act5}}   //Blinklicht
};
	
	
int main(void)
{
  Init_TI_Board(); 
	timerinit();
	TIM2->ARR = 0xFFFF;   //84000000
	TIM2->SR &= ~0x1;
	TIM2->PSC = 60;
	
	act1();
	
	unsigned int event;
	
	while(1)
	{
		event = checkEvent();
		fsm(event);
	}
}

void fsm(int ev)
{
	static int state=0;
	ERTab[state][ev].DoIt();
	state = ERTab[state][ev].NextState;
}

int checkEvent()
{
	int num = -1;
	int buttons = GPIOE->IDR;
	int taste0 = buttons & 0x01;
	int taste1 = buttons & 0x02;
	int taste2 = buttons & 0x04;
	static int taste0alt = 1;
	static int taste1alt = 1;
	static int taste2alt = 1;
	
	if((TIM2->SR & 0x01) == 1)
	{
		TIM2->SR &= ~0x01;
		return 4;
	}
	//------------------------------------------
	if(taste0 != taste0alt)
	{
		if(taste0 == 0)
		{
			taste0alt = taste0;
			return 0;
		}
		else
		{
			taste0alt = taste0;
			return 1;
		}
	}
	//-------------------------------------
	if(taste1 != taste1alt)
	{
		taste1alt = taste1;
		return 2;
	}
	//-------------------------------------
	if(taste2 != taste2alt)
	{
		taste2alt = taste2;
		return 3;
	}
	//----------------------------------------
	if(taste0 == taste0alt)
	{
		if(taste0 == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return num;
}


void noact()
{
}

void act0() //Ausgabe Blinklicht
{
	TFT_puts("Blinklicht\n\r");
}

void act1() //Ausgabe Lauflicht
{
	GPIOG->ODR = 0x8000;
	TFT_puts("Lauflicht\n\r");
}

void act2() //Schneller
{
	TIM2->PSC = TIM2->PSC *1.2;
}

void act3() //Langsamer
{
	TIM2->PSC = TIM2->PSC *0.8;
}

void act4() //NextLauflicht
{
	if(GPIOG->IDR == 0x0001)
	{
		GPIOG->ODR = 0x8000;
	}
	else
	{
		GPIOG->ODR = GPIOG->IDR/2;
	}
}

void act5() //NextBlinklicht
{
	if(GPIOG->IDR == 0)
	{
		GPIOG->ODR = 0xFFFF;
	}
	else
	{
		GPIOG->ODR = 0x0000;
	}
}


// EOF








