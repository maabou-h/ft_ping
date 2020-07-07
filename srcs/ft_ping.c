#include "ft_ping.h"

void pinger(int sig)
{
	sig = 1;
	genhdr((struct ip*)(g_data.packet), (struct icmp*)(g_data.packet + IPHDRLEN));
    if (sendto(g_data.sockfd, g_data.packet, ICMPHDRLEN + DATALEN, 0, g_data.info->ai_addr, g_data.info->ai_addrlen) < 0)
    {  
		printf("send failed\n");
            close(g_data.sockfd);
            exit(1);
    }
	else
		g_data.stat.nsend++;
	alarm(g_data.opt.interval);
}

void listener()
{
    struct msghdr       msg_h;
	ssize_t				responsesize;

	while (42)
	{
		msg_h = genresphdr();
		if ((responsesize = recvmsg(g_data.sockfd, &msg_h, 0)) == -1)
			printf("recv failed\n");
		g_data.stat.tsout = gettimestamp_ms(1);
		if (chkpkt(responsesize) < 0)
			continue;
	}
}

int			main(int argc, char **argv)
{
	signal(SIGINT, statistics);
	signal(SIGALRM, pinger);

	g_data.pid = getpid();

	options(argc, argv + 1)
	initprog();
	pinger(0);
	printf("ft_ping: %s (%s): %d(%d) bytes of data.\n", argv[argc - 1], g_data.ip, DATALEN, PKTLEN);
	listener();
	return (0);
}
