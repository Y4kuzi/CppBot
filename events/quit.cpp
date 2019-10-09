#include <iostream>

void Bot::event_quit(string recv)
{
    User& user = users_map.find(event_user)->second;
    user.userQuit();
}