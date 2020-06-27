#include "ft_ping.h"

static const char	        *icmptype[] =
{
	[ICMP_DEST_UNREACH]		= "Destination Unreachable",
	[ICMP_SOURCE_QUENCH]	= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect (change route)",
	[ICMP_ECHO]			    = "Echo Request",
	[ICMP_TIME_EXCEEDED]	= "Time to live exceeded",
	[ICMP_PARAMETERPROB]	= "Parameter Problem",
	[ICMP_TIMESTAMP]		= "Timestamp Request",
	[ICMP_TIMESTAMPREPLY]   = "Timestamp Reply",
	[ICMP_INFO_REQUEST]		= "Information Request",
	[ICMP_INFO_REPLY]		= "Information Reply",
	[ICMP_ADDRESS]			= "Address Mask Request",
	[ICMP_ADDRESSREPLY]		= "Address Mask Reply"
};

int                         chkpkt(int len, char *s)
{
        struct ip           *ip;
        struct icmp         *icmp;


        ip = (struct ip*)s;
        icmp = (struct icmp*)(s + IPHDRLEN);
        if (len < 8)
        {
            printf("ICMP packet length is less than 8\n");
            return(-2);
        }
        if (icmp->icmp_type != ICMP_ECHOREPLY)
        {
            if (icmp->icmp_type < sizeof(icmptype) && icmp->icmp_type == ICMP_ECHO)
            return (0);
            printf("From %s icmp_seq=%u %s\n", g_data->sender.ip, icmp->icmp_seq, \
            (icmp->icmp_type < sizeof(icmptype) ? icmptype[icmp->icmp_type] : NULL));
        }
        else
        {
            if (g_data->stat.comma[1] - g_data->stat.comma[0] <= 0)
                printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ldms\n",\
                len, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl, g_data->stat.s2 - g_data->stat.s1);
            else
                printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ld.%ldms\n",\
                len, g_data->sender.ip, icmp->icmp_seq, ip->ip_ttl,\
                g_data->stat.s2 - g_data->stat.s1, g_data->stat.comma[1] - g_data->stat.comma[0]);
        }
        g_data->stat.nreceived++;
        return (1);
}