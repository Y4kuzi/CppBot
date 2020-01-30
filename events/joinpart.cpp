#include <iostream>
#include <vector>

vector<Module *> registered_join;
vector<Module *> registered_part;

void Bot::hook_join(Module* mod)
{
    registered_join.push_back(mod);

}
void Bot::notify_join(Join& p)
{
    for (auto& mod : registered_join)
        mod->onJoin(p);
}


void Bot::hook_part(Module* mod)
{
    registered_part.push_back(mod);

}
void Bot::notify_part(Part& p)
{
    for (auto& mod : registered_part)
        mod->onPart(p);
}

void Bot::event_join(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string origin = vec[0].erase(0, 1);

    vector<string> parts;
    boost::split(parts, origin, boost::is_any_of("!@"));
    if (!users_map.count(event_user))
        create_user(event_user);

    if (!channels_map.count(event_target))
        create_channel(event_target);

    User &user = users_map.find(event_user)->second;
    Channel &channel = channels_map.find(event_target)->second;

    string ident = parts[1];
    string host = parts[2];
    user.ident = ident;
    user.host = host;

    cout << "[JOIN] Pushing back to channel.users" << endl;
    channel.users.push_back(&user);
    Join p = Join(user, channel);
    notify_join(p);
}

void Bot::event_part(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) vec.push_back(word);
    cout << "[PART] "+event_user+" has left channel channel "+ event_target << endl;

    User &user = users_map.find(event_user)->second;
    Channel &channel = channels_map.find(event_target)->second;
    user.onPart(channel);

    if (user.nickname == nickname) {
        channels_map.erase(channel.name);
        std::cout << "[PART] Removed channel from map." << std::endl;
        //channel.users.clear(); // Needs testing.
    }

    Part p = Part(user, channel);
    notify_part(p);
}
