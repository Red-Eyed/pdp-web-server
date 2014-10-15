#include <getopt.h>
#include <netdb.h>
#include <errno.h>
#include <exception>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Server.h"

/* Description of long options for getopt_long.  */

static const struct option long_options[] = {
    { "address",          1, NULL, 'a' },
    { "port",             1, NULL, 'p' },
};

/* Description of short options for getopt_long.  */

static const char* const short_options = "a:p:";

int main (int argc, char* const argv[]){
    struct in_addr local_address;
    uint16_t port;
    int next_option;

    local_address.s_addr = INADDR_ANY;
    port = 0;

    /* Parse options.  */
    do {
        next_option =
                getopt_long(argc, argv, short_options, long_options, NULL);
        switch (next_option) {
            case 'a':
            {
                struct hostent* local_host_name;

                /* Look up the host name the user specified.  */
                local_host_name = gethostbyname (optarg);
                if (local_host_name == NULL || local_host_name->h_length == 0)
                    /* Could not resolve the name.  */
                    std::cout << "Invalid host name\n";
                else
                    /* Host name is OK, so use it.  */
                    local_address.s_addr =
                            *((int*) (local_host_name->h_addr_list[0]));
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
    } while (next_option != -1);

    Server s(local_address, port);
    try{
        s.openConection();
    }
    catch(std::exception& e){
        std::cerr << e.what();
    }

    return 0;
}
