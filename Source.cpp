#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <EEPROM.h>  



//Esitellaan LSD nayttoon tarvittavat pinnit
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Ajan esittämiseen tarvittavat muuttujat 
int SEK1, SEK2, MIN1, MIN2;
volatile byte time = 0;
volatile int P1_RunningTime = 0;
volatile int P2_RunningTime = 0;
int P1 = 0;
int P2 = 0;


//"PelaajanVuoro" muuttaja 0 = pause, 1 = pelaajan 1 vuoro, 2 = pelaajan 2 vuoro
int PelaajanVuoro = 0;

//EEEPROM:iin mahtuu vain maksimiarvoltaan 254 kokoinen int muuttuja, joten RunningTime muuttuja tarvitsee jakaa kolmella tallentamiseen
int val1 = 0;
int val2 = 0;

//Esitellään napeissa kaytetyt peliajat
const int kello3min = 180;
const int kello5min = 300;
const int kello9min = 540;

//Esitellaan pelaajien vuoroa kuvastavat ledit
const int P1_Led = 6;
const int P2_Led = 8;


//Esitellaan napit
const int P1_Button = 7;
const int P2_Button = 9;
const int P1Min3_Button = 16;
const int P1Min5_Button = 15;
const int P1Min9_Button = 14;
const int P2Min3_Button = 19;
const int P2Min5_Button = 18;
const int P2Min9_Button = 17;
const int Pause_Button = 10;
const int EEPROM_Read_Button = 13;


void setup() {



    Timer1.initialize(1000000);
    Timer1.attachInterrupt(Timer_int_routine);

    lcd.begin(16, 2);
    P1 = 180;
    P2 = 180;
    P1_RunningTime = P1;
    P2_RunningTime = P2;

    //Asetetaan pinModet. Input = Nappi, output vuoroa kuvastavat ledit
    pinMode(P1_Led, OUTPUT);
    pinMode(P2_Led, OUTPUT);
    pinMode(P1_Button, INPUT);
    pinMode(P2_Button, INPUT);

    pinMode(P1Min3_Button, INPUT);
    pinMode(P1Min5_Button, INPUT);
    pinMode(P1Min9_Button, INPUT);

    pinMode(P2Min3_Button, INPUT);
    pinMode(P2Min5_Button, INPUT);
    pinMode(P2Min9_Button, INPUT);
    pinMode(Pause_Button, INPUT);
    pinMode(EEPROM_Read_Button, INPUT);

    //Asetetaan aluksi vuorot pauselle
    PelaajanVuoro = 0;



}
void loop() {

    //Vuoronvaihtonapit

    if (digitalRead(P1_Button) == HIGH)
    {
        PelaajanVuoro = 2;
    }

    if (digitalRead(P2_Button) == HIGH)
    {
        PelaajanVuoro = 1;
    }

    //Pause-nappi, joka kirjoittaa painettaessa RunningTime muuttujat EEPROMin indekseihin 1 ja 2

    if (digitalRead(Pause_Button) == HIGH)
    {
        PelaajanVuoro = 0;
        int val1 = P1_RunningTime / 3;
        int val2 = P2_RunningTime / 3;

        EEPROM.write(1, val1);
        EEPROM.write(2, val2);
        delay(500);
    }

    //Tuo muistista EEPROMIIN tallennetut kellonajat

    if (digitalRead(EEPROM_Read_Button) == HIGH && PelaajanVuoro == 0)
    {

        P1_RunningTime = EEPROM.read(1) * 3;
        P2_RunningTime = EEPROM.read(2) * 3;

        delay(500);
    }


    //Kun ei ole kenenkään vuoro, ledit ovat off
    if (PelaajanVuoro == 0)

    {
        digitalWrite(P1_Led, LOW);
        digitalWrite(P2_Led, LOW);
    }

    //Pelaajan 1 vuoro, pelaajalla 1 palaa LED
    if (PelaajanVuoro == 1)
    {
        digitalWrite(P1_Led, HIGH);
        digitalWrite(P2_Led, LOW);

    }

    //Pelaajan 2 vuoro, pelaajalla 2 palaa LED
    if (PelaajanVuoro == 2)
    {
        digitalWrite(P2_Led, HIGH);
        digitalWrite(P1_Led, LOW);

    }







    //PELAAJAN 1, KOLME AIKAVALINTAA
    if (digitalRead(P1Min3_Button) == HIGH)
    {
        P1_RunningTime = 180;
        PelaajanVuoro = 0;

    }
    if (digitalRead(P1Min5_Button) == HIGH)
    {
        P1_RunningTime = 300;
        PelaajanVuoro = 0;

    }
    if (digitalRead(P1Min9_Button) == HIGH)
    {
        P1_RunningTime = 540;
        PelaajanVuoro = 0;

    }

    //PELAAJAN 2, KOLME AIKAVALINTAA
    if (digitalRead(P2Min3_Button) == HIGH)
    {
        P2_RunningTime = 180;
        PelaajanVuoro = 0;

    }

    if (digitalRead(P2Min5_Button) == HIGH)
    {
        P2_RunningTime = 300;
        PelaajanVuoro = 0;

    }

    if (digitalRead(P2Min9_Button) == HIGH)
    {
        P2_RunningTime = 540;
        PelaajanVuoro = 0;

    }




}

//Kerran sekunnissa tapahtuva timer routine
void Timer_int_routine()

{

    lcd.clear();
    
    //Muutetaan RunningTime muotoon minuutteja ja sekuntteja
    SEK1 = P1_RunningTime % 60;
    MIN1 = (P1_RunningTime - SEK1) / 60;
    SEK2 = P2_RunningTime % 60;
    MIN2 = (P2_RunningTime - SEK2) / 60;

    //Estetaan valkkyminen kun Runningtime = 0 if lauseella
    //Printataan running time ja pelaajien kello joka sekuntti
    if (P1_RunningTime > 0 && P2_RunningTime > 0)
    {
        lcd.setCursor(0, 0);
        lcd.print("Player1");
        lcd.setCursor(0, 1);
        lcd.print(MIN1);
        lcd.print(":");

        if (SEK1 < 10)
        {
            lcd.print("0");
        }
        lcd.print(SEK1);

        lcd.setCursor(8, 0);
        lcd.print("Player2");
        lcd.setCursor(8, 1);
        lcd.print(MIN2);
        lcd.print(":");

        if (SEK2 < 10)
        {
            lcd.print("0");
        }
        lcd.print(SEK2);

    }

    //Vahennetaan joka timerInterrupt syklissa yksi arvo RunningTimesta pelaajalta kenen vuoro on
    if (PelaajanVuoro == 1)
    {
        P1_RunningTime--;
    }


    if (PelaajanVuoro == 2)
    {
        P2_RunningTime--;
    }

    if (PelaajanVuoro == 3)
    {
        P1_RunningTime--;
        P2_RunningTime--;
    }

    //Viesti kun kello on lopussa, pysaytetaan ajanotto
    if (P1_RunningTime < 1)
    {

        lcd.clear();
        lcd.print("P1 aika loppui");

        PelaajanVuoro = 0;

    }

    if (P2_RunningTime < 1)
    {

        lcd.clear();
        lcd.print("P2 aika loppui");
        P1_RunningTime = 60;
        P2_RunningTime = 60;
        PelaajanVuoro = 0;
    }

}
