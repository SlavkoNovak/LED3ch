/*
  Command.cpp
  LED3ch v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], July 2019
  License: LGPL
*/

#include "Command.h"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

namespace eSeN
{
	namespace LED3ch
	{
		namespace Util
		{
			Command::Command(const char *commandStr) : error(0), operation(0), argument(0)
			{
				memset(this->cmd, 0, 20);
				strncpy(this->cmd, commandStr, 19);				
			}

			Command::~Command()
			{
			}
			
			void Command::Execute(int &OP, int &argument)
			{
				char opr[11]; //operator
				char arg[7]; //operend object
				
				memset(opr, '\0', 11);
				memset(arg, '\0', 7);

				char *pcmd = this->cmd;
				
				//opr
				for(int i = 0; i < 11; i++)
				{
					if(10 == i) { this->error = OPR_TO_BIG; break; }
					if(' ' != *pcmd && '\0' != *pcmd) opr[i] = *pcmd++; else break;
				}
				pcmd++;
				if(0 == this->error) this->setOperation(opr);
				
				//arg
				if(0 == this->error)
				{
					for(int i = 0; i < 7; i++)
					{
						if(6 == i) { this->error = ARG_TO_BIG; break; }
						if(' ' != *pcmd && '\0' != *pcmd) arg[i] = *pcmd++; else break;
					}
					pcmd++;
				}
				if(0 == this->error) this->setArgument(arg);
								
				if(0 == error)
				{
        	OP = this->operation;
        	argument = this->argument;
				}
				else
				{
        	char tmp[10];
					Serial.print("ERROR: ");
					Serial.println(itoa(this->error, tmp, 10));
        	OP = -1;
        	argument = 0;
				}
			}
			
			void Command::setOperation(const char *opr)
			{
				char oprUpper[10];
				char *pOprUpper = oprUpper;

				while(*opr) *pOprUpper++ = toupper(*opr++);
				*pOprUpper = '\0';
				
				if(strstr(oprUpper, "CH1"))
				{
					this->operation = OPR_CH1;
				}
				else if(strstr(oprUpper, "CH2"))
				{
					this->operation = OPR_CH2;
				}
				else if(strstr(oprUpper, "CH3"))
				{
					this->operation = OPR_CH3;
				}
        else if(strstr(oprUpper, "OFF"))
        {
          this->operation = OPR_OFF;
        }
        else if(strstr(oprUpper, "ON"))
        {
          this->operation = OPR_ON;
        }
   			else if(strstr(oprUpper, "INPUT"))
  			{
  				this->operation = OPR_INPUT;
  			}
        else if(strstr(oprUpper, "THRESHOLD"))
        {
          this->operation = OPR_THRESHOLD;
        }
        else
				{
					this->operation = OPR_INVALID;
					this->error = INVALID_OPERATION;
				}
			}
			
			void Command::setArgument(const char *arg)
			{
				this->argument = atoi(arg);
			}
		}
	}
}

