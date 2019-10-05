#ifndef USER_H
#define USER_H

#include <vector>

#include "channelclass.hpp"

using namespace std; // prevent error: ‘string’ does not name a type

class User {
    public:
        string nickname;
        string ident = "*";
        string host = "*";
        vector<Channel> channels;
        User(string nickname)
        {
            this->nickname = nickname;
            this->ident = ident;
            this->host = host;
            this->channels = channels;
        }

    string fullmask()
    {
        string mask = this->nickname+"!"+this->ident+"@"+this->host;
        return mask;
    }

};



#endif
