#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

enum ConvertParam
{
    NO = 0,
    KEY = 1,
    CRT = 2
};

class Config
{
    std::string path = ".config.cfg";
    std::string certDir = "/etc/letsencrypt/archive";
    std::string targetDir = "Not specified";
    std::string certName = "Not specified";
    std::string email = "Not specified";
    std::string userPass = "Not specified";
    std::vector<std::string> domains;
    int convertCert = 0;

    bool emptyConfig = true;

    bool isEmailValid(std::string &_email);

public:
    void readConfig();
    static Config getOtherConfig();
    void writeConfig();
    void clearConfig();

    void setCertDir(std::string &_path);
    std::string getCertDir();

    void setEmail(std::string &_email);
    std::string getEmail();

    void addDomain(std::string &_domain);
    std::vector<std::string> getDomains();
    void removeDomain(int _num);
    int getDomainsNum();

    void setUserPass(std::string &_pass);
    std::string getUserPass();
    bool haveUserPassword();

    void setTargetDir(std::string _path);
    std::string getTargetDir();

    void setCertName(std::string _name);
    std::string getCertName();

    void addCertConvertParam(ConvertParam _param);
    void removeCerConvertParam(ConvertParam _param);
    void clearCertConvertParam();
    int getCertConvertParam();

    bool isEmpty();
    bool isTargetDirSpecified();
    bool isCertNameSpecified();

    Config();
};

#endif // CONFIG_H
