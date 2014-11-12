#include <getopt.h>
#include <netdb.h>
#include <errno.h>
#include <exception>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <cstdlib>

#include "Server.h"

void help();
void handleSignal(int);

/* Description of long options for getopt_long.  */

static const struct option longOptions[] = {
    { "address",          1, NULL, 'a' },
    { "port",             1, NULL, 'p' },
    { "start-page",       1, NULL, 's' },
};

/* Description of short options for getopt_long.  */

static const char* const shortOptions = "a:p:s:";

int main (int argc, char* const argv[]){
    struct in_addr localAddress;
    uint16_t port = 0;
    int nextOption = 0;
    std::string startPage;

    localAddress.s_addr = INADDR_ANY;
    port = 0;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &Server::closeServer;
    sigaction(SIGINT, &sa, NULL);

    /* Parse options.  */
    int ParamCount = 0;
    do {
        nextOption =
                getopt_long(argc, argv, shortOptions, longOptions, NULL);
        switch (nextOption) {
            case 'a':
            {
                ++ParamCount;
                struct hostent* localHostName;

                /* Look up the host name the user specified.  */
                localHostName = gethostbyname (optarg);
                if (localHostName == NULL || localHostName->h_length == 0)
                    /* Could not resolve the name.  */
                    std::cerr << "Invalid host name\n";
                else
                    /* Host name is OK, so use it.  */
                    localAddress.s_addr =
                            *((int*) (localHostName->h_addr_list[0]));
            }
                break;

            case 'p':
            {
                ++ParamCount;
                long value = 0;
                char* end;

                value = strtol (optarg, &end, 10);
                port = (uint16_t) htons (value);
            }
                break;

            case 's':
            {
                startPage = std::string(optarg);
            }
                break;

            case -1:
                if(ParamCount < 2)
                    help();
                break;

            default:
                help();
                return 0;
        }
    } while (nextOption != -1);

    try{
        const Server& srv = Server::instance(localAddress, port, startPage);
        srv.openConection();
    }
    catch(const std::exception& e){
        std::cerr << e.what();
    }
    std::atexit(Server::deleteInstance);
    return EXIT_SUCCESS;
}

void help(){
    std::cout << "Commands:\n";
    std::cout << "\t-a or --address - for setting address\n";
    std::cout << "\t-p or --port - for setting port\n";
    std::cout << "\t-s or --start-page - for setting path of start html page (not necessarily)\n";
}
