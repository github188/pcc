#include "StdAfx.h"

#include "my_NP_GridUserClient.h"
#include "np_scatteredsession.h"
TCPSError MY_NP_GridUserClient::ReplyCommitJob(
								 IN INT64 jobKey,
								 IN TCPSError replyCode,
								 IN const tcps_Binary& context
								 ) posting_callback
{
	if(m_service)
		return m_service->OnExecuted(jobKey,replyCode,context.Get(),context.Length());
	else
		return TCPS_OK;
}