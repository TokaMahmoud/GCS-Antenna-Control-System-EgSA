#include <LiquidCrystal.h>

#define ANGLE_NUM 50

//Azimuth and Elevation angle array
long AngleArray[ANGLE_NUM] = {0};

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
tDirection DetermineDirection(void);
float DetermineMotorSpeed(void);
float CalculateDegreePerSecond(void);
float DetermineRequiredTime(void);

void StartMotor();
void StopMotor(void);

/****** Variables *******/
//StatePin
int StatePin = 2;
int ManualStateLED = A4;
int AutomaticStateLED = A5;

int PinA = 3;
int PinB = 4;
int PinC = 5;

const int rs = 7, en = 6, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);

/****** Data ************/
//int AzimuthAngleValues[]
int SpeedIndex;
float MotorRPM[] = {1.36 , 2.76 , 4.15 , 5.58 , 7.00 , 11.21 , 11.94 , 12.63 , 13.33} ;
float MotorSpeeds[] = {0.5, 1.0, 1.5, 2.0, 2.5, 4.0, 4.25, 4.5, 4.75};

long RequiredAngle = 0;
long CurrentAngle = 0;
long AngleDifference = 0;
tDirection Direction;
float MotorSpeed = 0.0;
long RequiredTime = 0;

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

  LCD.begin(16, 2);

  //ARRAY DATA FILL
  for (int i = 0; i < ANGLE_NUM; i++) {
    AngleArray[i] = random(350);
  }
}


void loop() {

  if (isAutomatic() == AUTOMATIC)
  {
    digitalWrite(AutomaticStateLED, HIGH);
    digitalWrite(ManualStateLED, LOW);
    for (int i = 0; i < ANGLE_NUM; i++)
    {
      RequiredAngle = AngleArray[i];
      AutomaticState();

      if (isAutomatic() == MANUAL)
      {
        break;
      }
      delay(1000);
    }
  }
  else if (isAutomatic() == MANUAL)
  {
    ManualState();
  }
}

void switchState() {
  digitalWrite(ManualStateLED, HIGH);
  digitalWrite(AutomaticStateLED, LOW);
  ManualState();
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
  StopMotor();

  //POLLING
  while (digitalRead(StatePin) == HIGH);
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

  Direction = DetermineDirection();
  MotorSpeed = DetermineMotorSpeed();
  RequiredTime = DetermineRequiredTime();

  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  StartMotor();
  StopMotor();
}


//DIRECTION
tDirection DetermineDirection(void)
{
  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  AngleDifference = RequiredAngle - CurrentAngle;

  if (AngleDifference > 0) {
    return CCW;
  } else if (AngleDifference < 0) {
    return CW;
  } else {
    return STOP;
  }
}

float DetermineMotorSpeed(void)
{
  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  float sectorSpeedStep = 360.0 / 9.0;
  SpeedIndex =  abs(AngleDifference) / sectorSpeedStep;
  return MotorSpeeds[SpeedIndex];
}

// return Millisecond
float DetermineRequiredTime(void)
{
  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  float anlgePerSecond = CalculateDegreePerSecond() ;
  return (((abs(AngleDifference) / anlgePerSecond )) * pow(10, 3));
}

float CalculateDegreePerSecond(void)
{
  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  float ret = 0;
  ret =  (MotorRPM[SpeedIndex] * 360.0) / 60.0;
  return ret;
}


void StartMotor()
{
  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }

  //Setting Motor Direction
  if (Direction == CW)
  {
    digitalWrite(PinA, HIGH);
    digitalWrite(PinB, LOW);
  }
  else if (Direction == CCW)
  {
    digitalWrite(PinA, LOW);
    digitalWrite(PinB, HIGH);
  }

  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }
  //Setting Motor Speed
  while (RequiredTime > 0 ) {
    analogWrite(PinC, (int) ((MotorSpeed / 5) * 255)); //generate a pulse with different width
    delayMicroseconds(2000); //delay the time for one pulse (2 ms)
    RequiredTime -= 2; //decrement the required time by 2 ms
    //polling
    if (isAutomatic() == MANUAL) {
      ManualState();
    }
  }

  //polling
  if (isAutomatic() == MANUAL) {
    ManualState();
  }
  analogWrite(PinC, 0);
  CurrentAngle = RequiredAngle ;
}

void StopMotor(void)
{
  digitalWrite(PinA, LOW);
  digitalWrite(PinB, LOW);
  analogWrite(PinC, 0);
}
