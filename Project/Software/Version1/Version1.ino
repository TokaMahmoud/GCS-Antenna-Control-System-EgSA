//Azimuth Code
/****** Enums *******/
typedef enum
{
  Manual,
  Automatic
} tSystemState;

typedef enum
{
  Azimuth,
  Elevation
} tAxis;

typedef enum
{
  AzimuthMotor,
  ElevationMotor
} tMotor;

typedef enum
{
  ClockWise,
  CounterClockWise , 
  STOP 
} tDirection;
typedef enum 
{
  LIMIT_SWITCH_UNSAFE = 0 , 
  LIMIT_SWITCH_SAFE 
}tLimitSwitchS ; 
/******* Prototypes ******/
tSystemState isAutomatic(void);
void ManualState(void);
void AutomaticState(void);
tLimitSwitchS GetLimitSwitchesState(void) ; 
tDirection DetermineAzimuthDirection(void);
void StartAzimuthMotor();
float GetCurrentPosition();



/****** Variables *******/
int SystemStateSwitchPin = 13;
int AzimuthAPin = 12;
int AzimuthBPin = 8;
int AzimuthCPin = 11;

int SystemStateSwitchVal = 0;

/****** Data ************/
//int AzimuthAngleValues[]
int SpeedIndex ; 
float MotorRPM[] = {1.36 , 2.76 , 4.15 , 5.58 , 7.00 , 11.21 , 11.97 , 12.63 , 13.33} ; 
float MotorSpeeds[] = {0.5, 1, 1.5, 2, 2.5, 4, 4.25, 4.5, 4.75};
float AzimuthRequiredAngle = 35.0;
float AzimuthCurrentAngle = 0.0;
float AzimuthAbsolute = 0 ; 
float AzimuthDirection;
float AzimuthMotorSpeed;
unsigned int AzimuthRequiredTime;


void setup()
{
  pinMode(SystemStateSwitchPin, INPUT);
  pinMode(AzimuthAPin, OUTPUT);
  pinMode(AzimuthBPin, OUTPUT);
  pinMode(AzimuthCPin, OUTPUT);
}

void loop()
{
  //SystemStateSwitchVal = digitalRead(SystemStateSwitchPin);
  //if (SystemStateSwitchVal == Manual) {
    //ManualState();
  //} else {
    AutomaticState();
  //}
}

void ManualState(void)
{

}

void AutomaticState(void)
{
  AzimuthDirection = DetermineAzimuthDirection();
  AzimuthMotorSpeed = DetermineAzimuthMotorSpeed();
  AzimuthRequiredTime = DetermineAzimuthRequiredTime();

  StartAzimuthMotor();
  //StopAzimuthMotor();

}

float GetCurrentPosition()
{

}

void StartAzimuthMotor()
{
  float AzimutnAngleError = 0.0;

  //Setting Motor Direction
  if (AzimuthDirection == ClockWise)
  {
    digitalWrite(AzimuthAPin, HIGH);
    digitalWrite(AzimuthBPin, LOW);


  }
  else if (AzimuthDirection == CounterClockWise)
  {
    digitalWrite(AzimuthAPin, LOW);
    digitalWrite(AzimuthBPin, HIGH);
  }

  //Setting Motor Speed
  analogWrite(AzimuthCPin,(int) ((AzimuthMotorSpeed / 5) * 255));
  while(AzimuthRequiredTime != 0  )
  {
   delayMicroseconds(1) ;  
   AzimuthRequiredTime -- ; 
  }
 //delay(10000); 
  analogWrite(AzimuthCPin,(int) 0);
  AzimuthCurrentAngle = AzimuthRequiredAngle ;  
  
  
  /*do{
    AzimutnAngleError = AzimuthRequiredAngle - AzimuthCurrentAngle;
    AzimuthCurrentAngle++;
    delayMicroseconds((1 / 8.16) * pow(10, 6));
  }while (AzimutnAngleError != 0.0);*/
  
}

void StopAzimuthMotor(void)
{
    digitalWrite(AzimuthAPin, LOW);
    digitalWrite(AzimuthBPin, LOW);
}


// return microsecond 
unsigned int DetermineAzimuthRequiredTime(void)
{
  float anlgePerSecond = CalcDegrPerSecond() ; 
  return (AzimuthAbsolute / anlgePerSecond ) * pow(10, 6);
}

float DetermineAzimuthMotorSpeed(void)
{
  float stp = 360.0 / 9 , crntAngle = 0  ; 
  SpeedIndex =  abs(AzimuthAbsolute) / stp ;
  return MotorSpeeds[SpeedIndex];
}


tDirection DetermineAzimuthDirection(void)
{
  /*
  float AngleDifference = AzimuthRequiredAngle - AzimuthCurrentAngle;
  if (AngleDifference > 0) {
    if (AngleDifference > 180) {
      AzimuthRequiredAngle = 360 - AzimuthRequiredAngle;
      return ClockWise;
    } else {
      return CounterClockWise;
    }
  } else {
    if (abs(AngleDifference) > 180) {
      AzimuthRequiredAngle = 360 - abs(AzimuthRequiredAngle);
      return CounterClockWise;
    } else {
      return ClockWise;
    }
  }
  */
  AzimuthAbsolute = AzimuthRequiredAngle - AzimuthCurrentAngle; 
  if(AzimuthAbsolute > 0 ) 
  {
    return CounterClockWise ;  
  }else if (AzimuthAbsolute < 0)
  {
    return ClockWise; 
  }else 
  {
    return STOP ; 
  }
}
  float CalcDegrPerSecond(void)
  {
    float ret = 0 ; 
    ret =  (MotorRPM[SpeedIndex] * 360) / 60 ;
    return ret ;  
  }
  tLimitSwitchS GetLimitSwitchesState(void) 
  {
    /* Need to be modified when limit swithces connected */ 
    return LIMIT_SWITCH_SAFE ;  
  }
