/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static const char				**icmptypetable(void)
{
	return ((const char *[19]){\
		[ICMP_DEST_UNREACH]		= "Destination Unreachable",\
		[ICMP_SOURCE_QUENCH]	= "Source Quench",\
		[ICMP_REDIRECT]			= "Redirect",\
		[ICMP_ECHO]				= "Echo Request",\
		[ICMP_TIME_EXCEEDED]	= "Time to live exceeded",\
		[ICMP_PARAMETERPROB]	= "Parameter Problem",\
		[ICMP_TIMESTAMP]		= "Timestamp",\
		[ICMP_TIMESTAMPREPLY]	= "Timestamp Reply",\
		[ICMP_INFO_REQUEST]		= "Information Request",\
		[ICMP_INFO_REPLY]		= "Information Reply",\
		[ICMP_ADDRESS]			= "Address Mask Request",\
		[ICMP_ADDRESSREPLY]		= "Address Mask Reply"\
	});
}

static void						badpacket(struct icmp *icmp)
{
	const char					**icmptype = icmptypetable();

	if (icmp->icmp_id != g_data.pid)
		printf("Not our packet\n");
	else
	{
		if (g_data.opt.verbose)
			printf("From %s: type: %d code %d\n",\
					g_data.ip, icmp->icmp_type, icmp->icmp_code);
		else
			printf("From %s: icmp_seq:%d %s\n", g_data.ip,\
					icmp->icmp_seq + 1,\
					icmptype[((struct icmp*)(g_data.rcvpacket\
							+ IPHDRLEN))->icmp_type]);
		g_data.stat.errors++;
	}
}

static void						goodpacket(int len)
{
	struct ip					*ip;
	struct icmp					*icmp;

	ip = (struct ip*)g_data.rcvpacket;
	icmp = (struct icmp*)(g_data.rcvpacket + IPHDRLEN);
	len -= ip->ip_hl << 2;
	if (icmp->icmp_id != g_data.pid)
		printf("Not our packet1, %d\n", icmp->icmp_code);
	else
	{
		if (g_data.stat.comma[1] - g_data.stat.comma[0] <= 0)
			printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ldms\n",\
					len, g_data.ip, icmp->icmp_seq + 1, ip->ip_ttl,\
					g_data.stat.tsout - g_data.stat.tsin);
		else
			printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ld.%ldms\n",\
					len, g_data.ip, icmp->icmp_seq + 1, ip->ip_ttl,\
					g_data.stat.tsout - g_data.stat.tsin,\
					g_data.stat.comma[1] - g_data.stat.comma[0]);
		g_data.stat.nreceived++;
	}
}

int								chkpkt(int len)
{
	if (((struct icmp*)(g_data.rcvpacket + IPHDRLEN))->icmp_type\
			== ICMP_ECHOREPLY)
		goodpacket(len);
	else
		badpacket((struct icmp*)(g_data.rcvpacket + 48));
	return (1);
}
