#include "ft_ping.h"

static const char	*icmp_responses[] =
{
	[ICMP_UNREACH]			= "Destination Unreachable",
	[ICMP_SOURCEQUENCH]		= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect (change route)",
	[ICMP_ECHO]			= "Echo Request",
	[ICMP_TIMXCEED]			= "Time to live exceeded",
	[ICMP_PARAMPROB]		= "Parameter Problem",
	[ICMP_TSTAMP]			= "Timestamp Request",
	[ICMP_TSTAMPREPLY]		= "Timestamp Reply",
	[ICMP_IREQ]			= "Information Request",
	[ICMP_IREQREPLY]		= "Information Reply",
	[ICMP_MASKREQ]			= "Address Mask Request",
	[ICMP_MASKREPLY]		= "Address Mask Reply"
};

int     fillicmp(struct icmp *icmp)
{
        int                     packsize;

        icmp->icmp_type = ICMP_ECHO;
        icmp->icmp_code = 0;
        icmp->icmp_cksum = 0;
        icmp->icmp_seq = g_data->stat.nsend + 1;
        icmp->icmp_id = g_data->pid;
        packsize = BYTE + DATALEN;
        icmp->icmp_cksum = calculatechecksum((unsigned short*)icmp, packsize);
        return (packsize);
}

void	filliphdr(struct ip *ip)
{
	ip->ip_v = 4;
	ip->ip_hl = IPHDRLEN / 4;
	ip->ip_tos = 0;
	ip->ip_len = PACKET_SIZE;
	ip->ip_id = htons(0);
	ip->ip_off = 0;
	ip->ip_ttl = 100;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	ip->ip_src.s_addr = INADDR_ANY;
	ip->ip_dst.s_addr = ((const struct sockaddr_in*)g_data->sender.info->ai_addr)->sin_addr.s_addr;
}

int                                     packmsg()
{
        int ret;

        filliphdr((struct ip*)g_data->sender.sendpacket);
        ret = fillicmp((struct icmp*)(g_data->sender.sendpacket + 20));
        return (ret);
}

int                                     unpackmsg(int len, char *s)
{
        struct ip               *ip;
        struct icmp             *icmp;
        char addr[INET6_ADDRSTRLEN];


        ip = (struct ip*)s;
        icmp = (struct icmp*)(s + 20);
        len -= 20;
        inet_ntop(AF_INET, &ip->ip_src, addr, INET6_ADDRSTRLEN);
        if (len < 8)
        {
                printf("ICMP packet length is less than 8\n");
                return(-2);
        }
        if (icmp->icmp_id == g_data->pid)
        {
                if (icmp->icmp_type != ICMP_ECHOREPLY)
                {
                        printf("From %s icmp_seq=%u %s\n", addr, icmp->icmp_seq, (icmp->icmp_type < sizeof(icmp_responses) ? icmp_responses[icmp->icmp_type] : NULL));
                }
                else
                {
                                if (g_data->stat.comma[1] - g_data->stat.comma[0] <= 0)
                        printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d time=%ldms\n", len, addr, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl, g_data->stat.s2 - g_data->stat.s1);
                                else
                        printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d time=%ld.%ldms\n", len, addr, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl, g_data->stat.s2 - g_data->stat.s1, g_data->stat.comma[1] - g_data->stat.comma[0]);
                }
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
			printf("recv failed\n");
		g_data->stat.s2 = gettimestamp_ms(1);
		if ((ret = unpackmsg(responsesize, g_data->hdr.packet)) < 0)
			printf("unpack failed\n");
	}
}