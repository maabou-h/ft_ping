#include "ft_ping.h"

void		pinger(int sig)
{
	if (sig == 42)
		printf("ft_ping %s (%s): %d(%d) bytes of data.\n", g_data.dest,\
				g_data.ip, DATALEN + ICMPHDRLEN - ICMP_MINLEN, PKTLEN);
	pack();
	if (sendto(g_data.sockfd, g_data.packet, DATALEN + ICMPHDRLEN\
				+ IPHDRLEN, 0, g_data.info->ai_addr,\
				g_data.info->ai_addrlen) < 0)
	{
		printf("send failed\n");
		close(g_data.sockfd);
		exit(1);
	}
	else
		g_data.stat.nsend++;
	alarm(g_data.opt.interval);
}

void		listener(void)
{
	ssize_t	responsesize;

	responsesize = 0;
	while (1)
	{
		if ((responsesize = unpack()) == -1)
			printf("recv failed\n");
		g_data.stat.tsout = gettimestamp_ms(1);
		chkpkt(responsesize);
	}
}

int			main(int argc, char **argv)
{
	int		v;

	bzero(&g_data, sizeof(g_data));
	g_data.pid = getpid();
	signal(SIGINT, statistics);
	signal(SIGALRM, pinger);
	if ((v = options(argc, argv + 1)) < 1)
	{
		if (v == -1)
			printf("usage\n");
		else
			printf("arg eror\n");
		return (1);
	}
	initprog();
	pinger(42);
	listener();
	return (0);
}
