#include <iostream>
#include <vector>

class _8Ball : public Module
{
    public:
      _8Ball(Bot* bot) : Module(bot) {
          bot->hook_privmsg(this);
          }
      void onPrivmsg(Privmsg& p) {

        stringstream ss(p.message);
        vector<string> vec;
        string word;
        while (ss >> word) { vec.push_back(word); }
        string cmd = vec[0];
        cmd.erase(0, 2);

        if (cmd == "8ball") {
            if (vec.size() < 2) {
                bot->say("No question detected.");
                return;
            }
            string a[] = {"Ja", "Nee", "Misschien"};
            vector<string> answers (a, a + sizeof(a) / sizeof(a[0]) );
            int i = rand() % answers.size();
            string result = answers[i];
            bot->say(result);
        }
      }

    void onNotice(Notice& p) { }
    void onNick(Nick& p) { }
    void onJoin(Join& p) {}
    void onPart(Part& p) {}
    void onKick(Kick& p) {}
    void onQuit(Quit& p) {}


};
