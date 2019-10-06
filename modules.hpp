class Privmsg {
    public:
        User& user;
        Channel& channel;
        std::string message;
        Privmsg(User& user, Channel& channel, std::string message) : user(user), channel(channel), message(message)
        {
        //    this->user = user;
        //    this->channel = channel;
        //    this->message = message;
        }
};
class Bot;

/*
class Module {
    public:
        virtual void onPrivmsg(Bot& b, Privmsg& p) = 0;
};
*/


class Module {
    protected:
        Bot* bot;  // should always be *, never mix it. You probably can, but just don't...for now
    public:
        Module(Bot* bot) : bot(bot) {} // 'bot' MUST be passed along when constructing
        virtual void onPrivmsg(Privmsg& p) = 0;
};
