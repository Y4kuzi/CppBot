#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>

#include "userclass.hpp"

using namespace std;

class User;

class Channel {
    public:
        string name;
        vector<User> users;
        Channel(string name)
        {
            this->name = name;
            this->users = users;
        }
};

#endif
