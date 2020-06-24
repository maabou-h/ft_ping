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
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(g_data->host, NULL, &hints, &g_data->sender.info))
	{
		printf("ft_ping: %s: Name or service not known\n", g_data->host);
		close(g_data->sockfd);
		exit(1);
	}
	g_data->stat.name = g_data->host;
	inet_ntop(\
		AF_INET,\
		&((const struct sockaddr_in*)g_data->sender.info->ai_addr)->sin_addr,\
		g_data->sender.ip,\
		sizeof(g_data->sender.ip)\
		);
}

void initsocket()
{
	if ((g_data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
        printf("ft_ping: error creating socket\n");
		exit(1);
	}
	if (setsockopt(g_data->sockfd, SOL_SOCKET, SO_RCVTIMEO, \
		(struct timeval[1]){1,0}, sizeof(struct timeval)) == -1)
	{
        printf("ft_ping: error setting socket timeout\n");
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
	initsocket();
	initaddressdata();
	g_data->stat.starttime = gettimestamp_ms(-1);
}
