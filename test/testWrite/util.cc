#include "util.h"





// -----------------------------
// Global parameters
// -----------------------------
static const char *enableFlags = NULL;	// controls which LOGGER messages are printed





// -----------------------------
// Local functions
// -----------------------------
void LOGGER_Init (const char *flagList)
{
	enableFlags = flagList;
}


int LOGGER_IsEnabled (char flag)
{
	if (enableFlags == NULL)
		return 0;

	return	(0 != strchr(enableFlags, flag)) ||
			(0 != strchr(enableFlags, LOGGER_FLAG_ALL));
}


void LOGGER(char flag, const char *format, ...)
{
	if ((flag != LOGGER_FLAG_ALL) && (!LOGGER_IsEnabled (flag)))
		return;

	va_list ap;

	va_start (ap, format);
	vfprintf (stdout, format, ap);
	va_end (ap);
	fflush (stdout);
}


void LOGGER(const char *format, ...)
{
	va_list ap;

	va_start (ap, format);
	vfprintf (stdout, format, ap);
	va_end (ap);
	fflush (stdout);
}


void LOGGER_parameters(char loggerFlag, char * patternName, uint nbIteration, float computeTime, uint nbOutputChar, uint nbIoDevice, uint nbProc)
{
	LOGGER(loggerFlag, "\n\n\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	LOGGER(loggerFlag, "Pattern name                    : %s\n",			patternName);
	LOGGER(loggerFlag, "Nb iteration                    : %ld\n",			nbIteration);
	LOGGER(loggerFlag, "Computation time (per iteration): %f (seconds)\n",	computeTime);
	LOGGER(loggerFlag, "Nb output char                  : %ld\n",			nbOutputChar);
	LOGGER(loggerFlag, "Nb IO devices                   : %ld\n",			nbIoDevice);
	LOGGER(loggerFlag, "Nb processors                   : %ld\n",			nbProc);
	LOGGER(loggerFlag, "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}


void LOGGER_error(const char *msg, const char *param, char exit_b)
{
	fprintf(stderr, "\n\n**********************************\n");
	fprintf(stderr, "%s: %s\n", msg, param);
	fprintf(stderr, "**********************************\n");
	if (exit_b)
		exit(0);
}


void LOGGER_error(const char *msg, float param, char exit_b)
{
	char paramStr[128];
	sprintf(paramStr, "%f", param);
	LOGGER_error(msg, paramStr, exit_b);
}


unsigned long long get_cycle_count(void)
{
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


void getTime(char *output)
{
	std::time_t time = std::time(nullptr);
	sprintf(output, "%s", std::asctime(std::localtime(&time)));
}
