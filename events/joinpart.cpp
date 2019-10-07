#include <iostream>
#include <vector>

vector<Module *> registered_join;
vector<Module *> registered_part;

// Join hook
void Bot::hook_join(Module* mod)
{
    std::cout << "[JOIN] Registered module." << std::endl;
    registered_join.push_back(mod);

}
void Bot::notify_join(Join& p) {
    //std::cout << "[JOIN] Notify ALL modules!" << std::endl;
    for (auto& mod : registered_join) {
        //std::cout << "[JOIN] Notifying a module..." << std::endl;
        mod->onJoin(p);
    }
}


// Part hook
void Bot::hook_part(Module* mod)
{
    std::cout << "Registered module." << std::endl;
    registered_part.push_back(mod);

}
void Bot::notify_part(Part& p) {
    //std::cout << "[PART] Notify ALL modules!" << std::endl;
    for (auto& mod : registered_part) {
        //std::cout << "[PART] Notifying a module..." << std::endl;
        mod->onPart(p);
    }
}

void Bot::event_join(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }

    if (!users_map.count(event_user)) {
        create_user(event_user);
        }

    if (!channels_map.count(event_target)) {
        create_channel(event_target);
        }

    User& user = users_map.find(event_user)->second;
    Channel& channel = channels_map.find(event_target)->second;

    std::cout << "Check if user isin_channelusers_vector" << std::endl;
    std::cout << event_user << std::endl;
    std::cout << channel.name << std::endl;
    if (!isin_channelusers_vector(channel, event_user))
    {
        std::cout << "Pushing back to channel.users" << std::endl;
        channel.users.push_back(user);
        std::cout << "Done." << std::endl;
    }

    Join p = Join(user, channel);
    std::cout << "[JOIN] Checking for modules..." << std::endl;
    notify_join(p);

}

void Bot::event_part(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    cout << event_user +" has left channel channel "+ event_target << endl;

    User& user = users_map.find(event_user)->second;
    Channel& channel = channels_map.find(event_target)->second;

    for (int x = 0; x < channel.users.size(); x++)
    {
        if (channel.users[x].nickname == event_user)
        {
            channel.users.erase(channel.users.begin() + x);
            cout << "User "+user.nickname+" erased from channel vector." << endl;
            break;
        }
    }

    // I left, so remove channel class from Bot::channels
    if (user.nickname == nickname)
    {
        channels_map.erase(channel.name);
        std::cout << "[PART] Removed channel from map." << std::endl;
    }

    Part p = Part(user, channel);
    std::cout << "[PART] Checking for modules..." << std::endl;
    notify_part(p);


}
