#include "srv_redirect.hpp"
#include "srv_request.hpp"

void srv_redirect::request(std::string &domain_, std::string &port_)
{
    dns_library::getLibrary()->process_requests();

    if (answer)
    {
        ares_srv_reply *selected = dns_library::find_best(answer);
        dns_library::print(selected);
        host = selected->host;
        port = std::to_string(selected->port);
    }

    domain_ = host;
    port_ = port;
}