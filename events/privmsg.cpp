#include <iostream>
#include <algorithm> // for transform()
#include <vector>
#include <string>

vector<Module *> registered_privmsg;

void Bot::hook_privmsg(Module* mod)
{
    std::cout << "Registered module." << std::endl;
    registered_privmsg.push_back(mod);

}
void Bot::notify_privmsg(Privmsg& p) {
    std::cout << "Notify ALL modules!" << std::endl;
    for (auto& mod : registered_privmsg) {
        std::cout << "Notifying a module..." << std::endl;
        mod->onPrivmsg(p);
    }
}

void Bot::event_privmsg(string recv)
{
    stringstream ss(recv);
    vector<string> vec;
    string word;
    string trimmed_recv;
    while (ss >> word) { vec.push_back(word); }
    string origin = vec[0].erase(0, 1);

    vector<string> parts;
    boost::split(parts, origin, boost::is_any_of("!@"));
    string nick = parts[0];
    Channel& channel = channels_map.find(event_target)->second;

    if (users_map.count(nick))
    {
        string ident = parts[1];
        string host = parts[2];
        User& user = users_map.find(nick)->second;
        user.nickname = nick;
        //std::cout << "Setting nickname: "+ nick << std::endl;
        user.ident = ident;
        //std::cout << "Setting ident: "+ ident << std::endl;
        user.host = host;
        //std::cout << "Setting host: "+ host << std::endl;
        std::cout << "[PRIVMSG] Info of " +nick+ " updated: "+user.fullmask() << std::endl;
    }


    if (vec[3].size() == 1)
    {
        std::cout << "Current vec[3]: "+vec[3] << std::endl;
        std::rotate(vec.begin(), vec.begin()+1, vec.end());
        vec[3] = ":"+vec[3];
        std::cout << "New vec[3]: "+vec[3] << std::endl;
        vec.pop_back();
    }
    //for (auto& word : vec) {
    //    std::cout << "[" << word << "]" << std::endl;
    //}
    string prefix = vec[3];
    prefix = prefix.at(1);
    string cmd = vec[3];
    cmd.erase(0, 2);
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (prefix == bot_prefix) {
        // loop through modules to check for "privmsg" hook?

        if (cmd == "ping") {
            //cout << "Ping." << endl;
            say("PONG MOTHERFUCKER "+nick);
        }

        else if (cmd == "setprefix" and std::find(std::begin(admins), std::end(admins), nick) != admins.end() and vec.size() > 4) {
            bot_prefix = vec[4].at(0);
            say("Prefix changed to \""+bot_prefix+"\".");
        }

        else if (cmd == "whoami") {
            if (!users_map.count(event_user))
            {
                say("I don't know you.");
                return;
            }
            User& user = users_map.find(event_user)->second;
            say("This is what I know about you:");
            say("Your nickname is: "+user.nickname);
            say("Your ident is: "+user.ident);
            say("Your host is: "+user.host);
        }

        else if (cmd == "userlist") {
            say("I am woke about the following users:");
            for(auto it = users_map.cbegin(); it != users_map.cend(); ++it)
            {
                User& user = users_map.find(it->second.nickname)->second;
                say(user.fullmask());
                //say(it->second.fullmask());
            }
        }

        else if (cmd == "chanlist") {
            say("I am woke on the following channels:");
            for (auto it = channels_map.cbegin(); it != channels_map.cend(); ++it)
            {
                int usercount = it->second.users.size();
                std::string str_usercount = std::to_string(usercount);
                say(it->first+" -- Usercount: "+str_usercount); // << " " << it->second.first << " " << it->second.second << "\n";
            }
            /*
            for (int i = 0; i < channels.size(); i++)
            {
                int usercount = channels[i].users.size();
                std::string str_usercount = std::to_string(usercount);
                //string str_usercount = "N/A";
                say(channels[i].name+" -- Usercount: "+str_usercount);
            }
            */
        }

        else if (cmd == "c") {
            if (vec.size() < 5)
            {
                return;
            }
            if (!isNumber(vec[4]))
            {
                say("Invalid number.");
                return;
            }

            int exponent;
            long double result = 1;

            istringstream ( vec[4] ) >> exponent;

            while (exponent != 0) {
                result *= exponent;
                --exponent;
            }
            std::string str_result = std::to_string(result);
            say(str_result);
        }

        else if (cmd == "raw" and std::find(std::begin(admins), std::end(admins), nick) != admins.end()) {
            if (vec.size() < 5)
            {
                return;
            }
            string raw;
            for (int x = 4; x < vec.size(); x++)
            {
                raw = raw+" "+vec[x];
            }
            raw.erase(raw.begin(), std::find_if(raw.begin(), raw.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
            this->raw(raw);
        }

        if (users_map.count(event_user))
        {
            string msg;
            for (int x = 3; x < vec.size(); x++)
            {
                msg = msg+" "+vec[x];
            }
            msg.erase(msg.begin(), std::find_if(msg.begin(), msg.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
            User& user = users_map.find(nick)->second;
            Channel& channel = channels_map.find(event_target)->second;
            Privmsg p = Privmsg(user, channel, msg);
            std::cout << "Checking for modules..." << std::endl;
            notify_privmsg(p);

        }



    }
}
