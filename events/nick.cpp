#include <iostream>
#include <vector>

void Bot::event_nick(string recv)
{

    stringstream ss(recv);
    vector<string> vec;
    string word;
    while (ss >> word) { vec.push_back(word); }
    string newnick = vec[2];
    if (newnick.at(0) == ':') { // compare single char?
        // strip :
        newnick = newnick.erase(0, 1);
    }
    if (this->event_user == this->nickname)
    {
        this->nickname = newnick;
        cout << "Internal nickname set to: "+this->nickname << endl;
    }
    cout << this->event_user +" changed nickname to "+newnick << endl;


}
