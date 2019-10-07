#include <iostream>
#include <vector>

void Bot::event_kick(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string target = vec[3];
    cout << event_user +" kicked "+target+" off channel "+ event_target << endl;
    if (target == nickname)
    {
        Channel& channel = channels_map.find(event_target)->second;
        std::cout << "I got kicked off channel "+event_target+"." << std::endl;
        channels_map.erase(channel.name);
        raw("JOIN "+channel.name);
        say("Asshole.");
    }


    // fetch User class of event_user and remove from Channel class event_target

}
