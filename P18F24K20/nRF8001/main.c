/* Main.c file, nRF8001 configuration
 * DPTP System, don_peter@freemail.hu
 * Created:   Cs dec. 25 2014
 * Processor: PIC18F24K20
 * Compiler:  MPLAB C18
 */

#include <p18f24k20.h>
#include <stdio.h>
#include <delays.h>				//K�sleltet�sek
#include <stdlib.h>
//#include <timers.h>			//Timer-ek
//#include <pwm.h>				//PWM f�gv�nyk�nyvt�r
//#include <spi.h>				//SPI f�gv�nyk�nyvt�r
#include "header.h"				//�lltal�nos header be�ll�t�sok
//#include "lcd.c"				//LDC meghajt�s f�ggv�nyei
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
			/*if(nRFDataBuffer[0] == 0x86){	// Sz�tkapcsol�s eset�n �jra csatlakozunk...
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
			if(nRFDataBuffer[0] == 0x86){	// Sz�tkapcsol�s eset�n �jra csatlakozunk...
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
				// Adat k�ld�s
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
	
	RCONbits.IPEN = 1;		// K�tszint� megszak�t�si m�d be�ll�t�sa
    INTCONbits.GIEH = 1;	// A magas priorit�s� interrupt enged�lyez�se
    INTCONbits.GIEL = 0;	// Az alacsony priorit�s� interrupt tilt�sa
	INTCONbits.INT0IE = 0;	// INT0 K�ls� Interrupt enged�lyez�
	INTCON2 = 0;			// INT0 lefut� �lre megszak�t�s enged�lyezve
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
	nRFRESET = 1;			// nRF8001 ind�t�sa
	Delay_ms(62);			// ~62ms m�g az nRF RDYN fel�bred
	REQN = 1;				// nRF8001 tilt�sa
	SPI_SO_TRIS = 0;		// l�b SDI kimenet
	SPI_SCK_TRIS = 0;		// l�b SCK kimenet
	SPI_SI = 1;				// l�b SI bemenet

	SSPSTATbits.SMP = 0;	//Mintav�telez�s id�z�t�se Master m�d eset�n 1: mintav�telez�s a kimeneti id� v�g�n, 0: mintav�telez�s a kimeneti id� k�zep�n). Slave m�dban k�telez�en null�ba kell �ll�tani.
	SSPSTATbits.CKE = 1;	//�rajel f�zis�nak be�ll�t�sa (1: az adatkimenet be�ll�t�sa akkor t�rt�nik,amikor az �rajel akt�v �llapotb�l t�tlenre v�lt, 0: az adatkimenet be�ll�t�sa akkor t�rt�nik, amikor az �rajel t�tlen �llapotb�l akt�v szintre v�lt)
	SSPSTATbits.BF = 0;		//A buffer foglalts�g�t jelz� �llapotbit  (1: az adat�tvitel lezajlott, SSPBUF kiolvashat�, 0: az adatbuffer �res)

	SSPCON1bits.WCOL = 	0;	//�tk�z�s detekt�l�s �r�sn�l (1: �r�st/adatk�ld�st k�s�relt�nk meg, amikor nem lehetett, 0: nincs �tk�z�s)
	SSPCON1bits.SSPOV = 0;	//V�teli t�lcsordul�s-jelz� (1: �jabb b�jt �rkezett, de SSPBUF meg nincs kiolvasva, 0: nincs t�lfoly�s)
	SSPCON1bits.SSPEN = 1;	//Master Szinkron Soros Port enged�lyez�se (1: enged�lyezve, 0: letiltva)
	SSPCON1bits.CKP = 	0;	//�rajel polarit�sa (1: akt�v �llapotban alacsony szint, 0: akt�v �llapotban magas szint)
							//Master Szinkron Soros Port �zemm�dj�nak be�ll�t�sa:
							//0101 = SPI Slave m�d, az SS l�b haszn�lata n�lk�l
							//0100 = SPI Slave m�d, amelyn�l az SS l�b is haszn�latban van
							//0011 = SPI Master m�d, Timer2/2 �rajel frekvenci�val
							//0010 = SPI Master m�d, Fosc/64 �rajel frekvenci�val
							//0001 = SPI Master m�d, Fosc/16 �rajel frekvenci�val
							//0000 = SPI Master m�d, Fosc/4 �rajel frekvenci�val
	SSPCON1bits.SSPM0 = 0;
	SSPCON1bits.SSPM1 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM3 = 0;
	
}
