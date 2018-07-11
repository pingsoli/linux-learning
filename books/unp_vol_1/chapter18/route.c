#include "route.h"

#include <netinet/in.h>
#include <stdio.h>

/*
 * Round up 'a' to next multiple of 'size', which must be a power of 2
 */
#define ROUNDUP(a, size) (((a) & ((size)-1)) ? (1 + ((a) | ((size)-1))) : (a))

/*
 * Step to next socket address structure;
 * if sa_len is 0, assume it is sizeof(u_long).
 */
#define NEXT_SA(ap) ap = (SA *) \
    ((caddr_t) ap + (ap->sa_len ? ROUNDUP(ap->sa_len, sizeof (u_long)) : \
                           sizeof(u_long)))

void get_rtaddrs(int addrs, SA *sa, SA **rti_info)
{
  int   i;

  for (i = 0; i < RTAX_MAX; i++) {
    if (addrs & (1 << i)) {
      rti_info[i] = sa;
      NEXT_SA(sa);
    } else
      rti_info[i] = NULL;
  }
}

const char *sock_masktop(struct sockaddr*, socklen_t)
{
  static char str[IN6_ADDRSTRLEN];
  unsigned char *ptr = &sa->sa_data[2];

  if (sa->sa_len == 0)
    return ("0.0.0.0");
  else if (sa->sa_len == 5)
    snprintf(str, sizeof(str), "%d.0.0.0", *ptr);
  else if (sa->sa_len == 6)
    snprintf(str, sizeof(str), "%d.%d.0.0", *ptr, *(ptr + 1));
  else if (sa->sa_len == 7)
    snprintf(str, sizeof(str), "%d.%d.%d.0", *ptr, *(ptr + 1), *(ptr + 2));
  else if (sa->sa_len == 8)
    snprintf(str, sizeof(str), "%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2),
        *(ptr + 3));
  else
    snprintf(str, sizeof(str), "(unknown mask, len = %d, family = %d)",
        sa->sa_len, sa->sa_family);

  return str;
}

char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
  static char str[128];   /* Unix domain is largest */

  switch (sa->sa_family) {
    case AF_INET:
      {
        struct sockaddr_in  *sin = (struct sockaddr_in *) sa;

        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
          return(NULL);
        return(str);
      }

#ifdef  IPV6
    case AF_INET6:
      {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;

        if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
          return(NULL);
        return(str);
      }
#endif

#ifdef  AF_UNIX
    case AF_UNIX:
      {
        struct sockaddr_un  *unp = (struct sockaddr_un *) sa;

        /* OK to have no pathname bound to the socket: happens on
         *         every connect() unless client calls bind() first. */
        if (unp->sun_path[0] == 0)
          strcpy(str, "(no pathname bound)");
        else
          snprintf(str, sizeof(str), "%s", unp->sun_path);
        return(str);
      }
#endif

#ifdef  HAVE_SOCKADDR_DL_STRUCT
    case AF_LINK:
      {
        struct sockaddr_dl  *sdl = (struct sockaddr_dl *) sa;

        if (sdl->sdl_nlen > 0)
          snprintf(str, sizeof(str), "%*s",
              sdl->sdl_nlen, &sdl->sdl_data[0]);
        else
          snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
        return(str);
      }
#endif
    default:
      snprintf(str, sizeof(str), "sock_ntop_host: unknown AF_xxx: %d, len %d",
          sa->sa_family, salen);
      return(str);
  }
  return (NULL);
}