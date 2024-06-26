#include "config.h"

Config::Config()
{

}

bool Config::isEmailValid(std::string &_email)
{
    // define a regular expression
       const std::regex pattern
          ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
       // try to match the string with the regular expression
       return std::regex_match(_email, pattern);
}

bool Config::isTargetDirSpecified()
{
    return targetDir != "Not specified";
}

bool Config::isCertNameSpecified()
{
    return certName != "Not specified";
}

void Config::readConfig()
{
    std::fstream cfgFile;
    cfgFile.open(path, std::fstream::in);

    if(cfgFile.is_open())
    {
        std::string tempStr;
        while(!cfgFile.eof())
        {
            tempStr.clear();
            std::getline(cfgFile, tempStr);

            if(!tempStr.empty())
            {
//                std::cout << "String from file: " << tempStr << "\n";
                if(tempStr.find("certDir") != std::string::npos)
                {
                    certDir = tempStr.substr(tempStr.find("=") + 1);
                }
                else if(tempStr.find("email") != std::string::npos)
                {
                    email = tempStr.substr(tempStr.find("=") + 1);
                }
                else if(tempStr.find("domain") != std::string::npos)
                {
                    std::string tempDomainStr = tempStr.substr(tempStr.find("=") + 1);
                    domains.push_back(tempDomainStr);
                }
                else if(tempStr.find("pass") != std::string::npos)
                {
                    userPass = tempStr.substr(tempStr.find("=") + 1);
                }
                else if(tempStr.find("targetDir") != std::string::npos)
                {
                    targetDir = tempStr.substr(tempStr.find("=") + 1);
                }
                else if(tempStr.find("certName") != std::string::npos)
                {
                    certName = tempStr.substr(tempStr.find("=") + 1);
                }
                else if(tempStr.find("convert") != std::string::npos)
                {
                    convertCert = std::stoi(tempStr.substr(tempStr.find("=") + 1));
                }
            }
        }
        if(email != "Not specified" && userPass !="Not specified" && !domains.empty())
        {
            emptyConfig = false;
        }
    }
    else
    {
        std::cout << "Couldn't read the config file!\n";
    }
    cfgFile.close();
}

void Config::writeConfig()
{
    if(certDir == "Not specified" || domains.empty() || userPass == "Not specified")
    {
        std::cout << "\t!!!Please enter all the necessary information!!!\n";
    }
    else
    {
        std::fstream cfgFile;
        cfgFile.open(path, std::fstream::out);

        if(cfgFile.is_open())
        {
            cfgFile << "certDir=" << certDir << "\n";
            cfgFile << "email=" << email << "\n";
            if(!domains.empty())
            {
                for(auto i : domains)
                {
                    cfgFile << "domain=" << i << "\n";
                }
            }

            cfgFile << "pass=" << userPass << "\n";
            cfgFile << "targetDir=" << targetDir << "\n";
            cfgFile << "certName=" << certName <<"\n";
            cfgFile << "convert=" << convertCert;

            std::cout << "The config file has been saved!\n";
        }
        else
        {
            std::cout << "Couldn't write the file file!\n";
        }
        cfgFile.close();
    }
}

void Config::clearConfig()
{
    certDir = "Not specified";
    email = "Not specified";
    domains.clear();
    std::cout << "\tThe configuration has been cleared\n";
}

void Config::setCertDir(std::string &_path)
{
    certDir = _path;
}

std::string Config::getCertDir()
{
    return certDir;
}

void Config::setEmail(std::string &_email)
{
    if(isEmailValid(_email))
    {
        email = _email;
    }
    else
    {
        std::cout << "The wrong email was entered\n";
    }
}

std::string Config::getEmail()
{
    return email;
}

void Config::addDomain(std::string &_domain)
{
    domains.push_back(_domain);
}

std::vector<std::string> Config::getDomains()
{
    return domains;
}

void Config::removeDomain(int _num)
{
    if(domains.empty())
    {
        std::cout << "Config class: Domains are not specified\n";
    }
    else
    {
        std::cout << domains[_num] << " - deleted!\n";
        domains.erase(domains.begin() + _num);
    }
}

int Config::getDomainsNum()
{
    return domains.size();
}

void Config::setUserPass(std::string &_pass)
{
    userPass = _pass;
}

std::string Config::getUserPass()
{
    return userPass;
}

bool Config::haveUserPassword()
{
    return userPass != "Not specified";
}

void Config::setTargetDir(std::string _path)
{
    targetDir = _path;
}

std::string Config::getTargetDir()
{
    return targetDir;
}

void Config::setCertName(std::string _name)
{
    certName = _name;
}

std::string Config::getCertName()
{
    return certName;
}

void Config::addCertConvertParam(ConvertParam _param)
{
    convertCert |= _param;
}

void Config::removeCerConvertParam(ConvertParam _param)
{
    convertCert &= ~(_param);
}

void Config::clearCertConvertParam()
{
    convertCert = 0;
}

int Config::getCertConvertParam()
{
    return convertCert;
}

bool Config::isEmpty()
{
    return emptyConfig;
}
