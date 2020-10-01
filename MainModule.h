/*
  MainModule.h
  LED3ch v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], July 2019
  License: LGPL
*/

#ifndef _MAIN_MODULE_H
#define _MAIN_MODULE_H

#include <EEPROM.h>
#include "config.h"
	
using namespace uHDL;
using namespace eSeN::LED3ch::Util;

//Main module -->
uMODULE(LED3ch)
{
  //Inputs
  u_input<bool> Reset;
  u_input<uint16_t> InCh1;
  u_input<uint16_t> InCh2;
  u_input<uint16_t> InCh3;
  u_input<bool> Clk;
  
  //Outputs
  u_output<uint8_t> OutPWM1;
  u_output<uint8_t> OutPWM2;
  u_output<uint8_t> OutPWM3;
  u_output<bool> InputExternal;
  
  //Signals

  //Internal registers
  uint16_t Threshold;
  
  //Submodules
  
  //Init method
  uINIT(LED3ch)
  {
    ResetMethod();
    modules.Add(this);
  }
  
  //Module method
  void LED3chMethod()
  {
    if(Reset.NegEdge())
    {
      ResetMethod();
      Serial.begin(9600);
      Serial.setTimeout(9);
    }
    else Reset.Write(HIGH); //Pullup

    if(Serial.available())
    {
      String str = Serial.readStringUntil('\n');

      if(0 < str.length())
      {
        Command cmd(str.c_str());

        int op;
        int arg;
        cmd.Execute(op, arg);
        
        switch(op)
        {
          case OPR_CH1:
            InputExternal.Write(false);
            OutPWM1.Write(arg);
            break;

          case OPR_CH2:
            InputExternal.Write(false);
            OutPWM2.Write(arg);
            break;
            
          case OPR_CH3:
            InputExternal.Write(false);
            OutPWM3.Write(arg);
            break;
         
         case OPR_OFF:
            InputExternal.Write(false);
            OutPWM1.Write(0);
            OutPWM2.Write(0);
            OutPWM3.Write(0);
            break;

         case OPR_ON:
            InputExternal.Write(false);
            OutPWM1.Write(arg);
            OutPWM2.Write(arg);
            OutPWM3.Write(arg);
            break;
            
          case OPR_INPUT:
            InputExternal.Write(true);
            break;

          case OPR_THRESHOLD:
            Threshold = arg;
            EEPROM.put(0, Threshold);
            break;
            
          default:
            return;
        }

        if(-1 != op)
        {
          Serial.println("OK");
        }
      }
    }

    if(InputExternal.Read())
    {
      OutPWM1.Write(In2PWMMap(InCh1.Read()));
      OutPWM2.Write(In2PWMMap(InCh2.Read()));
      OutPWM3.Write(In2PWMMap(InCh3.Read()));
    }
  }

  void ResetMethod()
  {
    Clk.Write(LOW);
    Reset.Write(HIGH);

    InCh1.Write(0);
    InCh2.Write(0);
    InCh3.Write(0);

    OutPWM1.Write(0);
    OutPWM2.Write(0);
    OutPWM3.Write(0);
    InputExternal.Write(true);
    EEPROM.get(0, Threshold);
    Threshold = Threshold == 0 ? 1023 : Threshold;
  }

  int In2PWMMap(uint16_t inVal)
  {
      inVal = inVal > Threshold ? 1023 : inVal; //calibration line
      uint16_t retVal = (float)inVal / 1023.0 * 255;
      retVal = retVal > 255 ? 255 : retVal;
      return 255 - retVal;
  }
  
  //Sensitivity and always method
  uALWAYS(Clk.PosEdge(), LED3chMethod();)
};
//<-- Main module

#endif
