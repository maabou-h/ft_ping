#include "ft_ping.h"

unsigned short          calculatechecksum(unsigned short *addr)
{
        int                             nleft;
        int                             sum;
        unsigned short *w;
        unsigned short  answer;

        nleft = BYTE + DATALEN;
        sum = 0;
        w = addr;
        answer = 0;
        while (nleft > 1)
        {
                sum +=  *w++;
                nleft -= 2;
        }
        if (nleft == 1)
        {
                *(unsigned char*)(&answer) = *(unsigned char*)w;
                sum += answer;
        }
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = ~sum;
        return (answer);
}