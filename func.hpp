#include <iostream>
#include <filesystem>
#include <chrono>
#include <array>
#include <cstdio>

#include "config.h"

void sendCertRequest(Config &cfg);
void newLog();
void toLog(std::string _log);
void showConfig(Config &cfg);
void makeKeyCrtFiles(std::string &_path);

void sendCertRequest(Config &cfg)
{
    std::string passStr("echo " + cfg.getUserPass() + " | ");
    std::string request("sudo -S -k certbot certonly -n --standalone");
    std::string response;
    std::vector<std::string> domains = cfg.getDomains();

    for(auto i : domains)
    {
        request += " -d " + i;
    }

    toLog("Request: " + request);

    std::array<char, 128> buffer;
    std::string result;

    std::string command = passStr + request;
    toLog("Command: " + command);

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    toLog(result);

    std::string path = cfg.getCertDir() + "archive/" + cfg.getDomains()[0];

    toLog("Certificates dir: " + path);
    for (const auto & entry : std::filesystem::directory_iterator(path)){

        toLog(entry.path());
        std::string fileName = std::filesystem::path(entry.path()).filename().string();
        toLog("File name: " + fileName);
        if(fileName.find("fullchain") != std::string::npos)
        {
            toLog("Cert file: " + fileName);

        }
    }

    if(result.find("Certificate not yet due for renewal") != std::string::npos)
    {
        toLog("The current certificate is valid");
    }
    else
    {
        std::string path = cfg.getConfigPath() + "/" + cfg.getDomains()[0];

        for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
    }
}

void newLog()
{
    if(std::filesystem::exists("log.txt"))
    {
        std::remove("log.txt");
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
//    std::cout << "The system clock is currently at " << std::ctime(&currentTime);
    std::stringstream stream;
    stream << std::ctime(&currentTime);
    toLog(stream.str());
}

void toLog(std::string _log)
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

void showConfig(Config &cfg)
{
    toLog("Path to the certbot dir: " + cfg.getCertDir() + "\nYour e-mail: " + cfg.getEmail());

    std::vector<std::string> domains = cfg.getDomains();

    toLog("\nYour domains:");
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

    std::string tempStr = "User password: ";
    if(cfg.haveUserPassword())
    {
        tempStr += "specified";
    }
    else
    {
        tempStr += "Not specified";
    }

    toLog(tempStr + "\n");
}

void makeKeyCrtFiles(std::string &_path)
{
  /*
   * Добавить в конфиг:
   * выбор действий с полученным сертификатом
   * имя сертификата для приобразования в key crt
   * путь к папке в которую поместить готовые key crt
   *
   * Дописать функции преобразования и копирования сертификатов
  */
}
