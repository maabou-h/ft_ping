#include "ft_ping.h"

int			main(int argc, char **argv)
{
	signal(SIGINT, statistics);
	signal(SIGALRM, pinger);

	g_data.pid = getpid();

	if (!options(argc, argv + 1))
	{
			printf("ft_ping: usage: ./ft_ping [hostname|ip]\n");
			exit(1);
	}

	initprog();
	
	pinger(0);
	printf("ft_ping: %s (%s): %d(%d) bytes of data.\n", argv[argc - 1], g_data.ip, DATALEN, PKTLEN);
	listener();
	return (0);
}
