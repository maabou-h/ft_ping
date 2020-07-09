#include "ft_ping.h"

struct msghdr           genmsghdr()
{
	struct msghdr       msg_h;
    	struct sockaddr_in  sin;
        struct iovec        iov[1];
   	 char                cmsg[PACKET_SIZE];

        bzero(&g_data.rcvpacket, PACKET_SIZE);
                
        bzero(&msg_h, sizeof(msg_h));
        bzero(&sin, sizeof(sin));
        bzero(&cmsg, sizeof(cmsg));
        bzero(&iov, sizeof(iov));

        iov[0].iov_base = g_data.rcvpacket;
        iov[0].iov_len = sizeof(g_data.rcvpacket);

        msg_h.msg_name = &sin;
        msg_h.msg_namelen = sizeof(sin);
	msg_h.msg_control = 0;
	msg_h.msg_controllen = 0;
	msg_h.msg_iov = iov;
	msg_h.msg_iovlen = 1;
	msg_h.msg_flags = 0;

	return (msg_h);
}


void                    genicmphdr(struct ip *ip, struct icmp *icmp)
{
	bzero(&g_data.packet, 4096);

	g_data.stat.tsin = gettimestamp_ms(0);

	ip->ip_v = 4;
	ip->ip_hl = 5;
	ip->ip_tos = 0;
	ip->ip_len = IPHDRLEN + ICMPHDRLEN;
	ip->ip_off = htons(1 << 14);
	ip->ip_ttl = g_data.opt.ttl > 0 ? g_data.opt.ttl : STDTTL;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	ip->ip_id = g_data.pid;
	ip->ip_dst = (struct in_addr)((struct sockaddr_in*)g_data.info->ai_addr)->sin_addr;

	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = (uint16_t)g_data.pid;
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = g_data.stat.seq;
	icmp->icmp_cksum = calculatechecksum((unsigned short*)icmp, DATALEN + ICMPHDRLEN);
printf("good\n");	
	g_data.stat.seq += 1;
}
