#include "ft_ping.h"

int                                     packmsg()
{
        int                     i;
        int                     packsize;
        struct icmp             *icmp;

        icmp = (struct icmp*)g_data->sender.sendpacket;
        icmp->icmp_type = ICMP_ECHO;
        icmp->icmp_code = 0;
        icmp->icmp_cksum = 0;
        icmp->icmp_seq = g_data->stat.nsend + 1;
        icmp->icmp_id = g_data->pid;
        packsize = BYTE + DATALEN;
        icmp->icmp_cksum = calculatechecksum((unsigned short*)icmp, packsize);
        return (packsize);
}

int                                     unpackmsg(int len, char *s)
{
        int                     ip_hl;
        struct ip               *ip;
        struct icmp             *icmp;
        char addr[INET6_ADDRSTRLEN];


        ip = (struct ip*)s;
        ip_hl = ip->ip_hl << 2;
        icmp = (struct icmp*)(s + ip_hl);
        len -= ip_hl;
        inet_ntop(AF_INET, &ip->ip_src, addr, INET6_ADDRSTRLEN);
        if (len < 8)
        {
                printf("ICMP packet length is less than 8\n");
                return(-2);
        }
        if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == g_data->pid))
        {
			if (g_data->stat.comma[1] - g_data->stat.comma[0] <= 0)
                printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d time=%ldms\n", len, addr, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl, g_data->stat.s2 - g_data->stat.s1);
			else
                printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d time=%ld.%ldms\n", len, addr, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl, g_data->stat.s2 - g_data->stat.s1, g_data->stat.comma[1] - g_data->stat.comma[0]);
	}
        else
                return(-1);
        g_data->stat.nreceived++;
        return (1);
}

void sendecho(int sig)
{
	int packetsize;

	sig = 1;
	bzero(&g_data->sender.sendpacket, sizeof(g_data->sender.sendpacket));
	packetsize = packmsg();
	g_data->stat.s1 = gettimestamp_ms(0);
        if (sendto(g_data->sockfd, g_data->sender.sendpacket, packetsize, 0, g_data->sender.info->ai_addr, g_data->sender.info->ai_addrlen) < 0)
        {  
                perror("connect");
                close(g_data->sockfd);
                exit(1);
        }
	else
		g_data->stat.nsend++;
	alarm(1);
}

void receivereply()
{
	int ret;
	ssize_t				responsesize;

	while (42)
	{
		ret = 0;
		initheader();
		if ((responsesize = recvmsg(g_data->sockfd, &g_data->hdr.msg_h, 0)) == -1)
			continue ;
		g_data->stat.s2 = gettimestamp_ms(1);
		if ((ret = unpackmsg(responsesize, g_data->hdr.iov.iov_base)) < 0)
			continue ;
	}
}