#include "ft_ping.h"

static const char	        *icmptype[] =
{
	[ICMP_DEST_UNREACH]		= "Destination Unreachable",
	[ICMP_SOURCE_QUENCH]	= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect",
	[ICMP_ECHO]			    = "Echo Request",
	[ICMP_TIME_EXCEEDED]	= "Time to live exceeded",
	[ICMP_PARAMETERPROB]	= "Parameter Problem",
	[ICMP_TIMESTAMP]		= "Timestamp",
	[ICMP_TIMESTAMPREPLY]   = "Timestamp Reply",
	[ICMP_INFO_REQUEST]		= "Information Request",
	[ICMP_INFO_REPLY]		= "Information Reply",
	[ICMP_ADDRESS]			= "Address Mask Request",
	[ICMP_ADDRESSREPLY]		= "Address Mask Reply"
};

int                         chkpkt(int len)
{
        struct ip           *ip;
	struct icmp	*icmp;


        ip = (struct ip*)g_data.rcvpacket;
	if (((struct icmp*)(g_data.rcvpacket + IPHDRLEN))->icmp_type == ICMP_ECHOREPLY)
	{
        	icmp = (struct icmp*)(g_data.rcvpacket + IPHDRLEN);
		if (icmp->icmp_id != g_data.pid)
			printf("Not our packet1, %d\n", icmp->icmp_code);
        	else
        	{
            	if (g_data.stat.comma[1] - g_data.stat.comma[0] <= 0)
            	    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ldms\n",\
            	    len, g_data.ip, icmp->icmp_seq + 1, ip->ip_ttl, g_data.stat.tsout - g_data.stat.tsin);
            	else
            	    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%ld.%ldms\n",\
            	    len, g_data.ip, icmp->icmp_seq + 1, ip->ip_ttl,\
            	    g_data.stat.tsout - g_data.stat.tsin, g_data.stat.comma[1] - g_data.stat.comma[0]);
            	g_data.stat.nreceived++;
        	}
	}
	else 
	{
        	icmp = (struct icmp*)(g_data.rcvpacket + 48);
			if (icmp->icmp_id != g_data.pid)
				printf("Not our packet\n");
        	else
        	{
				if (g_data.opt.verbose)
            		printf("From %s: type: %d code %d\n",\
            	    	g_data.ip, icmp->icmp_type, icmp->icmp_code);
				else
            		printf("From %s: icmp_seq:%d %s\n",\
            	    	g_data.ip, icmp->icmp_seq + 1, icmptype[((struct icmp*)(g_data.rcvpacket + IPHDRLEN))->icmp_type]);
				
            	g_data.stat.errors++;
        	}
	}
        return (1);
}
