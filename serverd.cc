#include <cstring>
#include <signal.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>
#include <wiringPi.h>

#include "common.hh"
#include "sensors.hh"

#undef fatal_if
#define fatal_if(cond, msg)                                             \
    (static_cast<bool>(cond) ?                                          \
        [] (const char* _msg, const char* _func) {                      \
            syslog (LOG_ERR,                                            \
                "%s:%s:%d: %s: %s.\n",                                  \
                 __FILE__, _func, __LINE__, _msg, strerror (errno));    \
            daemon_exit ();                                             \
        } (msg, __func__)                                               \
        : void(0)                                                       \
    )

static std::vector<int> opened_fds {};

void daemon_exit ()
{
    for (const auto fd : opened_fds)
        close (fd);
    syslog (LOG_INFO, "Stopping the daemon");
    closelog ();
    exit (0);
}

void handle_sigint (int signum)
{
    syslog (LOG_INFO, "Received SIGINT. Handling it...");
    daemon_exit ();
}

int main (int argc, char** argv)
{
    daemon (0, 0);

    openlog (NULL, LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog (LOG_INFO, "Starting the daemon");

    signal (SIGINT, handle_sigint);

    setenv ("WIRINGPI_CODES", "1", true);
    fatal_if (-1 == wiringPiSetup (), "Cannot setup wiringPi");

    auto [server_sock, serv_addr] = create_unix_socket ();
    fatal_if (server_sock == -1, "Cannot create server socket");

    opened_fds.push_back (server_sock);

    unlink (UNIX_SOCKET_PATH);

    fatal_if (-1 == bind (server_sock, (sockaddr*)&serv_addr,
                          sizeof (serv_addr)),
              "Cannot bind the socket");

    syslog (LOG_INFO, "Socket file %s was created", UNIX_SOCKET_PATH);

    fatal_if (-1 == chmod (UNIX_SOCKET_PATH, S_IRWXU | S_IROTH | S_IWOTH),
              "Cannot change mode of socket file");

    fatal_if (-1 == listen (server_sock, 1),
              "Cannot prepare the socket to accept");

    syslog (LOG_INFO, "Daemon was initialized");

    int client_sock = -1;
    sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof (client_addr);
    char in_buffer[1024] = {0};
    int return_value = -1;

    while (true) {
        client_sock = accept (server_sock, (sockaddr*)&client_addr,
                              &client_addr_len);
        opened_fds.push_back (client_sock);

        ssize_t recvd_nbytes = read (client_sock, in_buffer, sizeof (in_buffer));
        // TODO: Add in_buffer validation
        if (recvd_nbytes != 0) {
            syslog (LOG_INFO, "Received request to poll %s sensor", in_buffer);
            return_value = sensors.poll_sensor (in_buffer);
            write (client_sock, &return_value, sizeof (return_value));
            std::memset (in_buffer, 0, recvd_nbytes);
        }

        close (client_sock);
        opened_fds.pop_back ();

        return_value = -1;
    }

    return 0;
}
