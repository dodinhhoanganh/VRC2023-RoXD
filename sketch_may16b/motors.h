#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PWM 0
#define MAX_PWM 4095

// PWM channels of pca9685 0-16
#define PWM_CHANNEL1 8
#define PWM_CHANNEL2 9
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11
#define PWM_CHANNEL6 13
#define PWM_CHANNEL8 15

#define PWM_SCHANNEL1 2
#define PWM_SCHANNEL2 3
#define PWM_SCHANNEL3 4
#define PWM_SCHANNEL4 5
#define PWM_SCHANNEL5 6
#define PWM_SCHANNEL6 7

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setPWMMotors(unsigned int c1,unsigned int c2, unsigned int c3, unsigned int c4, unsigned int c5, unsigned int c6, unsigned int c7)
{
  pwm.setPWM(PWM_CHANNEL1, 0, c1);
  pwm.setPWM(PWM_CHANNEL2, 0, c2);
  pwm.setPWM(PWM_CHANNEL3, 0, c3);
  pwm.setPWM(PWM_CHANNEL4, 0, c4);
  pwm.setPWM(12, 0, c7);
  pwm.setPWM(14, 0, 0);  
  pwm.setPWM(PWM_CHANNEL6, 0, c5);
  pwm.setPWM(PWM_CHANNEL8, 0, c6);
}
void setPWMServos(unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4, unsigned int c5, unsigned int c6)
{
  pwm.setPWM(PWM_SCHANNEL1, 0, c1);
  pwm.setPWM(PWM_SCHANNEL2, 0, c2);
  pwm.setPWM(PWM_SCHANNEL3, 0, c3);
  pwm.setPWM(PWM_SCHANNEL4, 0, c4);
  pwm.setPWM(PWM_SCHANNEL5, 0, c5);
  pwm.setPWM(PWM_SCHANNEL6, 0, c6);
}

void initMotors()
{
  Wire.begin(); // SDA, SCL,400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(60);
  Wire.setClock(400000);
  setPWMMotors(0, 0, 0, 0, 0, 0, 0); 
  setPWMServos(0, 0, 0, 0, 0, 0);
}
