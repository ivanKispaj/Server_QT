//
// Constants.h
// 2023-08-13
// Ivan Konishchev
//

#pragma once
#include <string>

class Constants
{


public:

    enum TableName
    {
        message, user
    };

    static struct _Constants
    {
        const std::string baseDIR{"./"};
        const std::string dataDIR{""};
        const std::string configFileName{"dbconf.config"};
        const std::string messagesFile{"Messages.txt"};
        const std::string usersFile{"Users.txt"};

    } dir;
};
