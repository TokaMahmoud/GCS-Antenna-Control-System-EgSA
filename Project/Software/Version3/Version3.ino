/****** Enums *******/
typedef enum
{
  Manual,
  Automatic
} tSystemState;

typedef enum
{
  ClockWise,
  CounterClockWise ,
  STOP
} tDirection;


/******* Prototypes ******/
tSystemState isAutomatic(void);
void ManualState(void);
void AutomaticState(void);

void ControlAzimuthMotor(tDirection dr);
void ControlElevationMotor(tDirection dr);

/****** Variables *******/
//StatePin
int StatePin = 2;
int ManualStateLED = A4; 
int AutomaticStateLED = A5; 

int AzimuthAPin = 7;
int AzimuthBPin = 8;
int AzimuthCPin = 9;

int ElevationAPin = 3;
int ElevationBPin = 4;
int ElevationCPin = 6;

char Buffer;

void setup() {
  //State LED 
  pinMode(ManualStateLED, OUTPUT); 
  pinMode(AutomaticStateLED, OUTPUT);
  pinMode(StatePin, INPUT);
  
    //Azimuth PinMode
  pinMode(AzimuthAPin, OUTPUT);
  pinMode(AzimuthBPin, OUTPUT);
  pinMode(AzimuthCPin, OUTPUT);

  //Elevation PinMode
  pinMode(ElevationAPin, OUTPUT);
  pinMode(ElevationBPin, OUTPUT);
  pinMode(ElevationCPin, OUTPUT);
  
  Serial.begin(9600);

}

void loop() {
  if(isAutomatic() == Automatic)
  {
    AutomaticState();
  }
  else if(isAutomatic() == Manual) 
  {
    ManualState();
  } 
}

tSystemState isAutomatic(void) {
  if(digitalRead(StatePin) == HIGH) {
    return Manual;
  } else if (digitalRead(StatePin) == LOW){
    return Automatic;
  }
}

void ManualState(void)
{
  //manual switch
  digitalWrite(ManualStateLED, HIGH); 
  digitalWrite(AutomaticStateLED,LOW);
  ControlAzimuthMotor(STOP);
  ControlElevationMotor(STOP);
  //while switch in manual mode 
  while(digitalRead(StatePin) == HIGH);
  //return to automatic mode
  digitalWrite(ManualStateLED, LOW); 
  digitalWrite(AutomaticStateLED,HIGH);
  loop();
}

void AutomaticState(void)
{
   //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }
  
  if(Serial.available() != 0){
   // strcpy(Buffer,Serial.read());
   Buffer = Serial.read();
 Serial.print(Buffer);
    if(Buffer == 'r')
    {
      ControlAzimuthMotor(ClockWise);
      Serial.println("Azimuth Motor Forward");
      Serial.print("speed->");
      Serial.println(analogRead(AzimuthCPin));
    }
    else if(Buffer == 'f')
    {
      ControlAzimuthMotor(CounterClockWise);
      Serial.println("Azimuth Motor Reverse");
      Serial.print("speed->");
      Serial.println(analogRead(AzimuthCPin));
    }
    else 
    {
      ControlAzimuthMotor(STOP);
      Serial.println("Azimuth Motor Stop");
      Serial.print("speed->");
      Serial.println(analogRead(AzimuthCPin));
    }
  

    if(Buffer == "F")
    {
      ControlElevationMotor(ClockWise);
      Serial.println("Elevation Motor Forward");
      Serial.print("speed->");
      Serial.println(analogRead(ElevationCPin));
    }
    else if(Buffer == "R")
    {
      ControlElevationMotor(CounterClockWise);
      Serial.println("Elevation Motor Reverse");
      Serial.print("speed->");
      Serial.println(analogRead(ElevationCPin));
    }
    else if(Buffer == "S")
    {
      ControlElevationMotor(STOP);
      Serial.println("Elevation Motor Stop");
      Serial.print("speed->");
      Serial.println(analogRead(ElevationCPin));
    }
  }
 //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }

}


void ControlAzimuthMotor(tDirection dr)
{
   //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }
  
    if(dr == ClockWise)
    {
      digitalWrite(AzimuthAPin,HIGH);
      digitalWrite(AzimuthBPin,LOW);
      analogWrite(AzimuthCPin,20);
    }
    else if (dr == CounterClockWise)
    {
      digitalWrite(AzimuthAPin,LOW);
      digitalWrite(AzimuthBPin,HIGH);
      analogWrite(AzimuthCPin,20);
    }
    else 
    {
      digitalWrite(AzimuthAPin,LOW);
      digitalWrite(AzimuthBPin,LOW);
      analogWrite(AzimuthCPin,0);
    }
 //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }
}

void ControlElevationMotor(tDirection dr)
{
   //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }
  
  if(dr == ClockWise)
    {
      digitalWrite(ElevationAPin,HIGH);
      digitalWrite(ElevationBPin,LOW);
      analogWrite(ElevationCPin,50);
    }
    else if (dr == CounterClockWise)
    {
      digitalWrite(ElevationAPin,LOW);
      digitalWrite(ElevationBPin,HIGH);
      analogWrite(ElevationCPin,50);
    }
    else if (dr == STOP)
    {
      digitalWrite(ElevationAPin,LOW);
      digitalWrite(ElevationBPin,LOW);
      analogWrite(ElevationCPin,0);
    }
     //polling
  if(isAutomatic() == Manual) {
    ManualState();
  }
}
