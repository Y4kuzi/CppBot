#include <iostream>

class Test : public Module
{
    public:
      Test(Bot* bot) : Module(bot) {
          bot->hook_privmsg(this);
          bot->hook_join(this);
          bot->hook_nick(this);
          }
      void onPrivmsg(Privmsg& p) {
        if (p.message == ":!test")
        {
            bot->say("Hoi? Ik sta in m_test.hpp o/");
        }
      }

    void onNotice(Notice& p) { }

      void onNick(Nick& p) {
        //bot->raw("PRIVMSG #Bots :"+p.user.nickname+ " jij deed nickchange naar "+p.newnick+", broer");
      }

    void onJoin(Join& p) { }
    void onPart(Part& p) {}
    void onKick(Part& p) {}
    void onQuit(Quit& p) {}
};

class TestNew {
    public:
        Bot* bot;
        User user;
        std::string target;
        // todo: some other data to read
        TestNew(Bot* bot, User user, std::string target) : bot(bot), user(user), target(target)
        {
        }
};
