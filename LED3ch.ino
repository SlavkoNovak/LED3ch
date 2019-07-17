/*
  LED3ch.ino
  LED3ch v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], July 2019
  License: LGPL
*/

#include "config.h"

#define IN_CH1 14    //A0
#define IN_CH2 15    //A1
#define IN_CH3 16    //A2
#define PWM1 3       //D3
#define PWM2 6       //D6
#define PWM3 9       //D9
#define LED 13       //D13

uMODULES(modules, NUM_OF_MODULES)

//Main module instance
LED3ch modLED3ch;
ClockMillis clck;

void setup()
{
  modules.Add(&clck);
  
  //Set inputs
  pinMode(IN_CH1, INPUT);
  pinMode(IN_CH2, INPUT);
  pinMode(IN_CH3, INPUT);

  //Set outputs
  pinMode(LED, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);

  //Initialize input ports
  clck.Delay_ms.Write(10); //Clock at 100 Hz

  modLED3ch.Reset.Write(LOW); //Reset the main module
}

void loop()
{
  //Read inputs
  modLED3ch.InCh1.Write(analogRead(IN_CH1));
  modLED3ch.InCh2.Write(analogRead(IN_CH2));
  modLED3ch.InCh3.Write(analogRead(IN_CH3));
  modLED3ch.Clk.Write(clck.ClckOut.Read());
  
  //Do uHDL engine clocking
  uHDL::uMain(modules);

  //Write outputs
  analogWrite(PWM1, modLED3ch.OutPWM1.Read());
  analogWrite(PWM2, modLED3ch.OutPWM2.Read());
  analogWrite(PWM3, modLED3ch.OutPWM3.Read());
  digitalWrite(LED, modLED3ch.InputExternal.Read());
}

