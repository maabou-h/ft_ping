#include "ft_ping.h"

int           unpack()
{
	struct msghdr       msg_h;
    	struct sockaddr_in  sin;
        struct iovec        iov[1];
	int		ans;

        bzero(&g_data.rcvpacket, sizeof(g_data.rcvpacket));
                
        bzero(&msg_h, sizeof(msg_h));
        bzero(&sin, sizeof(sin));
        bzero(&iov, sizeof(iov));

        iov[0].iov_base = g_data.rcvpacket;
        iov[0].iov_len = sizeof(g_data.rcvpacket);

        msg_h.msg_name = &sin;
        msg_h.msg_namelen = sizeof(sin);
	msg_h.msg_control = 0;
	msg_h.msg_controllen = 0;
	msg_h.msg_iov = iov;
	msg_h.msg_iovlen = 1;
	msg_h.msg_flags = 0;
	ans = recvmsg(g_data.sockfd, &msg_h, 0);
	return (ans);
}