#ifndef CHANNEL_H
#define CHANNEL_H

#include "userclass.h"

class Channel {
    public:
        string name;
        vector<User*> users;
        Channel(std::string name) : name(name)
        {
        }
};

#endif
