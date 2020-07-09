#include "ft_ping.h"

void pinger(int sig)
{
	char *content;

	content = (char*)(g_data.packet + IPHDRLEN + ICMPHDRLEN);
	sig = -1;
	while (++sig < DATALEN)
		content[sig] = 16 + sig;
	genicmphdr((struct ip*)(g_data.packet), (struct icmp*)(g_data.packet + IPHDRLEN));
    if (sendto(g_data.sockfd, g_data.packet, ICMPHDRLEN + DATALEN + IPHDRLEN , 0, g_data.info->ai_addr, g_data.info->ai_addrlen) < 0)
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
	struct msghdr       msg_h = {0};
	ssize_t				responsesize;
	
	responsesize = 0;
	while (1)
	{	
		msg_h = genmsghdr();
		if ((responsesize = recvmsg(g_data.sockfd, &msg_h, 0)) == -1)
			printf("recv failed\n");
		g_data.stat.tsout = gettimestamp_ms(1);
		chkpkt(responsesize);
	}
}

int			main(int argc, char **argv)
{
	bzero(&g_data, sizeof(g_data));
	g_data.pid = getpid();
	signal(SIGINT, statistics);
	signal(SIGALRM, pinger);

	options(argc, argv + 1);
	initprog();
	pinger(-1);
	printf("ft_ping %s (%s): %d(%d) bytes of data.\n", argv[argc - 1], g_data.ip, DATALEN + ICMPHDRLEN - ICMP_MINLEN , PKTLEN);
	listener();
	return (0);
}
