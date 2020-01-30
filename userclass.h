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
            std::cout << "[User] Created user class for: "+nickname << std::endl;
        }

    std::string fullmask()
    {
        std::string mask = nickname+"!"+ident+"@"+host;
        return mask;
    }

    void changeNick(std::string newnick)
    {
        User& user = bot->users_map.find(nickname)->second;
        cout << "this->nickname: " << nickname << endl;
        cout << "User& nickname: " << user.nickname << endl;
        if (nickname == bot->nickname) {
            nickname = newnick;
            return;
        }

        bot->users_map.emplace(newnick, user); 					// At this point, "user" still has the old nickname value.
																// All that has been done now, is assigning the new nickname to the current user object.

        bot->users_map.erase(nickname);							// Erase the old object with the current nickname. We have a new one now.
        nickname = newnick;
        User &new_user = bot->users_map.find(newnick)->second;

        for (auto &c : bot->channels_map) {
            Channel &channel = c.second;
            for (unsigned int x = 0; x < channel.users.size(); x++) {
                if (channel.users[x]->nickname == user.nickname) {
                    channel.users.erase(channel.users.begin() + x); // Erase old object from channel vector.
                    //cout << "Removed old user object " << user.nickname << " from channel vector." << endl;
                    channel.users.push_back(&new_user); // Adding newly created object to channel vector.
                    //cout << "Added new user object " << new_user.nickname << " to channel vector." << endl;
                    break;
                }
            }
        }


    }

    void userQuit()
    {
        for (auto &c : bot->channels_map) {
            Channel &channel = c.second;
            for (int x = 0; x < static_cast<int>(channel.users.size()); x++) { // Or instead of static_cast just use unsigned x in declaration.
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
        for (int x = 0; x < static_cast<int>(channel.users.size()); x++) { // Or instead of static_cast just use unsigned x in declaration.
            if (channel.users[x]->nickname == nickname) {
                channel.users.erase(channel.users.begin() + x);
                std::cout << "[onPart] User "+nickname+" erased from channel vector." << std::endl;
                break;
            }
        }

        // Erasing Channel from User.channels vector
        for (int x = 0; x < static_cast<int>(channels.size()); x++) { // Or instead of static_cast just use unsigned x in declaration.
            if (channels[x].name == channel.name) {
                channels.erase(channels.begin() + x);
                std::cout << "[onPart] Channel "+channel.name+" erased from user vector." << std::endl;
                break;
            }
        }
        if (channels.empty() && nickname != bot->nickname) {
            std::cout << "[onPart] I no longer share a channel with "+nickname+". Simulating quit." << std::endl;
            userQuit();
        }
    }

};


#endif
