#ifndef MC_SrvRequest
#define MC_SrvRequest

#include <ares.h>
#include <nameser.h>

void callback_for_ares(void *, int, int, unsigned char *, int);

struct dns_library
{
    ares_channel channel;
    ares_callback callback;
    static dns_library *instance;
    static dns_library *getLibrary();

    void winsock_init();
    void request(const char *domain, ares_srv_reply **answer);
    void process_requests();

    static void print(ares_srv_reply *reply);
    static ares_srv_reply *find_best(ares_srv_reply *reply);

    dns_library() : callback(callback_for_ares)
    {
        winsock_init();
        ::ares_library_init(ARES_LIB_INIT_ALL);
        ::ares_init(&channel);
    }

    ~dns_library()
    {
        ::ares_destroy(channel);
        ::ares_library_cleanup();
    }
};

#endif // MC_SrvRequest