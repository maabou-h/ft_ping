/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timestamp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

long				gettimestamp_ms(int flag)
{
	struct timeval	te;
	long			milliseconds;
	long			mm;

	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	mm = te.tv_sec * 10LL + te.tv_usec / 10;
	if (flag > -1)
	{
		g_data.stat.comma[flag] = mm % 100;
	}
	return (milliseconds);
}
