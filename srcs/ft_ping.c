#include "ft_ping.h"

void pinger(int sig)
{
	sig = 1;
	bzero(&g_data->sender.sendpacket, sizeof(g_data->sender.sendpacket));
	genhdr((struct ip*)(g_data->sender.sendpacket), (struct icmp*)(g_data->sender.sendpacket + IPHDRLEN));
	g_data->stat.s1 = gettimestamp_ms(0);
        if (sendto(g_data->sockfd, g_data->sender.sendpacket, BYTE + DATALEN, 0, g_data->sender.info->ai_addr, g_data->sender.info->ai_addrlen) < 0)
        {  
                perror("connect");
                close(g_data->sockfd);
                exit(1);
        }
	else
		g_data->stat.nsend++;
	alarm(1);
}

void listener()
{
	int ret;
        struct msghdr       msg_h;
        char                packet[PACKET_SIZE];
	ssize_t				responsesize;

	while (42)
	{
		ret = 0;
		msg_h = genresphdr();
		if ((responsesize = recvmsg(g_data->sockfd, &msg_h, 0)) == -1)
			printf("recv failed\n");
		g_data->stat.s2 = gettimestamp_ms(1);
		if ((ret = chkpkt(responsesize, g_data->rcvpacket)) < 0)
			printf("unpack failed\n");
	}
}