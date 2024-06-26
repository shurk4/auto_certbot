#include "certengine.h"

void CertEngine::sendCommand(std::string _request)
{
    toLog("- Send command");

    toLog("Request: " + _request);
    std::string passStr("echo " + config.getUserPass() + " | ");

    std::string command = passStr + _request;

    std::array<char, 128> buffer;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        termResponse += buffer.data();
    }

    toLog(termResponse);
}

CertEngine::CertEngine()
{
    config.readConfig();
    newLog();

    if(config.isEmpty())
    {
        toLog("Not configured!");
        return;
    }

    configured = true;
    showConfig();
}

bool CertEngine::isConfigured()
{
    return configured;
}

void CertEngine::newLog()
{
    if(std::filesystem::exists("log.txt"))
    {
        std::remove("log.txt");
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream stream;
    stream << std::ctime(&currentTime);
    toLog(stream.str());
}

void CertEngine::toLog(std::string _log)
{
    std::fstream logFile;
    std::string path = "log.txt";
    logFile.open(path, std::fstream::app);
    std::cout << "Log: " << _log << "\n";

    if(logFile.is_open())
    {
        logFile << _log << "\n";
    }
}

void CertEngine::showConfig()
{
    toLog("Path to the certbot certificstes dir: " + config.getCertDir());
    toLog("Your e-mail: " + config.getEmail());

    showDomains();

    toLog("Target certificates dir: " + config.getTargetDir());

    showConvertParam();

    std::string tempStr = "User password: ";
    if(config.haveUserPassword())
    {
        tempStr += "specified";
    }
    else
    {
        tempStr += "Not specified";
    }

    toLog(tempStr + "\n");
}

void CertEngine::showDomains()
{
    std::vector<std::string> domains = config.getDomains();
    toLog("Your domains:");
    if(domains.empty())
    {
        toLog("Not specified");
    }
    else
    {
        for(int i = 0; i < domains.size(); i++)
        {
            toLog(std::to_string(i) + ")" + domains[i]);
        }
    }
    toLog("");
}

void CertEngine::showConvertParam()
{
    toLog("Current convert parameters:");
    int param = config.getCertConvertParam();
    if(param & KEY)
    {
        toLog("*.key");
    }
    if(param & CRT)
    {
        toLog("*.crt");
    }
    if(param & NO || param == 0)
    {
        toLog("Do not convert");
    }

    toLog("Target certificate name: " + config.getCertName());
    toLog("");
}

void CertEngine::sendCertRequest()
{
    toLog("- Sending a certificate request");

    std::string request("sudo -S -k certbot certonly -n --standalone");
    std::vector<std::string> domains = config.getDomains();

    for(auto i : domains)
    {
        request += " -d " + i;
    }

    sendCommand(request);
}

void CertEngine::copyCerts()
{
    if(termResponse.find("Certificate not yet due for renewal") != std::string::npos)
    {
        toLog("The current certificate is valid");
    }
    else
    {
        updated = true;
        if(config.isTargetDirSpecified())
        {
            toLog("- Copying certificates");
            std::string tempDir = config.getTargetDir() + "/temp";
            toLog("Temp dir: " + tempDir);
            if(std::filesystem::exists(tempDir))
            {
                toLog("Temp directory exist");
            }
            else
            {
                toLog("temp directory is not exist");
                sendCommand("mkdir -p " + tempDir);
            }

            std::string certDir = config.getCertDir() + "/" + config.getDomains()[0];
            std::string certFilePath;

            toLog("Certificates dir: " + certDir);

            for (const auto & entry : std::filesystem::directory_iterator(certDir)){

                toLog(entry.path());

                std::string fileName = std::filesystem::path(entry.path()).filename().string();

                certFilePath = entry.path();

                if(config.getCertConvertParam() & NO || config.getCertConvertParam() == 0 || fileName.find("fullchain") != std::string::npos)
                {
                    toLog("Copying file: " + fileName);
                    std::string request = "cp ";
                    request += certFilePath + " " + tempDir + "/" + fileName;
                    sendCommand(request);
                    if(fileName.find("fullchain") != std::string::npos)
                    {
                        fileToConvert = tempDir + "/" + fileName;
                        toLog("File to convert: " + fileToConvert);
                    }
                }
            }
        }
    }
}

void CertEngine::convertCerts()
{
    if(updated)
    {
        toLog("- Converting certificates");
        std::string baseRequest = "sudo openssl x509 -outform der -in ";
        std::string request;

        if(config.getCertConvertParam() & KEY)
        {
            request = baseRequest + fileToConvert + " -out " + config.getTargetDir() + "/" + config.getCertName() + ".key";
            toLog("Convert to key request: " + request);
            sendCommand(request);
        }

        if(config.getCertConvertParam() & CRT)
        {
            request = baseRequest + fileToConvert + " -out " + config.getTargetDir() + "/" + config.getCertName() + ".crt";
            toLog("Convert to crt request: " + request);
            sendCommand(request);
        }
    }
}
