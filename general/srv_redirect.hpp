#ifndef MC_SrvRedirect
#define MC_SrvRedirect

#include <string>
// #include "ares.h"

// WIP - I think I am going to use C-Ares here
struct srv_redirect
{ 
    bool good, fail;

    std::string domain;
    std::string srv_domain;
    std::string host;
    std::string port;

    bool request(); // DNS request here

    srv_redirect(std::string domain_) : 
        domain ( domain_ ), 
        srv_domain ( domain_.insert(0, "_minecraft._tcp.") ),
        good ( false ), 
        fail ( false )
    {
        if ( !request() ) // set defaults
        {
            host = domain;
            port = 25565;
            good = true;
        }
    };

};

#endif // MC_SrvRedirect