#include "rdict.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <rpc/pmap_clnt.h>
#include  <string.h>
#include  <memory.h>
#include  <sys/socket.h>
#include  <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif
pthread_t p_thread;
pthread_attr_t attr;

/* Procedure to be run by thread */

void *
serv_request(void *data)
{
struct thr_data
{
struct svc_req *rqstp;
SVCXPRT *transp;
} *ptr_data;
	union {
		char *insertw_1_arg;
		char *deletew_1_arg;
		char *lookupw_1_arg;
		char *showupw_1_arg;
		char *getlenw_1_arg;
	} argument;
	union {
		int initw_1_res;
		int insertw_1_res;
		int deletew_1_res;
		int lookupw_1_res;
		char showupw_1_res;
		int getlenw_1_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

ptr_data = (struct thr_data  *)data;
struct svc_req *rqstp = ptr_data-> rqstp;
register SVCXPRT *transp = ptr_data-> transp;

switch (rqstp-> rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case INITW:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))initw_1_svc;
		break;

	case INSERTW:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))insertw_1_svc;
		break;

	case DELETEW:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))deletew_1_svc;
		break;

	case LOOKUPW:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))lookupw_1_svc;
		break;
	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) 
{
		svcerr_decode (transp);
		return;
	}
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (retval >  0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char 
*)&result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) 
{
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!rdictprog_1_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}

/*
   New code for procedure rdictprog_1 , starting thread
   in response for each clients request to invoke remote
   procedure
*/


static void
rdictprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
struct data_str
{
struct svc_req *rqstp;
SVCXPRT *transp;
} *data_ptr=(struct data_str*)malloc(sizeof(struct data_str));
data_ptr-> rqstp = rqstp;
data_ptr-> transp = transp;
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
pthread_create(&p_thread,&attr,serv_request,(void *)data_ptr);
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (RDICTPROG, RDICTVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, RDICTPROG, RDICTVERS, rdictprog_1, IPPROTO_UDP)) 
{
		fprintf (stderr, "%s", "unable to register (RDICTPROG, RDICTVERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, RDICTPROG, RDICTVERS, rdictprog_1, IPPROTO_TCP)) 
{
		fprintf (stderr, "%s", "unable to register (RDICTPROG, RDICTVERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
