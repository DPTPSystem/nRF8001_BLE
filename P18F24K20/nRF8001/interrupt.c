void hi_isr(void);     ///< Magas prioritású programmegszakítás kiszolgálása
void lo_isr(void);     ///< Alacsony prioritású programmegszakítás kiszolgálása

// Ha nem használok bootloadert
#pragma code high_vector_addr=0x08
void high_vector(void) {
  _asm GOTO hi_isr _endasm
}
#pragma code low_vector_addr=0x18
void low_vector(void) {
  _asm GOTO lo_isr _endasm
}
#pragma code             //visszatérünk az alapértelmezett kódszekcióba
#pragma interrupt hi_isr

void hi_isr (void) {
	/* Itt történik a magas prioritású megszakítás tényleges kiszolgálása */
	unsigned char length, i;
	LATAbits.LATA0 = !LATAbits.LATA0;	// Visszajelzünk interruptban
	
	length = nRF_Read(nRFDataBuffer);
	write_uart(length);
	for(i=0; i<length; i++){
		write_uart(nRFDataBuffer[i]);
	}
	
	INTCONbits.INT0IF = 0;		// INT0 flag törlése
}

#pragma interruptlow lo_isr
void lo_isr (void) {
  /* Itt történik az alacsony prioritású megszakítás tényleges kiszolgálása */
	
}
