#ifndef COMMON_HH
#define COMMON_HH

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <tuple>

#define fatal_if(cond, msg)                                             \
    (static_cast<bool>(cond) ?                                          \
        [] (const char* _msg, const char* _func) {                      \
            std::fprintf (stderr,                                       \
                "%s:%s:%d: %s: %s.\n",                                  \
                 __FILE__, _func, __LINE__, _msg, strerror (errno));    \
            std::exit (1);                                              \
        } (msg, __func__)                                               \
        : void(0)                                                       \
    )

#ifndef UNIX_SOCKET_DIR
const char UNIX_SOCKET_PATH[] = "/home/.sensors_sock/sock";
#else
const char UNIX_SOCKET_PATH[] = UNIX_SOCKET_DIR"/sock";
#endif

std::tuple<int, sockaddr_un>
create_unix_socket ()
{
    int sock_d = -1;

    sock_d = socket (AF_UNIX, SOCK_STREAM, 0);
    fatal_if (sock_d == -1, "Cannot create UNIX socket");

    sockaddr_un sock_addr = { .sun_family = AF_UNIX };
    std::strncpy (sock_addr.sun_path, UNIX_SOCKET_PATH,
                  sizeof (sock_addr.sun_path));

    fatal_if (0 != sock_addr.sun_path[sizeof (sock_addr.sun_path) - 1],
              "Invalid unix socket path");

    return {sock_d, sock_addr};
}

#endif