#include "ft_ping.h"

void				statistics(int sig)
{
	printf("\n--- %s ft_ping statistics ---\n", g_data->stat.name);
	printf("%d packets transmitted, %d received , %%%d packet loss, time %ldms\n", g_data->stat.nsend, g_data->stat.nreceived, 0, gettimestamp_ms(-1) - g_data->stat.starttime);
	close(g_data->sockfd);
	exit(1);
}