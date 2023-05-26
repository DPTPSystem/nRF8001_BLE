# Bluetooth Low Energy nRF8001
* BLE vagy is alacsony energiaigényű bluetooth (Android 4.0-tól)
* DPTP System 2023-05-25.
* Projekt keletkezése: 2014-12-25.
* PIC18F24K22 - nRF8001 BLE - MPLAB IDE 8.91 - C18

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_3.jpg "DPTP System nRF8001")

# Miért
Egy magyarországi fejlesztő csapat keresett meg egy olyan okos karorával kapcsolatos fejlesztésük keretein belül, amely a viselő bőrtípusától
függően vagy függetlenűl képes a börpigment elváltozásaiból megállapítani az egyes éllettani tulajdonásgait. Vagy is ez volt a cél, és ennek 
egyik eleme volt a alacsony energia igényű (BLE) bluetooth kapcsolat, amely egy nRF8001-es chip. A fő kérés a nyákokra való kisméretű chipek
szakszerű felforrasztása volt, de mivel ez a technológia engem is érdekelt el kezdtem a saját fejlesztéseimet. Ez a szál ezt mutaja és osztja
meg az érdeklődőkkel.

# Célok
Egyértelműen csak a tesztelés és kisérletezés érdekelt elsősorban, illetve az új technológia megismerése és majdnai felhasználása android rendszerek
használatával. Ennek egy része teljesült is, de aztán érdeklődés és valódi konkrét elhatározás a további fejlesztésekre nem voltak, így a 
projekt ennyibe is maradt.

# Elmélet
Az eszköz elméleti működése csak annyi, hogy BLE-n keresztül tudjak kapcsolódni android vagy más alkalmazásokkal és adatokat tudjak mind két oldalról 
küldeni és fogadni. Ezen adatokat valamilyen módon fény vagy másként jelezni a felhasználó felé.

# Hardver
A hardver faék egyszerű egy PIC18F24K22, nRF8001 és a hozzá tartozó gyártói oldalról ajánlott elemek (kondig, tekercsek és ellenállások), illetve 
néhány kiegészítő alkatrész, mint LED vagy UART kivezetése. UART csatlakozásra egy külön USB/UART átalakító adaptert használtam.

# PC ás Android
A program és a hardver tesztelése céljából készítettem egy windows alkalmazást, amely csak az UART-os soros kommunikáció egyfajta köztes állomása
volt, hogy megfelelően tudjam az egyes parancs teszteket végig vinni. Ezen felül használtam egy ingyenes android telefonokra elérhető nRF UART v2.0-es
alkalmazást, amely a 4.0-ás vagy magasabb android verziójú telefonoknál képes a BLE bluetooth kapcsolatokat megteremteni.

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/14484406329764_b.jpg "DPTP System nRF8001")

# Nyáktervek

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nRF8001.PNG "DPTP System nRF8001")

# PCB

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_1.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_2.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_5.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_4.jpg "DPTP System nRF8001")

![DPTP System nRF8001](https://github.com/DPTPSystem/nRF8001_BLE/blob/master/images/nrf8001_6.jpg "DPTP System nRF8001")


# Program


# Kapcsolodó videós tartalmak
DPTP System - nRF8001 Bluetooth kapcsolat
https://www.youtube.com/watch?v=UjtZtTLGWQU

DPTP System - nRF8001 - Android kapcsolat
https://www.youtube.com/watch?v=Bq7xkpVMoTk
