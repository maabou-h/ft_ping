#include "ft_ping.h"

void                            initaddressdata()
{
        struct addrinfo hints;

        memset(&hints, 0, sizeof(hints));

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_RAW;
        hints.ai_protocol = IPPROTO_ICMP;

        if(getaddrinfo(g_data.dest, NULL, &hints, &g_data.info))
        {
                printf("ft_ping: %s: Name or service not known\n", g_data.dest);
                exit(1);
        }
}

void                            initsocket()
{
    if ((g_data.sockfd = socket(g_data.info->ai_family,\
            g_data.info->ai_socktype, g_data.info->ai_protocol)) < 0)
    {
    	printf("ft_ping: error creating socket\n");
            exit(1);
    }
    if (setsockopt(g_data.sockfd, IPPROTO_IP, IP_HDRINCL, (int[1]){1}, sizeof(int)) < 0)
    {
    	close(g_data.sockfd);
            exit(1);
    }
}

void                            initprog()
{
    if (getuid() != 0)
    {
            printf("ft_ping: must run as root\n");
            exit(1);
    }
	g_data.stat.nsend = 0;
	g_data.stat.nreceived = 0;
    g_data.stat.seq = 1;
    g_data.stat.errors = 0;
    bzero(&g_data.packet, sizeof(g_data.packet));
    initaddressdata();
    initsocket();
    inet_ntop(\
            AF_INET,\
            &((const struct sockaddr_in*)g_data.info->ai_addr)->sin_addr,\
            g_data.ip,\
            sizeof(g_data.ip)\
            );
    g_data.stat.starttime = gettimestamp_ms(-1);
}