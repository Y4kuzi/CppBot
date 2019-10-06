#include <iostream>
#include <vector>

vector<Module *> registered_nick;
void Bot::hook_nick(Module* mod)
{
    std::cout << "Registered NICK module." << std::endl;
    registered_nick.push_back(mod);
}
void Bot::notify_nick(Nick& p) {
    std::cout << "Notify ALL modules!" << std::endl;
    for (auto& mod : registered_nick) {
        std::cout << "Notifying a module..." << std::endl;
        mod->onNick(p);
    }
}


void Bot::event_nick(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string newnick = vec[2];
    if (newnick.at(0) == ':') { // compare single char?
        // strip :
        newnick = newnick.erase(0, 1);
    }
    if (this->event_user == this->nickname)
    {
        cout << "[NICK] Internal nickname set to: "+this->nickname << endl;
    }
    cout << "[NICK] "+this->event_user +" changed nickname to "+newnick << endl;

    User& user = this->user_class(this->event_user);
    cout << "[NICK] Current known nickname: "+user.nickname << endl;
    string oldnick = user.nickname;
    Nick p = Nick(user, newnick);
    std::cout << "[NICK] Checking for hooks..." << std::endl;
    this->notify_nick(p);

    // Finally, change the nickname property of the User.
    user.nickname = newnick;

}
