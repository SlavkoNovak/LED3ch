/*
  Command.h
  LED3ch v0.1 by Slavko Novak [slavko.novak.esen@gmail.com], July 2019
  License: LGPL
*/

#ifndef COMMAND_H
#define COMMAND_H

//Errors
#define OPR_TO_BIG 200
#define ARG_TO_BIG 201
#define INVALID_OPERATION 300

//Operations
#define OPR_INVALID -1
#define OPR_CH1 0
#define OPR_CH2 1
#define OPR_CH3 2
#define OPR_OFF 3
#define OPR_ON 4
#define OPR_INPUT 5


namespace eSeN
{
	namespace GDC
	{
		namespace Util
		{
			class Command
			{
			private:
				int error;
				int operation;
        unsigned int argument;
				char cmd[20];
				
				void setOperation(const char *);
				void setArgument(const char *);
				
			public:
				Command(const char *);
				virtual ~Command();
				void Execute(int &, int &);
			};

		}
	}

}

#endif // COMMAND_H
