#include "ft_ping.h"

void initheader(void)
{
	bzero(&g_data->hdr.sin, sizeof(g_data->hdr.sin));
	bzero(&g_data->hdr.cmsg, sizeof(g_data->hdr.cmsg));
	g_data->hdr.msg_h.msg_name = &g_data->hdr.sin;
	g_data->hdr.msg_h.msg_namelen = sizeof(g_data->hdr.sin);
	g_data->hdr.msg_h.msg_control = &g_data->hdr.cmsg;
	g_data->hdr.msg_h.msg_controllen = sizeof(g_data->hdr.cmsg);
	g_data->hdr.msg_h.msg_flags = 0;
	bzero(&g_data->hdr.packet, sizeof(g_data->hdr.packet));
	g_data->hdr.iov.iov_base = g_data->hdr.packet;
	g_data->hdr.iov.iov_len = sizeof(g_data->hdr.packet);
	g_data->hdr.msg_h.msg_iov = &g_data->hdr.iov;
	g_data->hdr.msg_h.msg_iovlen = 1;
}

void initaddressdata()
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if(getaddrinfo(g_data->host, NULL, &hints, &g_data->sender.info))
	{
		printf("ft_ping: %s: Name or service not known\n", g_data->host);
		exit(1);
	}
	g_data->stat.name = g_data->host;
	initsocket();
	inet_ntop(\
		AF_INET,\
		&((const struct sockaddr_in*)g_data->sender.info->ai_addr)->sin_addr,\
		g_data->sender.ip,\
		sizeof(g_data->sender.ip)\
		);
}

void initsocket()
{
	int ret;
	int test = 0;

	ret = 1;
	if ((g_data->sockfd = socket(g_data->sender.info->ai_family, g_data->sender.info->ai_socktype, g_data->sender.info->ai_protocol)) < 0)
	{
        printf("ft_ping: error creating socket\n");
		exit(1);
	}
	if ((test = setsockopt(g_data->sockfd, IPPROTO_IP, IP_HDRINCL, &ret, sizeof(ret))) < 0)
	{
        printf("ft_ping %d: error setting socket timeoutttt\n", test);
        close(g_data->sockfd);
		exit(1);
    }
}

void initprog()
{
    if (getuid() != 0)
	{
		printf("ft_ping: must run as root\n");
		exit(1);
	}
	g_data->pid = getpid();
	g_data->stat.nsend = 0;
	g_data->stat.nreceived = 0;
	initaddressdata();
	g_data->stat.starttime = gettimestamp_ms(-1);
}
