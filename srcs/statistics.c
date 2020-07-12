/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statistics.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	statistics(int sig)
{
	(void)sig;
	printf("\n--- %s ft_ping statistics ---\n", g_data.dest);
	printf("%d packets transmitted, %d received,", g_data.stat.nsend,\
			g_data.stat.nreceived);
	if (g_data.stat.errors)
		printf(" +%d errors,", g_data.stat.errors);
	printf(" %d%% packet loss, time %ldms\n", (int)(100.0f - (float)\
				g_data.stat.nreceived / (float)g_data.stat.nsend\
				* 100.0f), g_data.stat.tsin - g_data.stat.starttime);
	close(g_data.sockfd);
	exit(1);
}
