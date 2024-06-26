#include <iostream>

#include "certengine.h"
#include "func.hpp"
#include "config.h"

int main()
{
    CertEngine engine;

    if(engine.isConfigured())
    {
        engine.sendCertRequest();
        engine.copyCerts();
        engine.convertCerts();
    }

    return 0;
}
