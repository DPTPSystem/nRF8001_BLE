/* Main.c file, nRF8001 configuration
 * DPTP System, don_peter@freemail.hu
 * Created:   Cs dec. 25 2014
 * Processor: PIC18F24K20
 * Compiler:  MPLAB C18
 */

#include <p18f24k20.h>
#include <stdio.h>
#include <delays.h>				//Késleltetések
#include <stdlib.h>
//#include <timers.h>			//Timer-ek
//#include <pwm.h>				//PWM fügvénykönyvtár
//#include <spi.h>				//SPI fügvénykönyvtár
#include "header.h"				//Álltalános header beállítások
//#include "lcd.c"				//LDC meghajtás függvényei
#include "eusart.c"
#include "nrf8001.c"
#include "interrupt.c"

#define LED LATAbits.LATA0

void delay_ms(unsigned int ms);
void init(void);

void main(){
	
	unsigned char ciklus = 0x00, length, result;
	unsigned char i;
	unsigned short ho;
	
	init();

	/*LATAbits.LATA1 = 1;

	lcd_init();

	printf("nRF8001             ");
	lcd_write(0xC0,0,1,1);   
	printf("                    ");
	lcd_write(0x94,0,1,1);
	printf("                    ");
	lcd_write(0xD4,0,1,1);
	printf("Adat                ");
	lcd_write(0x80,0,1,1);

	//LED = !LED;
	
	while(1){LED = !LED; Delay_ms(100);}*/
	
	LED = !LED;

	length=nRF_Read(nRFDataBuffer);
	//write_uart(length);
	for(i=0; i<length; i++){
		write_uart(nRFDataBuffer[i]);
		Delay_ms(100);
	}
	
   	nRFDataBuffer[0]=1;
	nRFDataBuffer[1]=GetTemperature;
	nRF_Write(nRFDataBuffer);
	Delay_ms(1000);
	length = nRF_Read(nRFDataBuffer);
	//write_uart(length);
	for(i=0; i<length; i++){
		write_uart(nRFDataBuffer[i]);
		Delay_ms(10);
	}
	
	nRF8001_INIT();
	
	while (1){
		LED = !LED;
		
		if(!RDYN){
			length = nRF_Read(nRFDataBuffer);
			//write_uart(length);
			for(i=0; i<length; i++){
				write_uart(nRFDataBuffer[i]);
				Delay_ms(10);
			}
		
			// BOND
			/*if(nRFDataBuffer[0] == 0x86){	// Szétkapcsolás esetén újra csatlakozunk...
					Delay_ms(1000);
					//---BOND---//
					nRFDataBuffer[0]=5;		//length
					nRFDataBuffer[1]=Bond;	//Command: Bond
					nRFDataBuffer[2]=0x1e;	//Timeout
					nRFDataBuffer[3]=0x00;	//Timeout
					nRFDataBuffer[4]=0x02;	//Adv interval
					nRFDataBuffer[5]=0x02;	//Adv interval
					nRF_Write(nRFDataBuffer);
					length = nRF_Read(nRFDataBuffer);
					write_uart(length);
					for(i=0; i<length; i++){
						write_uart(nRFDataBuffer[i]);
						Delay_ms(10);
					}
			}*/

			// CONNECT
			if(nRFDataBuffer[0] == 0x86){	// Szétkapcsolás esetén újra csatlakozunk...
					Delay_ms(1000);
					//---CONNECT---//
					nRFDataBuffer[0]=5;
					nRFDataBuffer[1]=Connect;
					nRFDataBuffer[2]=0x00;
					nRFDataBuffer[3]=0x00;
					nRFDataBuffer[4]=0x08;
					nRFDataBuffer[5]=0x08;
					nRF_Write(nRFDataBuffer);
					length = nRF_Read(nRFDataBuffer);
					write_uart(length);
					for(i=0; i<length; i++){
						write_uart(nRFDataBuffer[i]);
						Delay_ms(10);
					}
			}

		}
		
		if(DataRdyUSART()){
			result = read_uart();
			if(result=='c'){
				//putrsUSART("Homerseklet lekerese...\r\n");
				nRFDataBuffer[0]=1;
				nRFDataBuffer[1]=GetTemperature;
				nRF_Write(nRFDataBuffer);
				Delay_ms(100);
				length = nRF_Read(nRFDataBuffer);
				for(i=0; i<length; i++){
					write_uart(nRFDataBuffer[i]);
					Delay_ms(10);
				}
			}else if(result=='b'){  
				//putrsUSART("Setup inditasa\r\n");
				nRFDataBuffer[0]=5;		//length
				nRFDataBuffer[1]=Bond;	//Command: Bond
				nRFDataBuffer[2]=0x1e;	//Timeout
				nRFDataBuffer[3]=0x00;	//Timeout
				nRFDataBuffer[4]=0x02;	//Adv interval
				nRFDataBuffer[5]=0x02;	//Adv interval
				nRF_Write(nRFDataBuffer);
			}else if(result=='s'){  
				//putrsUSART("Setup inditasa\r\n");
				nRFDataBuffer[0]=5;
				nRFDataBuffer[1]=Connect;
				nRFDataBuffer[2]=0x00;
				nRFDataBuffer[3]=0x00;
				nRFDataBuffer[4]=0x08;
				nRFDataBuffer[5]=0x08;
				nRF_Write(nRFDataBuffer);
			}else{
				// Adat küldés
				nRFDataBuffer[0]=3;
				nRFDataBuffer[1]=SendData;
				nRFDataBuffer[2]=2;
				nRFDataBuffer[3]=result;
				nRF_Write(nRFDataBuffer);
			}
		 }

		 
		//write_uart(ciklus);
		/*printf("nRF8001             ");
		lcd_write(0xC0,0,1,1);   
		printf("                    ");
		lcd_write(0x94,0,1,1);
		printf("                    ");
		lcd_write(0xD4,0,1,1);
		printf("Ciklus: %i          ", ciklus);
		lcd_write(0x80,0,1,1);*/

		/*if(ciklus==50){
			LED = !LED;
			nRFDataBuffer[0]=3;
			nRFDataBuffer[1]=SendData;
			nRFDataBuffer[2]=2;
			nRFDataBuffer[3]=0xff;
			nRF_Write(nRFDataBuffer);
			ciklus=0;
		}else{
			ciklus++;
		}*/
		Delay_ms(100);
	}
	
 }

void init(void){

	TRISA = TRISC = 0;
	TRISB = 0b00000001;
	PORTA = PORTB = PORTC = 0;
	OSCCON = 0;
	OSCTUNE = 0b11011111;	// PLL maximum freki
	ANSEL = 0;
	ANSELH = 0;
	
	RCONbits.IPEN = 1;		// Kétszintû megszakítási mód beállítása
    INTCONbits.GIEH = 1;	// A magas prioritású interrupt engedélyezése
    INTCONbits.GIEL = 0;	// Az alacsony prioritású interrupt tiltása
	INTCONbits.INT0IE = 0;	// INT0 Külsõ Interrupt engedélyezõ
	INTCON2 = 0;			// INT0 lefutó élre megszakítás engedélyezve
	INTCON3 = 0;
	
	SLRCON = 0;
	CCP1CON = 0;			// CAPTURE - COMPARE - PWM DISABLED
	CCP2CON = 0;			// CAPTURE - COMPARE - PWM DISABLED
	ECCP1AS = 0;
	ADCON0 = 0;
	CM1CON0 = 0;
	CM2CON0 = 0;
	WPUB=0;
	IOCB=0;
	
	init_uart();
	
	/*********************************************
	+		SPI init
	**********************************************/
	nRFRESET = 1;			// nRF8001 indítása
	Delay_ms(62);			// ~62ms míg az nRF RDYN felébred
	REQN = 1;				// nRF8001 tiltása
	SPI_SO_TRIS = 0;		// láb SDI kimenet
	SPI_SCK_TRIS = 0;		// láb SCK kimenet
	SPI_SI = 1;				// láb SI bemenet

	SSPSTATbits.SMP = 0;	//Mintavételezés idõzítése Master mód esetén 1: mintavételezés a kimeneti idõ végén, 0: mintavételezés a kimeneti idõ közepén). Slave módban kötelezõen nullába kell állítani.
	SSPSTATbits.CKE = 1;	//Órajel fázisának beállítása (1: az adatkimenet beállítása akkor történik,amikor az órajel aktív állapotból tétlenre vált, 0: az adatkimenet beállítása akkor történik, amikor az órajel tétlen állapotból aktív szintre vált)
	SSPSTATbits.BF = 0;		//A buffer foglaltságát jelzõ állapotbit  (1: az adatátvitel lezajlott, SSPBUF kiolvasható, 0: az adatbuffer üres)

	SSPCON1bits.WCOL = 	0;	//Ütközés detektálás írásnál (1: írást/adatküldést kíséreltünk meg, amikor nem lehetett, 0: nincs ütközés)
	SSPCON1bits.SSPOV = 0;	//Vételi túlcsordulás-jelzõ (1: újabb bájt érkezett, de SSPBUF meg nincs kiolvasva, 0: nincs túlfolyás)
	SSPCON1bits.SSPEN = 1;	//Master Szinkron Soros Port engedélyezése (1: engedélyezve, 0: letiltva)
	SSPCON1bits.CKP = 	0;	//Órajel polaritása (1: aktív állapotban alacsony szint, 0: aktív állapotban magas szint)
							//Master Szinkron Soros Port üzemmódjának beállítása:
							//0101 = SPI Slave mód, az SS láb használata nélkül
							//0100 = SPI Slave mód, amelynél az SS láb is használatban van
							//0011 = SPI Master mód, Timer2/2 órajel frekvenciával
							//0010 = SPI Master mód, Fosc/64 órajel frekvenciával
							//0001 = SPI Master mód, Fosc/16 órajel frekvenciával
							//0000 = SPI Master mód, Fosc/4 órajel frekvenciával
	SSPCON1bits.SSPM0 = 0;
	SSPCON1bits.SSPM1 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM3 = 0;
	
}
