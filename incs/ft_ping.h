#ifndef FT_PING_H
# define FT_PING_H
# include <stdio.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <string.h>
# define ICMPPAYLOAD	1472
# define ICMPHDRLEN		8
# define DATALEN		56
# define IPHDRLEN		20
# define STDTTL			255
# define PKTLEN			84

typedef struct			s_stats
{
	long				comma[2];
	long				tsin;
	long				tsout;
	long				starttime;
	int					nsend;
	int					nreceived;
	int					errors;
	int					seq;
}						t_stats;

typedef struct			s_opt
{
	int					verbose;
	int					interval;
	int					ttl;
	int					nopt;
}						t_opt;

typedef struct			s_data
{
	t_stats				stat;
	int					sockfd;
	pid_t				pid;
	char				*dest;
	char				packet[ICMPPAYLOAD];
	char				rcvpacket[ICMPPAYLOAD];
	char				ip[INET_ADDRSTRLEN];
	t_opt				opt;
	struct addrinfo		*info;
}						t_data;

t_data					g_data;

unsigned short			calculatechecksum(unsigned short *addr, int len);
void					pinger(int sig);
void					listener(void);
void					pack(void);
int						unpack(void);
void					initprog(void);
int						options(int argc, char **av);
int						chkpkt(int len);
void					statistics(int sig);
long					gettimestamp_ms(int flag);

#endif
