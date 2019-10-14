class Privmsg { // todo: include vector<string> with message words
    public:
        User user;
        Channel channel;
        std::string message;
        Privmsg(User user, Channel channel, std::string message) : user(user), channel(channel), message(message)
        {
        }
};

class Notice {
    public:
        User user;
        Channel channel;
        std::string message;
        Notice(User user, Channel channel, std::string message) : user(user), channel(channel), message(message)
        {
        }
};

class Nick {
    public:
        User user;
        std::string newnick;
        Nick(User user, std::string newnick) : user(user), newnick(newnick)
        {
        }
};

class Join {
    public:
        User user;
        Channel channel;
        Join(User user, Channel channel) : user(user), channel(channel)
        {
        }
};

class Part {
    public:
        User user;
        Channel channel;
        Part(User user, Channel channel) : user(user), channel(channel)
        {
        }
};

class Kick {
    public:
        User user;
        Channel channel;
        Kick(User user, Channel channel) : user(user), channel(channel)
        {
        }
};

class Quit {
    public:
        User user;
        Quit(User user) : user(user)
        {
        }
};


class Module {
    public:
        Bot* bot;  // should always be *, never mix it. You probably can, but just don't...for now
        Module(Bot* bot) : bot(bot) { } // 'bot' MUST be passed along when constructing

        virtual void onPrivmsg(Privmsg& p) = 0;
        virtual void onNotice(Notice& p) = 0;
        virtual void onJoin(Join& p) = 0;
        virtual void onPart(Part& p) = 0;
        virtual void onKick(Kick& p) = 0;
        virtual void onQuit(Quit& p) = 0;
        virtual void onNick(Nick& p) = 0;
};
