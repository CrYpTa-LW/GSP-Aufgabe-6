
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "timer.h"

void fsm(int);
int checkEvent(void);
void noact(void);
void act0(void); //Ausgabe Blinklicht
void act1(void); //Ausgabe Lauflicht
void act2(void); //Schneller
void act3(void); //Langsamer
void act4(void); //NextLauflicht
void act5(void); //NextBlinklicht

struct EvReact 
{
	int NextState;
	void (*DoIt)(void);
};

