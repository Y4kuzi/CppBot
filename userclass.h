#ifndef USER_H
#define USER_H

#include "channelclass.h"

class User {
    public:
        Bot* bot;
        std::string nickname;
        std::string ident = "*";
        std::string host = "*";
        vector<Channel> channels;
        User(Bot* bot, std::string nickname) : bot(bot), nickname(nickname)
        {
            //std::cout << "[User] Created user class for: "+nickname << std::endl;
        }

    std::string fullmask()
    {
        std::string mask = nickname+"!"+ident+"@"+host;
        return mask;
    }

    void changeNick(std::string newnick)
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
                    std::cout << "[userQuit] User "+nickname+" erased from channel "+channel.name+" vector." << std::endl;
                }
            }
        }
        bot->users_map.erase(nickname);
        std::cout << "[userQuit] User "+nickname+" erased from users_map" << std::endl;
    }

    void onPart(Channel &channel)
    {
        // Erasing User from Channel.users vector
        for (int x = 0; x < channel.users.size(); x++) {
            if (channel.users[x]->nickname == nickname) {
                channel.users.erase(channel.users.begin() + x);
                std::cout << "[onPart] User "+nickname+" erased from channel vector." << std::endl;
                break;
            }
        }

        // Erasing Channel from User.channels vector
        for (int x = 0; x < channels.size(); x++) {
            if (channels[x].name == channel.name) {
                channels.erase(channels.begin() + x);
                std::cout << "[onPart] Channel "+channel.name+" erased from user vector." << std::endl;
                break;
            }
        }
        if (channels.empty()) {
            std::cout << "[onPart] I no longer share a channel with "+nickname+". Simulating quit." << std::endl;
            userQuit();
        }
    }

};



#endif
