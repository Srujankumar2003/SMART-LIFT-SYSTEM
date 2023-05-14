
////////////////////////////////////////////////////////////
// Bibliotek:
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD Pinner i bruk:
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int sensor = 8;  
// Tegn for bruk på LCD
byte ARROWUP[8] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B00000,
};

byte ARROWDOWN[8] = {
  B00000,
  B01110,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000,
};

byte OPENL[8] = {
  B00011,
  B00111,
  B01110,
  B11100,
  B01110,
  B00111,
  B00011,
};

byte OPENR[8] = {
  B11000,
  B11100,
  B01110,
  B001111,
  B01110,
  B11100,
  B11000,
};


Servo Servo1;       // oprette Servo

int call1 = A0;     // heis til 1.etg
int call2 = A1;     // heis til 2.etg
int call3 = A2;     // heis til 3.etg
int call4 = A3;     // heis til 4.etg
//int call5 = A4;     // heis til 5.etg (Penthouse)

int doorPin = 12;         // pinne for servo (Heisdør)
int motor1 = 9;        // motor retning 1
int motor2 = 11;       // motor retning 2

int cFloor = 1;        // nåværende etasje for Heis
int dFloor = 1;        // destinasjon etasje for Heis
int mFloor = 0;        // antall etasjer Heisen skal gå

int travelTime = 2000;   // tid mellom etasjen motor skal gå (kan endres)
int doorTime = 3000;     // tiden heisdøra står åpen (kan endres)

const int tempPin = A5;  // pinne for TMP36 (kan ikke endres)
const int buzzer = 10;   // pinne for buzzer (kan ikke endres)

void setup() {

  // Setter type OUTPUT for pinne 1-7:
  for (int o = 2; o < 8; o++)
  {
    pinMode(o, OUTPUT);
    pinMode(buzzer, OUTPUT);  // pinnemodus for "buzzer"
    pinMode(tempPin, INPUT);  // pinnemodus for "tempPin"
  }

  // Bruke innebygd motstand på inngang for knapper:
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  //pinMode(A4, INPUT_PULLUP);
  

  // Antall kolonner og rader på LCD:
  lcd.begin(16, 2);

  // Velkomst beskjed på LCD:
  lcd.setCursor(0, 0);
   lcd.print("--- Welcome ---");
    lcd.setCursor(0, 1);
     lcd.print("  Select Floor ");


  Servo1.attach(doorPin);   // knytter servo til doorpin
  Servo1.write(0);        // setter dør i lukket posisjon
}

void loop() {
  
  int val = digitalRead(sensor);
  
  if (digitalRead(call1) == LOW && val==HIGH) {   // første etasje
    lcd.clear();
     lcd.setCursor(0, 0);             // setter hvor i LCD tekst skal begynnet
      lcd.print("  First Floor ");    // skriv til LCD
    dFloor = 1;             // setter nåværende etasje
    if (cFloor != 1) {          // hvis nåværende etasje ikke er lik destinasjons etasje:
      mFloor = dFloor - cFloor;     // finn differansen mellom etasjene
      move();             // bruk move() funksjonen
    }                 // deretter:
    door();               // åpne dør med door() funksjonen
    cFloor = 1;             // sett nåværende etasje
  }
  

  if (digitalRead(call2) == LOW && val==HIGH)      // andre etasje
  { 
    lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("  Second Floor ");
    dFloor = 2;
    if (cFloor != 2) {
      mFloor = dFloor - cFloor;
      move();
    }
    door();
    cFloor = 2;
  }
  

  if (digitalRead(call3) == LOW && val==HIGH)      // tredje etasje
  { 
    lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("  Third Floor ");
    dFloor = 3;
    if (cFloor != 3) {
      mFloor = dFloor - cFloor;
      move();
    }
    door();
    cFloor = 3;
  }
  
  
   if (digitalRead(call4) == LOW && val==HIGH)     // fjerde etasje
   {  
    lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("  Fourth Floor ");
    dFloor = 4;
    if (cFloor != 4) {
      mFloor = dFloor - cFloor;
      move();
    }
    door();
    cFloor = 4;
  }
 

}

// Funksjon som kjører heis opp eller ned basert på nåværende og destinasjons etasje:
void move() 
{
  if (cFloor < dFloor) {      // hvis nåværende etasje er lavere enn destinasjons etasje
    up();             // bruk up() funksjon 
  }                 // eller
  if (cFloor > dFloor) {      // hvis nåværende etasje er høyere enn destinasjons etasje
    down();             // bruk down() funksjon
  }
}

// Funksjon som kjører heis oppover
void up() 
{
  analogWrite(motor1, 50);      // bruk PWM (Puls bredde modulasjon) for å kjøre motor (hastighet kan endres)
  analogWrite(motor2, 0);     // 
  lcd.setCursor(0, 1);
   lcd.print("   GOING  UP   ");  // skriv til LCD
    lcd.createChar(0, ARROWUP);     // henter tegn "ARROWUP"
     lcd.setCursor(15, 0);  
      lcd.write(byte(0));
  delay(travelTime*(abs(mFloor)));  // heisen kjører i satt "travel time", ganget med differansen mellom nåværende og destinasjons etasjen)
  analogWrite(motor1, 0);     // stopper motor
}

// Funksjon for å kjøre heis nedover
void down() 
{
  analogWrite(motor1, 0);
  analogWrite(motor2, 50);
  lcd.setCursor(0, 1);
   lcd.print("  GOING  DOWN  ");
    lcd.createChar(0, ARROWDOWN);   // henter tegn "ARROWDOWN"
     lcd.setCursor(15, 1);  
      lcd.write(byte(0));
  delay(travelTime*(abs(mFloor)));
  analogWrite(motor2, 0);
}

// Funksjon for å åpne og lukke heisdør
void door() 
{
  delay(1500);                 // utsett åpning av dør i ms
   lcd.clear();
    lcd.setCursor(0,1);
     lcd.print("   DOOR OPENS ");
  lcd.createChar(0, OPENR);       // henter tegn "OPENR"
   lcd.setCursor(10, 0);
    lcd.write(byte(0));
     lcd.createChar(1, OPENL);    // henter tegn "OPENL"
      lcd.setCursor(5, 0);
       lcd.write(byte(1));
  dingO();                       // kjører dingO på buzzer
   delay(500);
  Servo1.write(90);       // åpne dør
   delay(doorTime);     // holder dør åpen i ms
  lcd.setCursor(0,1);
   lcd.print("   DOOR CLOSES ");
  lcd.createChar(2, OPENR);
   lcd.setCursor(5, 0);
    lcd.write(byte(2));
     lcd.createChar(3, OPENL);
      lcd.setCursor(10, 0);
       lcd.write(byte(3));
  dingC();                   // kjører dingC på buzzer
   delay(1000);
  Servo1.write(0);      // lukker heisdør
   delay(1500);
  lcd.setCursor(0, 0);
   lcd.print("--- Welcome ---");
    lcd.setCursor(0,1);
     lcd.print("  Select Floor ");
}

  
  void dingO() 
  {
    tone(buzzer,200);   // buzzer frekvens
     delay(350);      // tid i ms
    noTone(buzzer);
     delay(110);
    tone(buzzer,300);
     delay(350);
    tone(buzzer,400);
     delay(450);
    noTone(buzzer);     // skru av buzzer
  } 
   

   void dingC()
   {
     tone(buzzer,400);
      delay(450);
     noTone(buzzer);
      delay(110);
     tone(buzzer,200);
      delay(350);
     noTone(buzzer);
   }
