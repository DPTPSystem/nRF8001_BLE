//PIC18F24K20 koncigur�ci�s regiszterek
#pragma config FOSC		= HSPLL		// Oscillator v�laszt�s
#pragma config FCMEN	= OFF		// FAIL SFE CLOCK MONITOR
#pragma config IESO		= OFF		// BELS�-K�LS� Osc. kapcsol�
#pragma config PWRT     = OFF		// Fesz�lts�g m�r� timer
#pragma config BOREN	= ON		// Alacsony fesz�lts�gn�l reset
#pragma config BORV     = 27		// 2.7V-n�l m�r resetel
#pragma config WDTEN	= OFF		// Watchdog bekapcsolva
#pragma config WDTPS    = 128		// 1:128 Watchdog id�z�t�s osztoja
#pragma config MCLRE	= ON		// MCLRE ENABLED
#pragma config HFOFST	= ON		// HFINTOSC FAST START-UP
#pragma config LPT1OSC	= OFF		// LOW-POWER TIMER1 OSC DISABLED
#pragma config PBADEN	= OFF		// PORT B A/D DISABLED
#pragma config CCP2MX	= PORTC		// PWM2 legyen az (RB3) (m�sik: PORTBE== RB3)
#pragma config STVREN	= ON		// Werem t�lcsordul�s eset�n reset
#pragma config LVP      = ON		// Alacsony fesz�lts�g� programoz�s enged�lyez�se
#pragma config DEBUG    = OFF		// Debug, nyomonk�vet�s kikapcsolva
#pragma config XINST	= OFF		// EXTENDED INSTRUCTION DISABLED
#pragma config CP0      = OFF		// K�d v�delem
#pragma config CP1      = OFF
#pragma config CPB      = OFF		// Boot block k�dv�delem
#pragma config CPD      = OFF		// Eeprom k�d v�delem
#pragma config WRT0     = OFF		// �r�s v�delem
#pragma config WRT1     = OFF
#pragma config WRTB     = OFF		// Boot block �r�sv�delem
#pragma config WRTC     = OFF		// Konfigur�ci�s regiszter �r�s v�delme
#pragma config WRTD     = OFF		// Eeprom �r�s v�delem
#pragma config EBTR0    = OFF		// T�bla olvas�si v�delem
#pragma config EBTR1    = OFF
#pragma config EBTRB    = OFF		// Boot block t�bla olvas�si v�delem
#define CLOCK_FREQ 64000000