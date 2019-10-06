class Bot;
class Privmsg { // todo: include vector<string> with message words
    public:
        User& user;
        Channel& channel;
        std::string message;
        Privmsg(User& user, Channel& channel, std::string message) : user(user), channel(channel), message(message)
        {
        }
};

/*
class Notice {
    public:
        User& user;
        Channel& channel;
        std::string message;
        Notice(User& user, Channel& channel, std::string message) : user(user), channel(channel), message(message)
        {
        }
};
*/
class Nick {
    public:
        User& user;
        std::string newnick;
        Nick(User& user, std::string newnick) : user(user), newnick(newnick)
        {
        }
};


class Module {
    public:
        Bot* bot;  // should always be *, never mix it. You probably can, but just don't...for now
        Module(Bot* bot) : bot(bot) {} // 'bot' MUST be passed along when constructing
        virtual void onPrivmsg(Privmsg& p) = 0;
        //virtual void onNotice(Notice& p) = 0;
        virtual void onNick(Nick& p) = 0;
};
//*/