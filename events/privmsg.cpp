#include <iostream>
#include <algorithm> // for transform()
#include <vector>
#include <string>

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

    if (this->is_user(nick))
    {
        string ident = parts[1];
        string host = parts[2];
        User& user = this->user_class(nick);
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
            this->say("PING MOTHERFUCKER "+nick);
        }

        else if (cmd == "setprefix" and std::find(std::begin(admins), std::end(admins), nick) != admins.end() and vec.size() > 4) {
            bot_prefix = vec[4].at(0);
            this->say("Prefix changed to \""+bot_prefix+"\".");
        }

        else if (cmd == "whereami") {
            this->say("I am on the following channels:");
            for (int i = 0; i < this->channels.size(); i++)
            {
                this->say(this->channels[i].name);
            }
        }

        else if (cmd == "userlist") {
            this->say("I am woke about the following users:");
            for (int i = 0; i < this->users.size(); i++)
            {
                this->say(this->users[i].fullmask());
            }
        }

        else if (cmd == "chanlist") {
            this->say("I am woke on the following channels:");
            for (int i = 0; i < this->channels.size(); i++)
            {
                int usercount = this->channels[i].users.size();
                std::string str_usercount = std::to_string(usercount);
                //string str_usercount = "N/A";
                this->say(this->channels[i].name+" -- Usercount: "+str_usercount);
            }
        }

        else if (cmd == "c") {
            if (vec.size() < 5)
            {
                return;
            }
            if (!isNumber(vec[4]))
            {
                this->say("Invalid number.");
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
            this->say(str_result);
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

        if (this->is_user(nick))
        {
            string msg;
            for (int x = 3; x < vec.size(); x++)
            {
                msg = msg+" "+vec[x];
            }
            User& user = this->user_class(nick);
            Channel& channel = this->channel_class(vec[2]);
            Privmsg p = Privmsg(user, channel, msg);
            std::cout << "Checking for modules..." << std::endl;
            this->notify_privmsg(p);
        }

    }
}