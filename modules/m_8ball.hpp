#include <iostream>
#include <vector>

class _8Ball : public Module
{
    public:
      _8Ball(Bot* bot) : Module(bot) {
          bot->hook_privmsg(this);
          }
      void onPrivmsg(Privmsg& p) {
        std::cout << "Module call Test" << std::endl;
        std::cout << "Privmsg.message: " << p.message << endl;
        if (p.message == ":!8ball")
        {
            string a[] = {"Ja", "Nee", "Misschien"};
            vector<string> answers (a, a + sizeof(a) / sizeof(a[0]) );
            int i = rand() % answers.size();
            std::cout << answers.size() << endl;
            string result = answers[i];
            this->bot->say(result);
        }
      }
      void onNick(Nick& p) { }

};
