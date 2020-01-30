#include <iostream>
#include <vector>

vector<Module *> registered_nick;
void Bot::hook_nick(Module* mod)
{
    registered_nick.push_back(mod);
}
void Bot::notify_nick(Nick& p) {
    for (auto& mod : registered_nick) {
        //std::cout << "[NICK] Notifying a module..." << std::endl;
        mod->onNick(p);
    }
}

void Bot::event_nick(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string nnick = vec[2];
    if (nnick.at(0) == ':') {
        nnick = nnick.erase(0, 1);
    }
    if (event_user == nickname) {
        cout << "[NICK] Internal nickname set to: "+nickname << endl;
    }
    cout << "[NICK] "+event_user +" changed nickname to "+nnick << endl;
    User& user = users_map.find(event_user)->second;
    user.changeNick(nnick);
    string oldnick = user.nickname;
    cout << "[NICK] Current known nickname: "+user.nickname << endl;
    Nick p = Nick(user, nnick);
    std::cout << "[NICK] Checking for hooks..." << std::endl;
    notify_nick(p);
}
