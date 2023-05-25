unsigned char chk_busy;
unsigned char i,ch;
/*const rom unsigned char betwk[] = {0x02,0x04,0x0E,0x01,0x0F,0x11,0x0F,0x00,    //á
                                   0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00,    //é
                                   0x02,0x04,0x0C,0x04,0x04,0x04,0x0E,0x00,    //í
                                   0x02,0x04,0x0E,0x11,0x11,0x11,0x0E,0x00,    //ó
                                   0x02,0x04,0x11,0x11,0x11,0x13,0x0D,0x00,    //ú
                                   0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00,    //ü
                                   0x05,0x0A,0x11,0x11,0x11,0x13,0x0D,0x00,    //û
                                   0x05,0x0A,0x0E,0x11,0x11,0x11,0x0E,0x00};   //õ*/

//-- A hardverfüggõ részletek elkülönítése
#define RSHIGH()         LATB |= 0x02;
#define RSLOW()          LATB &= 0xFD;
#define RWHIGH()         LATB |= 0x04;
#define RWLOW()          LATB &= 0xFB;
#define EHIGH()          LATB |= 0x08;
#define ELOW()           LATB &= 0xF7;
#define BUSY_FLAG PORTBbits.RB7;
#define DATA_DIR_RD()    TRISBbits.TRISB4 = 1; TRISBbits.TRISB5 = 1;  TRISBbits.TRISB6 = 1; TRISBbits.TRISB7 = 1;
#define DATA_DIR_WR()    TRISBbits.TRISB4 = 0; TRISBbits.TRISB5 = 0;  TRISBbits.TRISB6 = 0; TRISBbits.TRISB7 = 0;
#define OUTPUT_DATA(x) {LATB = (LATB & 0x0E) | x;} //4 alsó bit magas szintre állítva

//-- Az E vezérlõjel pulzálása 
void epulse(void){
    Delay10TCYx(2);  
    EHIGH();  Delay10TCYx(2); 
    ELOW(); Delay10TCYx(1);
}

//-- Egy bájt (parancs vagy adat) kiírása
void lcd_write(unsigned char cmd, unsigned char data_flag, unsigned char chk_busy, unsigned char dflag){
	char bflag,c;
	if(chk_busy){
		RSLOW();                 //RS = 0 a foglaltság figyeléshez
		DATA_DIR_RD();           //adatvonalak vételre állítása
		RWHIGH();                //R/W = 1 olvasáshoz
		do{
			EHIGH(); 
			Delay10TCYx(2);
			bflag = BUSY_FLAG;     //felsõ 4 bit olvasása
			ELOW(); Delay10TCYx(2);
			epulse();              //alsó félbájt kiléptetése
		}while(bflag);
	}else{
		Delay10KTCYx(12);        //Foglaltság figyelés helyett késleltetés
	}
	DATA_DIR_WR();             //Adatvonalak írásra állítása
	if(data_flag){ 
		RSHIGH();                //RS=1, ha adatküldés következik
	}
	else RSLOW();              //RS=0, ha parancsot küldünk
		RWLOW();                   //R/W = 0, íráshoz
	c = cmd & 0xF0;            //felsõ 4 bit kiírása
	OUTPUT_DATA(c);
	epulse();
	if(dflag){
		c = (cmd & 0x0F)<<4;     //alsó 4 bit kiírása
		OUTPUT_DATA(c);
		epulse();
	}
}

//-- Az LCD modul inicializálása
void lcd_init(void) {
	//-- Az LCD-t vezérlõ vonalak inicializálása
	DATA_DIR_WR();
	ELOW();
	RSLOW();
	RWLOW(); 
	//-- Az LCD modul szoftveres reset-elése és inicializálása
	Delay10KTCYx(60);          //50 ms várakozás az LCD feléledésére 
	lcd_write(0x30,0,0,0);     //8 bites üzemmód      
	Delay10KTCYx(6);           //5 ms várakozás
	lcd_write(0x30,0,0,0);     // 4 bites üzemmód
	lcd_write(0x30,0,0,0);     // 8 bites üzemmód
	lcd_write(0x20,0,0,0);     // 4 bites üzemmódba kapcsolunk
	//-- Innen kezdve a 4 bites üzemmód él (küldésnél dflag=1 kell)
	lcd_write(0x28,0,0,1);     // 2 soros display, 5x7 font
	//-- Innen kezdve figyelhetõ a BF jelzõbit (chk_busy=1)
	lcd_write(0x08,0,1,1);     // display letiltás
	lcd_write(0x01,0,1,1);     // képernyõtörlés
	lcd_write(0x0C,0,1,1);     // display be, cursor, és villogás ki
	stdout = _H_USER;          // a felhasználói függvény legyen a
                               // standard kimenet, ami az LCD-re ír
}

/*void lcd_init_cgram(void) {
    lcd_write(0x40,0,1,1);  // kurzor a CGRAM elejére
    for(i=0; i<64; i++) {
      lcd_write(betwk[i],1,1,1);    // definiálható karakterek feltöltése
    }                               // ékezetes karakterekkel
    lcd_write(0x80,0,1,1);          // kurzor vissza, a DDRAM elejére	
}*/

//-- LCD-re egy karaktert kiíró függvény a _H_USER stream számára
void _user_putc(char cx) {
	/*switch(cx) {
	    case 'á':  cx = 0x00; break;
	    case 'é':  cx = 0x01; break;
	    case 'í':  cx = 0x02; break;
	    case 'ó':  cx = 0x03; break;
	    case 'ú':  cx = 0x04; break;
	    case 'ü':  cx = 0x05; break;
	    case 'û':  cx = 0x06; break;
	    case 'õ':  cx = 0x07; break;
	    case 'ö':  cx = 0xEF; break;
    } */
    lcd_write(cx,1,1,1);
}

//ROM-ban tárolt stringek kiíratására
void RomStringWrite(const rom static unsigned char * pszovegek){
	char c_h;
	do{
		c_h =  *pszovegek;
		if (c_h) _user_putc(c_h);
		*pszovegek++;
	}while (c_h!=0);
}

/*void outdec(long data, unsigned int ndigits, unsigned int maxtizedes) {
    static char sign, s[12], volt;
    unsigned int x;
    x=0; sign='+'; volt='v';
    if(data<0) { sign='-'; data = -data;}
    do {
	    s[x]=data%10 + '0';
	    data=data/10;
	    x++;
	    if(x==ndigits) {s[x]='.'; x++;}
    } 
	while(data>0 || x<ndigits+2);
	_user_putc(sign);
    do{
		if(x>=maxtizedes){
    		_user_putc(s[--x]);
		}else{ break; }
    } 
	while(x);
 	_user_putc(volt); //Volt "v" kiíratása a végére
//	printf("                    "); //LCD sor frissítése
}*/

void epulse(void);
void lcd_write(unsigned char cmd, unsigned char data_flag, unsigned char chk_busy, unsigned char dflag);
void lcd_init(void);
void lcd_init_cgram(void);
void _user_putc(char cx);
void RomStringWrite(const rom static unsigned char * pszovegek);
