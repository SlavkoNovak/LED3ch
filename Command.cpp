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
	namespace GDC
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
				char opr[10]; //operator
				char arg[6]; //operend object
				
				memset(opr, '\0', 10);
				memset(arg, '\0', 6);

				char *pcmd = this->cmd;
				
				//opr
				for(int i = 0; i < 10; i++)
				{
					if(9 == i) { this->error = OPR_TO_BIG; break; }
					if(' ' != *pcmd && '\0' != *pcmd) opr[i] = *pcmd++; else break;
				}
				pcmd++;
				if(0 == this->error) this->setOperation(opr);
				
				//arg
				if(0 == this->error)
				{
					for(int i = 0; i < 6; i++)
					{
						if(5 == i) { this->error = ARG_TO_BIG; break; }
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
				else if(!strcmp(oprUpper, "CH2"))
				{
					this->operation = OPR_CH2;
				}
				else if(!strcmp(oprUpper, "CH3"))
				{
					this->operation = OPR_CH3;
				}
        else if(!strcmp(oprUpper, "OFF"))
        {
          this->operation = OPR_OFF;
        }
        else if(!strcmp(oprUpper, "ON"))
        {
          this->operation = OPR_ON;
        }
   			else if(strstr(oprUpper, "INPUT"))
  			{
  				this->operation = OPR_INPUT;
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

