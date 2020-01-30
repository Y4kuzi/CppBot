#include "header.h"
#include "bot.h"

#include "userclass.h"
#include "channelclass.h"
#include "modules.h"

#include "events/raw.cpp"
#include "events/privmsg.cpp"
#include "events/joinpart.cpp"
#include "events/kick.cpp"
#include "events/quit.cpp"
#include "events/nick.cpp"

void Bot::raw(string data)
{
    string d = data+"\r\n";
    send(sock, d.c_str(), d.size(), 0);
    stringstream ss(data);
    vector<string> vec;
    string word;
    while (ss >> word) vec.push_back(word);
    if (vec[0] != "PONG") cout << "<< " << data << endl;
}

void Bot::create_channel(string name)
{
    Channel channel = Channel(name);
    channels_map.emplace(channel.name, channel);
}

void Bot::create_user(string name)
{
    User user(this, name);
    users_map.emplace(user.nickname, user);
}

void Bot::handle_recv(string data) {
    stringstream ss(data);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    if (vec.size() == 0) {
        return;
	}
    if (vec[0] == "PING")
        raw("PONG "+vec[1]);

    if (vec.size() > 2) {
        if (vec[1] != "372") // Skip MOTD spam in cout.
            cout << ">> " << data << endl;

        if (isNumber(vec[1])) {
            int raw {};
            istringstream ( vec[1] ) >> raw;
            event_raw(raw, data);
        }
        else {
            string origin = vec[0].erase(0, 1);
            string nick = origin.substr(0, origin.find("!") +0);
            event_user = nick;
            event_target = vec[2];
            if (event_target.at(0) == ':')
                event_target = event_target.erase(0, 1);
            if (vec[1] == "PRIVMSG")
                event_privmsg(data);
            else if (vec[1] == "JOIN")
                event_join(data);
            else if (vec[1] == "PART")
                event_part(data);
            else if (vec[1] == "KICK")
                event_kick(data);
            else if (vec[1] == "QUIT")
                event_quit(data);
            else if (vec[1] == "NICK")
                event_nick(data);
        }
    }
}

void Bot::say(string msg)
{
    string target;
    if (channels_map.count(event_target)) {
        Channel &channel = channels_map.find(event_target)->second;
        target = channel.name;
    }
    else
        target = event_target;
    raw("PRIVMSG "+target +" :"+msg);
}

void Bot::load_modules()
{
    //Module* m;

    //#include "modules/m_test.hpp"
    //m = new Test(this);

    //#include "modules/m_8ball.hpp"
    //m = new _8Ball(this);

}

void Bot::listen()
{
    load_modules();
    //cout << "Listening..." << endl;
    char complete_char[512];
    int c_count = 0;

    while (1) {
        memset(&sockbuff, '\0', sizeof(sockbuff)); // make sure sockbuff[] is empty
        if (recv(sock, sockbuff, 512, 0)) { // Receive all the data from server to sockbuff[] 16384
            if (sockbuff[0] == '\0') { // If charray is empty
                cout << "Lost connection." << endl;
                break;
            }
            if (strlen(complete_char) > 0) {
                //cout << "We have data remaining from our previous read: " << complete_char << endl;
            }

            for (int i = 0; i < static_cast<int>(sizeof(sockbuff) / sizeof(sockbuff[0])); i++, c_count++) {
                if (sockbuff[i] == 0) // End of sockbuff has been reached, the rest is empty.
                    //cout << "Done reading complete sockbuff. Size was: " << i << endl;
                    break;
                if (sockbuff[i] != '\n') {
                    complete_char[c_count] = sockbuff[i];
                    continue;
                }
                handle_recv(complete_char);
                memset(&complete_char, '\0', sizeof(complete_char));
                c_count = -1;
            }
        }
    }
}

int main() {
    //Bot b1 = Bot("Lord-Ipsum_doctor_Sint-Ahmet");
    Bot b1 = Bot("hoi");
    b1.listen();
    return 0;
};
