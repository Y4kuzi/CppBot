#ifndef USER_H
#define USER_H

#include <vector>

#include "channelclass.hpp"

using namespace std; // prevent error: ‘string’ does not name a type

class Bot;

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
            cout << "Created user class for: "+this->nickname << endl;
        }

    string fullmask()
    {
        string mask = nickname+"!"+ident+"@"+host;
        return mask;
    }

};



#endif
