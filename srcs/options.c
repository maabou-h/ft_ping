/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabou-h <maabou-h@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 11:48:38 by maabou-h          #+#    #+#             */
/*   Updated: 2019/08/20 15:47:29 by maabou-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	getoptwitharg(char **av, char *opt)
{
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if (!ft_strcmp(av[i], opt))
		{
			if (av[i + 1] == NULL)
				return (-1);
			else if ((ret = ft_atoi(av[i + 1])) < 1)
				return (-1);
			else
			{
				g_data.opt.nopt += 2;
				return (ret);
			}
		}
		i++;
	}
	return (ret);
}

static int	setverbose(char **av)
{
	int		i;

	i = 0;
	while (av[i])
	{
		if (!ft_strcmp(av[i], "-v"))
		{
			g_data.opt.nopt++;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	helper(char **av)
{
	int		i;

	i = 0;
	while (av[i])
	{
		if (!ft_strcmp(av[i], "-h"))
			return (1);
		i++;
	}
	return (0);
}

int			options(int argc, char **av)
{
	g_data.opt.nopt = 0;
	if (argc < 2)
	{
		return (0);
	}
	if (helper(av))
		return (-1);
	g_data.opt.verbose = setverbose(av);
	if ((g_data.opt.interval = getoptwitharg(av, "-i")) == -1)
		return (0);
	if (g_data.opt.interval < 1)
		g_data.opt.interval = 1;
	if ((g_data.opt.ttl = getoptwitharg(av, "-t")) == -1)
		return (0);
	g_data.dest = av[argc - 2];
	if (g_data.dest == NULL)
		return (0);
	else
		g_data.opt.nopt++;
	if (argc - 1 != g_data.opt.nopt)
		return (0);
	return (1);
}
