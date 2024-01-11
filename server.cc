#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <wiringPi.h>

#include "common.hh"
#include "sensors.hh"

int main (int argc, char** argv)
{
    setenv ("WIRINGPI_CODES", "1", true);
    fatal_if (-1 == wiringPiSetup (), "Cannot setup wiringPi");

    auto [server_sock, serv_addr] = create_unix_socket ();
    fatal_if (server_sock == -1, "Cannot create server socket");

    unlink (UNIX_SOCKET_PATH);

    fatal_if (-1 == bind (server_sock, (sockaddr*)&serv_addr,
                          sizeof (serv_addr)),
              "Cannot bind the socket");

    fatal_if (-1 == chmod (UNIX_SOCKET_PATH, S_IRWXU | S_IROTH | S_IWOTH),
              "Cannot change mode of socket file");

    fatal_if (-1 == listen (server_sock, 1),
              "Cannot prepare the socket to accept");

    int client_sock = -1;
    sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof (client_addr);
    char in_buffer[1024] = {0};
    int return_value = -1;

    while (true) {
        client_sock = accept (server_sock, (sockaddr*)&client_addr,
                              &client_addr_len);
        ssize_t recvd_nbytes = read (client_sock, in_buffer, sizeof (in_buffer));
        // TODO: Add in_buffer validation
        if (recvd_nbytes != 0) {
            return_value = sensors.poll_sensor (in_buffer);
            write (client_sock, &return_value, sizeof (return_value));
            std::memset (in_buffer, 0, recvd_nbytes);
        }

        close (client_sock);
        return_value = -1;
    }

    return 0;
}
