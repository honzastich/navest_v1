#include <Servo.h>           //zahrnutí knihovny pro ovládání servo motoru
#include <EEPROM.h>

const byte switchnPin = 2;    // číslo pinu prepinace
const byte buttonPin = 3;     // číslo pinu na shazovací tlačítko
const byte ledPin =  13;      // číslo pinu LED
const byte servoPin_1 = 9;

const byte servoMin  = 30; //  servo jde dolu - navest nahoru
const byte servoMax  = 90;  // servo jde nahoru, navest dolu - cim vyssi, tim je navest niz a servo vys 

const byte servoDelay = 20;  // milisec mezi uhlz serva Cim vetsi, tim pomalejsi
byte statBtn = 0;            // schazovaci tlacitko
byte lastStatBtn = LOW;      // predchozi stav schazovaciho tlacitka
byte statSwitch = 0;         // proměnná stavu prepinace
byte lastStatSwitch = LOW;   // predchozi stav prepinace

byte servoStat = 20; // aby probehla inicializace
byte lastServoStat = 20; // aby probehla inicializace
byte stavNavestidla = 10; // aby probehla inicializace

unsigned long switchLastTime = 0;
unsigned long buttonLastTime = 0;
unsigned int debounce = 500; // jak dlouho se ceka na dalsi akci

Servo myservo;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  // inicializujte pin tlačítka jako vstup:
  pinMode(switchnPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // seriova komunikace
  //Serial.begin(9600);
  byte Servo1Position = EEPROM.read(1);
  myservo.attach(servoPin_1);
  if (Servo1Position == 1) {
    myservo.write(servoMin);
  }
  else {
    myservo.write(servoMax);
  }
  myservo.detach();
  //Serial.println("Poloha: " + String(Servo1Position));
}
void loop() {
  // Přečte stav hodnoty tlačítka:
  //servoStat = LOW;
  statBtn = digitalRead(buttonPin);
  if (statBtn == HIGH && lastStatBtn == LOW && millis() - buttonLastTime > debounce) {
    if (stavNavestidla == HIGH) {
      //Serial.println("Akt stav: " + String(test));
      akce(LOW);  //
    }
    buttonLastTime = millis();
  }
  lastStatBtn = statBtn;
  statSwitch = digitalRead(switchnPin);
  if (statSwitch != lastStatSwitch)
  {
    switchLastTime = millis();
  }
  if ((millis() - switchLastTime) > debounce)
  {
    servoStat = LOW;
    //Serial.println(statSwitch);
    if (statSwitch == HIGH) {
      servoStat = HIGH;
    }
    else
    {
      statSwitch = LOW;
      servoStat = LOW;
    }
    
    if (lastServoStat != servoStat)
    {
      lastServoStat = servoStat;
      if (stavNavestidla != servoStat) {
        akce(servoStat);
      }
    }
  }
  lastStatSwitch = statSwitch;
}
void akce(int x)
{
  EEPROM.update (1, x); //...zapis nove polohy navestidla...
  digitalWrite(ledPin, x);
  //Serial.println(x);
  stavNavestidla = x;
  if (x == LOW)
  {
    servoUp();
  }
  else
  {
    
    servoDown();
  }
}
void servoUp()
{
  myservo.attach(servoPin_1);
  byte pos = 0;
  byte aktPos = 0;
  aktPos = myservo.read();
  //for (pos = servoMin; pos <= servoMax; pos += 1) //je od úhlu 0 do úhlu 180
  for (pos = aktPos; pos <= servoMax; pos += 1) //je od úhlu 0 do úhlu 180
  {
    //Serial.println("Up: " + String(pos));
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(servoDelay);           //chvilka čekání než se motor natočí
  }
  myservo.detach();
}
void servoDown()
{
  myservo.attach(servoPin_1);
  byte pos = 0;
  byte aktPos = 0;
  aktPos = myservo.read();
  //Serial.println(aktPos);
  //myservo.write(93);
  //for (pos = servoMax; pos >= servoMin; pos -= 1) //je od úhlu 180 zpět do úhlu 0
  for (pos = aktPos; pos >= servoMin; pos -= 1) //je od úhlu 180 zpět do úhlu 0
  {
    //Serial.println("down: " + String(pos));
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(servoDelay);           //chvilka čekání než se motor natočí
  }
  myservo.detach();
}
