#include <usart.h>

void init_uart(void);
void write_uart(unsigned char data);
unsigned char read_uart(void);

 void init_uart(void){
        TRISC |= 0xc0;	// RX/TX input
	 
        //Baud rate kiszámolása
		/*((FOSC/Desired Baud Rate)/64) – 1
		= ((64000000/9600)/64) – 1
		= [103,167] = 103*/
		
		// Configure EUSART
		TXSTAbits.CSRC	= 1;	// Clock Source Select bit: Slave mode (clock from external source)
		TXSTAbits.TX9	= 0;	// 9-Bit Transmit Enable bit: Selects 8-bit transmission
		TXSTAbits.TXEN	= 1;	// Transmit Enable bit: Transmit enabled
		TXSTAbits.SYNC	= 0;	// EUSART Mode Select bit: Asynchronous mode
		TXSTAbits.SENDB	= 0;	// Send Break Character bit: Sync Break transmission completed
		TXSTAbits.BRGH	= 0;	// High Baud Rate Select bit: Low speed
		TXSTAbits.TRMT	= 0;	// Transmit Shift Register Status bit: TSR full
		TXSTAbits.TX9D	= 0;	// 9th bit of Transmit Data
		
		RCSTAbits.SPEN	= 0; 	// Serial Port Enable bit: Serial port enabled
		RCSTAbits.RX9	= 0;	// 9-Bit Receive Enable bit: Selects 8-bit reception
		RCSTAbits.SREN	= 0;	// Single Receive Enable bit: Disables single receive
		RCSTAbits.CREN	= 1;	// Continuous Receive Enable bit: Enables receiver
		RCSTAbits.ADDEN	= 0;	// Address Detect Enable bit: Disables address detection, all bytes are received and ninth bit can be used as parity bit
		RCSTAbits.FERR	= 0;	// Framing Error bit: No framing error
		RCSTAbits.OERR	= 0;	// Overrun Error bit: No overrun error
		RCSTAbits.RX9D	= 0;	// 9th bit of Received Data
		
		BAUDCONbits.ABDOVF	= 0;	// Auto-Baud Acquisition Rollover Status bit: No BRG rollover has occurred
		BAUDCONbits.RCIDL	= 0;	// Receive Operation Idle Status bit: Receive operation is active
		BAUDCONbits.DTRXP	= 0;	// Received Data Polarity Select bit: RX data received is not inverted
		BAUDCONbits.CKTXP	= 0;	// Clock and Data Polarity Select bit: TX data is inverted
		BAUDCONbits.BRG16	= 0;	// 16-Bit Baud Rate Register Enable bit: 8-bit Baud Rate Generator – SPBRG only (Compatible mode), SPBRGH value ignored
		BAUDCONbits.WUE		= 0;	// Wake-up Enable bit: RX pin not monitored or rising edge detected
		BAUDCONbits.ABDEN	= 0;	// Auto-Baud Detect Enable bit: Baud rate measurement disabled or completed
		SPBRG = 103;
		RCSTAbits.SPEN = 1;
    }
	
    void write_uart(unsigned char data){
        TXREG = data;
        while(!PIR1bits.TXIF);
    }
	
    unsigned char read_uart(void){
        RCSTAbits.CREN = 1;
        while(!PIR1bits.RCIF);
        return RCREG;
    }
