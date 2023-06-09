#include <LiquidCrystal.h>

/****** Enums *******/
typedef enum
{
  MANUAL,
  AUTOMATIC
} tSystemState;

typedef enum
{
  CW,
  CCW,
  STOP
} tDirection;

/******* Prototypes ******/
tSystemState isAutomatic(void);
void ManualState(void);
void AutomaticState(void);

void ReadCurrentAngle(void);
void DisplayCurrentAngle(void);

void ControlAzimuthMotor(tDirection dr);

/****** Variables *******/
String dir ;
String angle;
int len;

//StatePin
int StatePin = 2;
int ManualStateLED = A4;
int AutomaticStateLED = A5;

int PinA = 3;
int PinB = 4;
int PinC = 5;

const int rs = 12, en = 11, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);

void setup()
{
  //STATE LEDs
  pinMode(ManualStateLED, OUTPUT);
  pinMode(AutomaticStateLED, OUTPUT);

  //STATE PIN MODE
  pinMode(StatePin, INPUT);

  //CONTROL PIN MODE
  pinMode(PinA, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(PinC, OUTPUT);

  //CONFIGURE LCD
  LCD.begin(16, 2);
  Serial.begin(9600);
}


void loop() {

 if(isAutomatic() == AUTOMATIC)
  {
    AutomaticState();
  }
  else if(isAutomatic() == MANUAL) 
  {
    ManualState();
  } 
}

tSystemState isAutomatic(void) {
  if (digitalRead(StatePin) == HIGH) {
    return MANUAL;
  } else if (digitalRead(StatePin) == LOW) {
    return AUTOMATIC;
  }
}

void ManualState(void)
{
  LCD.setCursor(0, 0);
  LCD.print("MANUAL STATE");
  
  digitalWrite(ManualStateLED, HIGH);
  digitalWrite(AutomaticStateLED, LOW);
  ControlAzimuthMotor(STOP);
  
  //POLLING
  while(digitalRead(StatePin) == HIGH)
  {
    ReadCurrentAngle();
    DisplayCurrentAngle();
  }
  //return to automatic mode
  digitalWrite(ManualStateLED, LOW);
  digitalWrite(AutomaticStateLED, HIGH);
  loop();
}

void AutomaticState(void)
{
  LCD.setCursor(0, 0);
  LCD.print("AUTO STATE");

  digitalWrite(AutomaticStateLED, HIGH);
  digitalWrite(ManualStateLED, LOW);

  //polling
  if(isAutomatic() == MANUAL) {
    ManualState();
  }

  ReadCurrentAngle();
  if(dir == "r")
    {
      ControlAzimuthMotor(CW);
     /* Serial.println("Azimuth Motor Forward");
      Serial.print("speed->");
      Serial.println(analogRead(PinC));*/
    }
    else if(dir == "f")
    {
      ControlAzimuthMotor(CCW);
     /* Serial.println("Azimuth Motor Reverse");
      Serial.print("speed->");
      Serial.println(analogRead(PinC));*/
    }
    else 
    {
      ControlAzimuthMotor(STOP);
     /* Serial.println("Azimuth Motor Stop");
      Serial.print("speed->");
      Serial.println(analogRead(PinC));*/
    }
    DisplayCurrentAngle();
}

void ControlAzimuthMotor(tDirection dr)
{  
    if(dr == CW)
    {
      digitalWrite(PinA,HIGH);
      digitalWrite(PinB,LOW);
      analogWrite(PinC,20);
    }
    else if (dr == CCW)
    {
      digitalWrite(PinA,LOW);
      digitalWrite(PinB,HIGH);
      analogWrite(PinC,20);
    }
    else 
    {
      digitalWrite(PinA,LOW);
      digitalWrite(PinB,LOW);
      analogWrite(PinC,0);
    }

}

void ReadCurrentAngle(void)
{
  
  if(Serial.available() > 0 && Serial.read()=='(')
  {
    angle = "";
    dir = "";
    dir = Serial.readStringUntil(',');
    dir.trim();
    Serial.println(dir);
    angle = Serial.readStringUntil(')');
    angle.trim();
    Serial.println(angle);
  }
  
}
void DisplayCurrentAngle(void)
{
  LCD.setCursor(0, 1);
  LCD.print("AZM ANGLE: ");
  if(angle.length()==1)
  {
    LCD.print("00");
    LCD.print(angle);
  }
  else if(angle.length()==2)
  {
    LCD.print("0");
    LCD.print(angle);
  }
  else
  {
    LCD.print(angle);
  }
}
