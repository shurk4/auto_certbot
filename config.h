#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>


class Config
{
    std::string path = ".config.cfg";
    std::string certDir = "Not specified";
    std::string email = "Not specified";
    std::string userPass = "Not specified";

    std::vector<std::string> domains;

    bool emptyConfig = true;

    bool isEmailValid(std::string &_email);

public:
    void readConfig();
    void writeConfig();
    void clearConfig();

    void setCertDir(std::string &_path);
    std::string getCertDir();
    void setEmail(std::string &_email);
    std::string getEmail();
    void addDomain(std::string &_domain);
    std::vector<std::string> getDomains();
    void removeDomain(int num);
    int getDomainsNum();
    void setUserPass(std::string &_pass);
    std::string getUserPass();
    bool haveUserPassword();

    std::string getConfigPath();

    bool isEmpty();

    Config();
};

#endif // CONFIG_H
