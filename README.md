
# Dokumentation - Det inteligente hus
Programmet er lavet til at køre på en: **Arduino MEGA2560**
## General beskrivelse af projekt
Projektet skal forestille, at være et intelligent hus.
Jeg har gået udfra følgende:
- Man kan logge ind på LCD displayet, med en 4-cifret kode. Hvis koden er rigtigt, så kører en servo motor og simulere, at der er låst op. Samtidig så slukker en rød lys diode og en grøn tænder. Efter det så kommer det frem, at man kan vælge at se temperaturen.
- Man kan logge ud og servo motoren vil så kører den anden vej, for at simulere at der nu er låst og den røde lys diode vil tænde og den grønne vil slukke.
- Der er brugt keypad til at styre menuen/login, som er visualiseret på et LCD display.
- Der er en DC motor, som er tænkt til, at skulle være til at nedkøle et rum. Motoren bliver aktiveret hvis temperaturen er over 30 grader celsius, som bliver målt ved hjælp af en NTC thermistor.
- RGB LED'en vil lyse rød, når DC motoren er slukket og vil skifte til grøn, når DC motoren er tændt
## Perifære enheder
Jeg har brugt følgende:
- LCD Display
- Keypad
- RGB LED
- Servo motor
- DC motor
- NTC Thermistor
- Potentiometer
- 2 lys dioder (1 rød og 1 grøn)

#### Context Diagram:
![Context](https://github.com/nitram1337/IntelligentHouse/blob/master/images/contextNyeste.png?raw=true)
## Timers brugt
- **Timer0**
*Opsætning:*
**PF0/ADC0:** NTC Thermistor

- **Timer1**
*Opsætning:*
**PB5/OC1A:** DC motor
**PB6/OC1B:** Rødt lys på LED RGB
**PB7/OC1C:** Grønt lys på LED RGB

- **Timer3**
*Opsætning:*
**PE3/OC3A:** Servo motoren

- **Timer4**
Bliver brugt til interrupt, som trigger på compare match A. Interrupten udregner hvad temperaturen er og tænder eller slukker motoren og R og G i RGB LED'en, i forhold til temperaturen.

## **Ændring af portene**
**Keypad:**
Hvis man vil ændre hvilke porte, som bliver brugt til keypad'en, så kan det nemt ændres i keypad.h filen. Der er der define på hvilke porte, som der bliver brugt for hvilke columns og rows.

**De to lys dioder (rød/grøn)**
Hvis man vil ændre hvilke porte, som bliver brugt til de to lys dioder (rød og grøn), så kan det nemt ændres i diode.h filen, hvor der er defines på dioderne osv.

## Funktion beskrivelse
Ikke alle funktioner er beskrevet, kun dem som kræver beskrivelse ellers så er nogle af dem selv-beskrivende. De funktioner der ikke er beskrevet, står der som et overblik på programmet og dens funktioner
### **diode.c**
- ***INIT_LOCK_DIODES()***
- ***OnlyTurnOnGreen()***
- ***OnlyTurnOnRed()***
- ***LoggedInDiode(int loggedIn)*** Bliver brugt til lock funktionen. Hvis funktionen modtager et 1, så er det "logget ind" og den vil køre metoden OnlyTurnOnGreen og vil derfor tænde for den grønne diode og slukke den røde og ellers så omvendt.

### **keypad.c**
- Keypad’en kører ved hjælp af polling, så den tjekker column og rows’ne ved at kører uendeligt igennem while løkken i main
- ***InitKeypad()***  er en function, som jeg har lavet. Funktionen bruges til at sætte de forskellige porte. Funktionen bruges til, at enable pull-up, sætte data direction til output på 4 af portene og data direction til input på de sidste 4. Data direction er dog standard sat til input på de sidste fire, hvis ikke man rører ved dem, så det er egentligt ikke et behov.
- ***ColumnScan()***  er en function, som for hver gang den bliver kaldt, så sætter den den næste port til logisk 0 af de fire output porte og starter forfra igen, når den har nået enden af de fire output porte. Der er kun en port, som er sat til logisk 0, pr. gang funktionen er kørt. Funktionen returnere den column, som den lige har sat porten for til logisk 0.
- ***ReadRows()***  er en function, som tjekker om en af de 4 porte, som bruges som input, om den er blevet til logisk 0. Den tjekker en efter en hver af de 4 porte. Hvis den første port, som den tjekker er blevet sat til logisk 0, så vil den returnere det row, som hører til den pågældende port og derved ikke tjekke næste port, ellers så vil den tjekke næste og så næste igen osv. Hvis ingen er sat til logisk 0, så returnere funktionen bare 0.
- ***DecodeKeyboard(row, column)***  funktionen, bliver brugt til, at styre menuen på LCD displayet. Der er et multidemensional array, som indeholder alle karaktererne og så udfra hvad row og column, de 2 beskrevede funktioner over returnere, så finder den karakteren i arrayet, ved hjælp af index og det bruger den så til en switch case til at styre menuen.

### **temp_motor.c**

- ***INIT_TEMP()*** init til temperatur. NTC Thermistor, som er på ADC0.
- ***INIT_MOTOR()*** init til DC motoren.
- ***INIT_TEMP_INTERRUPT_TIMER4(unsigned int ms)*** init til opsætning af interrupt, som er på timer 4.
- ***ISR(TIMER4_COMPA_vect)*** bliver kørt, når interrupten bliver triggered. Interrupted bliver pt. triggered hvert sekund, men det kan ændres i main, hvor lang tid der skal gå. Den bruges til, at udregne den aktuelle temperatur fra NTC Thermistor og udfra om temperaturen er over 30 eller under 30 grader celsius, så skal den starte/slukke DC motoren og den røde og grønne diode. Derefter sætter en global variabel til den udregnede temperatur i celsius.
- ***GetTemp()*** Den globale variabel, som bliver forsynet med temperaturen, som beskrevet ovenover, den bliver i denne metode returned. Metoden bruges sådan set bare til at få temperaturen.

### **servo.c**
- ***INIT_SERVO()***
- ***CloseLock()*** bliver brugt til at dreje servo motoren og derved simulere at en lås bliver låst
- ***OpenLock()*** bliver brugt til at dreje servo motoren og derved simulere at en lås bliver åbnet

## Hippo
![Hippo](https://github.com/nitram1337/IntelligentHouse/blob/master/images/hippo.png?raw=true)

## Flowchart af menu
![menu](https://github.com/nitram1337/IntelligentHouse/blob/master/images/menu.png?raw=true)

## Flowchart af keypad
![keypadflowchart](https://github.com/nitram1337/IntelligentHouse/blob/master/images/FlowChart.png?raw=true)

## Logic analyser af keypad
![keypadlogic](https://github.com/nitram1337/IntelligentHouse/blob/master/images/OutputLogicTest.png?raw=true)

## Port tabel af keypad
![Porttabel](https://github.com/nitram1337/IntelligentHouse/blob/master/images/porttabel.png?raw=true)

![Porttegning](https://github.com/nitram1337/IntelligentHouse/blob/master/images/PortTegning.png?raw=true)

## Perifære enheder på portene
![Tilsluttet porte](https://github.com/nitram1337/IntelligentHouse/blob/master/images/tilsluttetporte.png?raw=true)

#### Lavet af Martin Sonne
