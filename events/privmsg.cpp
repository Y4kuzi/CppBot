#include <iostream>
#include <algorithm> // for transform()
#include <vector>
#include <string>

vector<Module *> registered_privmsg;
void Bot::hook_privmsg(Module* mod)
{
    registered_privmsg.push_back(mod);

}
void Bot::notify_privmsg(Privmsg& p)
{
    //std::cout << "Notify ALL modules! (/PRIVMSG)" << std::endl;
    for (auto& mod : registered_privmsg) {
        //std::cout << "Notifying a module..." << std::endl;
        mod->onPrivmsg(p);
    }
}

void Bot::event_privmsg(string recv)
{
	cout << recv << endl;
    stringstream ss(recv);
    vector<string> vec;
    string word;
    string trimmed_recv;
    while (ss >> word) { vec.push_back(word); }
    string origin = vec[0].erase(0, 1);
    vector<string> parts;
    boost::split(parts, origin, boost::is_any_of("!@"));
    string nick = parts[0];
	cout << event_target << endl;
    Channel &channel = channels_map.find(event_target)->second;
    if (users_map.count(nick)) {
        string ident = parts[1];
        string host = parts[2];
        User &user = users_map.find(nick)->second; // & in declaration, so reference to users_map.find(nick)->second
												   // Remember, users_map contains the "real" Users. This allows us to change values.
        user.nickname = nick;
        user.ident = ident;
        user.host = host;
        //std::cout << "[PRIVMSG] Info of " +nick+ " updated: "+user.fullmask() << std::endl;
    }

	// Workaround for stuff like: PRIVMSG #channel : message here (note the space after colon)
    if (vec[3].size() == 1) {
        std::rotate(vec.begin(), vec.begin()+1, vec.end());
        vec[3] = ":"+vec[3];
        vec.pop_back();
    }

    string prefix = vec[3];
    prefix = prefix.at(1);
    string cmd = vec[3];
    cmd.erase(0, 2);
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (prefix == bot_prefix) {
        if (cmd == "ping")
            say("PONG MOTHERFUCKER "+nick);

        else if (cmd == "setprefix" && std::find(std::begin(admins), std::end(admins), nick) != admins.end() && vec.size() > 4) {
            bot_prefix = vec[4].at(0);
            say("Prefix changed to \""+bot_prefix+"\".");
        }

        else if (cmd == "whoami") {
            if (!users_map.count(event_user)) {
                say("I don't know you.");
                return;
            }
            User &user = users_map.find(event_user)->second;
            say("This is what I know about you:");
            say("Your nickname is: "+user.nickname);
            say("Your ident is: "+user.ident);
            say("Your host is: "+user.host);
        }

        else if (cmd == "whoishere") {
            //Channel& channel = channels_map.find(event_target)->second;
            for (auto &u : channel.users)
                say("I found: "+u->fullmask()+" on "+channel.name);
        }

        else if (cmd == "userlist") {
            say("I am woke about the following users:");
            for (auto &u : users_map)
                say(u.second.fullmask());
        }

        else if (cmd == "chanlist") {
            say("I am woke on the following channels:");
            for (auto &c : channels_map) {
                int usercount = c.second.users.size();
                std::string str_usercount = std::to_string(usercount);
                say(c.first+" -- Usercount: "+str_usercount);
            }
        }

        else if (cmd == "c") {
            if (vec.size() < 5) {
                return;
            }
            if (!isNumber(vec[4])) {
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

        else if (cmd == "join") {
			string chan = vec[4];
            char first = chan.at(0);
            if (chantypes.find(first) == string::npos) { // Not found
               //say("Invalid channel type '"+first+"'. Valid types are: "+chantypes);
               say("Invalid channel type. Valid types are: "+chantypes);
               return;
            }

			// Check if I am already on that channel. channels_map.count(nick)
			for (auto &c : channels_map) {
				string chan_loop = c.second.name;
				transform(chan_loop.begin(), chan_loop.end(), chan_loop.begin(), ::tolower);
				transform(chan.begin(), chan.end(), chan.begin(), ::tolower);
				if (chan_loop == chan) {
					say("I am already on channel "+c.second.name+".");
					return;
				}
			}

			say("Attempting to join "+vec[4]);
			raw("JOIN "+vec[4]);
        }

        else if (cmd == "part") {
			string chan = vec[4];
            char first {chan.at(0)};
            if (chantypes.find(first) == string::npos) { // Not found
               //say("Invalid channel type '"+first+"'. Valid types are: "+chantypes);
               say("Invalid channel type. Valid types are: "+chantypes);
               return;
            }

			// Check if I am already on that channel. channels_map.count(nick)
			bool found {false};
			for (auto &c : channels_map) {
				string chan_loop = c.second.name;
				transform(chan_loop.begin(), chan_loop.end(), chan_loop.begin(), ::tolower);
				transform(chan.begin(), chan.end(), chan.begin(), ::tolower);
				if (chan_loop == chan) {
					found = true;
					chan = c.second.name;
					break;
				}
			}
			if (!found) {
				say("I am not on that channel.");
				return;
			}
			say("Leaving "+chan);
			raw("PART "+chan);
        }

        else if (cmd == "raw" && std::find(std::begin(admins), std::end(admins), nick) != admins.end()) {
            if (vec.size() < 5) {
                return;
            }
            string rawcmd;
            for (int x = 4; x < static_cast<int>(vec.size()); x++) { // Or instead of static_cast just use unsigned x in declaration.
                rawcmd = rawcmd+" "+vec[x];
            }
            rawcmd.erase(rawcmd.begin(), std::find_if(rawcmd.begin(), rawcmd.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
            raw(rawcmd);
        }

        if (users_map.count(event_user)) {
            string msg;
            for (int x = 3; x < static_cast<int>(vec.size()); x++) { // Or instead of static_cast just use unsigned x in declaration.
                msg = msg+" "+vec[x];
            }
            msg.erase(msg.begin(), std::find_if(msg.begin(), msg.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
            User& user = users_map.find(nick)->second;
            //Channel& channel = channels_map.find(event_target)->second;
            Privmsg p = Privmsg(user, channel, msg);
            //std::cout << "[PRIVMSG] Checking for modules..." << std::endl;
            notify_privmsg(p);
        }


    }
}
