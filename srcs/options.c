#include "ft_ping.h"


int getoptwitharg(char **av, char *opt)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if (!strcmp(av[i], opt))
		{
			if (av[i + 1] == NULL)
				return (-1);
			else if ((ret = atoi(av[i + 1])) < 1)
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

int setverbose(char **av)
{
	int i;

	i = 0;
	while (av[i])
	{
		if (!strcmp(av[i], "-v"))
		{
			g_data.opt.nopt++;
			return (1);
		}
			i++;
	}
	return (0);
}

int helper(char **av)
{
	int i;

	i = 0;
	while (av[i])
	{
		if (!strcmp(av[i], "-h"))
			return (1);
		i++;
	}
	return (0);
}

int options(int argc, char **av)
{
	g_data.opt.nopt = 0;
	if (argc < 2)
	{
		return (0);
	}
	g_data.opt.verbose = setverbose(av);
	if ((g_data.opt.interval = getoptwitharg(av, "-i")) == -1){printf("interval error\n");
		return (0);}
	if ((g_data.opt.ttl = getoptwitharg(av, "-t")) == -1){printf("ttl error\n");
		return (0);}
	g_data.host = av[argc - 2];
	if (g_data.host == NULL){printf("host error\n");
		return (0);}
	else
		g_data.opt.nopt++;
	if (argc - 1 != g_data.opt.nopt || helper(av)){printf(helper(av) ? "help option" : "arg error\n");
		return (0);}
		return 1;
}