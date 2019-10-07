#ifndef _HEADER
#define _HEADER

#include <iostream>
#include <vector>
#include <map>

#include "userclass.hpp"
#include "channelclass.hpp"
#include "modules.hpp"

bool isNumber(string s)
// Check if s is a number.
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

string bot_prefix = "!";
string server = "irc.devirc.net";

string c[] = {"##", "#Bots"};
string a[] {"Sirius", "Y4kuzi", "Maanalainen"};

vector<string> irc_channel (c, c + sizeof(c) / sizeof(c[0]) );
vector<string> admins (a, a + sizeof(a) / sizeof(a[0]) );

using namespace std;


int port = 6667;

class Bot {
    public:
        char sockbuff[4096];
        int sock;
        string nickname;
        string event_user;
        string event_target;
        map<string, User> users_map;
        map<string, Channel> channels_map;

        Bot(string nickname)
        {
            struct sockaddr_in addr;
            struct hostent *host;
            host = gethostbyname(server.c_str());
            addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons((unsigned short)port);
            sock = socket(AF_INET, SOCK_STREAM, 0);
            //this->nickname = nickname;
            connect(sock, (struct sockaddr *)&addr, sizeof(addr));
            raw("NICK Sint-Ahmet_Lord-Ipsum");
            raw("USER ahmet 0 0 :Sint Ahmet C++ ding");
        }


    // Declaring hooks.

    void notify_privmsg(Privmsg& p);
    void hook_privmsg(Module* mod);

    void notify_nick(Nick& p);
    void hook_nick(Module* mod);

    void notify_join(Join& p);
    void hook_join(Module* mod);

    void notify_part(Part& p);
    void hook_part(Module* mod);


    void event_raw(int raw, string line); // raw.cpp
    void event_privmsg(string line); // privmsg.cpp
    void event_join(string line); // join.cpp
    void event_part(string line); // part.cpp
    void event_kick(string line); // kick.cpp
    void event_quit(string line); // quit.cpp
    void event_nick(string line); // nick.cpp
    void load_modules();
    void listen();
    void raw(string data);
    bool isin_channelusers_vector(Channel& channel, string nickname);
    void create_user(string name);
    void create_channel(string name);
    void say(string msg);
    void handle_recv(string sockbuff);

};

#endif
