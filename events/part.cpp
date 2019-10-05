#include <iostream>
#include <vector>

void Bot::event_part(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    cout << this->event_user +" has left channel channel "+ this->event_target << endl;
    if (this->event_user == this->nickname)
    {

        //this->channels.erase(std::remove(this->channels.begin(), this->channels.end(), this->event_target), this->channels.end());

        /*
        std::cout << "I left channel "+this->event_target+"." << std::endl;
        string active_channels;
        for (int i = 0; i < this->channels.size(); i++)
        {
            active_channels = active_channels + this->channels[i];
        }
        std::cout << "[PART] Active channels now: "+active_channels << std::endl;

        */
    }


    // fetch User class of event_user and remove from Channel class event_target
}
