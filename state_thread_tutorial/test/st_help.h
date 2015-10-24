#ifndef _ST_HELP_H_
#define _ST_HELP_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <st.h>


void srs_close_stfd(st_netfd_t& stfd);


// free the p and set to NULL.
// p must be a T*.
#define srs_freep(p) \
    if (p) { \
        delete p; \
        p = NULL; \
    } \
    (void)0

#endif