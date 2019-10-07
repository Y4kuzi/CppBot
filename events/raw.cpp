#include <iostream>
#include <algorithm> // for std::remove
#include <vector>

void Bot::event_raw(int raw, string data) {
    stringstream ss(data);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    //std::cout << "Raw: "+raw << std::endl;
    switch(raw)
    {
        case 001:
            nickname = vec[2];
            std::cout << "Nickname set: "+nickname << endl;
            for (int x = 0; x < irc_channel.size(); x++)
            {
                this->raw("JOIN "+irc_channel[x]);
            }
            break;

        case 353: // names reply
            Channel& channel = channels_map.find(vec[4])->second;
            string nick;

            char chars[] = ":*!~&@%+.";
            for (int x = 5; x<vec.size(); x++) {
                nick = vec[x];
                for (int y = 0; y < strlen(chars); y++) {
                    nick.erase(std::remove(nick.begin(), nick.end(), chars[y]), nick.end());
                }

                if (!users_map.count(nick)) {
                    create_user(nick);
                    }

                User& user = users_map.find(nick)->second;
                if (!isin_channelusers_vector(channel, user.nickname))
                {
                    channel.users.push_back(&user);
                }
                user.channels.push_back(channel);
                //std::cout << "Channels: "+user.channels[0] << std::endl;
            };

            break;
    }
}