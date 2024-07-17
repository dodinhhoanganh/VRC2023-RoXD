#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class object

#define PS2_DAT 12 // MISO  19
#define PS2_CMD 13 // MOSI  23
#define PS2_SEL 15 // SS     5
#define PS2_CLK 14 // SLK   18

#define TOP_SPEED 4095
#define NORM_SPEED 2048
#define BRAKE 0
#define TURNING_FACTOR 1

bool isBrushOn = 0, isShootOn = 0, reverseBrush = 0, isServo1Reversed = 0, isServo2Reversed = 0;
unsigned long previousMillisGate;
unsigned long previousMillisServo1, previousMillisServo2;
unsigned long intervalServoGate = 1500;
unsigned long intervalServo = 50;
bool isWaitForGate = 0, isWaitForServo1 = 0, isWaitForServo2 = 0, isGateOpen = 0;
int cs1 = 0, cs2 = 0, cs3 = 600, cs4 = 150;
void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }
}
bool PS2control()
{
  //Motors
  int speed = BRAKE;
  int nJoyX = 128 - ps2x.Analog(PSS_LY); // read x-joystick
  int nJoyY = 128 - ps2x.Analog(PSS_LX); // read y-joystick
  int nMotMixL;                          // Motor (left) mixed output
  int nMotMixR;                          // Motor (right) mixed output
  if(nJoyX == -1 && nJoyY == 0) // in case of lost connection with the wireless controller, only used in VRC2023 PS2 wireless controller 
  {
    setPWMMotors(0, 0, 0, 0, 0, 0, 0);
    int cs1, cs2;
    unsigned long currentMillis = millis();
    unsigned long previousMillisGate;
    if (!isWaitForGate && isGateOpen)
    {
      cs1=600;
      cs2=150;
      previousMillisGate = currentMillis;
      isWaitForGate = 1;
      isGateOpen = 0;
      isBrushOn = isShootOn = reverseBrush = isServo1Reversed = isServo2Reversed = 0;
    }
    if (currentMillis - previousMillisGate >= intervalServoGate)
    {
      cs1 = cs2 = 0;
      isWaitForGate = 0;
    }
    cs1 = 0, cs2 = 0, cs3 = 600, cs4 = 150;
    setPWMServos(cs1, cs2, cs3, cs4, 0, 0);
    return 0;
  }
  int brushValue = 0;
  if (nJoyX != 0 || (nJoyY != 1 && nJoyY != 0)) 
  {
    speed=NORM_SPEED;
    if (ps2x.Button(PSB_R2))
      speed = TOP_SPEED;
    if (ps2x.Button(PSB_L2))
      speed = BRAKE;
  }
  int c5 = 0,c6 = 0;
  if (ps2x.ButtonPressed(PSB_CROSS))
    isBrushOn = !isBrushOn;
  if (ps2x.ButtonPressed(PSB_TRIANGLE))
    reverseBrush = !reverseBrush;
  if (ps2x.ButtonPressed(PSB_CIRCLE))
    isShootOn = !isShootOn;
  c5 = isBrushOn ? (!reverseBrush ? 0 : 3072) : 0;
  int c7 = isBrushOn ? (reverseBrush ? 0 : 3072) : 0;
  c6 = isShootOn ? 2457 : 0;

  bool temp = (nJoyY * nJoyX > 0);
  if (nJoyX) // Turning
  {
    nMotMixL = -nJoyX + (nJoyY * temp);
    nMotMixR = nJoyX + (nJoyY * !temp);
  }
  else // Forward or Reverse
  {
    nMotMixL = nJoyY;
    nMotMixR = nJoyY;
  }

  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (nMotMixR > 0)
  {
    c3 = nMotMixR;
    c3 = map(c3, 0, 128, 0, speed);
  }

  else if (nMotMixR < 0)
  {
    c4 = abs(nMotMixR) + 1;
    c4 = map(c4, 0, 128, 0, speed);
  }

  if (nMotMixL > 0)
  {
    c1 = nMotMixL;
    c1 = map(c1, 0, 128, 0, speed);
  }
  else if (nMotMixL < 0)
  {
    c2 = abs(nMotMixL)+1;
    c2 = map(c2, 0, 128, 0, speed);
  }
  setPWMMotors(c1, c2, c3, c4, c5, c6, c7);
    
  //Servos
  unsigned long currentMillis = millis();
  if (ps2x.ButtonPressed(PSB_L1) && !isWaitForServo1)
  {
    previousMillisServo1=millis();
    isServo1Reversed = !isServo1Reversed;
  }
  if (ps2x.ButtonPressed(PSB_R1) && !isWaitForServo2)
  {
    previousMillisServo2=millis();
    isServo2Reversed = !isServo2Reversed;
  }
  if (isServo1Reversed)
  {
    isWaitForServo1 = 1;
    unsigned long progress = millis() - previousMillisServo1;
    if (progress <= intervalServo) 
      cs3 = map(progress, 0, intervalServo, 275, 600);
    else isWaitForServo1 = 0;
  }
  else {
    isWaitForServo1 = 1;
    unsigned long progress = millis() - previousMillisServo1;
    if (progress <= intervalServo) 
      cs3 = map(progress, 0, intervalServo, 600, 275);
    isWaitForServo1 = 0;
  }
  if (!isServo2Reversed)
  {
    isWaitForServo2 = 1;
    unsigned long progress = millis() - previousMillisServo2;
    if (progress <= intervalServo) 
      cs4 = map(progress, 0, intervalServo, 475, 150);
    isWaitForServo2 = 0;
  }
  else {
    isWaitForServo2 = 1;
    unsigned long progress = millis() - previousMillisServo2;
    if (progress <= intervalServo) 
      cs4 = map(progress, 0, intervalServo, 150, 475);
    isWaitForServo2 = 0;
  }

  if (ps2x.ButtonPressed(PSB_PAD_DOWN))
  {
    if (!isWaitForGate && !isGateOpen)
    {
      cs1=150;
      cs2=600;
      previousMillisGate = currentMillis;
      isWaitForGate = 1;
      isGateOpen = 1;
    }
  }
  else if (ps2x.ButtonPressed(PSB_PAD_UP))
  {
    if (!isWaitForGate && isGateOpen)
    {
      cs1=600;
      cs2=150;
      previousMillisGate = currentMillis;
      isWaitForGate = 1;
      isGateOpen = 0;
    }
  }
  if (currentMillis - previousMillisGate >= intervalServoGate)
  {
    cs1 = cs2 = 0;
    isWaitForGate = 0;
  }
  setPWMServos(cs1,cs2,cs3,cs4,0,0);
  return 1;
}
