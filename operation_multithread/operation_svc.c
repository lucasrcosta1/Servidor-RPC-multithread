#include "operation.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void (*)(int)
#endif

pthread_t p_thread;
pthread_attr_t attr;

void *
serv_request(void *data) {
	struct thr_data {
		struct svc_req *rqstp;
		SVCXPRT *transp;
	} *ptr_data;

	union {
		operandos soma_1_arg;
		operandos sub_1_arg;
		operandos divisao_1_arg;
		operandos mult_1_arg;
	} argument;
	union {
		int soma_1_res;
		int sub_1_res;
		int divisao_1_res;
		int mult_1_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	ptr_data = (struct thr_data  *)data;
	struct svc_req *rqstp = ptr_data-> rqstp;
	register SVCXPRT *transp = ptr_data-> transp;

	switch (rqstp->rq_proc)
	{
	case NULLPROC:
		(void)svc_sendreply(transp, (xdrproc_t)xdr_void, (char *)NULL);

	case SOMA:
		_xdr_argument = (xdrproc_t)xdr_operandos;
		_xdr_result = (xdrproc_t)xdr_int;
		local = (bool_t(*)(char *, void *, struct svc_req *))soma_1_svc;
		break;

	case SUB:
		_xdr_argument = (xdrproc_t)xdr_operandos;
		_xdr_result = (xdrproc_t)xdr_int;
		local = (bool_t(*)(char *, void *, struct svc_req *))sub_1_svc;
		break;

	case DIVISAO:
		_xdr_argument = (xdrproc_t)xdr_operandos;
		_xdr_result = (xdrproc_t)xdr_int;
		local = (bool_t(*)(char *, void *, struct svc_req *))divisao_1_svc;
		break;

	case MULT:
		_xdr_argument = (xdrproc_t)xdr_operandos;
		_xdr_result = (xdrproc_t)xdr_int;
		local = (bool_t(*)(char *, void *, struct svc_req *))mult_1_svc;
		break;

	default:
		svcerr_noproc(transp);
	}
	memset((char *)&argument, 0, sizeof(argument));
	if (!svc_getargs(transp, (xdrproc_t)_xdr_argument, (caddr_t)&argument))
	{
		svcerr_decode(transp);
	}
	retval = (bool_t)(*local)((char *)&argument, (void *)&result, rqstp);
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t)_xdr_result, (char *)&result))
	{
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, (xdrproc_t)_xdr_argument, (caddr_t)&argument))
	{
		fprintf(stderr, "%s", "unable to free arguments");
		exit(1);
	}
	if (!operation_prog_1_freeresult(transp, _xdr_result, (caddr_t)&result))
		fprintf(stderr, "%s", "unable to free results");
}

static void
operation_prog_1(struct svc_req *rqstp, register SVCXPRT *transp) {
	int s;
	struct data_str {
		struct svc_req *rqstp;
		SVCXPRT *transp;
	} *data_ptr = (struct data_str*)malloc(sizeof(struct data_str));
	data_ptr-> rqstp = rqstp;
	data_ptr-> transp = transp;
	//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	s = pthread_create(&p_thread,&attr,serv_request,(void *)data_ptr);
	s = pthread_join(p_thread, NULL);
	
}

int 
main(int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset(OPERATION_PROG, OPERATION_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL)
	{
		fprintf(stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, OPERATION_PROG, OPERATION_VERSION, operation_prog_1, IPPROTO_UDP))
	{
		fprintf(stderr, "%s", "unable to register (OPERATION_PROG, OPERATION_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL)
	{
		fprintf(stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, OPERATION_PROG, OPERATION_VERSION, operation_prog_1, IPPROTO_TCP))
	{
		fprintf(stderr, "%s", "unable to register (OPERATION_PROG, OPERATION_VERSION, tcp).");
		exit(1);
	}

	svc_run();
	fprintf(stderr, "%s", "svc_run returned");
	exit(1);
	/* NOTREACHED */
}

