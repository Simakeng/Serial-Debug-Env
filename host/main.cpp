#include <port_api.h>
#include "host.hpp"

int main(int argc, char *argv[])
{
    // prepare the host environment
    Serial::HostInit();

    // start the application
    app_main(argc, argv);

    // clean up
    Serial::HostExit();
    return 0;
}
