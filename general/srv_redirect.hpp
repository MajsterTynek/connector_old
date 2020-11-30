#ifndef MC_SrvRedirect
#define MC_SrvRedirect

#include <string>
#include "srv_request.hpp"
struct srv_redirect
{ 
    // bool good, fail;

    std::string domain;
    std::string srv_domain;
    ares_srv_reply *answer;
    std::string host;
    std::string port;

    // TODO result cache of SRV responses
    void request(std::string &domain_, std::string &port_);

    srv_redirect(std::string &domain_, std::string &port_) :
        domain ( domain_ ),
        answer ( nullptr ),
        host ( domain_ ),
        port ( port_ )
    {
        srv_domain = std::string("_minecraft._tcp.") + domain_;
        dns_library *library = dns_library::getLibrary();
        library->request(srv_domain.c_str(), &answer);
    }

    ~srv_redirect(){ ares_free_data(answer); }
};

#endif // MC_SrvRedirect