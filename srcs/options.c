#include "ft_ping.h"

int	getinterval(char **av)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if (!strcmp(av[i], "-i"))
		{
			if (*av + (i + 1) == NULL)
				return (-1);
			else if ((ret = atoi(av[i + 1]) < 1))
				return (-1);
			else
			{
				g_data->argno += 2;
				return (ret);
			}
		}
		i++;
	}
	return (ret);
}

int getcount(char **av)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if (!strcmp(av[i], "-c"))
		{
			if (*av + (i + 1) == NULL)
				return (-1);
			else if ((ret = atoi(av[i + 1]) < 1))
				return (-1);
			else
			{
				g_data->argno += 2;
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
			g_data->argno++;
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
	g_data->argno = 0;
	if (argc < 2)
	{
		return (0);
	}
	g_data->verbose = setverbose(av);
	if ((g_data->count = getcount(av)) == -1){printf("count error\n");
		return (0);}
	if ((g_data->interval = getinterval(av)) == -1){printf("interval error\n");
		return (0);}
	g_data->host = av[argc - 2];
	if (g_data->host == NULL){printf("host error\n");
		return (0);}
	else
		g_data->argno++;
	if (argc - 1 != g_data->argno || helper(av)){printf(helper(av) ? "help option" : "arg error\n");
		return (0);}
		return 1;
}