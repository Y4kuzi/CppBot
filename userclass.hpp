#ifndef USER_H
#define USER_H

#include <vector>
#include <map>

#include "channelclass.hpp"

using namespace std;

class Channel;

class User {
    public:
        Bot* bot;  // should always be *, never mix it. You probably can, but just don't...for now

        string nickname;
        string ident = "*";
        string host = "*";
        vector<Channel> channels;
        User(Bot* bot, string nickname) : bot(bot), nickname(nickname)
        {
            //cout << "[User] Created user class for: "+nickname << endl;
        }

    string fullmask()
    {
        string mask = nickname+"!"+ident+"@"+host;
        return mask;
    }

    void changeNick(string newnick)
    {
        User& user = bot->users_map.find(nickname)->second;
        bot->users_map.emplace(newnick, user);
        bot->users_map.erase(nickname);
        User& new_user = bot->users_map.find(newnick)->second;
        nickname = newnick;
        for (auto &c : bot->channels_map) {
            Channel &channel = c.second;
            for (int x = 0; x < channel.users.size(); x++) {
                if (channel.users[x]->nickname == user.nickname) {
                    channel.users.erase(channel.users.begin() + x); // Erase old object from channel vector.
                    channel.users.push_back(&new_user); // Adding newly created object to channel vector.
                    break;
                }
            }
        }
    }

    void userQuit()
    {
        for (auto &c : bot->channels_map) {
            Channel& channel = c.second;
            for (int x = 0; x < channel.users.size(); x++) {
                if (channel.users[x]->nickname == nickname) {
                    channel.users.erase(channel.users.begin() + x);
                    cout << "[userQuit] User "+nickname+" erased from channel "+channel.name+" vector." << endl;
                }
            }
        }
        bot->users_map.erase(nickname);
        cout << "[userQuit] User "+nickname+" erased from users_map" << endl;
    }

    void onPart(Channel &channel)
    {
        // Erasing User from Channel.users vector
        for (int x = 0; x < channel.users.size(); x++) {
            if (channel.users[x]->nickname == nickname) {
                channel.users.erase(channel.users.begin() + x);
                cout << "[onPart] User "+nickname+" erased from channel vector." << endl;
                break;
            }
        }

        // Erasing Channel from User.channels vector
        for (int x = 0; x < channels.size(); x++) {
            if (channels[x].name == channel.name) {
                channels.erase(channels.begin() + x);
                cout << "[onPart] Channel "+channel.name+" erased from user vector." << endl;
                break;
            }
        }
        if (channels.empty()) {
            cout << "[onPart] I no longer share a channel with "+nickname+". Simulating quit." << endl;
            userQuit();
        }
    }

};



#endif
