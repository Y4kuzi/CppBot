#ifndef _HEADER
#define _HEADER

#include <iostream>
#include <vector>

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
        string event_target; // where events like privmsg and join occur. todo: assign Channel clas
        vector<User> users;
        vector<Channel> channels;
        //User event_user_class; // whoever triggered the event. todo: assign User class
        //vector<string> channels;
        Bot(string nickname)
        {
            struct sockaddr_in addr;
            struct hostent *host;
            host = gethostbyname(server.c_str());
            addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons((unsigned short)port);
            this->sock = socket(AF_INET, SOCK_STREAM, 0);
            this->nickname = nickname;
            connect(this->sock, (struct sockaddr *)&addr, sizeof(addr));
            this->raw("NICK Sint-Ahmet_Lord-Ipsum");
            this->raw("USER ahmet 0 0 :Sint Ahmet C++ ding");
            this->event_user = event_user;
            //this->event_user_class = event_user_class;
            this->event_target = event_target;
            this->users = users;
            this->channels = channels;
        }


    // Declaring hooks.

    void notify_privmsg(Privmsg& p);
    void hook_privmsg(Module* mod);

    //void notify_notice(Notice& p);
    //void hook_notice(Module* mod);

    void notify_nick(Nick& p);
    void hook_nick(Module* mod);


    void event_raw(int raw, string line); // raw.cpp
    void connected_to_irc(); // welcome.cpp
    void event_privmsg(string line); // privmsg.cpp
    void event_join(string line); // join.cpp
    void event_part(string line); // part.cpp
    void event_kick(string line); // kick.cpp
    void event_nick(string line); // nick.cpp
    void load_modules();
    void hello();
    void listen();
    void raw(string data);
    bool is_user(string nickname);
    bool is_channel(string name);
    User& user_class(string nickname);
    Channel& channel_class(string name);
    void create_channel(string name);
    void say(string msg);
    void handle_recv(string sockbuff);
    void insert_in_channels(Channel& channel, User& user);
};

#endif
