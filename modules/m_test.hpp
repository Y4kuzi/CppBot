#include <iostream>

class Test : public Module
{
    public:
      Test(Bot* bot) : Module(bot) {
          bot->hook_privmsg(this);
          bot->hook_nick(this);
          }
      void onPrivmsg(Privmsg& p) {
        std::cout << "Module call Test" << std::endl;
        std::cout << "Privmsg.message: " << p.message << endl;
        if (p.message == ":!test")
        {
            this->bot->raw("PRIVMSG #Bots :Hoi? Ik sta in m_test.hpp o/");
        }
      }


      void onNick(Nick& p) {
        this->bot->raw("PRIVMSG #Bots :"+p.user.nickname+ " jij deed nickchange naar "+p.newnick+" broer");
      }

};
