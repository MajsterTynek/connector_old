#include "srv_redirect.hpp"

// Ha hah, stolen snippet goes brrr
// Really I am such a moron not to use docs?

/*#include "ares.h"
#include <arpa/nameser.h>
#include <stdio.h>
#include <string.h>*/

/*static void srv_callback(void *arg, int status, int timeouts, unsigned char *abuf, int alen)
{   
	struct mosquitto *mosq = arg;
	struct ares_srv_reply *reply = NULL;
	if(status == ARES_SUCCESS){
		status = ares_parse_srv_reply(abuf, alen, &reply);
		if(status == ARES_SUCCESS){
			// FIXME - choose which answer to use based on rfc2782 page 3. 
			mosquitto_connect(mosq, reply->host, reply->port, mosq->keepalive);
		}
	}else{
		log__printf(mosq, MOSQ_LOG_ERR, "Error: SRV lookup failed (%d).", status);
		// /* FIXME - calling on_disconnect here isn't correct.
		pthread_mutex_lock(&mosq->callback_mutex);
		if(mosq->on_disconnect){
			mosq->in_callback = true;
			mosq->on_disconnect(mosq, mosq->userdata, MOSQ_ERR_LOOKUP);
			mosq->in_callback = false;
		}
		pthread_mutex_unlock(&mosq->callback_mutex);
	}
}

int mosquitto_connect_srv(struct mosquitto *mosq, const char *host, int keepalive, const char *bind_address)
{
	char *h;
	int rc;
	if(!mosq) return MOSQ_ERR_INVAL;

	rc = ares_init(&mosq->achan);
	if(rc != ARES_SUCCESS){
		return MOSQ_ERR_UNKNOWN;
	}

	if(!host){
		// get local domain
	}else{
        h = mosquitto__malloc(strlen(host) + strlen("_mqtt._tcp.") + 1);
        if(!h) return MOSQ_ERR_NOMEM;
        sprintf(h, "_mqtt._tcp.%s", host);

		ares_search(mosq->achan, h, ns_c_in, ns_t_srv, srv_callback, mosq);
		mosquitto__free(h);
	}

	pthread_mutex_lock(&mosq->state_mutex);
	mosq->state = mosq_cs_connect_srv;
	pthread_mutex_unlock(&mosq->state_mutex);

	mosq->keepalive = keepalive;

	return MOSQ_ERR_SUCCESS;
}*/

// also gotta check this: // Simple SRV lookup?
// https://c-ares.haxx.se/mail/c-ares-archive-2009-12/0006.shtml