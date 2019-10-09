#include <iostream>
#include <vector>

class _8Ball : public Module
{
    public:
      _8Ball(Bot* bot) : Module(bot) {
          bot->hook_privmsg(this);

          }
      void onPrivmsg(Privmsg& p) {
        //cout << "Module call Test" << endl;
        //cout << "Privmsg.message: " << p.message << endl;
        if (p.message == ":!8ball")
        {
            string a[] = {"Ja", "Nee", "Misschien"};
            vector<string> answers (a, a + sizeof(a) / sizeof(a[0]) );
            int i = rand() % answers.size();
            cout << answers.size() << endl;
            string result = answers[i];
            bot->say(result);
        }
      }
    void onNotice(Notice& p) { }
    void onNick(Nick& p) { }
    void onJoin(Join& p) {}
    void onPart(Part& p) {}
    void onKick(Part& p) {}
    void onQuit(Quit& p) {}


};
