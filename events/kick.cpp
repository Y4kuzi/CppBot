#include <iostream>
#include <vector>

vector<Module *> registered_kick;
void Bot::hook_kick(Module* mod)
{
    registered_kick.push_back(mod);

}
void Bot::notify_kick(Kick& p) {
    for (auto& mod : registered_kick) {
        //std::cout << "[KICK] Notifying a module..." << std::endl;
        mod->onKick(p);
    }
}

void Bot::event_kick(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string target = vec[3];
    cout << event_user +" kicked "+target+" off channel "+ event_target << endl;
    User& user = users_map.find(target)->second;
    Channel& channel = channels_map.find(event_target)->second;
    user.onPart(channel);
    if (target == nickname)
    {
        Channel& channel = channels_map.find(event_target)->second;
        std::cout << "I got kicked off channel "+event_target+"." << std::endl;
        channels_map.erase(channel.name);
        raw("JOIN "+channel.name);
        say("Asshole.");
    }
    Kick p = Kick(user, channel);
    std::cout << "[KICK] Checking for modules..." << std::endl;
    notify_kick(p);
}
