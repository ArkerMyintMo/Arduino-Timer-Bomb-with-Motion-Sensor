
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal.h>
//LiquidCrystal_I2C lcd(0x3F,16,2);  //Change the HEX address
//LiquidCrystal_I2C lcd(0x38,16,2);
LiquidCrystal lcd(12, 11, 10, A3, A4, A5);

int Hours = 00;
int Minutes = 00;
int Seconds = 00;
int trycount = 0;
int keycount = 0;
int PIRvalue = 0;
int counter = 0;
int i = 0;

int redled = A1;
int greenled = A0;
int relay = A2;
int PIRpin = 13;

int hourstenscode;
int hoursonescode;
int mintenscode;
int minonescode;
int sectenscode;
int seconescode;

long secMillis = 0;
long interval = 1000;

char password[8];
char entered[8];

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5};  //7,2,3,5 for Black 4x3 keypad
byte colPins[COLS] = {4, 3, 2};     //6,8,4 for Black 4x3 Keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(PIRpin, INPUT);  
  digitalWrite(redled,HIGH);
  digitalWrite(greenled,HIGH);
  //digitalWrite(greenled,HIGH);

  //lcd.init();
  //lcd.backlight();
  lcd.begin(16, 2);
  Serial.begin(9600);
  activate();
  
}
void loop()
{
  motion();
  timer();
  char disarmcode = keypad.getKey();

  if (disarmcode == '*')
  {
    tone(9, 5000, 100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Code: ");

    while (keycount < 8)
    {
      timer();

      char disarmcode = keypad.getKey();
      if (disarmcode == '#')
      {
        tone(9, 5000, 100);
        keycount = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code: ");
      }
      else if (disarmcode != NO_KEY)
      {
        lcd.setCursor(keycount + 5, 0);
        lcd.blink();
        lcd.print(disarmcode);
        entered[keycount] = disarmcode;
        keycount++;
        tone(9, 5000, 100);
        delay(100);
        lcd.noBlink();
        lcd.setCursor(keycount + 4, 0);
        lcd.print("*");
        lcd.setCursor(keycount + 5, 0);
        lcd.blink();
      }
    }

    if (keycount == 8)
    {
      if (entered[0] == password[0] && entered[1] == password[1] && entered[2] == password[2] && entered[3] == password[3]&& entered[4] == password[4] && entered[5] == password[5] &&  entered[6] == password[6] && entered[7] == password[7])
      {
        lcd.noBlink();
        lcd.clear();
        lcd.home();
        lcd.print("Bomb Defused!");
        lcd.setCursor(0, 1);
        lcd.print("Well Done!");
        keycount = 0;
        digitalWrite(greenled, LOW);
        delay(15000);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Reset the Bomb");
        delay(5000);
        activate();
      }
      else
      {
        lcd.noBlink();
        lcd.clear();
        lcd.home();
        lcd.print("Wrong Password!");
        trycount++;

        if (Hours > 0)
        {
          Hours = Hours / 2;
        }

        if (Minutes > 0)
        {
          Minutes = Minutes / 2;
        }
        if (Seconds > 0)
        {
          Seconds = Seconds / 2;
        }
        if (trycount == 2)
        {
          interval = interval / 10;
        }
        if (trycount == 3)
        {
          Minutes = Minutes - 59;
          Hours = Hours - 59;
          Seconds = Seconds - 59;
        }
        delay(1000);
        keycount = 0;
      }
    }
  }
}


void timer()
{
  Serial.print(Seconds);
  Serial.println();

  if (Hours <= 0)
  {
    if ( Minutes < 0 )
    {
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("The Bomb Has ");
      lcd.setCursor (0, 1);
      lcd.print("Exploded!");

      while (Minutes < 0)
      {
        digitalWrite(redled, LOW);
        tone(9, 7000, 100);
        delay(100);
        digitalWrite(redled, HIGH);
        tone(9, 7000, 100);
        delay(100);
        digitalWrite(greenled, LOW);
        tone(9, 7000, 100);
        delay(100);
        digitalWrite(greenled, HIGH);
        tone(9, 7000, 100);
        delay(100);
        activateRelay();
      }
    }
  }
  lcd.setCursor (0, 1);
  lcd.print ("Timer:");

  if (Hours >= 10)
  {
    lcd.setCursor (7, 1);
    lcd.print (Hours);
  }
  if (Hours < 10)
  {
    lcd.setCursor (7, 1);
    lcd.write ("0");
    lcd.setCursor (8, 1);
    lcd.print (Hours);
  }
  lcd.print (":");

  if (Minutes >= 10)
  {
    lcd.setCursor (10, 1);
    lcd.print (Minutes);
  }
  if (Minutes < 10)
  {
    lcd.setCursor (10, 1);
    lcd.write ("0");
    lcd.setCursor (11, 1);
    lcd.print (Minutes);
  }
  lcd.print (":");

  if (Seconds >= 10)
  {
    lcd.setCursor (13, 1);
    lcd.print (Seconds);
  }

  if (Seconds < 10)
  {
    lcd.setCursor (13, 1);
    lcd.write ("0");
    lcd.setCursor (14, 1);
    lcd.print (Seconds);
  }

  if (Hours < 0)
  {
    Hours = 0;
  }

  if (Minutes < 0)
  {
    Hours --;
    Minutes = 59;
  }

  if (Seconds < 1)
  {
    Minutes --;
    Seconds = 59;
  }

  if (Seconds > 0)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - secMillis > interval)
    {
      tone(9, 7000, 50);
      secMillis = currentMillis;
      Seconds --;
      digitalWrite(greenled, LOW);
      delay(10);
      digitalWrite(greenled, HIGH);
      delay(10);
    }
  }
}
void activate(){
lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MISSION STARTING");
  lcd.setCursor(0, 1);
  lcd.print("Set Key:");
  while (keycount < 8)
  {
    lcd.setCursor(keycount + 8, 1);
    lcd.blink();
    char armcode = keypad.getKey();
    armcode == NO_KEY;
    if (armcode != NO_KEY)
    {
      if ((armcode != '*') && (armcode != '#'))
      {
        lcd.print(armcode);
        tone(9, 5000, 100);
        password[keycount] = armcode;
        keycount++;
      }
    }
  }

  if (keycount == 8)
  {
    delay(500);
    lcd.noBlink();
    lcd.clear();
    lcd.home();
    lcd.print("Security key is: ");
    lcd.setCursor(4, 1);
    lcd.print(password[0]);
    lcd.print(password[1]);
    lcd.print(password[2]);
    lcd.print(password[3]);
    lcd.print(password[4]);
    lcd.print(password[5]);
    lcd.print(password[6]);
    lcd.print(password[7]);
    delay(3000);
    lcd.clear();
  }
  lcd.setCursor(0, 0);
  lcd.print("Timer: HH:MM:SS");
  lcd.setCursor(0, 1);
  lcd.print("SET:   :  :");
  keycount = 9;

  while (keycount == 9)
  {
    char hourstens = keypad.getKey();
    lcd.setCursor(5, 1);
    lcd.blink();
    if (hourstens >= '0' && hourstens <= '9')
    {
      hourstenscode = hourstens - '0';
      tone(9, 5000, 100);
      lcd.print(hourstens);
      keycount++;
    }
  }

  while (keycount == 10)
  {
    char hoursones = keypad.getKey();
    lcd.setCursor(6, 1);
    lcd.blink();
    if (hoursones >= '0' && hoursones <= '9')
    {
      hoursonescode = hoursones - '0';
      tone(9, 5000, 100);
      lcd.print(hoursones);
      keycount++;
    }
  }

  while (keycount == 11)
  {
    char mintens = keypad.getKey();
    lcd.setCursor(8, 1);
    lcd.blink();
    if (mintens >= '0' && mintens <= '9')
    {
      mintenscode = mintens - '0';
      tone(9, 5000, 100);
      lcd.print(mintens);
      keycount++;
    }
  }

  while (keycount == 12)
  {
    char minones = keypad.getKey();
    lcd.setCursor(9, 1);
    lcd.blink();
    if (minones >= '0' && minones <= '9')
    {
      minonescode = minones - '0';
      tone(9, 5000, 100);
      lcd.print(minones);
      keycount++;
    }
  }

  while (keycount == 13)
  {
    char sectens = keypad.getKey();
    lcd.setCursor(11, 1);
    lcd.blink();
    if (sectens >= '0' && sectens <= '9')
    {
      sectenscode = sectens - '0';
      tone(9, 5000, 100);
      lcd.print(sectens);
      keycount = 14;
    }
  }

  while (keycount == 14)
  {
    char secones = keypad.getKey();
    lcd.setCursor(12, 1);
    lcd.blink();
    if (secones >= '0' && secones <= '9')
    {
      seconescode = secones - '0';
      tone(9, 5000, 100);
      lcd.print(secones);
      keycount = 15;
    }
  }

  if (keycount == 15);
  {
    Hours = (hourstenscode * 10) + hoursonescode;
    Minutes = (mintenscode * 10) + minonescode;
    Seconds = (sectenscode * 10) + seconescode;
    delay(100);
    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Timer set at:");
    if (Hours >= 10)
    {
      lcd.setCursor (7, 1);
      lcd.print (Hours);
    }
    if (Hours < 10)
    {
      lcd.setCursor (7, 1);
      lcd.write ("0");
      lcd.setCursor (8, 1);
      lcd.print (Hours);
    }
    lcd.print (":");

    if (Minutes >= 10)
    {
      lcd.setCursor (10, 1);
      lcd.print (Minutes);
    }
    if (Minutes < 10)
    {
      lcd.setCursor (10, 1);
      lcd.write ("0");
      lcd.setCursor (11, 1);
      lcd.print (Minutes);
    }
    lcd.print (":");

    if (Seconds >= 10)
    {
      lcd.setCursor (13, 1);
      lcd.print (Seconds);
    }

    if (Seconds < 10)
    {
      lcd.setCursor (13, 1);
      lcd.write ("0");
      lcd.setCursor (14, 1);
      lcd.print (Seconds);
    }
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press # to start");
    delay (50);
    keycount = 16;
  }

  while (keycount == 16)
  {
    char armkey = keypad.getKey();

    if (armkey == '#')
    {
      tone(9, 5000, 100);
      delay(50);
      tone(9, 0, 100);
      delay(50);
      tone(9, 5000, 100);
      delay(50);
      tone(9, 0, 100);
      delay(50);
      tone(9, 5000, 100);
      delay(50);
      tone(9, 0, 100);
      lcd.clear();
      lcd.print ("Bomb Active!");
      lcd.setCursor(0, 1);
      lcd.print("Start Counting..");
      delay(3000);
      lcd.clear();
      keycount = 0;
    }
  }
}
void motion(){
  
  PIRvalue = digitalRead(PIRpin); 
  if( PIRvalue == HIGH ){
      lcd.setCursor(0, 0);
      lcd.print("Motion detected");
      digitalWrite(redled, HIGH);
      tone(9, 5000, 100);
      counter++;
    }
  delay(1000); 
  digitalWrite(redled, LOW); 
  if(counter == 6 || counter == 12 || counter == 18){
    lcd.setCursor(0, 0);
    lcd.print("Reduced Time....");
    tone(9, 5000, 100);
    trycount++;

    if (Hours > 0)
    {
      Hours = Hours / 2;
    }

    if (Minutes > 0)
    {
      Minutes = Minutes / 2;
    }
    if (Seconds > 0)
    {
      Seconds = Seconds / 2;
    }
    if (trycount == 5)
    {
      interval = interval / 10;
    }
    if (trycount == 6)
    {
      Minutes = Minutes - 59;
      Hours = Hours - 59;
      Seconds = Seconds - 59;
    }
    counter++;
  } 
}
void activateRelay(){

  digitalWrite(relay, HIGH); 
  tone(8, 7000, 100);  // turn the LED on (HIGH is the voltage level)
  //digitalWrite(relay, LOW);

}

