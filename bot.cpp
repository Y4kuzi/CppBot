#include <iostream>
//#include <sys/socket.h>
//#include <sys/types.h>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <boost/algorithm/string.hpp> // libboost-dev

#include "bot.hpp"

#include "events/raw.cpp"
#include "events/welcome.cpp"
#include "events/privmsg.cpp"
#include "events/joinpart.cpp"
#include "events/kick.cpp"
#include "events/nick.cpp"


#include "modules/m_test.cpp"
//#include "modules/m_haha.cpp"


int main() {
        Bot b1 = Bot("Sint-Ahmet_Lord-Ipsum");
        std::cout << "Wat?" << std::endl;
        return 0;
};
