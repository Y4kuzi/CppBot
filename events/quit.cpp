#include <iostream>

void Bot::event_quit(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    User& user = users_map.find(event_user)->second;

    for (auto it = channels_map.cbegin(); it != channels_map.cend(); ++it)
    {
        Channel& channel = channels_map.find(it->second.name)->second;
        // Check if user was on this channel.

        for (int x = 0; x < channel.users.size(); x++)
        {
            cout << "[QUIT] I found "+channel.users[x]->nickname+" on "+channel.name << endl;
            if (channel.users[x]->nickname == user.nickname)
            {
                channel.users.erase(channel.users.begin() + x);
                cout << "[QUIT] User "+user.nickname+" erased from channel "+channel.name+" vector." << endl;
                //break;
            }
        }
    }
    users_map.erase(user.nickname);
    cout << "[QUIT] User "+user.nickname+" erased from users_map" << endl;
}