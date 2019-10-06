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
            this->nickname = vec[2];
            std::cout << "Nickname set: "+this->nickname << endl;
            for (int x = 0; x < irc_channel.size(); x++)
            {
                this->raw("JOIN "+irc_channel[x]);
            }
            break;

        case 353: // names reply
            std::cout << "Raw names reply" << std::endl;
            std::cout << "Data: "+data << std::endl;
            //string channel = vec[4];
            Channel& channel = this->channel_class(vec[4]);
            string nick;

            char chars[] = ":*!~&@%+.";
            for (int x = 5; x<vec.size(); x++) {
                nick = vec[x];
                for (int y = 0; y < strlen(chars); y++) {
                    nick.erase(std::remove(nick.begin(), nick.end(), chars[y]), nick.end());
                }

                // only do this if not yet exist.

                if (!this->is_user(nick))
                {
                    User u = User(nick);
                    this->users.push_back(u);
                    std::cout << "[NAMES] Added "+nick+" to userlist." << std::endl;
                }
                User& user = this->user_class(nick);
                channel.users.push_back(user);
                user.channels.push_back(channel);
                //std::cout << "Channels: "+user.channels[0] << std::endl;
            };

            break;
    }
}