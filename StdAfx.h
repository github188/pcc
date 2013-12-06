// StdAfx.h

#ifndef _ServeNP_SCATTERED_StdAfx_h_
#define _ServeNP_SCATTERED_StdAfx_h_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
	#pragma once
#endif

#include "npdebug.h"
#include "NP_GridUserTypesDefine.h"
//#include "np_scatteredsession.h"
class PCC_Scatter_S;
class PCC_Service_S;
struct Node_Id
{
	Node_Id(IPP p,INT32 sk):ipp(p),senssionKey(sk){}
	IPP ipp;
	INT32 senssionKey;
	bool operator<(const Node_Id& oth) const
	{
		return (
			ipp.i64_<oth.ipp.i64_?true:(ipp.i64_>oth.ipp.i64_?false:(senssionKey<oth.senssionKey?true:
			false)))
			;
	}
};

struct SCNode  
{
	INT32 key;
	PCC_Scatter_S* ps;
};
struct SCJob
{
	INT64 moduleKey;
	INT64 jobKey;
	PCC_Service_S* ss;
	GRID_JobInfo info;
};
#endif	// #ifndef _ServeNP_SCATTERED_StdAfx_h_
