//PIC18F24K20 koncigurációs regiszterek
#pragma config FOSC		= HSPLL		// Oscillator választás
#pragma config FCMEN	= OFF		// FAIL SFE CLOCK MONITOR
#pragma config IESO		= OFF		// BELSÕ-KÜLSÕ Osc. kapcsoló
#pragma config PWRT     = OFF		// Feszültség mérõ timer
#pragma config BOREN	= ON		// Alacsony feszültségnél reset
#pragma config BORV     = 27		// 2.7V-nál már resetel
#pragma config WDTEN	= OFF		// Watchdog bekapcsolva
#pragma config WDTPS    = 128		// 1:128 Watchdog idõzítés osztoja
#pragma config MCLRE	= ON		// MCLRE ENABLED
#pragma config HFOFST	= ON		// HFINTOSC FAST START-UP
#pragma config LPT1OSC	= OFF		// LOW-POWER TIMER1 OSC DISABLED
#pragma config PBADEN	= OFF		// PORT B A/D DISABLED
#pragma config CCP2MX	= PORTC		// PWM2 legyen az (RB3) (másik: PORTBE== RB3)
#pragma config STVREN	= ON		// Werem túlcsordulás esetén reset
#pragma config LVP      = ON		// Alacsony feszültségû programozás engedélyezése
#pragma config DEBUG    = OFF		// Debug, nyomonkövetés kikapcsolva
#pragma config XINST	= OFF		// EXTENDED INSTRUCTION DISABLED
#pragma config CP0      = OFF		// Kód védelem
#pragma config CP1      = OFF
#pragma config CPB      = OFF		// Boot block kódvédelem
#pragma config CPD      = OFF		// Eeprom kód védelem
#pragma config WRT0     = OFF		// Írás védelem
#pragma config WRT1     = OFF
#pragma config WRTB     = OFF		// Boot block írásvédelem
#pragma config WRTC     = OFF		// Konfigurációs regiszter írás védelme
#pragma config WRTD     = OFF		// Eeprom írás védelem
#pragma config EBTR0    = OFF		// Tábla olvasási védelem
#pragma config EBTR1    = OFF
#pragma config EBTRB    = OFF		// Boot block tábla olvasási védelem
#define CLOCK_FREQ 64000000