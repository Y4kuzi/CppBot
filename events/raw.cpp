#include <iostream>
#include <algorithm> // for std::remove
#include <vector>

void Bot::event_raw(int raw, string data) {
    stringstream ss(data);
    vector<string> vec;
    string word;
    while (ss >> word) vec.push_back(word);
    switch(raw)
    {
        case RPL_WELCOME:
            nickname = vec[2];
            cout << "Welcome. Nickname set: "+nickname << endl;
            for (int x = 0; x < irc_channel.size(); x++)
                this->raw("JOIN "+irc_channel[x]);
            break;

        case RPL_ISUPPORT:
            for (int x = 3; x < vec.size(); x++) {
                vector<std::string> raw_parts;
                boost::split(raw_parts, vec[x], boost::is_any_of("="));
                if (vec[x].find("CHANTYPES=") != string::npos) {
                    cout << "Found chantypes in word: "+vec[x] << endl;
                    chantypes = raw_parts[1];
                }

            }
            break;

        case RPL_NAMEREPLY:
            {
                Channel& channel = channels_map.find(vec[4])->second;
                string nick;

                char remove_chars[] = ":*!~&@%+.";
                for (int x = 5; x<vec.size(); x++) {
                    nick = vec[x];
                    for (int y = 0; y < strlen(remove_chars); y++)
                        nick.erase(std::remove(nick.begin(), nick.end(), remove_chars[y]), nick.end());

                    if (!users_map.count(nick))
                        create_user(nick);

                    User &user = users_map.find(nick)->second;
                    if (find(channel.users.begin(), channel.users.end(), &user) == channel.users.end())
                        channel.users.push_back(&user);

                    user.channels.push_back(channel);
                };
            }
            break;

        case RPL_ENDOFMOTD:
            break;

    }
}
