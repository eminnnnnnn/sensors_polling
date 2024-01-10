#include <cstring>
#include <iostream>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "common.hh"

int main (int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "-1" << std::endl;
        return 0;
    }

    int sensor_value = -1;

    auto [server_sock, serv_addr] = create_unix_socket ();
    fatal_if (server_sock == -1, "Cannot create server socket");

    int connect_failed = connect (server_sock, (sockaddr*)&serv_addr,
                                  sizeof (serv_addr));

    pollfd poll_fds[] = { {.fd = server_sock, .events = POLLIN } };
    if (!connect_failed) {
        write (server_sock, argv[1], std::strlen (argv[1]));

        int timeout = 3000;
        int events_num = 1;
        int poll_result = poll (poll_fds, events_num, timeout);
        if (poll_result == events_num && (poll_fds[0].revents & POLLIN)) {
            read (server_sock, &sensor_value, sizeof (sensor_value));
        }
    }

    std::cout << sensor_value << std::endl;
    close (server_sock);

    return 0;
}
