#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>

#include "userclass.hpp"

using namespace std; // prevent error: ‘string’ does not name a type

class User;

class Channel {
    public:
        string name;
        vector<User> users;
        Channel(string name)
        {
            this->name = name;
            this->users = users;
            //Bot::create_chan_class(this->name);
        }
};

#endif
