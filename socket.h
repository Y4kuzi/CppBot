#ifndef _SOCKET
#define _SOCKET

//class Bot;
class Socket {
    public:
        //Bot* bot;
        //Socket(Bot* bot) : bot(bot) { }

        char sockbuff[512]; // 16384
        int sock;
        int port = 6667;
        //std::string server = "irc.devirc.net";
        Socket()
        {
            struct sockaddr_in addr;
            struct hostent *host;
            host = gethostbyname(server.c_str());
            addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons((unsigned short)port);
            sock = socket(AF_INET, SOCK_STREAM, 0);
            connect(sock, (struct sockaddr *)&addr, sizeof(addr));
            //bot->raw("NICK "+nickname);
            //Elite-Conducteur adipiscing
            //bot->raw("USER ahmet 0 0 :Sint Ahmet C++ ding");
        }
    int getSock() {
        //std::cout << "wtf" << std::endl;
        return sock;
    }

};

#endif
