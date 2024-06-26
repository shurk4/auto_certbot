#ifndef CERTENGINE_H
#define CERTENGINE_H

#include <iostream>
#include <filesystem>
#include <chrono>
#include <array>
#include <cstdio>

#include "config.h"

class CertEngine
{
    Config config;

    bool configured = false;
    std::string termResponse;
    std::string fileToConvert;
    bool updated = false;
    void sendCommand(std::string _command);

public:
    CertEngine();

    bool isConfigured();

    void showConfig();
    void showDomains();
    void showConvertParam();

    void newLog();
    void toLog(std::string _log);
    void sendCertRequest();

    void copyCerts();
    void convertCerts();
};

#endif // CERTENGINE_H
