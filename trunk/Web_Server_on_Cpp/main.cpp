#include <getopt.h>
#include <netdb.h>
#include <errno.h>
#include <exception>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "Server.h"


void cleanUpChildProcess(int sig);

/* Description of long options for getopt_long.  */

static const struct option longOptions[] = {
    { "address",          1, NULL, 'a' },
    { "port",             1, NULL, 'p' },
};

/* Description of short options for getopt_long.  */

static const char* const shortOptions = "a:p:";

int main (int argc, char* const argv[]){
    struct in_addr localAddress;
    uint16_t port;
    int nextOption;

    localAddress.s_addr = INADDR_ANY;
    port = 0;

    /* Parse options.  */
    do {
        nextOption =
                getopt_long(argc, argv, shortOptions, longOptions, NULL);
        switch (nextOption) {
            case 'a':
            {
                struct hostent* localHostName;

                /* Look up the host name the user specified.  */
                localHostName = gethostbyname (optarg);
                if (localHostName == NULL || localHostName->h_length == 0)
                    /* Could not resolve the name.  */
                    std::cout << "Invalid host name\n";
                else
                    /* Host name is OK, so use it.  */
                    localAddress.s_addr =
                            *((int*) (localHostName->h_addr_list[0]));
            }
                break;

            case 'p':
            {
                long value = 0;
                char* end;

                std::cout << optarg;
                value = strtol (optarg, &end, 10);
                port = (uint16_t) htons (value);
            }
                break;

            case -1:
                break;

            default:
                abort ();
        }
    } while (nextOption != -1);

    Server s(localAddress, port);
    try{
        s.openConection();
    }
    catch(std::exception& e){
        std::cerr << e.what();
        s.closeConnection();
    }
    s.closeConnection();
    return 0;
}


