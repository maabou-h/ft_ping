/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	genmsgcontent(char *content)
{
	ssize_t i;

	i = -1;
	while (++i < DATALEN)
		content[i] = 16 + i;
}

static void	geniphdr(struct ip *ip)
{
	ip->ip_v = 4;
	ip->ip_hl = 5;
	ip->ip_tos = 0;
	ip->ip_len = IPHDRLEN + ICMPHDRLEN;
	ip->ip_off = 0;
	ip->ip_ttl = g_data.opt.ttl > 0 ? g_data.opt.ttl : STDTTL;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	ip->ip_id = (uint16_t)g_data.pid;
	ip->ip_dst = (struct in_addr)\
				((struct sockaddr_in*)g_data.info->ai_addr)->sin_addr;
}

static void	genicmphdr(struct icmp *icmp)
{
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = (uint16_t)g_data.pid;
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = g_data.stat.seq;
	icmp->icmp_cksum = calculatechecksum(\
			(unsigned short*)icmp, DATALEN + ICMPHDRLEN);
}

void		pack(void)
{
	ft_bzero(&g_data.packet, sizeof(g_data.packet));
	g_data.stat.tsin = gettimestamp_ms(0);
	genmsgcontent((char*)(g_data.packet + IPHDRLEN + ICMPHDRLEN));
	geniphdr((struct ip*)(g_data.packet));
	genicmphdr((struct icmp*)(g_data.packet + IPHDRLEN));
	g_data.stat.seq += 1;
}
