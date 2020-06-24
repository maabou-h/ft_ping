#include "ft_ping.h"

int					main(int argc, char **argv)
{
	t_data data;
	g_data = &data;

	signal(SIGINT, statistics);
	signal(SIGALRM, sendecho);
	if (!options(argc, argv + 1))
	{
			printf("ft_ping: usage: ./ft_ping [hostname|ip]\n");
			exit(1);
	}
	initprog();
	sendecho(0);
	printf("ft_ping: %s (%s): %d(%d) bytes of data.\n", argv[argc - 1], g_data->sender.ip, DATALEN, PKTLEN);
	receivereply();
	return (0);
}
