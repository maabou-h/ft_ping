#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#ifndef FT_PING_H
# define FT_PING_H

# define PACKET_SIZE    4096
# define MAXNPACKETS    5
# define BYTE           8
# define DATALEN        56
# define IPHDRLEN       20
# define PKTLEN         (ICMP_MINLEN + DATALEN + IPHDRLEN)


typedef struct s_ipdata
{
    char                sendpacket[PACKET_SIZE];
    char                ip[INET_ADDRSTRLEN];
    struct addrinfo     *info;
}                       t_ipdata;

typedef struct          s_stats
{
    char                *name;
    int                 nsend;
    int                 seq;
    int                 nreceived;
    long                comma[2];
    long                s1;
    long                s2;
    long                starttime;
    uint8_t             to;
}                       t_stats;

typedef struct          s_data
{
    int                 sockfd;
    int                 verbose;
    int                 count;
    int                 interval;
    int                 argno;
    char                *host;
    char                rcvpacket[PACKET_SIZE];
    pid_t               pid;
    t_stats             stat;
    t_ipdata            sender;
}                       t_data;

t_data                  *g_data;

unsigned short          calculatechecksum(unsigned short *addr);

void                    pinger(int sig);
void                    listener(void);

struct msghdr           genresphdr(void);
void                     genhdr(struct ip *ip, struct icmp *icmp);

void                    initaddressdata(void);
void                    initsocket(void);
void                    initprog(void);

int                     options(int argc, char **av);

int                     chkpkt(int len, char *s);

void				    statistics(int sig);

long                    gettimestamp_ms(int flag);

#endif