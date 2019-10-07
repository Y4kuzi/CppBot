#include <iostream>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <cstring>
#include <netdb.h>
#include <sstream>
#include <boost/algorithm/string.hpp> // libboost-dev

#include "header.hpp"

#include "events/raw.cpp"
#include "events/privmsg.cpp"
#include "events/joinpart.cpp"
#include "events/kick.cpp"
#include "events/quit.cpp"
#include "events/nick.cpp"

void Bot::raw(string data)
{
    string d = data+"\r\n";
    send(this->sock, d.c_str(), d.size(), 0);
    stringstream ss(data);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    if (vec[0] != "PONG") { cout << "<< " << data << endl; }
}

void Bot::create_channel(string name)
{
    Channel channel = Channel(name);
    channels_map.emplace(channel.name, channel);
}

void Bot::create_user(string name)
{
    User user(name);
    users_map.emplace(user.nickname, user);
}

bool Bot::isin_channelusers_vector(Channel& channel, string nickname)
{
    //std::cout << "Checking channel.users vector..." << std::endl;
    //std::cout << channel.users.size() << std::endl;
    for (int x = 0; x < channel.users.size(); x++)
    {
        //std::cout << "Does "+channel.users[x].nickname+" equal "+nickname+"?" << std::endl;
        if (channel.users[x]->nickname == nickname) { return true; }
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
            raw("PONG "+vec[1]);
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
                event_user = nick;
                event_target = vec[2]; // todo: channel classes and assign instance here.
                User& event_user_class = users_map.find(nick)->second;
                if (event_target.at(0) == ':') { // compare single char?
                    // strip :
                    event_target = event_target.erase(0, 1);
                }

                if (vec[1] == "PRIVMSG") { event_privmsg(line); }
                else if (vec[1] == "JOIN") { event_join(line); }
                else if (vec[1] == "PART") { event_part(line); }
                else if (vec[1] == "KICK") { event_kick(line); }
                else if (vec[1] == "QUIT") { event_quit(line); }
                else if (vec[1] == "NICK") { event_nick(line); }

            }
        }

    }
}

void Bot::say(string msg)
{
    string target;
    if (channels_map.count(event_target))
    {
        Channel& channel = channels_map.find(event_target)->second;
        target = channel.name;
    }
    else
    {
        target = event_target;
    }
    raw("PRIVMSG "+target +" :"+msg);
}

void Bot::load_modules()
{
    Module* m;

    #include "modules/m_test.hpp"
    m = new Test(this);

    #include "modules/m_8ball.hpp"
    m = new _8Ball(this);

}

void Bot::listen()
{
    load_modules();
    cout << "Listening..." << endl;
    while (1) {
            memset(&sockbuff, '\0', sizeof(sockbuff)); // make sure sockbuff[] is empty
            recv(sock, sockbuff, 4096, 0); // Receive all the data from server to sockbuff[]
            if (sockbuff[0] == '\0') { // If charray is empty
                cout << "Lost connection." << endl;
                break;
            }
            handle_recv(sockbuff);
    }
}


int main() {
    Bot b1 = Bot("Sint-Ahmet_Lord-Ipsum");
    b1.listen();
    std::cout << "Wat?" << std::endl;
    return 0;
};
