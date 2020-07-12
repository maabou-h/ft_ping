/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void		pinger(int sig)
{
	if (sig == 42)
		printf("ft_ping %s (%s): %d(%d) bytes of data.\n", g_data.dest,\
				g_data.ip, DATALEN + ICMPHDRLEN - ICMP_MINLEN, PKTLEN);
	pack();
	if (sendto(g_data.sockfd, g_data.packet, DATALEN + ICMPHDRLEN\
				+ IPHDRLEN, 0, g_data.info->ai_addr,\
				g_data.info->ai_addrlen) < 0)
	{
		close(g_data.sockfd);
		exit(1);
	}
	else
		g_data.stat.nsend++;
	alarm(g_data.opt.interval);
}

void		listener(void)
{
	ssize_t	responsesize;

	responsesize = 0;
	while (1)
	{
		if ((responsesize = unpack()) == -1)
			continue ;
		g_data.stat.tsout = gettimestamp_ms(1);
		chkpkt(responsesize);
	}
}

int			main(int argc, char **argv)
{
	int		v;

	ft_bzero(&g_data, sizeof(g_data));
	g_data.pid = getpid();
	signal(SIGINT, statistics);
	signal(SIGALRM, pinger);
	if ((v = options(argc, argv + 1)) < 1)
	{
		printf("Usage: ping [-h] [-i interval] [-t ttl] destination\n");
		return (1);
	}
	initprog();
	pinger(42);
	listener();
	return (0);
}
