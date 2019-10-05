#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <vector>

#include "userclass.hpp"
#include "channelclass.hpp"

#include "modules.hpp"

using namespace std;

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
            //this->listen();
        }


        vector<Module *> registered_privmsg;

        void hook_privmsg(Module* mod)
        {
            std::cout << "Registered module." << std::endl;
            registered_privmsg.push_back(mod);
        }
        void notify_privmsg(Privmsg& p) {
            std::cout << "Notify ALL modules!" << std::endl;
            for (auto& mod : registered_privmsg) {
                std::cout << "Notifying a module..." << std::endl;
                mod->onPrivmsg(p);
            }
        }

    void event_raw(int raw, string line); // raw.cpp
    void connected_to_irc(); // welcome.cpp
    void event_privmsg(string line); // privmsg.cpp
    void event_join(string line); // join.cpp
    void event_part(string line); // part.cpp
    void event_kick(string line); // kick.cpp
    void event_nick(string line); // nick.cpp
    void load_modules();
    //void Bot::load_module() { Module* m = new Test(this); this->hook_privmsg(m); }


    void hello()
    {
        cout << "Hello." << endl;
    }

    void listen()
    {
        //this->load_modules();
        cout << "Listening..." << endl;
        while (1) {
                memset(&sockbuff, '\0', sizeof(sockbuff)); // make sure sockbuff[] is empty
                recv(this->sock, sockbuff, 4096, 0); // Receive all the data from server to sockbuff[]
                if (sockbuff[0] == '\0') { // If charray is empty
                    cout << "Lost connection." << endl;
                    break;
                }
                this->handle_recv(sockbuff);
        }
    }

    void raw(string data)
    {
        string d = data+"\r\n";
        send(this->sock, d.c_str(), d.size(), 0);
        stringstream ss(data);
        vector<string> vec;
        string word;
        while (ss >> word) { vec.push_back(word); }
        if (vec[0] != "PONG")
        {
            cout << "<< " << data << endl;
        }
    }

    bool is_user(string nickname)
    {
        for (int x = 0; x < this->users.size(); x++)
        {
            if (this->users[x].nickname == nickname)
            {
                return true;
            }
        }
        return false;
    }

    bool is_channel(string name)
    {
        for (int x = 0; x < this->channels.size(); x++)
        {
            if (this->channels[x].name == name)
            {
                return true;
            }
        }
        return false;
    }

    User& user_class(string nickname)
    {
        for (int x = 0; x < this->users.size(); x++)
        {
            if (this->users[x].nickname == nickname)
            {
                return this->users[x];
            }
        }
    }

    Channel& channel_class(string name)
    {
        for (int x = 0; x < this->channels.size(); x++)
        {
            //cout << "[FETCHCLASS] Does "+this->channels[x].name+" equal "+name+ "?" << endl;
            if (this->channels[x].name == name)
            {
                //cout << "Found matching class for "+nickname << endl;
                return this->channels[x];
            }
        }
    }

    void create_channel(string name)
    {
        if (is_channel(name))
        {
            cout << "Channel "+name+" already exists. Not creating." << endl;
            return;
        }
        Channel chan = Channel(name);
        this->channels.push_back(chan);
        //vector<User> users;
        //chan.users = users;
        cout << "Channel "+name+" created." << endl;
    }

    void say(string msg)
    {
        string target;
        if (is_channel(this->event_target))
        {
            Channel& c = channel_class(this->event_target);
            target = c.name;
        }
        else
        {
            target = this->event_target;
        }
        this->raw("PRIVMSG "+target +" :"+msg);
    }

    void handle_recv(string sockbuff) {
        stringstream ss(sockbuff);
        string line;
        while (getline(ss, line, '\n')) {

            stringstream ss(line);
            vector<string> vec;
            string word;
            while (ss >> word) { vec.push_back(word); }
            if (vec.size() == 0) {
                continue;
            }
            if (vec[0] == "PING") {
                this->raw("PONG "+vec[1]);
                continue;
            }
            cout << ">> " << line << endl;

            if (vec.size() > 2) {
                if (isNumber(vec[1])) {
                    int raw;
                    istringstream ( vec[1] ) >> raw;
                    event_raw(raw, line);
                }
                else {
                    string origin = vec[0].erase(0, 1);
                    string nick = origin.substr(0, origin.find("!") +0);
                    this->event_user = nick;
                    this->event_target = vec[2]; // todo: channel classes and assign instance here.
                    if (this->event_target.at(0) == ':') { // compare single char?
                        // strip :
                        this->event_target = this->event_target.erase(0, 1);
                    }

                    if (vec[1] == "PRIVMSG") { event_privmsg(line); }
                    else if (vec[1] == "JOIN") { event_join(line); }
                    else if (vec[1] == "PART") { event_part(line); }
                    else if (vec[1] == "KICK") { event_kick(line); }
                    else if (vec[1] == "NICK") { event_nick(line); }

                }
            }

        }
    }

};

#endif
