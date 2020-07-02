#include "ft_ping.h"

void pinger(int sig)
{
	sig = 1;
	genhdr((struct ip*)(g_data.packet), (struct icmp*)(g_data.packet + IPHDRLEN));
        if (sendto(g_data.sockfd, g_data.packet, ICMPHDRLEN + DATALEN, 0, g_data.info->ai_addr, g_data.info->ai_addrlen) < 0)
        {  
                perror("connect");
                close(g_data.sockfd);
                exit(1);
        }
	else
		g_data.stat.nsend++;
	alarm(g_data.opt.interval);
}

void listener()
{
	int ret;
    struct msghdr       msg_h;
	ssize_t				responsesize;

	while (42)
	{
		ret = 0;
		msg_h = genresphdr();
		if ((responsesize = recvmsg(g_data.sockfd, &msg_h, 0)) == -1)
			printf("recv failed\n");
		g_data.stat.tsout = gettimestamp_ms(1);
		if ((ret = chkpkt(responsesize)) < 0)
			continue;
	}
}
