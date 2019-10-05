#include <iostream>
#include <vector>

void Bot::event_kick(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string target = vec[3];
    cout << this->event_user +" kicked "+target+" off channel "+ this->event_target << endl;
    if (target == this->nickname)
    {
        Channel& channel = this->channel_class(this->event_target);
        //this->channels.erase(std::remove(this->channels.begin(), this->channels.end(), this->event_target), this->channels.end());
        std::cout << "I got kicked off channel "+this->event_target+"." << std::endl;

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
        this->raw("JOIN "+channel.name);
        this->say("Asshole.");
    }


    // fetch User class of event_user and remove from Channel class event_target

}
