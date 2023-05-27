# Bluetooth Low Energy nRF8001
* BLE vagy is alacsony energiaigényű bluetooth (Android 4.0-tól)
* DPTP System 2023-05-25.
* Projekt keletkezése: 2014-12-25.
* PIC18F24K22 - nRF8001 BLE - MPLAB IDE 8.91 - C18

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_3.jpg "DPTP System nRF8001")

# Miért
Egy magyarországi fejlesztő csapat keresett meg egy olyan okos karorával kapcsolatos fejlesztésük keretein belül, amely többeközt a viselő 
bőrtípusától függően vagy függetlenűl képes a börpigment elváltozásaiból megállapítani az egyes éllettani tulajdonásgait. Vagy is ez volt a
cél, és ennek egyik eleme volt a alacsony energia igényű (BLE) bluetooth kapcsolat, amelyet egy nRF8001-es chip biztosította. A fő kérés a 
nyákokra való kisméretű chipek szakszerű felforrasztása volt, de mivel ez a technológia engem is érdekelt el kezdtem a saját fejlesztéseimet.
Ez a szál ezt mutaja és osztja meg az érdeklődőkkel.

# Célok
Egyértelműen csak a tesztelés és kisérletezés érdekelt elsősorban, illetve az új technológia megismerése és késövvi felhasználása android 
rendszerek használatával. Ennek egy része teljesült is, de aztán érdeklődés és valódi konkrét elhatározás a további fejlesztésekre nem voltak,
így a projekt ennyibe is maradt.

# Elmélet
Az eszköz elméleti működése csak annyi, hogy BLE-n keresztül tudjunk kapcsolódni android vagy más alkalmazásokkal és adatokat tudjak mind két 
oldalról küldeni és fogadni. Ezen adatokat valamilyen módon fény vagy másként jelezni a felhasználó felé. A működés ellenőrzésére egy UART-os 
kommunikációs adaptert használtam.

# Hardver
A hardver faék egyszerű egy PIC18F24K22, nRF8001 és a hozzá tartozó gyártói oldalról ajánlott elemek (kondig, tekercsek és ellenállások), illetve 
néhány kiegészítő alkatrész, mint LED vagy UART vezetékek kivezetése. UART csatlakozásra egy külön USB/UART átalakító adaptert használtam.

# PC ás Android
A program és a hardver tesztelése céljából készítettem egy windows alkalmazást, amely csak az UART-os soros kommunikáció egyfajta köztes állomása
volt, hogy megfelelően tudjam az egyes parancsokat, teszteket végig vinni. Ezen felül használtam egy ingyenes android telefonokra elérhető nRF UART 
v2.0-ás alkalmazást, amely a 4.0-ás vagy magasabb android verziójú telefonoknál képes a BLE bluetooth kapcsolatokat megteremteni.

# Nyáktervek
A nyáktervek még vasalos kivitelezéshez, egyoldalas áramköri rajzolattal készültek.

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nRF8001.PNG "DPTP System nRF8001")

# PCB

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_1.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_2.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_5.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_4.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_6.jpg "DPTP System nRF8001")


# Program
A program működése rendkívűl egyszerű. Egy végtelen ciklusban várakozik és figyeli, hogy érkezik e a bluetooth chiptől vagy UART felől valamilyen
adat. Annak megfelelően, hogy éppen az nRF8001 vagy külső UART-ról érkezik adat, a célzott feltétel lefut és kiértékelődik az érkezett adat.

Ha az nRF8001-re érkezik adat:

```
if(!RDYN){
	// további kód része az érkezett adatokat dolgozza fel
		}
```
Ha UART-on érkezik adat a következő struktúrát kell követnünk:

```
if(DataRdyUSART()){
			result = read_uart();
			if(result=='c'){
				//Homerseklet lekerese
			}else if(result=='b'){  
				//Command: Bond
			}else if(result=='s'){  
				//Command: Connect
			}else{
				// Adat küldés
			}
		 }
```

Mind a kettő esetben a nRFDataBuffer nevű változó (tömb) tartalmazza az érkezett adatokat.

# Kapcsolodó videós tartalmak
DPTP System - nRF8001 Bluetooth kapcsolat

[![DPTP System - nRF8001 Bluetooth kapcsolat](https://img.youtube.com/vi/UjtZtTLGWQU/0.jpg)](https://www.youtube.com/watch?v=UjtZtTLGWQU)

DPTP System - nRF8001 - Android kapcsolat

[![DPTP System - nRF8001 Android kapcsolat](https://img.youtube.com/vi/Bq7xkpVMoTk/0.jpg)](https://www.youtube.com/watch?v=Bq7xkpVMoTk)

