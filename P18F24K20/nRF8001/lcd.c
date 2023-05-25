unsigned char chk_busy;
unsigned char i,ch;
/*const rom unsigned char betwk[] = {0x02,0x04,0x0E,0x01,0x0F,0x11,0x0F,0x00,    //�
                                   0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00,    //�
                                   0x02,0x04,0x0C,0x04,0x04,0x04,0x0E,0x00,    //�
                                   0x02,0x04,0x0E,0x11,0x11,0x11,0x0E,0x00,    //�
                                   0x02,0x04,0x11,0x11,0x11,0x13,0x0D,0x00,    //�
                                   0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00,    //�
                                   0x05,0x0A,0x11,0x11,0x11,0x13,0x0D,0x00,    //�
                                   0x05,0x0A,0x0E,0x11,0x11,0x11,0x0E,0x00};   //�*/

//-- A hardverf�gg� r�szletek elk�l�n�t�se
#define RSHIGH()         LATB |= 0x02;
#define RSLOW()          LATB &= 0xFD;
#define RWHIGH()         LATB |= 0x04;
#define RWLOW()          LATB &= 0xFB;
#define EHIGH()          LATB |= 0x08;
#define ELOW()           LATB &= 0xF7;
#define BUSY_FLAG PORTBbits.RB7;
#define DATA_DIR_RD()    TRISBbits.TRISB4 = 1; TRISBbits.TRISB5 = 1;  TRISBbits.TRISB6 = 1; TRISBbits.TRISB7 = 1;
#define DATA_DIR_WR()    TRISBbits.TRISB4 = 0; TRISBbits.TRISB5 = 0;  TRISBbits.TRISB6 = 0; TRISBbits.TRISB7 = 0;
#define OUTPUT_DATA(x) {LATB = (LATB & 0x0E) | x;} //4 als� bit magas szintre �ll�tva

//-- Az E vez�rl�jel pulz�l�sa 
void epulse(void){
    Delay10TCYx(2);  
    EHIGH();  Delay10TCYx(2); 
    ELOW(); Delay10TCYx(1);
}

//-- Egy b�jt (parancs vagy adat) ki�r�sa
void lcd_write(unsigned char cmd, unsigned char data_flag, unsigned char chk_busy, unsigned char dflag){
	char bflag,c;
	if(chk_busy){
		RSLOW();                 //RS = 0 a foglalts�g figyel�shez
		DATA_DIR_RD();           //adatvonalak v�telre �ll�t�sa
		RWHIGH();                //R/W = 1 olvas�shoz
		do{
			EHIGH(); 
			Delay10TCYx(2);
			bflag = BUSY_FLAG;     //fels� 4 bit olvas�sa
			ELOW(); Delay10TCYx(2);
			epulse();              //als� f�lb�jt kil�ptet�se
		}while(bflag);
	}else{
		Delay10KTCYx(12);        //Foglalts�g figyel�s helyett k�sleltet�s
	}
	DATA_DIR_WR();             //Adatvonalak �r�sra �ll�t�sa
	if(data_flag){ 
		RSHIGH();                //RS=1, ha adatk�ld�s k�vetkezik
	}
	else RSLOW();              //RS=0, ha parancsot k�ld�nk
		RWLOW();                   //R/W = 0, �r�shoz
	c = cmd & 0xF0;            //fels� 4 bit ki�r�sa
	OUTPUT_DATA(c);
	epulse();
	if(dflag){
		c = (cmd & 0x0F)<<4;     //als� 4 bit ki�r�sa
		OUTPUT_DATA(c);
		epulse();
	}
}

//-- Az LCD modul inicializ�l�sa
void lcd_init(void) {
	//-- Az LCD-t vez�rl� vonalak inicializ�l�sa
	DATA_DIR_WR();
	ELOW();
	RSLOW();
	RWLOW(); 
	//-- Az LCD modul szoftveres reset-el�se �s inicializ�l�sa
	Delay10KTCYx(60);          //50 ms v�rakoz�s az LCD fel�led�s�re 
	lcd_write(0x30,0,0,0);     //8 bites �zemm�d      
	Delay10KTCYx(6);           //5 ms v�rakoz�s
	lcd_write(0x30,0,0,0);     // 4 bites �zemm�d
	lcd_write(0x30,0,0,0);     // 8 bites �zemm�d
	lcd_write(0x20,0,0,0);     // 4 bites �zemm�dba kapcsolunk
	//-- Innen kezdve a 4 bites �zemm�d �l (k�ld�sn�l dflag=1 kell)
	lcd_write(0x28,0,0,1);     // 2 soros display, 5x7 font
	//-- Innen kezdve figyelhet� a BF jelz�bit (chk_busy=1)
	lcd_write(0x08,0,1,1);     // display letilt�s
	lcd_write(0x01,0,1,1);     // k�perny�t�rl�s
	lcd_write(0x0C,0,1,1);     // display be, cursor, �s villog�s ki
	stdout = _H_USER;          // a felhaszn�l�i f�ggv�ny legyen a
                               // standard kimenet, ami az LCD-re �r
}

/*void lcd_init_cgram(void) {
    lcd_write(0x40,0,1,1);  // kurzor a CGRAM elej�re
    for(i=0; i<64; i++) {
      lcd_write(betwk[i],1,1,1);    // defini�lhat� karakterek felt�lt�se
    }                               // �kezetes karakterekkel
    lcd_write(0x80,0,1,1);          // kurzor vissza, a DDRAM elej�re	
}*/

//-- LCD-re egy karaktert ki�r� f�ggv�ny a _H_USER stream sz�m�ra
void _user_putc(char cx) {
	/*switch(cx) {
	    case '�':  cx = 0x00; break;
	    case '�':  cx = 0x01; break;
	    case '�':  cx = 0x02; break;
	    case '�':  cx = 0x03; break;
	    case '�':  cx = 0x04; break;
	    case '�':  cx = 0x05; break;
	    case '�':  cx = 0x06; break;
	    case '�':  cx = 0x07; break;
	    case '�':  cx = 0xEF; break;
    } */
    lcd_write(cx,1,1,1);
}

//ROM-ban t�rolt stringek ki�rat�s�ra
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
 	_user_putc(volt); //Volt "v" ki�rat�sa a v�g�re
//	printf("                    "); //LCD sor friss�t�se
}*/

void epulse(void);
void lcd_write(unsigned char cmd, unsigned char data_flag, unsigned char chk_busy, unsigned char dflag);
void lcd_init(void);
void lcd_init_cgram(void);
void _user_putc(char cx);
void RomStringWrite(const rom static unsigned char * pszovegek);
