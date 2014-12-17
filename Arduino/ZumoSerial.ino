#include <ZumoMotors.h>
char inData[10]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

void setup()
{
  Serial.begin(9600);
  Serial.println("ZumoSerialV1.00");
}

void loop()
{
  if (Comp("100:100")==0) {
  }
  if (Comp("000:000")==0) {
  }  
}

int convertToMotorSpeed(int power)
{
  // Validate
  if(power > 100)
  {
    power = 100;
  }
  if(power < -100)
  {
    power =-100;
  }

  int pwm_min = 20;
  int pwm_max = 400;

  int abs_power = abs(power);
  float result =pwm_min +( (pwm_max - pwm_min)/100* abs_power);

  if(power < 0)
  {
    result = - result;
  }

  return round(result);
}

void setMotorSpeed(int leftPower, int rightPower)
{
  ZumoMotors::setSpeeds(convertToMotorSpeed(leftPower),convertToMotorSpeed(rightPower));
}

int Comp(char* This) {
  readCommand();  
  if(index == 9)
  {
    SetMotorPowerFormCommand();
    index=0;
  }

  if (strcmp(inData,This)  == 0) {
    for (int i=0;i<9;i++) {
      inData[i]=0;
    }
    index=0;
    return(0);
  }
  else {
    return(1);
  }
}

void readCommand()
{
  while (Serial.available() > 0) // Don't read unless
    // there you know there is data
  {
    if(index < 9) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
      inData[index] = '\0'; // Null terminate the string
    }
  }
}

void SetMotorPowerFormCommand()
{
  if(inData[4] != ':' || inData[9] != '\0')
  {
    setMotorSpeed(0,0);
    Serial.print("ErrorCommand:");
    Serial.println(inData);
    return;
  }
  
  char l[5];
  l[0] = inData[0];
  l[1] = inData[1];
  l[2] = inData[2];
  l[3] = inData[3];
  l[4] = '\0';
  
  char r[5];
  r[0] = inData[5];
  r[1] = inData[6];
  r[2] = inData[7];
  r[3] = inData[8];
  r[4] = '\0';
  
  String left(l);
  String right(r);
  
  
  setMotorSpeed(left.toInt(),right.toInt());
  Serial.println(left + ':' + right);
  
}



