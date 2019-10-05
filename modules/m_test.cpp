#include <iostream>

class Test : public Module
{
  Test(Bot* bot) : Module(bot) {
      //bot->hello();
      bot->hook_privmsg(this);
      } // will call parent's (Module) constructor. Good.
    public:
      void onPrivmsg(Bot& b, Privmsg& p) override {
        std::cout << "Module call" << std::endl;
        this->bot->raw("PRIVMSG #Bots :Hoi?");
      }
};
