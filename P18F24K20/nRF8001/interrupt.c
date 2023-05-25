void hi_isr(void);     ///< Magas priorit�s� programmegszak�t�s kiszolg�l�sa
void lo_isr(void);     ///< Alacsony priorit�s� programmegszak�t�s kiszolg�l�sa

// Ha nem haszn�lok bootloadert
#pragma code high_vector_addr=0x08
void high_vector(void) {
  _asm GOTO hi_isr _endasm
}
#pragma code low_vector_addr=0x18
void low_vector(void) {
  _asm GOTO lo_isr _endasm
}
#pragma code             //visszat�r�nk az alap�rtelmezett k�dszekci�ba
#pragma interrupt hi_isr

void hi_isr (void) {
	/* Itt t�rt�nik a magas priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
	unsigned char length, i;
	LATAbits.LATA0 = !LATAbits.LATA0;	// Visszajelz�nk interruptban
	
	length = nRF_Read(nRFDataBuffer);
	write_uart(length);
	for(i=0; i<length; i++){
		write_uart(nRFDataBuffer[i]);
	}
	
	INTCONbits.INT0IF = 0;		// INT0 flag t�rl�se
}

#pragma interruptlow lo_isr
void lo_isr (void) {
  /* Itt t�rt�nik az alacsony priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
	
}
