void Bot::load_modules(string recv)
{
    Module* m = new Test(); this->hook_privmsg(m);
}