#include <iostream>
#include "srv_request.hpp"

dns_library *dns_library::instance = nullptr;

dns_library *dns_library::getLibrary()
{
    if (!instance)
        instance = new dns_library();
    return instance;
}

void dns_library::winsock_init()
{
#ifdef _WIN32
    {
        WORD requested = MAKEWORD(2, 2);
        WSADATA data;
        if (::WSAStartup(requested, &data) != 0)
            std::clog << "fatal: WSAStartup() failed" << std::endl;
    }
#endif
}

void dns_library::request(const char* domain, ares_srv_reply** answer)
{
    ::ares_query(channel, domain,
                 __ns_class::ns_c_in,
                 __ns_type::ns_t_srv,
                 callback, (void *)answer);
}


void dns_library::process_requests()
{
    // TODO check thread safety of this
    int nfds, count;
    fd_set readers, writers;
    struct timeval tv, *tvp;

    for (;;)
    {
        FD_ZERO(&readers);
        FD_ZERO(&writers);
        nfds = ares_fds(channel, &readers, &writers);
        if (nfds == 0)
            break;
        tvp = ares_timeout(channel, NULL, &tv);
        count = select(nfds, &readers, &writers, NULL, tvp);
        ares_process(channel, &readers, &writers);
    }
}

ares_srv_reply *dns_library::find_best(ares_srv_reply *reply)
{
    if (!reply)
        return nullptr;
    ares_srv_reply *curr = reply; // the best
    ares_srv_reply *next = reply->next;

    // print(curr);
    while (next)
    {
        if (curr->priority > next->priority)
            curr = next;

        else if (curr->priority == next->priority && curr->weight < next->weight)
            curr = next;

        // print(next);
        next = next->next;
    }
    return curr;
}

void dns_library::print(ares_srv_reply *entry)
{
    if (!entry)
    {
        std::clog << "Service entry is null!" << std::endl;
        return;
    }

    std::clog << "Service found: " << entry->host << ':' << entry->port << ' '
              << '[' << entry->priority << ',' << entry->weight << ']' << std::endl;
}

void callback_for_ares(void *answer, int status, int timeouts, unsigned char *abuf, int alen)
{
    if (!abuf || status != ARES_SUCCESS)
    {
        std::clog << "Service error: " << ares_strerror(status) << std::endl;
        return;
    }

    ares_parse_srv_reply(abuf, alen, (ares_srv_reply **)answer);
}