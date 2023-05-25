#include "nrfsetup.h"
//System commands
#define Test					0x01
#define Echo					0x02
#define DtmCommand				0x03
#define SLEEP					0x04
#define Wakeup 					0x05
#define Setup 					0x06
#define ReadDynamicData 		0x07
#define WriteDynamicData 		0x08
#define GetDeviceVersion 		0x09
#define GetDeviceAddress 		0x0A
#define GetBatteryLevel 		0x0B
#define GetTemperature 			0x0C
#define RadioReset 				0x0E
#define Connect 				0x0F
#define Bond 					0x10
#define Disconnect 				0x11
#define SetTxPower 				0x12
#define ChangeTimingRequest 	0x13
#define OpenRemotePipe 			0x14
#define SetApplicationLatency 	0x19
#define SetKey 					0x1A
#define OpenAdvPipe 			0x1B
#define Broadcast 				0x1C
#define BondSecRequest 			0x1D
#define DirectedConnect 		0x1E
#define CloseRemotePipe			0x1F

//System events
#define DeviceStartedEvent		0x81
#define EchoEvent				0x82
#define HardwareErrorEvent		0x83
#define CommandResponseEvent	0x84
#define ConnectedEvent			0x85
#define DisconnectedEvent		0x86
#define BondStatusEvent			0x87
#define PipeStatusEvent			0x88
#define TimingEvent				0x89
#define DisplayKeyEvent			0x8E
#define KeyRequestEvent			0x8F

//Data commands
#define SetLocalData			0x0D
#define SendData				0x15
#define SendDataAck 			0x16
#define RequestData				0x17
#define SendDataNack			0x18

//Data events
#define DataCreditEvent			0x8A
#define DataAckEvent			0x8B
#define DataReceivedEvent		0x8C
#define PipeErrorEvent			0x8D

// ACTIVE láb az nRF8001 rádiós aktivítást jelzi (LED-es visszajelzés)
#define nRFRESET		LATCbits.LATC0		// nRF reset láb

#define RDYN			PORTBbits.RB0		// Bemenet (nRF visszajelzõ láb)
#define REQN			LATCbits.LATC2		// nRF8001 engedélyezõ

#define SPI_SO_TRIS		TRISCbits.TRISC5	// SPI kimenet
#define SPI_SCK_TRIS	TRISCbits.TRISC3	// SPI órajel
#define SPI_SI			TRISCbits.TRISC4	// SPI bemenet

const rom struct nRF8001Setup_t {
	unsigned char dummy;
	unsigned char command[32];
} nRF8001Setup[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;

static rom unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

// Az elsõ byte, Debug byte, ha adatot fogadunk, ha küldünk, akkor length byte
// Ha adatot fogadunk, akkor a második byte a length byte, külömben már adat
volatile unsigned char nRFDataBuffer[32] = {0};	// Maximum 32byte
void Delay_ms(unsigned int ms);
unsigned char SPI_IO(unsigned char adat);
unsigned char nRF_Read(unsigned char *ptrbuff);
void nRF_Write(unsigned char *ptrbuff);
unsigned char TombKarakterHossz(const char string[]);
void nRF8001_INIT(void);

/*********************************************
+		Késleltetés
**********************************************/
void Delay_ms(unsigned int ms){ //Bemenet*ms várakozás
   unsigned int i=0;
   for (i=0;i<=ms;i++) Delay1KTCYx(16);
}

unsigned char SPI_IO(unsigned char adat){
	unsigned char tmp;  		// Tároló változó
	SSPSTATbits.BF = 0;			// törli foglaltság jelzõ álapotát a státusz regiszterben
	SSPCON1bits.WCOL = 0;		// törli az esetleges írás ütközés hibajelzõt
	tmp = SSPBUF;				// törli a BF jelzõbitet
	SSPBUF = BitReverseTable256[adat];				// kirakja a kimenõ adatot az SSPBUF regiszterbe
	while(!SSPSTATbits.BF);		// megvárjuk a busz ciklus végét 
	return (BitReverseTable256[SSPBUF]);			// a vett bájttal térünk vissza
}

/*if(!RDYN){	// RDYN-t figyeljük és, ha alacsony szintre kerül, akkor indulhat az olvasás
	nRF_Read(nRFDataBuffer);
}*/
unsigned char nRF_Read(unsigned char *ptrbuff){
	unsigned char i;					// Implementáló
	unsigned char LengthByte;			// nRF adat hossz
	unsigned char Debug;				// nRF debug
	while(RDYN);						// Várunk nRF8001 aktív állapotára (RDYN alacsony szintre kerûl)
	//Delay_ms(1);						// Késleltetünk picit RDYN beállta után
	REQN = 0;							// ACI adatport engedélyezése
	Debug = SPI_IO(0x00);				// Debug olvasása, eldobjuk
	LengthByte = SPI_IO(0x00);			// Fogadásra váró adathossz bájtban
	for (i=0; i<LengthByte; i++){		// Ciklus pergése "length" adatmennyiségig
		*ptrbuff++=SPI_IO(0x00);		// Olvasunk, de valamit akkor is küldeni kell!
	}
	REQN = 1;							// ACI adatport tiltása
	while(!RDYN);						// Várakozunk nRF8001 kapcsolat bontására
	Delay_ms(2);						// A kommunikáció minimális inaktivitási ideje 250ns (Table 4, Page 26)
	return LengthByte;					// Visszatérünk a kiolvasott adathosszal
}

// Írásnál az elsõ byte a tömb hosszát kell tartalmaznia
void nRF_Write(unsigned char *ptrbuff){
	unsigned short i;					// Implementáló
	REQN = 0;							// ACI adatport engedélyezése
	while(RDYN);						// Várunk nRF8001 aktív állapotára (RDYN alacsony szintre kerûl)
		Delay_ms(50);
	for (i=0; i<(ptrbuff[0]+1); i++){	// Ciklus pergése "length" adatmennyiségig
		SPI_IO(ptrbuff[i]);				// Az adatbuffer kiírása
	}
	REQN = 1;							// ACI adatport tiltása
	while(!RDYN);						// Várakozunk nRF8001 kapcsolat bontására
	Delay_ms(2);						// A kommunikáció minimális inaktivitási ideje 250ns (Table 4, Page 26)
}

//Tömb hossza
unsigned char TombKarakterHossz(const char string[]){
	int count = 0;
	while(string[count] != '\0')
        count++;
    return count;
}

// nRF8001 SETUP
void nRF8001_INIT(void){
	// Ide jön az inicializálás
	unsigned char i,x,y,length;
	
	for(i=0; i<NB_SETUP_MESSAGES; i++){						// Ciklus SETUP parancssorok végéig
		for(y=0; y<nRF8001Setup[i].command[0]+1; y++){		// Ciklus SETUP CMD parancsok végéig
			nRFDataBuffer[y] = nRF8001Setup[i].command[y];	// nRF buffer feltöltése
		}
		nRF_Write(nRFDataBuffer);							// Parancsok kiírása
		Delay_ms(50);										// Várakozunk egy picit
		
		length = nRF_Read(nRFDataBuffer);					// Visszajelzés olvasása
		for(x=0; x<length; x++){							// Ciklus visszajezés hosszuságáig
			write_uart(nRFDataBuffer[x]);					// Kiküldjük UART-on a kapot adatokat
			Delay_ms(10);									// Késleltetüj a küldést
		}
    }
	
	//---BOND---//
	/*nRFDataBuffer[0]=5;		//length
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
	}*/

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
