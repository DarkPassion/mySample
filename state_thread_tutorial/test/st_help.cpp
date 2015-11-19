#include <st_help.h>


void srs_close_stfd(st_netfd_t& stfd)
{
    if (stfd) {
        int fd = st_netfd_fileno(stfd);
        st_netfd_close(stfd);
        stfd = NULL;
        
        // st does not close it sometimes, 
        // close it manually.
        close(fd);
    }
}
