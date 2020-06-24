#include "ft_ping.h"

long gettimestamp_ms(int flag)
{
    struct timeval te;
	long milliseconds;
	long mm;

    gettimeofday(&te, NULL);
    milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
	mm = te.tv_sec*10LL + te.tv_usec/10;
	if (flag > -1)
	{
	if (mm % 100 > 9)
		g_data->stat.comma[flag] = mm % 100;
	if (mm % 10 > 0)
		g_data->stat.comma[flag] = mm % 10;
	}
    return milliseconds;
}