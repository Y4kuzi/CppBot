#include <iostream>
#include <vector>

void Bot::event_join(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string nick = vec[0].erase(0, 1);
    string channel = vec[2];
    if (channel.at(0) == ':') { // compare single char?
        // strip :
        channel = channel.erase(0, 1);
    }
    nick = nick.substr(0, nick.find("!") +0);
    if (!this->is_user(nick))
    {
        User u = User(nick);
        this->users.push_back(u);
        std::cout << "[JOIN] Added "+nick+" to userlist." << std::endl;
    }
    if (nick == this->nickname)
    {
        this->create_channel(this->event_target);
    }
    else
    {
        this->say("Hello, "+nick);
    }
}

void Bot::event_part(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    cout << this->event_user +" has left channel channel "+ this->event_target << endl;
    if (this->event_user == this->nickname)
    {
        Channel& chan = this->channel_class(this->event_target);
        for (int x = 0; x < this->channels.size(); x++)
        {
            if (this->channels[x].name == chan.name)
            {
                this->channels.erase(this->channels.begin() + x);
                cout << "Channel "+chan.name+" erased from vector." << endl;
                break;
            }
        }
        //this->channels.erase(std::remove(this->channels.begin(), this->channels.end(), chan), this->channels.end());
        /*
        std::cout << "I left channel "+chan.name+"." << std::endl;
        string active_channels;
        for (int i = 0; i < this->channels.size(); i++)
        {
            active_channels = active_channels + this->channels[i].name;
        }
        std::cout << "[PART] Active channels now: "+active_channels << std::endl;
        */
    }


    // fetch User class of event_user and remove from Channel class event_target
}
