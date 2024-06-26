#include <iostream>

#include "func.hpp"
#include "config.h"

int main()
{
    Config config;
    config.readConfig();

    newLog();

    if(config.isEmpty())
    {
        toLog("Not configured!");
        return 1;
    }

    showConfig(config);
    sendCertRequest(config);

    return 0;
}
