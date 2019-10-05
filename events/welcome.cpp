#include <iostream>

void Bot::connected_to_irc() {
    cout << "Connected to the server, raw 001!" << endl;
    for (int x = 0; x < irc_channel.size(); x++)
    {
        this->raw("JOIN "+irc_channel[x]);
    }
}