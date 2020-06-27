#include "ft_ping.h"

struct msghdr           genresphdr()
{
    struct msghdr       msg_h;
    struct sockaddr_in  sin;
	struct iovec        iov;
    char                cmsg[PACKET_SIZE];

	bzero(&sin, sizeof(sin));
	bzero(&cmsg, sizeof(cmsg));
	msg_h.msg_name = &sin;
	msg_h.msg_namelen = sizeof(sin);
	msg_h.msg_control = &cmsg;
	msg_h.msg_controllen = sizeof(cmsg);
	msg_h.msg_flags = 0;
	bzero(&g_data->rcvpacket, sizeof(g_data->rcvpacket));
	iov.iov_base = g_data->rcvpacket;
	iov.iov_len = sizeof(g_data->rcvpacket);
	msg_h.msg_iov = &iov;
	msg_h.msg_iovlen = 1;
    return (msg_h);
}


void                    genhdr(struct ip *ip, struct icmp *icmp)
{
    ip->ip_v = 4;
	ip->ip_hl = IPHDRLEN / 4;
	ip->ip_tos = 0;
	ip->ip_len = PACKET_SIZE;
	ip->ip_off = 0;
	ip->ip_ttl = 1;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	ip->ip_src.s_addr = INADDR_ANY;
	ip->ip_dst.s_addr = ((const struct sockaddr_in*)g_data->sender.info->ai_addr)->sin_addr.s_addr;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = g_data->stat.seq++;
    icmp->icmp_id = g_data->pid;
    icmp->icmp_cksum = calculatechecksum((unsigned short*)icmp);
}