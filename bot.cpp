#include <iostream>
//#include <sys/socket.h>
//#include <sys/types.h>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <boost/algorithm/string.hpp> // libboost-dev

#include "header.hpp" // Fully define Bot class

#include "events/raw.cpp"
#include "events/joinpart.cpp"
#include "events/privmsg.cpp"
#include "events/kick.cpp"
#include "events/nick.cpp"


void Bot::hello()
{
    cout << "Hello." << endl;
}

void Bot::raw(string data)
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

void Bot::create_channel(string name)
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

User& Bot::user_class(string nickname)
{
    for (int x = 0; x < this->users.size(); x++)
    {
        if (this->users[x].nickname == nickname)
        {
            return this->users[x];
        }
    }
}

Channel& Bot::channel_class(string name)
{
    cout << "calling channel_class() for "+name << endl;
    cout << this->channels.size() << endl;
    for (int x = 0; x < this->channels.size(); x++)
    {
        cout << "[FETCHCLASS] Does "+this->channels[x].name+" equal "+name+ "?" << endl;
        if (this->channels[x].name == name)
        {
            cout << "Found matching class for "+nickname << endl;
            return this->channels[x];
        }
    }
}

bool Bot::is_user(string nickname)
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
bool Bot::is_channel(string name)
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

void Bot::handle_recv(string sockbuff) {
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

void Bot::say(string msg)
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

void Bot::load_modules()
{
    #include "modules/m_test.hpp"
    Module* m;
    m = new Test(this);

    #include "modules/m_8ball.hpp"
    m = new _8Ball(this);

}

void Bot::listen()
{
    this->load_modules();
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

int main() {
        Bot b1 = Bot("Sint-Ahmet_Lord-Ipsum");
        b1.listen();
        std::cout << "Wat?" << std::endl;
        return 0;
};
