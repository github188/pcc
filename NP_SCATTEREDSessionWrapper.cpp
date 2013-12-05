// NP_SCATTEREDSessionWrapper.cpp
//
// 注意: 此文件为工具生成，请不要修改

#include "StdAfx.h"
#include "NP_SCATTEREDSession.h"
#include "ScopeGuard.h"
#include "ipcvt.h"
#include "ILocker.h"
#include "ModuleInfo.h"
#include "smlib.h"
#include "NPTime.h"
#include "TaskMan.h"
#include "nplog.h"
#include "iscm_smart_container.h"
#include "iscm_posting_caller.h"

//////////////////////////////////////////////////////////////
// NP_SCATTEREDSessionMaker
static void RegisterLocalSessionMakeFunction_(const IPP& serveIPP, NP_SCATTEREDSessionMaker& sessionMaker);
static void UnregisterLocalSessionMakeFunction_(const IPP& serveIPP);

NP_SCATTEREDSessionMaker::NP_SCATTEREDSessionMaker(void* userParameter /*= NULL*/)
	: m_userParameter(userParameter)
	, m_lock(3)
	, m_serveMan(NULL)
	, m_sessionCount(0)
{
}

NP_SCATTEREDSessionMaker::~NP_SCATTEREDSessionMaker()
{
	while(m_localSessionsSet.size() > 0)
	{
		iscm_ILocalCallbackBase* session = *(m_localSessionsSet.begin());
		ASSERT(session);
		session->CloseSession_(); // 服务退出时，关闭进程内还未关闭的客户端
	}
	ASSERT(0 == m_sessionCount);
}

int NP_SCATTEREDSessionMaker::GetSessionObjSize() const
	{	return sizeof(NP_SCATTEREDSession);	}
void NP_SCATTEREDSessionMaker::MakeSessionObj(void* session)
	{	::new(session) NP_SCATTEREDSession(*this);	}

void NP_SCATTEREDSessionMaker::OnServiceCreated(
				IN const IPP& serveIPP,
				IN iscm_IRPCServeMan* rpcMan
				)
{
	ASSERT(NULL==m_serveMan && rpcMan);
	m_serveMan = rpcMan->GetTcpsServeMan();
	RegisterLocalSessionMakeFunction_(serveIPP, *this);
}

void NP_SCATTEREDSessionMaker::OnServiceClose(
				IN const IPP& serveIPP,
				IN iscm_IRPCServeMan* rpcMan
				)
{
	UnregisterLocalSessionMakeFunction_(serveIPP);
	(void)rpcMan;
	m_serveMan = NULL;
}

void NP_SCATTEREDSessionMaker::RegisterLocalSession_(iscm_ILocalCallbackBase* session)
{
	ASSERT(session);
	CNPAutoLock locker(m_lock);
	VERIFY(m_localSessionsSet.insert(session).second);
}

void NP_SCATTEREDSessionMaker::UnregisterLocalSession_(iscm_ILocalCallbackBase* session)
{
	ASSERT(session);
	CNPAutoLock locker(m_lock);
	VERIFY(1 == m_localSessionsSet.erase(session));
}

void NP_SCATTEREDSessionMaker::OnSessionConnect_(INT32* nextSessionKey /*= NULL*/, INT32& sessionCount)
{
	CNPAutoLock locker(m_lock);
	if(nextSessionKey)
	{
		if(m_serveMan)
			*nextSessionKey = m_serveMan->GetNextSessionKey();
		else
			*nextSessionKey = m_sessionKeyGenerater.GetNext();
	}
	ASSERT(m_sessionCount >= 0);
	sessionCount = ++m_sessionCount;
}

void NP_SCATTEREDSessionMaker::OnSessionClosed_()
{
	CNPAutoLock locker(m_lock);
	ASSERT(m_sessionCount > 0);
	--m_sessionCount;
}

//////////////////////////////////////////////////////////////
// NP_SCATTEREDSession
#if defined(_MSC_VER)
	#pragma warning(disable: 4702) // unreachable code
#endif

//[[begin_session_wrap_body]]

INT32 NP_SCATTEREDSession::GetSessionKey() const
{
	tcps_ISession* bindedSession = m_bindedSession;
	if(NULL == bindedSession)
		return 0;
	INT32 sessionKey = 0;
	bindedSession->GetInfos(NULL, NULL, NULL, NULL, &sessionKey);
//	ASSERT(0 != sessionKey);
	return sessionKey;
}

void NP_SCATTEREDSession::CloseSession_(
				IN TCPSError cause /*= TCPS_OK*/
				)
{
	iscm_IRPCServeMan* const serveMan = m_serveMan;
	if(NULL == serveMan)
		return;
	INT32 const sessionKey = GetSessionKey();
	if(0 != sessionKey)
	{
		char sid[16];
		itoa_Traits(sid, sessionKey);
		TCPSError errClose = serveMan->CloseSessionEx(sid, sessionKey, cause);
		(void)errClose;
	}
	if(0 != m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().CloseSession(m_postingProxy.callerKey_);
}

inline NP_SCATTEREDSession::FTVMap_& NP_SCATTEREDSession::GetFTVMap_()
	{	LOCAL_SAFE_STATIC_OBJ(NP_SCATTEREDSession::FTVMap_);	}
NP_SCATTEREDSession::FTVMap_* NP_SCATTEREDSession::sm_FTVMap = NULL;

inline NP_SCATTEREDSession::MethodCallMap_& NP_SCATTEREDSession::GetMethodCallMap_()
	{	LOCAL_SAFE_STATIC_OBJ(NP_SCATTEREDSession::MethodCallMap_);	}
NP_SCATTEREDSession::MethodCallMap_* NP_SCATTEREDSession::sm_methodCallMap = NULL;

void NP_SCATTEREDSession::InitFTVMap_()
{
	if(sm_FTVMap)
		return;

	IscmRPCGlobalLocker locker;
	if(sm_FTVMap)
		return;

	FTVMap_& ftvMap = GetFTVMap_();
	ASSERT(0 == ftvMap.size());
	MethodCallMap_& mdCallMap = GetMethodCallMap_();
	ASSERT(0 == mdCallMap.size());
	//[[begin_init_method_map]]
	VERIFY(ftvMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User"), ftv_GRID_User)).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::AddJobProgram"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_AddJobProgram, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::DelJobProgram"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_DelJobProgram, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::FindJobProgram"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_FindJobProgram, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListJobPrograms"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListJobPrograms, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::CommitJob"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_CommitJob, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::CancelJob"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_CancelJob, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::SetJobPriority"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_SetJobPriority, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListJobs"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListJobs, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::QueryJobs"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_QueryJobs, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::UpdateGrid"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_UpdateGrid, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListModules"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListModules, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::GetLogCount"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_GetLogCount, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::LoadLogInfos"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_LoadLogInfos, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::Login"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_Login, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::Logout"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_Logout, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::AddUser"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_AddUser, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::DelUser"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_DelUser, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::UpdatePassword"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_UpdatePassword, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ManageUser"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ManageUser, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListAllUsers"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListAllUsers, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::GetUserInfo"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_GetUserInfo, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListJTMs"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListJTMs, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::GetJTMInfo"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_GetJTMInfo, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::GetGridProperty"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_GetGridProperty, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::AddSplitter"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_AddSplitter, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::DelSplitter"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_DelSplitter, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListSplitters"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListSplitters, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ListSplitterParam"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_ListSplitterParam, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::GetGridStatistics"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_GetGridStatistics, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::UDPLink_"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_UDPLink_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::UDPLinkConfirm_"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_UDPLinkConfirm_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::SetTimeout_"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_SetTimeout_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::SetSessionBufferSize_"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_SetSessionBufferSize_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::MethodCheck_"), MethodSite_(&NP_SCATTEREDSession::Wrap_GRID_User_MethodCheck_, false))).second);
	VERIFY(ftvMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter"), ftv_PCC_Scatter)).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::OnComputed"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_OnComputed, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::UDPLink_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_UDPLink_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::UDPLinkConfirm_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_UDPLinkConfirm_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::SetTimeout_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_SetTimeout_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::SetSessionBufferSize_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_SetSessionBufferSize_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Scatter::MethodCheck_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Scatter_MethodCheck_, false))).second);
	VERIFY(ftvMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service"), ftv_PCC_Service)).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::Login"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_Login, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::Logout"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_Logout, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::ListModules"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_ListModules, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::GetModuleFile"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_GetModuleFile, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::Execute"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_Execute, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::QueryJobs"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_QueryJobs, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::CancelJob"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_CancelJob, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::UDPLink_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_UDPLink_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::UDPLinkConfirm_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_UDPLinkConfirm_, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::SetTimeout_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_SetTimeout_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::SetSessionBufferSize_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_SetSessionBufferSize_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Service::MethodCheck_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Service_MethodCheck_, false))).second);
	VERIFY(ftvMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit"), ftv_PCC_Toolkit)).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::Login"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_Login, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::Logout"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_Logout, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::AddModule"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_AddModule, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::AddModuleFile"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_AddModuleFile, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::RemoveModule"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_RemoveModule, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::RemoveModuleFiles"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_RemoveModuleFiles, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::ListModules"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_ListModules, false))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::SetTimeout_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_SetTimeout_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::SetSessionBufferSize_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_SetSessionBufferSize_, true))).second);
	VERIFY(mdCallMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("PCC_Toolkit::MethodCheck_"), MethodSite_(&NP_SCATTEREDSession::Wrap_PCC_Toolkit_MethodCheck_, false))).second);
	//[[end_init_method_map]]
	sm_methodCallMap = &mdCallMap;
	sm_FTVMap = &ftvMap;
}

NP_SCATTEREDSession::NP_SCATTEREDSession(NP_SCATTEREDSessionMaker& sessionMaker)
	: m_sessionMaker(sessionMaker), m_postingProxy(*this)
{
	m_serveMan = NULL;
	m_bindedSession = NULL;
	m_sessionCount = 0;
	m_sessionDummyPtr = NULL;
	DEBUG_EXP(m_closingPromptTime = INVALID_UTC_MSEL);
	DEBUG_EXP(m_callingPostingTaskTID = INVALID_OSTHREADID);

	// 初始化方法表
	InitFTVMap_();
}

NP_SCATTEREDSession::~NP_SCATTEREDSession()
{
	ASSERT(NULL == m_sessionDummyPtr);
	ASSERT(NULL == m_callbackRequester.face_);
	ASSERT(NULL==m_serveMan && !m_serveIPP.IsValid() && !m_peerIPP.IsValid() && NULL==m_bindedSession);
	ASSERT(0 == m_postingProxy.callerKey_);
}

TCPSError NP_SCATTEREDSession::OnConnected(
				IN iscm_IRPCServeMan* serveMan,
				IN tcps_ISession* bindedSession,
				IN const IPP& serveIPP,
				IN const IPP& peerIPP,
				IN INT32 sessionKey,
				IN INT32 sessionCount,
				IN tcps_ITrusteeParameter* trusteeParam /*= NULL*/
				)
{
	ASSERT(NULL == m_callbackRequester.face_);
	ASSERT(!m_udpSite.IsOn());
	ASSERT(NULL == m_sessionDummyPtr);
	ASSERT(NULL==m_serveMan && !m_peerIPP.IsValid() && NULL==m_bindedSession);
	ASSERT(0 == m_postingProxy.callerKey_);
	ASSERT(serveMan && bindedSession);
	ASSERT(!m_peerID_IPP.IsValid() && m_bindedInterface.IsEmpty());
	ASSERT(INVALID_UTC_MSEL==m_closingPromptTime && INVALID_OSTHREADID==m_callingPostingTaskTID);
	(void)sessionKey; (void)sessionCount; (void)trusteeParam;
	m_serveMan = serveMan;
	m_bindedSession = bindedSession;
	m_serveIPP = serveIPP;
	m_peerIPP = peerIPP;
	m_sessionCount = sessionCount;
	m_sessionMaker.m_sessionRegister.BaseRegister_(this);
	return TCPS_OK;
}

void NP_SCATTEREDSession::OnPrepareClose(
				IN TCPSError cause
				)
{
	(void)cause;
	iscm_IRequester* const rpcRequester = m_callbackRequester.face_;
	if(rpcRequester)
		rpcRequester->PrepareDisconnect();
	if(0 != m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().CloseSession(m_postingProxy.callerKey_);
	m_serveMan = NULL;
	DEBUG_EXP(m_closingPromptTime = GetMsel()+1);
}

BOOL NP_SCATTEREDSession::IsSessionBusying() const
{
	// 一定是处于待关闭态的会话（调用过OnPrepareClose()的会话）
	ASSERT(NULL==m_serveMan && m_bindedSession);
	ASSERT(m_postingMethods.callingCount >= 0);
	BOOL const busying = (m_postingMethods.callingCount>0 || m_postingProxy.callerKey_!=0);
#ifdef _DEBUG
	if(busying)
	{
		LTMSEL ltNow = GetMsel();
		if(ltNow-m_closingPromptTime >= 1000*2)
		{
			m_closingPromptTime = ltNow;
			NPLogWarning(("任务线程[%s|0x%08X|%d]执行中，会话[0x%08X]无法关闭"
						, INT64_TO_STR_A(m_callingPostingTaskTID)
						, (UINT32)m_callingPostingTaskTID
						, (int)m_callingPostingTaskTID_int
						, (UINT32)(UINT_PTR)m_sessionDummyPtr
						));
		}
	}
#endif
	return busying;
}

void NP_SCATTEREDSession::OnClose(
				IN TCPSError cause
				)
{
	// 不再assert，压力测试时，当posting会话还未建立时，整个会话开始进入关闭状态，而在过程中，
	// 客户端又发来了posting会话建立请求，此种情况，OnPrepareClose()和IsSessionBusying()保证
	// 不了m_postingProxy.callerKey_一定为0
	//ASSERT(0 == m_postingProxy.callerKey_);

	// ISCM会调用OnPrepareClose()和IsSessionBusying()，保证了不再有任务会执行
	ASSERT(0==m_postingMethods.callingsDataBytes && 0==m_postingMethods.callingCount);

	INT32 const sessionKey = GetSessionKey(); // 必须在m_bindedSession = NULL;之前
	ASSERT(0 != sessionKey);

	// 在OnPrepareClose()中已提前置为NULL
	ASSERT(NULL == m_serveMan);
	// 再赋为NULL（容错）
	m_serveMan = NULL;

	m_bindedSession = NULL;

	ASSERT(NULL==m_postingMethods.head && NULL==m_postingMethods.tail);
	m_sessionMaker.m_sessionRegister.BaseUnregister_(this);

	if(!m_bindedInterface.IsEmpty())
	{
		ASSERT(m_peerID_IPP.IsValid());
		m_sessionMaker.m_sessionRegister.UpdateFaceStat_(iscm_SessionRegister::ufst_remove, m_peerID_IPP, m_bindedInterface);

		ASSERT(sm_FTVMap);
		FTVMap_::iterator itFTV = sm_FTVMap->find(m_bindedInterface.Get());
		ASSERT(itFTV != sm_FTVMap->end());
		FaceTypeValue const ftv = itFTV->second;
		switch(ftv)
		{
		//[[begin_OnClose]]
			case ftv_GRID_User:
			{
				ASSERT(m_gRID_User && "GRID_User"==m_bindedInterface);
				m_sessionMaker.m_sessionRegister.Unregister(m_gRID_User);
				ISCM_BEGIN_TRY_()
					if(TCPS_OK!=cause && TCPS_ERR_SESSION_DROPED!=cause && TCPS_ERR_EXITING!=cause && TCPS_ERR_SERVE_EXITING!=cause)
						m_gRID_User->OnPeerBroken(sessionKey, m_peerID_IPP, cause);
					m_gRID_User->OnClose(sessionKey, m_peerID_IPP, cause);
				ISCM_END_TRY_()
				ISCM_BEGIN_CATCH_ALL_()
				ISCM_END_CATCH_ALL_()
				m_gRID_User->~GRID_User_S();
				tcps_Free(m_gRID_User);
				m_gRID_User = NULL;
			}
			break;

			case ftv_PCC_Scatter:
			{
				ASSERT(m_pCC_Scatter && "PCC_Scatter"==m_bindedInterface);
				m_sessionMaker.m_sessionRegister.Unregister(m_pCC_Scatter);
				ISCM_BEGIN_TRY_()
					if(TCPS_OK!=cause && TCPS_ERR_SESSION_DROPED!=cause && TCPS_ERR_EXITING!=cause && TCPS_ERR_SERVE_EXITING!=cause)
						m_pCC_Scatter->OnPeerBroken(sessionKey, m_peerID_IPP, cause);
					m_pCC_Scatter->OnClose(sessionKey, m_peerID_IPP, cause);
				ISCM_END_TRY_()
				ISCM_BEGIN_CATCH_ALL_()
				ISCM_END_CATCH_ALL_()
				m_pCC_Scatter->~PCC_Scatter_S();
				tcps_Free(m_pCC_Scatter);
				m_pCC_Scatter = NULL;
			}
			break;

			case ftv_PCC_Service:
			{
				ASSERT(m_pCC_Service && "PCC_Service"==m_bindedInterface);
				m_sessionMaker.m_sessionRegister.Unregister(m_pCC_Service);
				ISCM_BEGIN_TRY_()
					if(TCPS_OK!=cause && TCPS_ERR_SESSION_DROPED!=cause && TCPS_ERR_EXITING!=cause && TCPS_ERR_SERVE_EXITING!=cause)
						m_pCC_Service->OnPeerBroken(sessionKey, m_peerID_IPP, cause);
					m_pCC_Service->OnClose(sessionKey, m_peerID_IPP, cause);
				ISCM_END_TRY_()
				ISCM_BEGIN_CATCH_ALL_()
				ISCM_END_CATCH_ALL_()
				m_pCC_Service->~PCC_Service_S();
				tcps_Free(m_pCC_Service);
				m_pCC_Service = NULL;
			}
			break;

			case ftv_PCC_Toolkit:
			{
				ASSERT(m_pCC_Toolkit && "PCC_Toolkit"==m_bindedInterface);
				m_sessionMaker.m_sessionRegister.Unregister(m_pCC_Toolkit);
				ISCM_BEGIN_TRY_()
					if(TCPS_OK!=cause && TCPS_ERR_SESSION_DROPED!=cause && TCPS_ERR_EXITING!=cause && TCPS_ERR_SERVE_EXITING!=cause)
						m_pCC_Toolkit->OnPeerBroken(sessionKey, m_peerID_IPP, cause);
					m_pCC_Toolkit->OnClose(sessionKey, m_peerID_IPP, cause);
				ISCM_END_TRY_()
				ISCM_BEGIN_CATCH_ALL_()
				ISCM_END_CATCH_ALL_()
				m_pCC_Toolkit->~PCC_Toolkit_S();
				tcps_Free(m_pCC_Toolkit);
				m_pCC_Toolkit = NULL;
			}
			break;
		//[[end_OnClose]]
			default:
			ASSERT(false);
			break;
		}
		m_sessionMaker.OnSessionClosed_();

		if(SockValid(m_udpSite.sock_))
			VERIFY(0 == SockClose(m_udpSite.sock_));
		if(m_udpSite.IsOn())
			iscm_FetchUDPServeMan().CloseSession(sessionKey);
		m_udpSite.localPort_ = -1;
		m_udpSite.sock_ = INVALID_SOCKET;
	}
	else
	{
		// 程序运行到此处，一般是此会话转化为了回调通道
	//	ASSERT(TCPS_ERR_SESSION_DROPED == cause);
		ASSERT(!m_peerID_IPP.IsValid());
		ASSERT(NULL == m_sessionDummyPtr);
		ASSERT(-1==m_udpSite.localPort_ && !SockValid(m_udpSite.sock_));
	}

	m_callbackRequester.Release();

	m_sessionDummyPtr = NULL; // 容错处理
	DEBUG_EXP(m_closingPromptTime = INVALID_UTC_MSEL);
	DEBUG_EXP(m_callingPostingTaskTID = INVALID_OSTHREADID);
	m_serveIPP = INVALID_IPP;
	m_peerIPP = INVALID_IPP;
	m_sessionCount = 0;
	m_peerID_IPP = INVALID_IPP;
	m_bindedInterface.Release();
}

void NP_SCATTEREDSession::OnPrepareCall(
				IN const iscm_RPCReq& req,
				IN const void* data,
				OUT BOOL& notifyByTaskPool /*= true*/
				)
{
	(void)req; (void)data; (void)notifyByTaskPool;
	ASSERT(RPCCT_SET_BINDED_INTERFACE == req.cmd
		|| RPCCT_TO_CALLBACK == req.cmd
		|| RPCCT_TO_POSTING_CHANNEL == req.cmd
		|| RPCCT_RPC == req.cmd);
	ASSERT(notifyByTaskPool);
}

TCPSError NP_SCATTEREDSession::OnCall(
				OUT BOOL& requireReplyData /*= true*/,
				OUT BOOL& destroySession /*= false*/,
				IN const iscm_RPCReq& req,
				IN const void* data,
				OUT BOOL& dataTransferred /*= false*/,
				IN iscm_IRPCOutfiter* outfiter
				)
{
#ifdef _DEBUG
	ASSERT(requireReplyData && !destroySession);
	ASSERT(req.len >= (int)sizeof(iscm_RPCReq));
	if(req.len > (int)sizeof(iscm_RPCReq))
		ASSERT(data);
	ASSERT(outfiter);
#endif

	if(RPCCT_RPC == req.cmd)
	{
		ASSERT(req.len > (int)sizeof(iscm_RPCReq));
		TCPSError err;
		while(true)
		{
			err = OnRPCCall_(this, NULL, requireReplyData, data, dataTransferred, req.len-sizeof(iscm_RPCReq), outfiter);
			if(TCPS_ERR_POSTING_PENDING_FULL == err)
			{
				ASSERT(!dataTransferred);
				Sleep(1);
				continue;
			}
			else
				break;
		}
		return err;
	}
	else if(RPCCT_SET_BINDED_INTERFACE == req.cmd)
	{
		if(req.len < (int)(sizeof(req) + sizeof(INT16)*2 + sizeof(IPP) + sizeof(INT32)+1)) // 标识IPP + 字符串interfaceName
		{
			NPLogError(("RPCCT_SET_BINDED_INTERFACE: %s(%d)", tcps_GetErrTxt(TCPS_ERR_MALFORMED_REQ), TCPS_ERR_MALFORMED_REQ));
			destroySession = true;
			return TCPS_ERR_MALFORMED_REQ;
		}
		if(m_peerID_IPP.IsValid() || !m_bindedInterface.IsEmpty())
		{
			NPLogError(("RPCCT_SET_BINDED_INTERFACE: %s(%d)", tcps_GetErrTxt(TCPS_ERR_FUNCTION_RUNNING), TCPS_ERR_FUNCTION_RUNNING));
			destroySession = true;
			return TCPS_ERR_FUNCTION_RUNNING;
		}

		const BYTE* inPar = (const BYTE*)data;
		INT16 protocolVer = *(INT16*)inPar;
		inPar += sizeof(INT16);
		INT16 localPeerFlag = *(INT16*)inPar;
		inPar += sizeof(INT16);
		if(0!=protocolVer || 0!=localPeerFlag)
		{
			NPLogError(("RPCCT_SET_BINDED_INTERFACE: Invalid network protocol version"));
			destroySession = true;
			return TCPS_ERR_MALFORMED_REQ;
		}

		const IPP& peerID_IPP = *(IPP*)inPar;
		inPar += sizeof(IPP);
		INT32 faceNameLen = *(INT32*)inPar;
		inPar += sizeof(INT32);
		const char* faceName = (const char*)inPar;
		inPar += faceNameLen+1;
		if(req.len-(int)sizeof(req) != inPar-(const BYTE*)data
			|| !peerID_IPP.IsValid() || 0 != faceName[faceNameLen])
		{
			NPLogError(("RPCCT_SET_BINDED_INTERFACE: %s(%d)", tcps_GetErrTxt(TCPS_ERR_MALFORMED_REQ), TCPS_ERR_MALFORMED_REQ));
			destroySession = true;
			return TCPS_ERR_MALFORMED_REQ;
		}

		INT32 const sessionKey = GetSessionKey();
		ASSERT(0 != sessionKey);
		char sid[16];
		itoa_Traits(sid, sessionKey);
		TCPSError err = m_bindedSession->SetSessionID(sid);
		if(TCPS_OK != err)
		{
			NPLogError(("设置会话ID失败，接口名: '%s', 错误号: %d(%s)", faceName, err, tcps_GetErrTxt(err)));
			destroySession = true;
			return err;
		}

		ASSERT(sm_FTVMap);
		FTVMap_::iterator itFTV = sm_FTVMap->find(faceName);
		if(itFTV == sm_FTVMap->end())
		{
			NPLogError(("接口'%s'未定义, from '%s'", faceName, IPP_TO_STR_A(m_peerIPP)));
			destroySession = true;
			return TCPS_ERR_INVALID_INTERFACE;
		}

		INT32 sessionCount2;
		m_sessionMaker.OnSessionConnect_(NULL, sessionCount2);

		FaceTypeValue const ftv = itFTV->second;
		switch(ftv)
		{
		//[[begin_OnConnected]]
			case ftv_GRID_User:
			{
				ASSERT(NULL==m_gRID_User && 0==strcmp("GRID_User", faceName));
				m_gRID_User = tcps_NewEx(GRID_User_S, (m_sessionMaker, this, NULL));
				TCPSError err = TCPS_ERROR;
				try
				{
					err = m_gRID_User->OnConnected(sessionKey, peerID_IPP, sessionCount2);
				}
				catch(TCPSError ret)
				{
					ASSERT(TCPS_OK != ret);
					err = ret;
				}
				catch(int ret)
				{
					ASSERT(TCPS_OK != ret);
					err = (TCPSError)ret;
				}
				catch(std::bad_alloc)
				{
					NPLogError(("GRID_User_S::OnConnected(), Out of memory"));
					err = TCPS_ERR_OUT_OF_MEMORY;
				}
				ISCM_BEGIN_CATCH_ALL_()
					NPLogError(("GRID_User_S::OnConnected(), Unknown exception"));
					err = TCPS_ERR_UNKNOWN_EXCEPTION;
				ISCM_END_CATCH_ALL_()
				if(TCPS_OK != err)
				{
					ISCM_BEGIN_TRY_()
						m_gRID_User->OnClose(sessionKey, peerID_IPP, err);
					ISCM_END_TRY_()
					ISCM_BEGIN_CATCH_ALL_()
					ISCM_END_CATCH_ALL_()
					m_sessionMaker.OnSessionClosed_();
					m_gRID_User->~GRID_User_S();
					tcps_Free(m_gRID_User);
					m_gRID_User = NULL;
					destroySession = true;
					return err;
				}
			}
			break;

			case ftv_PCC_Scatter:
			{
				ASSERT(NULL==m_pCC_Scatter && 0==strcmp("PCC_Scatter", faceName));
				m_pCC_Scatter = tcps_NewEx(PCC_Scatter_S, (m_sessionMaker, this, NULL));
				TCPSError err = TCPS_ERROR;
				try
				{
					err = m_pCC_Scatter->OnConnected(sessionKey, peerID_IPP, sessionCount2);
				}
				catch(TCPSError ret)
				{
					ASSERT(TCPS_OK != ret);
					err = ret;
				}
				catch(int ret)
				{
					ASSERT(TCPS_OK != ret);
					err = (TCPSError)ret;
				}
				catch(std::bad_alloc)
				{
					NPLogError(("PCC_Scatter_S::OnConnected(), Out of memory"));
					err = TCPS_ERR_OUT_OF_MEMORY;
				}
				ISCM_BEGIN_CATCH_ALL_()
					NPLogError(("PCC_Scatter_S::OnConnected(), Unknown exception"));
					err = TCPS_ERR_UNKNOWN_EXCEPTION;
				ISCM_END_CATCH_ALL_()
				if(TCPS_OK != err)
				{
					ISCM_BEGIN_TRY_()
						m_pCC_Scatter->OnClose(sessionKey, peerID_IPP, err);
					ISCM_END_TRY_()
					ISCM_BEGIN_CATCH_ALL_()
					ISCM_END_CATCH_ALL_()
					m_sessionMaker.OnSessionClosed_();
					m_pCC_Scatter->~PCC_Scatter_S();
					tcps_Free(m_pCC_Scatter);
					m_pCC_Scatter = NULL;
					destroySession = true;
					return err;
				}
			}
			break;

			case ftv_PCC_Service:
			{
				ASSERT(NULL==m_pCC_Service && 0==strcmp("PCC_Service", faceName));
				m_pCC_Service = tcps_NewEx(PCC_Service_S, (m_sessionMaker, this, NULL));
				TCPSError err = TCPS_ERROR;
				try
				{
					err = m_pCC_Service->OnConnected(sessionKey, peerID_IPP, sessionCount2);
				}
				catch(TCPSError ret)
				{
					ASSERT(TCPS_OK != ret);
					err = ret;
				}
				catch(int ret)
				{
					ASSERT(TCPS_OK != ret);
					err = (TCPSError)ret;
				}
				catch(std::bad_alloc)
				{
					NPLogError(("PCC_Service_S::OnConnected(), Out of memory"));
					err = TCPS_ERR_OUT_OF_MEMORY;
				}
				ISCM_BEGIN_CATCH_ALL_()
					NPLogError(("PCC_Service_S::OnConnected(), Unknown exception"));
					err = TCPS_ERR_UNKNOWN_EXCEPTION;
				ISCM_END_CATCH_ALL_()
				if(TCPS_OK != err)
				{
					ISCM_BEGIN_TRY_()
						m_pCC_Service->OnClose(sessionKey, peerID_IPP, err);
					ISCM_END_TRY_()
					ISCM_BEGIN_CATCH_ALL_()
					ISCM_END_CATCH_ALL_()
					m_sessionMaker.OnSessionClosed_();
					m_pCC_Service->~PCC_Service_S();
					tcps_Free(m_pCC_Service);
					m_pCC_Service = NULL;
					destroySession = true;
					return err;
				}
			}
			break;

			case ftv_PCC_Toolkit:
			{
				ASSERT(NULL==m_pCC_Toolkit && 0==strcmp("PCC_Toolkit", faceName));
				m_pCC_Toolkit = tcps_NewEx(PCC_Toolkit_S, (m_sessionMaker, this, NULL));
				TCPSError err = TCPS_ERROR;
				try
				{
					err = m_pCC_Toolkit->OnConnected(sessionKey, peerID_IPP, sessionCount2);
				}
				catch(TCPSError ret)
				{
					ASSERT(TCPS_OK != ret);
					err = ret;
				}
				catch(int ret)
				{
					ASSERT(TCPS_OK != ret);
					err = (TCPSError)ret;
				}
				catch(std::bad_alloc)
				{
					NPLogError(("PCC_Toolkit_S::OnConnected(), Out of memory"));
					err = TCPS_ERR_OUT_OF_MEMORY;
				}
				ISCM_BEGIN_CATCH_ALL_()
					NPLogError(("PCC_Toolkit_S::OnConnected(), Unknown exception"));
					err = TCPS_ERR_UNKNOWN_EXCEPTION;
				ISCM_END_CATCH_ALL_()
				if(TCPS_OK != err)
				{
					ISCM_BEGIN_TRY_()
						m_pCC_Toolkit->OnClose(sessionKey, peerID_IPP, err);
					ISCM_END_TRY_()
					ISCM_BEGIN_CATCH_ALL_()
					ISCM_END_CATCH_ALL_()
					m_sessionMaker.OnSessionClosed_();
					m_pCC_Toolkit->~PCC_Toolkit_S();
					tcps_Free(m_pCC_Toolkit);
					m_pCC_Toolkit = NULL;
					destroySession = true;
					return err;
				}
			}
			break;
		//[[end_OnConnected]]
			default:
			ASSERT(false);
			break;
		}

		m_peerID_IPP = peerID_IPP;
		m_bindedInterface = faceName;
		BYTE* replyData = (BYTE*)tcps_Alloc(sizeof(INT64)+sizeof(INT32));
		*(INT64*)replyData = (INT64)this;
		*(INT32*)(replyData+sizeof(INT64)) = sessionKey;
		outfiter->Push(replyData, sizeof(INT64)+sizeof(INT32), true, NULL);
		m_sessionMaker.m_sessionRegister.UpdateFaceStat_(iscm_SessionRegister::ufst_add, m_peerID_IPP, m_bindedInterface);
		return TCPS_OK;
	}
	else if(RPCCT_TO_CALLBACK == req.cmd)
	{
		if(m_sessionDummyPtr)
		{
			NPLogError(("RPCCT_TO_CALLBACK: %s(%d)", tcps_GetErrTxt(TCPS_ERR_REFUSED), TCPS_ERR_REFUSED));
			destroySession = true;
			return TCPS_ERR_REFUSED;
		}
		if(req.len != (int)(sizeof(req)+sizeof(INT64)+sizeof(INT32)))
		{
			NPLogError(("RPCCT_TO_CALLBACK: %s(%d)", tcps_GetErrTxt(TCPS_ERR_MALFORMED_REQ), TCPS_ERR_MALFORMED_REQ));
			destroySession = true;
			return TCPS_ERR_MALFORMED_REQ;
		}
		char peerIPPTxt[32];
		GetIPPortTxt(m_peerIPP, peerIPPTxt);

		iscm_IRPCServeMan* const serveMan = m_serveMan;
		if(NULL == serveMan)
			return TCPS_ERR_SESSION_NOT_CONNECTED;
		SOCKET sock;
		TCPSError err = serveMan->DropSession(this, sock);
		if(TCPS_OK != err)
		{
			NPLogError(("NP_SCATTEREDSession::DropSession(%s) failed, %s(%d)", peerIPPTxt, tcps_GetErrTxt(err), err));
			destroySession = true;
			return err;
		}

		INT64 const bindedSessionID = *(INT64*)data;
		INT32 const bindedSessionKey = *(INT32*)((BYTE*)data + sizeof(INT64));
		if(0==bindedSessionID || 0==bindedSessionKey)
		{
			// 容错处理
			NPLogError(("NP_SCATTEREDSession::BindCallbackSocket(%s) failed, bindedSessionID is NULL", peerIPPTxt));
			VERIFY(0 == SockClose(sock));
			destroySession = true;
			return TCPS_ERR_INVALID_PARAM;
		}

		NP_SCATTEREDSession* const bindedSession = (NP_SCATTEREDSession*)bindedSessionID;
		INT32 const sessionKey = bindedSession->GetSessionKey();
		if(0 == sessionKey)
			err = TCPS_ERR_SESSION_NOT_CONNECTED;
		else if(sessionKey != bindedSessionKey)
			err = TCPS_ERR_SESSION_NOT_EXISTED;
		else
			err = m_sessionMaker.m_sessionRegister.BindCallbackSocket_(bindedSession, sock, peerIPPTxt);
		if(TCPS_OK != err)
		{
#if defined(__linux__) && (32==NPR_SYS_BITS)
			NPLogError(("%s::BindCallbackSocket(%s, 0x%08X%08X) failed, %s(%d)"
						, m_bindedInterface.Get()
						, peerIPPTxt
						, (UINT)((UINT64)bindedSessionID >> 32)
						, (UINT)bindedSessionID
						, tcps_GetErrTxt(err), err
						));
#else
			NPLogError(("%s::BindCallbackSocket(%s, 0x%08X%08X) failed, %s(%d)"
						, m_bindedInterface.Get()
						, peerIPPTxt
						, (DWORD)((UINT64)bindedSessionID >> 32)
						, (DWORD)bindedSessionID
						, tcps_GetErrTxt(err), err
						));
#endif
			// 发送准确的错误信息给客户端
			iscm_RPCReply reply;
			reply.len = sizeof(reply);
			reply.ret = err;
			DWORD timeoutMSELs = iscm_GetDefaultSendTimeout();
			SockMustWriteByTime(sock, &reply, sizeof(reply), timeoutMSELs);
			VERIFY(0 == SockSetLinger(sock, true, timeoutMSELs)); // 设置关闭延迟发送超时，以使客户端收到准确错误信息
			VERIFY(0 == SockClose(sock));
			destroySession = true;
			return err;
		}

		return TCPS_ERR_SESSION_DROPED;
	}
	else if(RPCCT_TO_POSTING_CHANNEL == req.cmd)
	{
		if(m_sessionDummyPtr)
		{
			NPLogError(("RPCCT_TO_POSTING_CHANNEL: %s(%d)", tcps_GetErrTxt(TCPS_ERR_REFUSED), TCPS_ERR_REFUSED));
			destroySession = true;
			return TCPS_ERR_REFUSED;
		}
		if(req.len != (int)(sizeof(req)+sizeof(INT64)+sizeof(INT32)))
		{
			NPLogError(("RPCCT_TO_POSTING_CHANNEL: %s(%d)", tcps_GetErrTxt(TCPS_ERR_MALFORMED_REQ), TCPS_ERR_MALFORMED_REQ));
			destroySession = true;
			return TCPS_ERR_MALFORMED_REQ;
		}
		char peerIPPTxt[32];
		IPPToString(m_peerIPP, peerIPPTxt);

		iscm_IRPCServeMan* const serveMan = m_serveMan;
		if(NULL == serveMan)
			return TCPS_ERR_SESSION_NOT_CONNECTED;
		SOCKET sock;
		TCPSError err = serveMan->DropSession(this, sock);
		if(TCPS_OK != err)
		{
			NPLogError(("NP_SCATTEREDSession::DropSession(%s) failed, %s(%d)", peerIPPTxt, tcps_GetErrTxt(err), err));
			destroySession = true;
			return err;
		}

		INT64 const bindedSessionID = *(INT64*)data;
		INT32 const bindedSessionKey = *(INT32*)((BYTE*)data + sizeof(INT64));
		if(0==bindedSessionID || 0==bindedSessionKey)
		{
			// 容错处理
			NPLogError(("NP_SCATTEREDSession::BindPostingSocket(%s) failed, bindedSessionID is NULL", peerIPPTxt));
			VERIFY(0 == SockClose(sock));
			destroySession = true;
			return TCPS_ERR_INVALID_PARAM;
		}

		NP_SCATTEREDSession* const bindedSession = (NP_SCATTEREDSession*)bindedSessionID;
		INT32 const sessionKey = bindedSession->GetSessionKey();
		if(0 == sessionKey)
			err = TCPS_ERR_SESSION_NOT_CONNECTED;
		else if(sessionKey != bindedSessionKey)
			err = TCPS_ERR_SESSION_NOT_EXISTED;
		else
			err = m_sessionMaker.m_sessionRegister.BindPostingSocket_(bindedSession, sock, peerIPPTxt);
		if(TCPS_OK != err)
		{
#if defined(__linux__) && (32==NPR_SYS_BITS)
			NPLogError(("%s::BindPostingSocket(%s, 0x%08X%08X) failed, %s(%d)"
						, m_bindedInterface.Get()
						, peerIPPTxt
						, (UINT)((UINT64)bindedSessionID >> 32)
						, (UINT)bindedSessionID
						, tcps_GetErrTxt(err), err
						));
#else
			NPLogError(("%s::BindPostingSocket(%s, 0x%08X%08X) failed, %s(%d)"
						, m_bindedInterface.Get()
						, peerIPPTxt
						, (DWORD)((UINT64)bindedSessionID >> 32)
						, (DWORD)bindedSessionID
						, tcps_GetErrTxt(err), err
						));
#endif
			// 发送准确的错误信息给客户端
			iscm_RPCReply reply;
			reply.len = sizeof(reply);
			reply.ret = err;
			DWORD timeoutMSELs = iscm_GetDefaultSendTimeout();
			SockMustWriteByTime(sock, &reply, sizeof(reply), timeoutMSELs);
			VERIFY(0 == SockSetLinger(sock, true, timeoutMSELs)); // 设置关闭延迟发送超时，以使客户端收到准确错误信息
			VERIFY(0 == SockClose(sock));
			destroySession = true;
			return err;
		}

		return TCPS_ERR_SESSION_DROPED;
	}

	NPLogWarning(("NP_SCATTEREDSession::OnCall(), Invalid cmd %d from '%s'", req.cmd, IPP_TO_STR_A(m_peerIPP)));
	destroySession = true;
	return TCPS_ERR_NOT_SUPPORTED;
}

iscm_IRPCServeMan* NP_SCATTEREDSession::GetServeMan() const
	{	return m_serveMan;	}

void NP_SCATTEREDSession::OnUDPCall(
				IN tcps_Binary& frame
				)
{
	ASSERT(frame.Length() > 0);
	while(true)
	{
		BOOL requireReplyData = true;
		const void* inParamsData = frame.Get();
		BOOL dataTransferred = false;
		INT_PTR inParamsDataLen = frame.Length();
		TCPSError err = OnRPCCall_(this, NULL, requireReplyData, inParamsData, dataTransferred, inParamsDataLen, NULL/*outfiter*/);
		ASSERT(TCPS_OK!=err || !requireReplyData);
		if(TCPS_ERR_POSTING_PENDING_FULL == err)
		{
			ASSERT(!dataTransferred);
			Sleep(1);
			continue;
		}
		else
		{
			if(dataTransferred)
				frame.Drop();
			break;
		}
	}
}

TCPSError NP_SCATTEREDSession::BindCallbackSocket_(
				IN SOCKET sock,
				IN const char* peerIPPTxt
				)
{
	ASSERT(SockValid(sock));
	if(NULL == m_serveMan)
		return TCPS_ERR_SESSION_NOT_CONNECTED; // 客户端正在做异步重连恢复，此时又发生了客户端对象被关闭动作

	if(NULL == m_callbackRequester.face_)
	{
		m_callbackRequester.face_ = iscm_MakeRequester(false);
		if(NULL == m_callbackRequester.face_)
		{
			ASSERT(false);
			return TCPS_ERR_CALLBACK_BIND_FAILED; // 容错处理
		}
	}

	// 肯定可以成功转化为回调通道，在AttachSocket()之前发送成功回应码，
	// 以避免若马上转化为异步并行发送模式，可能发生发送的普通数据被当回应码的问题。
	iscm_RPCReply reply;
	reply.len = sizeof(reply);
	reply.ret = TCPS_OK;
	if((int)sizeof(reply) != SockWriteAllByTime(sock, &reply, sizeof(reply), iscm_GetDefaultSendTimeout()))
		NPLogError(("NP_SCATTEREDSession::BindCallbackSocket_(%s), send reply failed", peerIPPTxt));
	else
		ASSERT(true); // NPLogInfo(("NP_SCATTEREDSession::BindCallbackSocket_(%s) ok", peerIPPTxt));

	TCPSError err = m_callbackRequester.face_->AttachSocket(sock);
	ASSERT(TCPS_OK == err);

	ASSERT(sm_FTVMap);
	FTVMap_::iterator itFTV = sm_FTVMap->find(m_bindedInterface.Get());
	if(itFTV != sm_FTVMap->end())
	{
		switch(itFTV->second)
		{
		//[[begin_OnCallbackReady]]
			case ftv_GRID_User:
				m_gRID_User->OnCallbackReady();
				break;
			case ftv_PCC_Scatter:
				m_pCC_Scatter->OnCallbackReady();
				break;
			case ftv_PCC_Service:
				m_pCC_Service->OnCallbackReady();
				break;
			case ftv_PCC_Toolkit:
				m_pCC_Toolkit->OnCallbackReady();
				break;
		//[[end_OnCallbackReady]]
			default:
				ASSERT(false);
				break;
		}
	}

	return err;
}

TCPSError NP_SCATTEREDSession::BindPostingSocket_(
				IN SOCKET sock,
				IN const char* peerIPPTxt
				)
{
	ASSERT(SockValid(sock));
	ASSERT(0 == m_postingProxy.callerKey_);
	if(NULL == m_serveMan)
		return TCPS_ERR_SESSION_NOT_CONNECTED; // 客户端正在做异步重连恢复，此时又发生了客户端对象被关闭动作

	// 肯定可以成功转化为回调通道，在AddSession()之前发送成功回应码，
	// 以避免若马上转化为异步发送模式，可能发生发送的普通数据被当回应码的问题。
	iscm_RPCReply reply;
	reply.len = sizeof(reply);
	reply.ret = TCPS_OK;
	if((int)sizeof(reply) != SockMustWriteByTime(sock, &reply, sizeof(reply), iscm_GetDefaultSendTimeout()))
		NPLogError(("NP_SCATTEREDSession::BindCallbackSocket_(%s), send reply failed", peerIPPTxt));
	else
		ASSERT(true); // NPLogInfo(("NP_SCATTEREDSession::BindCallbackSocket_(%s) ok", peerIPPTxt));

	TCPSError err = iscm_FetchPostingCallerMan().AddSession(m_postingProxy.callerKey_, sock, true, &m_postingProxy);
	ASSERT(TCPS_OK==err /*&& 0!=m_postingProxy.callerKey_*/);
	if(0 == m_postingProxy.callerKey_)
		return err; // 极端情况处理（可能此时立即又发生了网络断线，m_postingProxy.callerKey_已被清理为0）

	ASSERT(sm_FTVMap);
	FTVMap_::iterator itFTV = sm_FTVMap->find(m_bindedInterface.Get());
	if(itFTV != sm_FTVMap->end())
	{
		switch(itFTV->second)
		{
		//[[begin_OnPostingCallReady]]
			case ftv_GRID_User:
				if(!m_gRID_User->m_postingSendParam.IsDefault())
					iscm_FetchPostingCallerMan().SetBufferingParam(m_postingProxy.callerKey_, m_gRID_User->m_postingSendParam.maxSendingBytes_, m_gRID_User->m_postingSendParam.maxSendings_);
				m_gRID_User->OnPostingCallReady();
				break;
			case ftv_PCC_Scatter:
				if(!m_pCC_Scatter->m_postingSendParam.IsDefault())
					iscm_FetchPostingCallerMan().SetBufferingParam(m_postingProxy.callerKey_, m_pCC_Scatter->m_postingSendParam.maxSendingBytes_, m_pCC_Scatter->m_postingSendParam.maxSendings_);
				m_pCC_Scatter->OnPostingCallReady();
				break;
			case ftv_PCC_Service:
				if(!m_pCC_Service->m_postingSendParam.IsDefault())
					iscm_FetchPostingCallerMan().SetBufferingParam(m_postingProxy.callerKey_, m_pCC_Service->m_postingSendParam.maxSendingBytes_, m_pCC_Service->m_postingSendParam.maxSendings_);
				m_pCC_Service->OnPostingCallReady();
				break;
			case ftv_PCC_Toolkit:
				if(!m_pCC_Toolkit->m_postingSendParam.IsDefault())
					iscm_FetchPostingCallerMan().SetBufferingParam(m_postingProxy.callerKey_, m_pCC_Toolkit->m_postingSendParam.maxSendingBytes_, m_pCC_Toolkit->m_postingSendParam.maxSendings_);
				m_pCC_Toolkit->OnPostingCallReady();
				break;
		//[[end_OnPostingCallReady]]
			default:
				ASSERT(false);
				break;
		}
	}

	return err;
}

void NP_SCATTEREDSession::PostingTask_::OnPoolTask()
{
	ASSERT(baseInParamsData_ && inParamsData_ && inParamsDataLen_>=0);
	ASSERT((INT_PTR)(inParamsData_ - (const BYTE*)baseInParamsData_) > 0);
	INT_PTR const inParamsDataLen_t = inParamsDataLen_;
	if(session_.m_serveMan)
	{
		CNPAutoLock locker(session_.m_methodCallLock);
		ASSERT(INVALID_OSTHREADID == session_.m_callingPostingTaskTID);
		DEBUG_EXP(session_.m_callingPostingTaskTID = OSThreadSelf());
		TCPSError err = (*handler_)(
								&session_,
								NULL,
								peerCallFlags_,
								inParamsData_,
								inParamsDataLen_,
								NULL/*outfiter*/
								);
		DEBUG_EXP(session_.m_callingPostingTaskTID = INVALID_OSTHREADID);
		ASSERT(TCPS_OK!=err || 0==inParamsDataLen_);
		(void)err;
	}
	else
		ASSERT(true); // 处于退出等待状态

	tcps_Free(baseInParamsData_);
	NP_SCATTEREDSession& session = session_;
	tcps_Delete(this);

	CNPAutoLock locker(session.m_postingMethods.lock);
	ASSERT(session.m_postingMethods.callingsDataBytes >= inParamsDataLen_t);
	session.m_postingMethods.callingsDataBytes -= inParamsDataLen_t;
	ASSERT(session.m_postingMethods.callingCount > 0);
	--(session.m_postingMethods.callingCount);
	if(NULL == session.m_postingMethods.head)
	{
		ASSERT(0 == session.m_postingMethods.callingCount);
		return; // 队列处理完毕
	}
	else
		ASSERT(session.m_postingMethods.callingCount > 0);

	NPBaseTask* task = session.m_postingMethods.head;
	if(session.m_postingMethods.head == session.m_postingMethods.tail)
		session.m_postingMethods.head = session.m_postingMethods.tail = NULL;
	else
		session.m_postingMethods.head = task->m_nextPoolTask;
	FetchTaskPool()->Push(task);
}

TCPSError NP_SCATTEREDSession::OnRPCCall_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				OUT BOOL& requireReplyData /*= true*/,
				IN const void* inParamsData,
				OUT BOOL& dataTransferred /*= false*/,
				IN INT_PTR inParamsDataLen,
				OUT iscm_IRPCOutfiter* outfiter
				)
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	ASSERT(requireReplyData && inParamsData && inParamsDataLen>0/* && outfiter*/);
	const BYTE* ptrInParams = (const BYTE*)inParamsData;
	INT_PTR ptrInParamsLen = inParamsDataLen;

	if(ptrInParamsLen < (int)sizeof(iscm_PeerCallFlags))
		return TCPS_ERR_MALFORMED_REQ;
	iscm_PeerCallFlags peerCallFlags = *(iscm_PeerCallFlags*)ptrInParams;
	if((int)sizeof(BOOL) != peerCallFlags.sizeofBOOL_req)
		return TCPS_ERR_MALFORMED_REQ;
	if((int)sizeof(DummyEnumType) != peerCallFlags.sizeofEnum_req)
		return TCPS_ERR_MALFORMED_REQ;
	ptrInParams += sizeof(iscm_PeerCallFlags);
	ptrInParamsLen -= sizeof(iscm_PeerCallFlags);

	if(ptrInParamsLen < (int)sizeof(INT32))
		return TCPS_ERR_MALFORMED_REQ;
	INT32 call_id_len = *(INT32*)ptrInParams;
	ptrInParams += sizeof(INT32);
	ptrInParamsLen -= sizeof(INT32);
	if(ptrInParamsLen < call_id_len+1)
		return TCPS_ERR_MALFORMED_REQ;
	const char* call_id = (const char*)ptrInParams;
	ptrInParams += call_id_len+1;
	ptrInParamsLen -= call_id_len+1;
	const char* call_id_sep = strstr(call_id, "::");
	if(NULL == call_id_sep)
		return TCPS_ERR_INVALID_PARAM;
	if(thisObj)
	{
		//ASSERT(!thisObj->m_bindedInterface.IsEmpty());
		if(thisObj->m_bindedInterface.Length() != call_id_sep-call_id
			|| 0 != strncmp(thisObj->m_bindedInterface.Get(), call_id, call_id_sep-call_id))
			return TCPS_ERR_METHOD_NOT_MATCH;
	}

	InitFTVMap_();
	ASSERT(sm_methodCallMap);
	MethodCallMap_::const_iterator cit = sm_methodCallMap->find(call_id, call_id_len);
	if(sm_methodCallMap->end() == cit)
		return TCPS_ERR_NOT_SUPPORTED;

	// posting方法/回调
	if(cit->second.isPosting)
	{
		if(thisObj && NULL==thisObj->m_serveMan)
			return TCPS_ERR_EXITING;

		// 当缓存的未决调用小于1个时，直接调用
		if(NULL==thisObj || thisObj->m_postingPendingParam.maxPendings_<=1)
		{
			TCPSError errIscm;
			if(thisObj)
			{
				CNPAutoLock locker(thisObj->m_methodCallLock);
				errIscm = (*(cit->second.handler))(thisObj, faceObj, peerCallFlags, ptrInParams, ptrInParamsLen, NULL/*outfiter*/);
			}
			else
			{
				errIscm = (*(cit->second.handler))(thisObj, faceObj, peerCallFlags, ptrInParams, ptrInParamsLen, NULL/*outfiter*/);
			}

#ifdef _DEBUG
			if(TCPS_OK == errIscm)
			{
				ASSERT(ptrInParams-(const BYTE*)inParamsData == inParamsDataLen);
				ASSERT(0 == ptrInParamsLen);
			}
#else
			(void)errIscm;
#endif
		//	ASSERT(!peerCallFlags.requireReply); // ??
			requireReplyData = false;
			ASSERT(!dataTransferred);
			return TCPS_OK;
		}

		// 投递一个task，异步执行
		ASSERT(thisObj);
		if(thisObj->m_postingMethods.callingsDataBytes >= thisObj->m_postingPendingParam.maxPendingBytes_
			|| thisObj->m_postingMethods.callingCount >= thisObj->m_postingPendingParam.maxPendings_
			)
		{
		//	NPLogWarning(("Posting calling lost: ([total, %s bytes] >= [max, %s bytes] || [total, %d calls] >= [max, %d calls])"
		//				, INT64_TO_STR_A(thisObj->m_postingMethods.callingsDataBytes), INT64_TO_STR_A(thisObj->m_postingPendingParam.maxPendingBytes_)
		//				, thisObj->m_postingMethods.callingCount, thisObj->m_postingPendingParam.maxPendings_
		//				));
			return TCPS_ERR_POSTING_PENDING_FULL;
		}
		PostingTask_* task = tcps_NewEx(PostingTask_, (*thisObj));
		task->peerCallFlags_ = peerCallFlags;
		task->baseInParamsData_ = (void*)inParamsData;
		task->inParamsData_ = ptrInParams;
		task->inParamsDataLen_ = ptrInParamsLen;
		task->handler_ = cit->second.handler;
		{
			CNPAutoLock locker(thisObj->m_postingMethods.lock);
			thisObj->m_postingMethods.callingsDataBytes += ptrInParamsLen;
			++(thisObj->m_postingMethods.callingCount);
			if(thisObj->m_postingMethods.callingCount > 1)
			{
				task->m_nextPoolTask = NULL;
				if(thisObj->m_postingMethods.tail)
				{
					thisObj->m_postingMethods.tail->m_nextPoolTask = task;
					thisObj->m_postingMethods.tail = task;
				}
				else
					thisObj->m_postingMethods.head = thisObj->m_postingMethods.tail = task;
			}
			else
			{
				ASSERT(NULL==thisObj->m_postingMethods.head && NULL==thisObj->m_postingMethods.tail);
				ASSERT(ptrInParamsLen==thisObj->m_postingMethods.callingsDataBytes && 1==thisObj->m_postingMethods.callingCount);
				FetchTaskPool()->Push(task);
			}
		}

		if(peerCallFlags.requireReply)
		{
			// 给outfiter填充内置返回参数
			struct PostOutParamWrapSite
			{
				iscm_RPCReplyPrefix replyPrefix;
				PostOutParamWrapSite() { replyPrefix.Init(); }
				static void FreeAction(void* p)
				{
					PostOutParamWrapSite* ptr = (PostOutParamWrapSite*)((BYTE*)p - offset_of(PostOutParamWrapSite, replyPrefix));
					ptr->~PostOutParamWrapSite();
					tcps_Free(ptr);
				}
			};
			PostOutParamWrapSite* opWrapper = tcps_New(PostOutParamWrapSite);
			Set_BaseType_(outfiter, opWrapper->replyPrefix, PostOutParamWrapSite::FreeAction);
		}
		else
		{
			// 指示不需要发送回应数据了
			requireReplyData = false;
		}

		dataTransferred = true; // 指示输入参数数据指针inParamsData被转移
		return TCPS_OK; // 立即返回成功
	}

	// 非posting方法/回调
	TCPSError errTcps;
	if(thisObj)
	{
		CNPAutoLock locker(thisObj->m_methodCallLock);
		errTcps = (*(cit->second.handler))(thisObj, faceObj, peerCallFlags, ptrInParams, ptrInParamsLen, outfiter);
	}
	else
	{
		errTcps = (*(cit->second.handler))(thisObj, faceObj, peerCallFlags, ptrInParams, ptrInParamsLen, outfiter);
	}
#ifdef _DEBUG
	if(TCPS_OK == errTcps)
	{
		ASSERT(ptrInParams-(const BYTE*)inParamsData == inParamsDataLen);
		ASSERT(0 == ptrInParamsLen);
	}
#endif
	return errTcps;
}

//[[begin_method_wrap_body]]

static const char* s_GRID_User_AddJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_AddJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_DelJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_FindJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_FindJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJobPrograms_TypeInfo_ = NULL;
static int s_GRID_User_ListJobPrograms_TypeInfo_len_ = 0;

static const char* s_GRID_User_CommitJob_TypeInfo_ = NULL;
static int s_GRID_User_CommitJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_ReplyCommitJob_TypeInfo_ = NULL;
static int s_GRID_User_ReplyCommitJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_CancelJob_TypeInfo_ = NULL;
static int s_GRID_User_CancelJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_SetJobPriority_TypeInfo_ = NULL;
static int s_GRID_User_SetJobPriority_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJobs_TypeInfo_ = NULL;
static int s_GRID_User_ListJobs_TypeInfo_len_ = 0;

static const char* s_GRID_User_QueryJobs_TypeInfo_ = NULL;
static int s_GRID_User_QueryJobs_TypeInfo_len_ = 0;

static const char* s_GRID_User_UpdateGrid_TypeInfo_ = NULL;
static int s_GRID_User_UpdateGrid_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListModules_TypeInfo_ = NULL;
static int s_GRID_User_ListModules_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetLogCount_TypeInfo_ = NULL;
static int s_GRID_User_GetLogCount_TypeInfo_len_ = 0;

static const char* s_GRID_User_LoadLogInfos_TypeInfo_ = NULL;
static int s_GRID_User_LoadLogInfos_TypeInfo_len_ = 0;

static const char* s_GRID_User_Login_TypeInfo_ = NULL;
static int s_GRID_User_Login_TypeInfo_len_ = 0;

static const char* s_GRID_User_Logout_TypeInfo_ = NULL;
static int s_GRID_User_Logout_TypeInfo_len_ = 0;

static const char* s_GRID_User_AddUser_TypeInfo_ = NULL;
static int s_GRID_User_AddUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelUser_TypeInfo_ = NULL;
static int s_GRID_User_DelUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_UpdatePassword_TypeInfo_ = NULL;
static int s_GRID_User_UpdatePassword_TypeInfo_len_ = 0;

static const char* s_GRID_User_ManageUser_TypeInfo_ = NULL;
static int s_GRID_User_ManageUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListAllUsers_TypeInfo_ = NULL;
static int s_GRID_User_ListAllUsers_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetUserInfo_TypeInfo_ = NULL;
static int s_GRID_User_GetUserInfo_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJTMs_TypeInfo_ = NULL;
static int s_GRID_User_ListJTMs_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetJTMInfo_TypeInfo_ = NULL;
static int s_GRID_User_GetJTMInfo_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetGridProperty_TypeInfo_ = NULL;
static int s_GRID_User_GetGridProperty_TypeInfo_len_ = 0;

static const char* s_GRID_User_AddSplitter_TypeInfo_ = NULL;
static int s_GRID_User_AddSplitter_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelSplitter_TypeInfo_ = NULL;
static int s_GRID_User_DelSplitter_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListSplitters_TypeInfo_ = NULL;
static int s_GRID_User_ListSplitters_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListSplitterParam_TypeInfo_ = NULL;
static int s_GRID_User_ListSplitterParam_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetGridStatistics_TypeInfo_ = NULL;
static int s_GRID_User_GetGridStatistics_TypeInfo_len_ = 0;

static const char* s_GRID_User_UDPLink__TypeInfo_ = NULL;
static int s_GRID_User_UDPLink__TypeInfo_len_ = 0;

static const char* s_GRID_User_UDPLinkConfirm__TypeInfo_ = NULL;
static int s_GRID_User_UDPLinkConfirm__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetRedirect__TypeInfo_ = NULL;
static int s_GRID_User_SetRedirect__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetTimeout__TypeInfo_ = NULL;
static int s_GRID_User_SetTimeout__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetSessionBufferSize__TypeInfo_ = NULL;
static int s_GRID_User_SetSessionBufferSize__TypeInfo_len_ = 0;

static const char* s_GRID_User_MethodCheck__TypeInfo_ = NULL;
static int s_GRID_User_MethodCheck__TypeInfo_len_ = 0;

static const char* s_GRID_User_CallbackCheck__TypeInfo_ = NULL;
static int s_GRID_User_CallbackCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_Compute_TypeInfo_ = NULL;
static int s_PCC_Scatter_Compute_TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_OnComputed_TypeInfo_ = NULL;
static int s_PCC_Scatter_OnComputed_TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_AddMoudle_TypeInfo_ = NULL;
static int s_PCC_Scatter_AddMoudle_TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_RemoveModule_TypeInfo_ = NULL;
static int s_PCC_Scatter_RemoveModule_TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_ListModules_TypeInfo_ = NULL;
static int s_PCC_Scatter_ListModules_TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_UDPLink__TypeInfo_ = NULL;
static int s_PCC_Scatter_UDPLink__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_UDPLinkConfirm__TypeInfo_ = NULL;
static int s_PCC_Scatter_UDPLinkConfirm__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_SetRedirect__TypeInfo_ = NULL;
static int s_PCC_Scatter_SetRedirect__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_SetTimeout__TypeInfo_ = NULL;
static int s_PCC_Scatter_SetTimeout__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_SetSessionBufferSize__TypeInfo_ = NULL;
static int s_PCC_Scatter_SetSessionBufferSize__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_MethodCheck__TypeInfo_ = NULL;
static int s_PCC_Scatter_MethodCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Scatter_CallbackCheck__TypeInfo_ = NULL;
static int s_PCC_Scatter_CallbackCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Service_Login_TypeInfo_ = NULL;
static int s_PCC_Service_Login_TypeInfo_len_ = 0;

static const char* s_PCC_Service_Logout_TypeInfo_ = NULL;
static int s_PCC_Service_Logout_TypeInfo_len_ = 0;

static const char* s_PCC_Service_ListModules_TypeInfo_ = NULL;
static int s_PCC_Service_ListModules_TypeInfo_len_ = 0;

static const char* s_PCC_Service_GetModuleFile_TypeInfo_ = NULL;
static int s_PCC_Service_GetModuleFile_TypeInfo_len_ = 0;

static const char* s_PCC_Service_Execute_TypeInfo_ = NULL;
static int s_PCC_Service_Execute_TypeInfo_len_ = 0;

static const char* s_PCC_Service_OnExecuted_TypeInfo_ = NULL;
static int s_PCC_Service_OnExecuted_TypeInfo_len_ = 0;

static const char* s_PCC_Service_QueryJobs_TypeInfo_ = NULL;
static int s_PCC_Service_QueryJobs_TypeInfo_len_ = 0;

static const char* s_PCC_Service_CancelJob_TypeInfo_ = NULL;
static int s_PCC_Service_CancelJob_TypeInfo_len_ = 0;

static const char* s_PCC_Service_UDPLink__TypeInfo_ = NULL;
static int s_PCC_Service_UDPLink__TypeInfo_len_ = 0;

static const char* s_PCC_Service_UDPLinkConfirm__TypeInfo_ = NULL;
static int s_PCC_Service_UDPLinkConfirm__TypeInfo_len_ = 0;

static const char* s_PCC_Service_SetRedirect__TypeInfo_ = NULL;
static int s_PCC_Service_SetRedirect__TypeInfo_len_ = 0;

static const char* s_PCC_Service_SetTimeout__TypeInfo_ = NULL;
static int s_PCC_Service_SetTimeout__TypeInfo_len_ = 0;

static const char* s_PCC_Service_SetSessionBufferSize__TypeInfo_ = NULL;
static int s_PCC_Service_SetSessionBufferSize__TypeInfo_len_ = 0;

static const char* s_PCC_Service_MethodCheck__TypeInfo_ = NULL;
static int s_PCC_Service_MethodCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Service_CallbackCheck__TypeInfo_ = NULL;
static int s_PCC_Service_CallbackCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_Login_TypeInfo_ = NULL;
static int s_PCC_Toolkit_Login_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_Logout_TypeInfo_ = NULL;
static int s_PCC_Toolkit_Logout_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_AddModule_TypeInfo_ = NULL;
static int s_PCC_Toolkit_AddModule_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_AddModuleFile_TypeInfo_ = NULL;
static int s_PCC_Toolkit_AddModuleFile_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_RemoveModule_TypeInfo_ = NULL;
static int s_PCC_Toolkit_RemoveModule_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_ = NULL;
static int s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_ListModules_TypeInfo_ = NULL;
static int s_PCC_Toolkit_ListModules_TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_SetRedirect__TypeInfo_ = NULL;
static int s_PCC_Toolkit_SetRedirect__TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_SetTimeout__TypeInfo_ = NULL;
static int s_PCC_Toolkit_SetTimeout__TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_ = NULL;
static int s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_MethodCheck__TypeInfo_ = NULL;
static int s_PCC_Toolkit_MethodCheck__TypeInfo_len_ = 0;

static const char* s_PCC_Toolkit_CallbackCheck__TypeInfo_ = NULL;
static int s_PCC_Toolkit_CallbackCheck__TypeInfo_len_ = 0;

static INT_PTR InitializeAllCallsTypeInfo_()
{
	// BOOL和枚举类型长度必须为4字节（网络协议已规定为固定4字节）
	STATIC_ASSERT(4 == sizeof(BOOL));
	STATIC_ASSERT(4 == sizeof(DummyEnumType));

	BEGIN_LOCAL_SAFE_STATIC_OBJ(INT_PTR, dummyVar);

	{
		BYTE const chZipped[] =
		{
			0x75,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0xC2,0x20,0x14,0x86,0xF7,0x5F,
			0xEC,0x25,0x17,0xB5,0xED,0x3A,0x8D,0xEB,0x12,0xAB,0x9B,0xE9,0x62,0xD4,0xF8,0xB5,
			0xDD,0x11,0x5A,0x99,0x25,0x5B,0xA1,0x01,0x8C,0x6D,0xE2,0x8F,0x1F,0x05,0xCC,0xCA,
			0x6E,0xC8,0x79,0x1F,0x0E,0xEF,0x39,0x9C,0xB3,0xD8,0x66,0x73,0x98,0x12,0x8A,0x78,
			0xBB,0x6F,0x6B,0x3C,0xC8,0x25,0xAC,0x50,0x51,0x12,0x8A,0xB7,0xE8,0x9A,0xF8,0x40,
			0xE9,0xF7,0xE9,0x71,0x0A,0xD3,0x56,0xE2,0x82,0x9D,0x70,0xD2,0x91,0xD9,0xAE,0x44,
			0xBC,0xEE,0xB1,0xC9,0xA2,0xB3,0x99,0x6D,0x0E,0xDA,0xA3,0xA8,0x2F,0xB0,0x19,0xC5,
			0xB0,0x89,0x23,0xE5,0xD0,0x29,0xC4,0xAB,0x44,0x07,0x15,0xA9,0x45,0x02,0x4C,0xFA,
			0x6B,0x83,0x8B,0x8B,0xC4,0x29,0x91,0x62,0x80,0x73,0x18,0x06,0xB9,0x8A,0x92,0x30,
			0x00,0x4A,0xC4,0x91,0x16,0x71,0x64,0x73,0xD7,0x3B,0xED,0xCC,0x84,0x84,0x07,0xFA,
			0x4D,0xD9,0x95,0x2A,0xE7,0x4E,0x7D,0x10,0x1A,0x06,0xCA,0xD2,0xC6,0x27,0x76,0x15,
			0x70,0xFC,0x08,0xC7,0x23,0x17,0xAD,0xF6,0xAE,0x0E,0x7C,0xDF,0x77,0xC9,0xE7,0xC6,
			0xD5,0x4B,0x46,0xCF,0x25,0xE3,0xD4,0xA5,0x47,0x22,0x24,0x72,0xD1,0x93,0x2B,0x6D,
			0xE1,0x25,0xA1,0x97,0x06,0x06,0x30,0x4A,0x86,0xBE,0xE9,0xF4,0x4E,0x62,0x45,0x86,
			0xF6,0x5B,0x1B,0xCE,0xCE,0x1C,0x55,0x6F,0xE4,0x07,0x7B,0xE9,0x7A,0xBD,0x04,0x42,
			0x72,0x42,0xCF,0x20,0xD7,0x0B,0x71,0x93,0xB2,0xB9,0x67,0x6F,0xF5,0xB0,0x8F,0x98,
			0x0B,0xC2,0x28,0xE8,0x4F,0xDE,0x08,0x33,0x2B,0x13,0xF7,0x66,0x6C,0xC0,0xDF,0xAA,
			0xFF,0xB9,0xD3,0x2F,0xE6,0x69,0x2F,0xDB,0x53,0x36,0xBF,0x37,0x63,0xB6,0x65,0xEB,
			0x79,0xD9,0x6A,0xAF,0x56,0x64,0xCE,0x49,0x85,0x65,0xC9,0x4E,0xB7,0x6C,0x75,0x73,
			0x9E,0x2A,0x2F,0x95,0x71,0x43,0x9C,0xA3,0xF6,0xB9,0x7F,0xD3,0x7D,0xF4,0x05,0x3C,
			0xFC,0x02,0xC9,0xC0,0xDC,0xC7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x18,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0xF1,0xF4,0x0B,0x31,0x33,0xB1,0xD3,0x61,0x00,0x00,
			0x74,0x73,0x08,0x2B,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x14,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0x41,0x6F,0x83,0x20,0x18,0x86,0xF7,0x5F,
			0xDA,0xA3,0x07,0x6B,0x9D,0x6B,0xD3,0x78,0xA8,0xED,0xD2,0xB8,0x18,0x6D,0x5A,0xEB,
			0x76,0x23,0xA8,0x44,0xC9,0x22,0x18,0xC0,0xA8,0x89,0x3F,0x7E,0x28,0x6C,0x2B,0x17,
			0xF2,0x3D,0x4F,0xC8,0x0B,0xBC,0x5C,0x6E,0xE1,0x19,0x04,0x98,0x40,0x36,0xA6,0x63,
			0x8B,0x56,0xB9,0x00,0x0D,0x2C,0x6A,0x4C,0xD0,0x0D,0xF6,0xBE,0x6D,0x49,0xFE,0x38,
			0x66,0x47,0x10,0x8C,0x02,0x15,0xB4,0x44,0xFE,0x6C,0x4E,0xF7,0x1A,0xB2,0xF6,0xC9,
			0x1D,0x2E,0x73,0xCC,0xE9,0xFA,0x58,0x32,0x8A,0xB6,0x03,0xC3,0xCE,0x03,0x83,0xE7,
			0xCA,0x84,0x99,0x20,0x6B,0xFC,0x65,0x68,0x70,0xCB,0x7D,0x4B,0x6D,0x7F,0x1F,0x50,
			0xD1,0x09,0x14,0x60,0xC1,0x57,0x28,0x07,0x5B,0x27,0x97,0x93,0xBF,0x75,0x2C,0x09,
			0x9E,0xBB,0x80,0xE7,0xEA,0xBD,0xC9,0x7D,0x49,0xA6,0x5C,0x80,0x07,0xF9,0x26,0xB4,
			0x27,0x32,0x79,0xA6,0x4F,0x4C,0xB6,0x8E,0x8C,0xD4,0x73,0x49,0x7B,0x0E,0xF6,0xAF,
			0x60,0xBF,0x33,0x55,0x9C,0x9A,0xEC,0xD8,0xB6,0x6D,0x9A,0xAF,0xAB,0xC9,0x11,0x25,
			0x55,0x4D,0x19,0x31,0x6D,0x86,0xB9,0x80,0xA6,0x7A,0x33,0x51,0x1F,0x1C,0x61,0xD2,
			0x0D,0xC0,0x01,0xAE,0xBF,0xB1,0xD5,0x4D,0x7F,0x8D,0x27,0xCD,0x46,0x3F,0xEB,0xCA,
			0x68,0xC5,0x60,0x13,0x9E,0xD7,0x5C,0x30,0x4C,0x2A,0x6B,0xE9,0x31,0x43,0x8C,0x63,
			0x4A,0x14,0xE8,0x52,0x15,0xA8,0x1A,0xD4,0xFC,0x54,0x9F,0x12,0xFF,0xBF,0xA8,0xD3,
			0x75,0xCE,0x3A,0x8C,0x53,0xD9,0xAA,0x5A,0x0F,0x0D,0x12,0x35,0x2D,0xA7,0x30,0x9E,
			0x96,0xB3,0xFE,0x6E,0x60,0x25,0x8F,0x74,0x0A,0x92,0x24,0xB2,0x5E,0x7E,0x00,0x21,
			0xBF,0xB9,0xB6,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_FindJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_FindJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x72,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0x82,0x30,0x14,0x86,0xF7,0x5F,
			0xF4,0xB2,0x17,0x88,0x8C,0xF9,0x31,0x96,0xF8,0x15,0xA3,0x61,0x6A,0x14,0xDC,0xEE,
			0x9A,0x82,0x9D,0x34,0x0B,0x2D,0x69,0x6B,0x84,0xC4,0x1F,0xBF,0x96,0x76,0xD3,0xEE,
			0xA6,0x39,0xEF,0xDB,0xD3,0xE7,0xC0,0x7B,0x96,0xFB,0xD5,0x1C,0x4E,0x09,0x45,0xBC,
			0x49,0x9A,0x0A,0x77,0x32,0x09,0x4B,0x94,0x17,0x84,0xE2,0x3D,0xBA,0x46,0x1E,0x50,
			0x7A,0x3D,0x39,0x4E,0xE0,0xB4,0x91,0x38,0x67,0x27,0x1C,0x69,0x67,0x76,0x28,0x10,
			0xAF,0x1E,0xBC,0xF1,0x52,0x63,0x66,0xBB,0xB4,0x65,0xE4,0xD5,0x05,0xD6,0x83,0x10,
			0xD6,0x61,0xA0,0x08,0x5A,0x21,0x5E,0x46,0x6D,0x51,0x92,0x4A,0x44,0xC0,0xB4,0x2F,
			0x6A,0x9C,0x5F,0x24,0x9E,0x12,0x29,0x3A,0x38,0x83,0x7D,0x3F,0x53,0x55,0xD4,0xF7,
			0x81,0x12,0x61,0xD0,0x8A,0x30,0xB0,0xBD,0xDB,0x43,0x4B,0x66,0x42,0xC2,0x94,0x7E,
			0x53,0x76,0xA5,0x8A,0xAC,0xD5,0x07,0xA1,0x7D,0x5F,0x21,0x6D,0x7D,0x62,0x57,0x01,
			0x87,0xCF,0x70,0x38,0x70,0xAD,0x4D,0xE2,0x6A,0xDF,0xF3,0x3C,0xD7,0xF9,0xDC,0xB9,
			0x3A,0x66,0xF4,0x5C,0x30,0x4E,0x5D,0xF7,0x48,0x84,0x44,0xAE,0xF5,0xE2,0x4A,0x3B,
			0x38,0x26,0xF4,0x52,0x43,0x1F,0x06,0x51,0xCF,0x33,0x5F,0xFA,0xEB,0x84,0xCA,0xE9,
			0xD9,0xDF,0xDA,0x71,0x76,0xE6,0xA8,0x5C,0xCD,0xBB,0x42,0x72,0x42,0xCF,0xA0,0xCD,
			0xF1,0x88,0xB9,0x20,0x8C,0x1A,0x61,0x43,0x35,0xC2,0xC4,0x60,0xEA,0x87,0xF8,0x8C,
			0x71,0xDF,0xE2,0x3F,0x3A,0xFD,0x62,0xDD,0x16,0xFC,0x37,0x0E,0xD8,0x71,0x66,0x11,
			0x76,0x5E,0x77,0xB5,0x49,0x54,0xFA,0xE6,0x34,0x37,0xA9,0xC0,0x7C,0x34,0x5A,0xB3,
			0xCC,0xBE,0xD4,0x2D,0x6A,0x27,0x0E,0x4C,0xD1,0x41,0x9C,0xBC,0x1F,0x16,0xB1,0x7E,
			0xAA,0x37,0x56,0x62,0x59,0xB0,0xD3,0x6D,0x9B,0x26,0x37,0xC4,0x39,0x6A,0x5E,0xEF,
			0x80,0x37,0xF0,0xF4,0x03,0x8F,0x81,0xDA,0x0C,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJobPrograms_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJobPrograms_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xFF,0x02,0x00,0x80,0x78,0x01,0x5D,0x92,0xC1,0x8E,0x82,0x30,0x10,0x86,0xF7,0x5D,
			0xF4,0xD8,0x03,0x22,0xCB,0x6A,0x36,0x1C,0x44,0x89,0xD6,0xA0,0x10,0x44,0xDC,0x5B,
			0x03,0xD8,0x95,0x66,0x43,0x6B,0x4A,0x8D,0x9A,0xF8,0xF0,0x5B,0x68,0xDD,0xA5,0x5E,
			0xE8,0xFC,0x1F,0xED,0x4C,0xFB,0xCF,0x2C,0x13,0xB8,0x40,0x3E,0xA1,0x39,0xBF,0xA7,
			0xF7,0x33,0x1E,0x14,0x02,0xD5,0x79,0x59,0x11,0x8A,0x93,0xFC,0xEA,0x59,0x40,0xEA,
			0xF5,0x2C,0x9B,0x21,0xFF,0x2E,0x70,0xC9,0x8E,0xD8,0x6B,0xC9,0x7C,0x57,0xE5,0xFC,
			0xDC,0x63,0x9F,0xCB,0x36,0xCD,0x3C,0xDE,0x77,0x39,0xCA,0xF3,0x05,0xDD,0x26,0x2E,
			0xBA,0xB9,0x8E,0xCC,0xD0,0xAA,0x9C,0xD7,0x5E,0x17,0xD4,0xE4,0xDC,0x78,0x40,0x6D,
			0x0F,0x6E,0xB8,0xBC,0x08,0xEC,0x13,0xD1,0x0C,0x70,0x81,0xC6,0x76,0x21,0x23,0x6F,
			0x6C,0x03,0x29,0x5C,0xA7,0x13,0xAE,0xA3,0xF7,0xAE,0x59,0x01,0xE9,0x37,0x1B,0x76,
			0x75,0x62,0xCE,0x4E,0x3C,0xAF,0xE1,0x02,0xBC,0xC8,0x46,0x70,0x42,0x4F,0x40,0x2F,
			0xDD,0x4F,0x79,0x30,0xE6,0x84,0x71,0x22,0xEE,0x20,0x4C,0x37,0xBB,0x20,0x04,0x70,
			0x9B,0xCA,0x22,0xEA,0xAB,0xB7,0x16,0x9D,0x01,0x40,0x2F,0xEA,0x7A,0xBD,0x93,0x03,
			0x95,0x2A,0xF2,0xE3,0x04,0x46,0x08,0x2E,0xC2,0x40,0xBE,0xCB,0x60,0x61,0x74,0xF0,
			0x4C,0xB2,0x8D,0x92,0xCD,0x2C,0x7C,0x81,0x2B,0xB8,0x5C,0xBD,0xA0,0x38,0x09,0x82,
			0x4D,0x9C,0xC2,0x2C,0x78,0xFA,0x12,0xED,0x3A,0x17,0x59,0x23,0xD0,0x9E,0xFE,0x50,
			0x76,0xA5,0xB2,0x5A,0xAB,0x0E,0x84,0x8E,0x6D,0x69,0x9F,0x8E,0x8F,0xEC,0xDA,0xA0,
			0xE9,0x3B,0x9A,0x4E,0x4C,0xB4,0x4D,0x4D,0x6D,0x5B,0x96,0x65,0x92,0xAF,0xD8,0xD4,
			0x21,0xA3,0xA7,0x8A,0x71,0x6A,0xD2,0x8C,0x34,0x22,0x37,0xD1,0x87,0x29,0x75,0xE1,
			0x90,0xD0,0xCB,0x0D,0xD9,0xC8,0xF1,0x46,0x96,0xBA,0xE9,0x93,0xB8,0x92,0x8C,0x74,
			0x0B,0xFF,0xBA,0x36,0xEC,0xF7,0x27,0xC3,0xBC,0x21,0x8C,0x2A,0xEF,0xF4,0x00,0x29,
			0xA1,0x6C,0x50,0x71,0x6F,0x54,0x14,0xF8,0x9F,0x58,0x9D,0x5D,0xE7,0x19,0xF6,0x9B,
			0xFB,0x59,0x63,0x51,0xB1,0xE3,0x23,0xDA,0xA7,0x0F,0xC9,0xE5,0x30,0xC1,0xED,0xE3,
			0x39,0x15,0xED,0x38,0x81,0xB7,0x5F,0xE9,0xBF,0x05,0x29,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CommitJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CommitJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x2E,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x20,
			0x23,0xB5,0xA8,0x28,0xBF,0x08,0xC4,0x48,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0x61,0x00,
			0x00,0x9D,0x70,0x10,0xCE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ReplyCommitJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ReplyCommitJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x20,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0xF1,0xF4,
			0x0B,0x31,0x33,0xB1,0xD3,0x61,0x00,0x00,0xD5,0x5D,0x0B,0x8E,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CancelJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CancelJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x9E,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0xF7,0xCA,0x4F,0x0A,0x28,
			0xCA,0xCC,0x2F,0xCA,0x2C,0xA9,0x54,0x76,0x07,0x0B,0xF8,0x3B,0x05,0x04,0x79,0xFA,
			0xC7,0x7B,0xBA,0xF8,0xB8,0xDA,0x1A,0xE8,0xA0,0x88,0xF9,0xF8,0x87,0xDB,0xA2,0x8A,
			0xF8,0xF9,0x07,0xF9,0x3A,0xFA,0xA0,0x09,0x7A,0x78,0xBA,0x7B,0xA0,0x09,0x05,0x04,
			0xB9,0xBA,0xFA,0x06,0x84,0x78,0x86,0xB9,0xDA,0xEA,0x58,0xE7,0xA6,0x96,0x64,0xE4,
			0xA7,0xD4,0x78,0xFA,0x01,0x51,0x88,0x99,0x89,0x0E,0x90,0x01,0xB1,0x06,0xE1,0x16,
			0x1D,0x06,0x00,0x30,0xC6,0x31,0x47,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetJobPriority_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetJobPriority_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xCE,0x03,0x00,0x80,0x78,0x01,0x5D,0x52,0x5D,0x93,0x9A,0x30,0x14,0xED,0x7F,0xD1,
			0xC7,0x74,0x46,0x91,0x5A,0x77,0xB7,0xD9,0x19,0x5D,0xA9,0xCB,0x16,0x85,0x41,0xD4,
			0xBE,0x65,0x02,0xA4,0x9A,0x56,0x12,0x27,0x89,0xA3,0xCC,0xF8,0xE3,0x1B,0x48,0x70,
			0xC1,0x97,0xE4,0x9E,0x93,0xFB,0x95,0x7B,0xCF,0x22,0xF6,0xE7,0x68,0x46,0x19,0x16,
			0x65,0x52,0x9E,0x48,0x2F,0x55,0xA8,0xC0,0xD9,0x81,0x32,0x12,0xE3,0x0B,0x1C,0x00,
			0x8D,0x3F,0xA6,0xDB,0x29,0x9A,0x95,0x8A,0x64,0x3C,0x27,0xB0,0x62,0xDE,0xD6,0x07,
			0x2C,0x4E,0x2D,0xEE,0x65,0x51,0xA5,0x79,0x8B,0x36,0x75,0x8E,0xEC,0x74,0x46,0xD7,
			0xC9,0x18,0x5D,0xC7,0xAE,0xCE,0x50,0x21,0x2C,0x0A,0x58,0x1B,0x05,0x3D,0x49,0x08,
			0x8C,0xBB,0x77,0x25,0xD9,0x59,0x91,0x19,0x55,0xB2,0x47,0x52,0x34,0x72,0x52,0x6D,
			0xC1,0x91,0x03,0x34,0x18,0xBB,0x35,0x18,0xBB,0xD6,0xF7,0x83,0xA7,0x3E,0xFB,0xC3,
			0xFB,0x75,0x9D,0x48,0xF0,0xBD,0xC0,0x85,0x3F,0x07,0x0F,0x50,0x2A,0x41,0xD9,0x1E,
			0xD8,0xAB,0x7E,0xD4,0x81,0x91,0xA0,0x5C,0x50,0x55,0x82,0x20,0x59,0xAE,0xBD,0x00,
			0xF8,0xAB,0x44,0x17,0x31,0xA7,0x75,0x4D,0xEB,0x01,0x00,0x7B,0x99,0xF6,0x5A,0x91,
			0x3D,0x93,0x2A,0x9C,0x45,0xB1,0x1F,0x22,0x7F,0x1E,0x78,0xFA,0x5F,0x1D,0x2E,0x08,
			0x77,0xB0,0xCB,0xAC,0xC2,0x78,0x39,0x0D,0x1E,0xC8,0x77,0x7F,0xF1,0xFE,0x40,0x45,
			0xB1,0xE7,0x2D,0xA3,0xC4,0xDF,0x7A,0xCD,0x5C,0xC2,0x75,0x3D,0x45,0x2E,0x15,0xDA,
			0xB0,0x7F,0x8C,0x5F,0x98,0xAE,0x56,0xA1,0x1D,0x65,0x23,0x47,0x8F,0xCF,0xDA,0x39,
			0xBF,0x48,0xF4,0xF4,0x0D,0x3D,0x4D,0xBA,0xD4,0x2A,0xE9,0x62,0x67,0x30,0x18,0x74,
			0x99,0xDF,0x51,0x17,0x07,0x9C,0xED,0x0F,0x5C,0xB0,0x2E,0xBB,0xA5,0x52,0xE1,0x2E,
			0xF5,0xBD,0x0B,0x6D,0xE1,0x80,0xB2,0xF3,0x15,0x39,0xC8,0x85,0xC3,0x81,0xE9,0xB4,
			0x61,0xC6,0x9A,0x19,0xDA,0x15,0xDE,0xB7,0xD6,0x6F,0xEF,0x67,0x4B,0x84,0xA4,0x9C,
			0x99,0xD9,0x59,0x01,0x19,0x60,0xC6,0x60,0xEC,0x96,0x54,0x0C,0xF1,0xA9,0x58,0x9B,
			0xDD,0xE6,0xE9,0xB7,0x97,0x6B,0xF6,0xB8,0x91,0x44,0x3C,0x3F,0xEB,0x6D,0xC6,0xE4,
			0xC4,0x85,0xAA,0x3C,0xB4,0xAC,0x1A,0x71,0x54,0xAA,0x02,0xFA,0x71,0xAD,0xB0,0x22,
			0x56,0x14,0x56,0x27,0xF6,0x22,0x42,0x70,0xD1,0x68,0xEA,0x21,0x65,0x1D,0xD5,0xFB,
			0x2B,0xD1,0xF4,0x78,0x84,0x5F,0x87,0x40,0x5B,0x3B,0x4C,0x95,0x16,0x21,0xAC,0x81,
			0xFE,0x73,0x46,0xA4,0xAC,0xB0,0x53,0x3D,0x5A,0x4C,0x72,0x14,0xFE,0x82,0x6E,0x97,
			0xF9,0x89,0xE9,0x91,0xE4,0x70,0x02,0x5E,0x0A,0xA2,0x0E,0x3C,0xBF,0x85,0x9B,0xE4,
			0x86,0x85,0xC0,0xE5,0x8F,0x7B,0xF3,0xAF,0xBA,0xC3,0x5B,0xF3,0xC5,0xDB,0x5D,0xCF,
			0x8D,0xF5,0xE5,0x3F,0xC7,0xEE,0x4B,0xF4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJobs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJobs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xC1,0x03,0x00,0x80,0x78,0x01,0x5D,0x52,0xD1,0x6E,0xDA,0x30,0x14,0xDD,0xBF,0xC0,
			0xA3,0x27,0x85,0x90,0x65,0xB4,0x9D,0x2B,0x41,0xC9,0x68,0xBA,0x40,0xA2,0x10,0x60,
			0x6F,0x96,0x93,0x78,0xE0,0x8D,0xD8,0xC8,0x36,0x82,0x48,0xFD,0xF8,0x3A,0xB1,0xE9,
			0x12,0x5E,0xEC,0x7B,0x8E,0xAF,0xCF,0xB5,0xEF,0x3D,0x8B,0x34,0x9C,0xA3,0x19,0x65,
			0x58,0xD4,0x59,0x7D,0x22,0x83,0x5C,0xA1,0x0A,0x17,0x07,0xCA,0x48,0x8A,0x2F,0xD0,
			0x01,0x1A,0xBF,0x4D,0xB7,0x53,0x34,0xAB,0x15,0x29,0x78,0x49,0x60,0xC3,0xBC,0xAC,
			0x0F,0x58,0x9C,0x3A,0xDC,0xD3,0xA2,0x91,0x79,0x49,0x36,0xAD,0x46,0x71,0x3A,0xA3,
			0xEB,0xC4,0x47,0x57,0xDF,0xD3,0x0A,0x0D,0xC2,0xA2,0x82,0x6D,0x50,0xD1,0x93,0x84,
			0xC0,0xA4,0x07,0x57,0x52,0x9C,0x15,0x99,0x51,0x25,0x07,0x24,0x47,0x63,0x37,0xD7,
			0x11,0x1C,0xBB,0x40,0x03,0xDF,0x6B,0x81,0xEF,0xD9,0xDC,0x37,0x9E,0x87,0xEC,0x0F,
			0x1F,0xB6,0x75,0x12,0xC1,0xF7,0x02,0x57,0xE1,0x1C,0xDC,0x41,0xA9,0x04,0x65,0x7B,
			0x60,0xB7,0xF6,0x50,0x5F,0x4C,0x04,0xE5,0x82,0xAA,0x1A,0x44,0xD9,0x72,0x1D,0x44,
			0x20,0x5C,0x65,0xBA,0x88,0x59,0x6D,0x6A,0xDE,0x36,0x00,0xD8,0xCD,0x3C,0xAF,0x73,
			0x73,0x60,0xA4,0xE2,0x59,0x92,0x86,0x31,0x0A,0xE7,0x51,0xA0,0xFF,0xD5,0xE3,0xA2,
			0x78,0x07,0xFB,0xCC,0x2A,0x4E,0x97,0xD3,0xE8,0x8E,0x7C,0x0D,0x17,0xAF,0x77,0x54,
			0x92,0x06,0xC1,0x32,0xC9,0xC2,0x6D,0x70,0xEB,0x4B,0xBC,0x6E,0xBB,0xC8,0xA5,0x42,
			0x1B,0xF6,0x8F,0xF1,0x0B,0xD3,0xD5,0x1A,0xB4,0xA3,0x6C,0xEC,0xEA,0xF6,0xD9,0xB8,
			0xE4,0x17,0x89,0x1E,0xBE,0xA1,0x87,0x49,0x9F,0x5A,0x65,0x7D,0xEC,0x3A,0x8E,0xD3,
			0x67,0x7E,0x27,0x7D,0x1C,0x71,0xB6,0x3F,0x70,0xC1,0xFA,0xEC,0x96,0x4A,0x85,0xFB,
			0xD4,0xF7,0x3E,0xB4,0x85,0x23,0xCA,0xCE,0x57,0xE4,0x22,0x0F,0x8E,0x1C,0xF3,0xD2,
			0x1B,0xE3,0x6B,0x66,0x64,0x47,0xF8,0x39,0xB5,0x61,0x77,0x3E,0x5B,0x22,0x24,0xE5,
			0xCC,0xF4,0xCE,0x1A,0xC8,0x00,0xD3,0x06,0x13,0x77,0xAC,0x62,0x88,0xFF,0x8E,0xB5,
			0xEA,0x56,0x67,0xD8,0x1D,0xAE,0x99,0xE3,0x46,0x12,0xF1,0xF8,0xA8,0xA7,0x99,0x92,
			0x13,0x17,0xAA,0xC9,0xD0,0xB6,0xBA,0x99,0xA3,0x71,0x15,0xD0,0x87,0x6B,0x85,0x15,
			0xB1,0xA6,0xB0,0x3E,0xB1,0x1B,0x11,0x82,0x8B,0x9B,0xA7,0xEE,0x24,0xDB,0x5B,0x83,
			0xBF,0x12,0x4D,0x8F,0x47,0xF8,0x75,0x04,0x74,0xB4,0xC3,0x54,0x69,0x13,0xC2,0x16,
			0xE8,0x3F,0x17,0x44,0xCA,0x06,0xBB,0xCD,0xA1,0xC5,0xA4,0x44,0xF1,0x2F,0xE8,0xF5,
			0x99,0x9F,0x98,0x1E,0x49,0x09,0x27,0xE0,0xA9,0x22,0xEA,0xC0,0xCB,0xF7,0x70,0xF5,
			0x8E,0x85,0xC0,0xF5,0x8F,0xF6,0xC5,0xCF,0x20,0xDE,0x64,0x96,0xF8,0xFC,0xCC,0x33,
			0xF8,0xF2,0x01,0xF6,0x45,0x48,0x72,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_QueryJobs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_QueryJobs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x48,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x28,0xCA,0x4F,0x2F,
			0x4A,0xCC,0x75,0xCB,0xCC,0x49,0x55,0x71,0xF2,0xF7,0xF7,0xD1,0x29,0x2E,0x29,0xCA,
			0xCC,0x4B,0xD7,0x49,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0xB1,0xCE,0x4D,0x2D,0xC9,0xC8,
			0x4F,0xA9,0xF1,0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x71,0x47,0xD3,0x64,0xA7,
			0xC3,0x00,0x00,0xB2,0x09,0x19,0xBB,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UpdateGrid_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UpdateGrid_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3D,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x51,0x6F,0xC2,0x20,0x18,0x45,0xF7,0x5F,
			0xF4,0x91,0x87,0xDA,0x76,0x9D,0xC6,0xB1,0xC4,0xEA,0x62,0x5C,0x8C,0x1A,0x6D,0xDD,
			0xDE,0x08,0xAD,0xCC,0x92,0xA5,0xD0,0x00,0xC6,0x36,0xF1,0xC7,0x8F,0x02,0xDB,0x64,
			0x2F,0xCD,0x77,0x6E,0xC8,0xF9,0xE8,0x65,0xB9,0x5F,0x2D,0x50,0x4A,0x19,0x16,0x5D,
			0xD6,0x35,0x64,0x50,0x28,0x54,0xE3,0xB2,0xA2,0x8C,0xEC,0xF1,0x15,0x06,0x40,0xF3,
			0xDB,0xEC,0x38,0x43,0x69,0xA7,0x48,0xC9,0x4F,0x04,0xF6,0xC9,0xFC,0x50,0x61,0xD1,
			0xDC,0x65,0xD3,0x65,0xAF,0x99,0xEF,0x72,0xE3,0x28,0x9B,0x0B,0x6A,0xC7,0x09,0x6A,
			0x93,0x58,0x1B,0x7A,0xC2,0xA2,0x86,0x66,0xA8,0x69,0x23,0x21,0xB0,0xC7,0x5F,0x5B,
			0x52,0x5E,0x14,0x49,0xA9,0x92,0x03,0x52,0xA0,0x28,0x2C,0xF4,0x04,0xA3,0x10,0x68,
			0x48,0x62,0x03,0x49,0xEC,0xCE,0x6E,0x0F,0xC6,0xCC,0xA5,0x42,0x39,0xFB,0x62,0xFC,
			0xCA,0xB4,0xB9,0xA7,0x77,0xCA,0xA2,0x50,0x2B,0xDD,0x7C,0xE2,0x57,0x89,0x26,0x8F,
			0x68,0x32,0xF6,0xA3,0x4D,0xE6,0x73,0x18,0x04,0x81,0x9F,0x7C,0xEC,0x7C,0x5E,0x73,
			0x76,0xAE,0xB8,0x60,0x7E,0x7A,0xA4,0x52,0x61,0x3F,0x7A,0xF2,0xD1,0x2D,0x5E,0x53,
			0x76,0x69,0x51,0x88,0x62,0x38,0x0A,0xEC,0x4D,0x7F,0x92,0x44,0x27,0x23,0xF7,0x5B,
			0x3B,0xC1,0xCF,0x02,0xD7,0xAB,0xC5,0x50,0x2A,0x41,0xD9,0x19,0x98,0x1E,0x8F,0x44,
			0x48,0xCA,0x99,0x05,0x57,0xAA,0x05,0x5B,0x83,0x9D,0xEF,0xEA,0xB3,0xC1,0xDF,0x2B,
			0xFE,0xB3,0xB3,0x4F,0x3E,0x34,0xE2,0xDF,0x75,0xC0,0xAD,0xB3,0x0F,0xE1,0xF6,0x0D,
			0x57,0x9B,0x4C,0xB7,0x6F,0xBF,0xD3,0x9A,0xA8,0x8A,0x9F,0x6E,0xDB,0x3C,0xBB,0x61,
			0x21,0x70,0xF7,0xEC,0x19,0xB4,0xF2,0x05,0x3C,0x7C,0x03,0x66,0xB8,0xC9,0x04,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListModules_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListModules_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x26,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,0x41,0xB0,0xFC,0x43,0x43,0x80,0xE2,0x21,0xC6,
			0x46,0x3A,0x0C,0x00,0x03,0x2A,0x0C,0x0E,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetLogCount_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetLogCount_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x63,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0xF2,0xC9,0x4F,0xF7,0xCC,0x4B,0xCB,0x57,0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0xB1,0xCE,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,
			0x0F,0x0D,0xA9,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x81,0xAA,0xB3,0xD3,0xF1,0xF4,0xAB,
			0x81,0xAA,0x45,0x61,0x79,0xFA,0x85,0x18,0x1B,0x81,0x24,0x21,0x0C,0x06,0x00,0xA8,
			0xFC,0x20,0x88,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_LoadLogInfos_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_LoadLogInfos_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x41,0xB0,0x18,0x00,0xA3,0xA1,0x0A,0xAA,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_Login_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_Login_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x10,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,0x39,0x8D,0x06,0x61,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_Logout_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_Logout_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x7B,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x30,0x5B,0x74,0x18,0x00,0x50,0x39,0x29,
			0x89,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x19,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0x61,0x00,
			0x00,0x83,0xA0,0x09,0x6D,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x41,0xB0,0x18,0x00,0xA3,0xA1,0x0A,0xAA,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UpdatePassword_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UpdatePassword_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x7B,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x30,0x5B,0x74,0x18,0x00,0x50,0x39,0x29,
			0x89,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ManageUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ManageUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x83,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0xFE,0xA1,0x21,0x35,0x89,0x45,0x45,0x89,0x95,0x36,0x30,
			0xCB,0xEC,0x74,0x18,0x00,0xAF,0x09,0x2C,0x83,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListAllUsers_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListAllUsers_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x86,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x50,0x53,0xFD,0x43,0x43,0x6A,0x60,0x16,
			0xEA,0x30,0x00,0x00,0x39,0x0D,0x2D,0xC0,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetUserInfo_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetUserInfo_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0x61,
			0x00,0x00,0x8D,0xB9,0x09,0xCE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJTMs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJTMs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xA9,0x02,0x00,0x80,0x78,0x01,0x85,0x51,0x4D,0x8F,0x82,0x30,0x14,0xDC,0xFF,0xA2,
			0xC7,0x1E,0x10,0x59,0xD6,0x8F,0x70,0x50,0x77,0x63,0x30,0x8A,0x46,0xC1,0xDD,0x5B,
			0x53,0xB0,0x2B,0xCD,0x86,0x96,0xD0,0x12,0x21,0xF1,0xC7,0x6F,0xA1,0xD5,0x6D,0x4F,
			0x7B,0x69,0xDE,0x4C,0x27,0xF3,0x26,0xF3,0xD6,0xC7,0xF0,0x1D,0x2E,0x09,0x45,0x55,
			0x1B,0xB7,0x25,0x1E,0xA4,0x02,0x16,0x28,0xCB,0x09,0xC5,0x47,0x74,0x0B,0x1C,0x20,
			0xF1,0x66,0x71,0x5E,0xC0,0x65,0x2B,0x70,0xC6,0x2E,0x38,0xE8,0x98,0xD5,0x29,0x47,
			0x55,0x69,0x70,0xF3,0x75,0x67,0xB3,0x3A,0x24,0xBD,0x47,0x56,0xD6,0xB0,0x99,0xF8,
			0xB0,0xF1,0x3D,0xE9,0xD0,0x21,0x54,0x15,0x41,0x3F,0x14,0xA4,0xE4,0x01,0x50,0xF2,
			0x8F,0x06,0x67,0xB5,0xC0,0x4B,0x22,0xF8,0x00,0xA7,0x70,0xEC,0xA6,0x72,0x0A,0xC6,
			0x2E,0x90,0xC0,0xF7,0x7A,0xE0,0x7B,0x5A,0xBB,0x53,0x99,0x4E,0x02,0x09,0x92,0xAD,
			0x18,0x15,0xB8,0x11,0x43,0x73,0x29,0x08,0xA3,0x58,0xAA,0xE5,0x2B,0x0D,0x9E,0xAF,
			0x62,0xE4,0xDB,0x4B,0xF7,0xA7,0x2E,0x1E,0xE0,0xA2,0x22,0xF4,0xAA,0x28,0x23,0x83,
			0x22,0xFE,0xAA,0x78,0xE8,0x54,0x58,0x33,0x00,0x97,0x19,0x86,0xCF,0x1D,0xE6,0xBE,
			0x7F,0x67,0x65,0xA6,0x82,0x0C,0x18,0x17,0x30,0xA1,0x3F,0x94,0xDD,0xA8,0xEC,0xA9,
			0x43,0x9F,0x84,0x8E,0x5D,0x59,0x90,0x9E,0x2F,0xEC,0xC6,0xE1,0xF4,0x15,0x4E,0x27,
			0x36,0x15,0xC5,0x36,0x76,0x1D,0xC7,0xB1,0x99,0xAF,0x83,0x8D,0xB7,0x8C,0x5E,0x73,
			0x56,0x51,0x9B,0x3D,0x13,0x2E,0x90,0x4D,0xBD,0xD9,0x50,0x2F,0xDE,0x12,0x5A,0x37,
			0xD0,0x85,0x5E,0x30,0x72,0x54,0xD2,0x07,0xE3,0x4B,0x66,0xA4,0x8F,0x94,0x70,0x5C,
			0xCD,0x66,0x9B,0x78,0x17,0xD2,0x6F,0xA6,0xAE,0x63,0xD6,0xF6,0xB8,0x9B,0x2A,0xDA,
			0xFC,0xE9,0x0A,0x05,0xF3,0x02,0x8B,0x9C,0x5D,0xEE,0x61,0x74,0xD7,0x17,0xDA,0x27,
			0xF1,0x5D,0xBB,0x81,0x97,0x5F,0x05,0x14,0xE9,0x1C,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetJTMInfo_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetJTMInfo_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x72,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x4B,0x2D,0x2A,0xCE,
			0xCC,0xCF,0x53,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x81,0x90,0xD6,0xEE,0x20,0x99,0xD0,
			0xE2,0xD4,0x22,0x2B,0x2B,0xF7,0xA2,0xCC,0x94,0x80,0xA2,0xFC,0x82,0xD4,0xA2,0x92,
			0x4A,0x15,0xB0,0x38,0x54,0x07,0x54,0x2D,0xB2,0xBE,0xE2,0x92,0xA2,0xCC,0xBC,0x74,
			0x1D,0xEB,0xDC,0xD4,0x92,0x8C,0xFC,0x94,0x1A,0xFF,0xD0,0x90,0x1A,0x64,0xED,0x3A,
			0x0C,0x00,0x52,0x57,0x25,0xFB,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetGridProperty_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetGridProperty_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x75,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0xC2,0x20,0x14,0x86,0xF7,0x5F,
			0xEC,0x25,0x17,0xB5,0xED,0x3A,0x8D,0xEB,0x12,0xAB,0x9B,0xE9,0x62,0xD4,0xF8,0xB5,
			0xDD,0x11,0x5A,0x99,0x25,0x5B,0xA1,0x01,0x8C,0x6D,0xE2,0x8F,0x1F,0x05,0xCC,0xCA,
			0x6E,0xC8,0x79,0x1F,0x0E,0xEF,0x39,0x9C,0xB3,0xD8,0x66,0x73,0x98,0x12,0x8A,0x78,
			0xBB,0x6F,0x6B,0x3C,0xC8,0x25,0xAC,0x50,0x51,0x12,0x8A,0xB7,0xE8,0x9A,0xF8,0x40,
			0xE9,0xF7,0xE9,0x71,0x0A,0xD3,0x56,0xE2,0x82,0x9D,0x70,0xD2,0x91,0xD9,0xAE,0x44,
			0xBC,0xEE,0xB1,0xC9,0xA2,0xB3,0x99,0x6D,0x0E,0xDA,0xA3,0xA8,0x2F,0xB0,0x19,0xC5,
			0xB0,0x89,0x23,0xE5,0xD0,0x29,0xC4,0xAB,0x44,0x07,0x15,0xA9,0x45,0x02,0x4C,0xFA,
			0x6B,0x83,0x8B,0x8B,0xC4,0x29,0x91,0x62,0x80,0x73,0x18,0x06,0xB9,0x8A,0x92,0x30,
			0x00,0x4A,0xC4,0x91,0x16,0x71,0x64,0x73,0xD7,0x3B,0xED,0xCC,0x84,0x84,0x07,0xFA,
			0x4D,0xD9,0x95,0x2A,0xE7,0x4E,0x7D,0x10,0x1A,0x06,0xCA,0xD2,0xC6,0x27,0x76,0x15,
			0x70,0xFC,0x08,0xC7,0x23,0x17,0xAD,0xF6,0xAE,0x0E,0x7C,0xDF,0x77,0xC9,0xE7,0xC6,
			0xD5,0x4B,0x46,0xCF,0x25,0xE3,0xD4,0xA5,0x47,0x22,0x24,0x72,0xD1,0x93,0x2B,0x6D,
			0xE1,0x25,0xA1,0x97,0x06,0x06,0x30,0x4A,0x86,0xBE,0xE9,0xF4,0x4E,0x62,0x45,0x86,
			0xF6,0x5B,0x1B,0xCE,0xCE,0x1C,0x55,0x6F,0xE4,0x07,0x7B,0xE9,0x7A,0xBD,0x04,0x42,
			0x72,0x42,0xCF,0x20,0xD7,0x0B,0x71,0x93,0xB2,0xB9,0x67,0x6F,0xF5,0xB0,0x8F,0x98,
			0x0B,0xC2,0x28,0xE8,0x4F,0xDE,0x08,0x33,0x2B,0x13,0xF7,0x66,0x6C,0xC0,0xDF,0xAA,
			0xFF,0xB9,0xD3,0x2F,0xE6,0x69,0x2F,0xDB,0x53,0x36,0xBF,0x37,0x63,0xB6,0x65,0xEB,
			0x79,0xD9,0x6A,0xAF,0x56,0x64,0xCE,0x49,0x85,0x65,0xC9,0x4E,0xB7,0x6C,0x75,0x73,
			0x9E,0x2A,0x2F,0x95,0x71,0x43,0x9C,0xA3,0xF6,0xB9,0x7F,0xD3,0x7D,0xF4,0x05,0x3C,
			0xFC,0x02,0xC9,0xC0,0xDC,0xC7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddSplitter_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddSplitter_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x35,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6B,0x83,0x30,0x14,0x86,0xF7,0x5F,
			0xEA,0xA5,0x17,0x56,0x9D,0x6B,0x29,0x5E,0xD4,0x76,0x14,0x47,0x71,0xD2,0x0F,0xB7,
			0xBB,0x10,0x35,0xD3,0x30,0x4C,0x24,0x89,0xA8,0xD0,0x1F,0xBF,0x68,0xD2,0xAD,0xD9,
			0x8D,0x9C,0xE7,0x25,0x3E,0x47,0xDF,0x1C,0x4E,0xF1,0x1E,0x44,0x98,0x40,0x36,0x5E,
			0xC6,0x16,0x2D,0x72,0x01,0x1A,0x58,0xD4,0x98,0xA0,0x13,0xEC,0x43,0xC7,0x96,0xFC,
			0xB6,0xCD,0xB6,0x20,0x1A,0x05,0x2A,0x68,0x89,0xC2,0x29,0xD9,0x9D,0x6B,0xC8,0xDA,
			0x87,0x6C,0x73,0x98,0x34,0xBB,0xF4,0x3A,0x3B,0x8A,0xB6,0x03,0xC3,0x2A,0x00,0x43,
			0xE0,0x4B,0xC3,0x44,0x90,0x35,0xE1,0x3C,0x34,0xB8,0xE5,0xA1,0xAD,0x8E,0xBF,0x0E,
			0xA8,0xE8,0x04,0x8A,0xB0,0xE0,0x0B,0x94,0x03,0xCF,0xCD,0xE5,0x14,0x7A,0xAE,0x2D,
			0x21,0xF0,0x67,0x08,0x7C,0x7D,0xF6,0xFD,0x3C,0x9B,0x29,0x17,0xE0,0x4A,0xBE,0x09,
			0xED,0x89,0x34,0x4F,0xF4,0x81,0x89,0xE7,0x4A,0xA5,0x9E,0x4B,0xDA,0x73,0xB0,0x7E,
			0x06,0xEB,0x95,0x19,0x25,0x17,0x93,0x5D,0xC7,0x71,0xCC,0xE4,0x33,0x35,0xF9,0x48,
			0x49,0x55,0x53,0x46,0xCC,0x34,0xC3,0x5C,0x40,0x33,0x7A,0x31,0x51,0x2F,0x3E,0x62,
			0xD2,0x0D,0xC0,0x05,0x7E,0xB8,0x74,0xD4,0x97,0xDE,0x93,0x40,0x26,0x4B,0xFD,0x5B,
			0x29,0xA3,0x15,0x83,0x4D,0xBC,0xB7,0xB8,0x60,0x98,0x54,0xF6,0xDC,0x63,0x86,0x18,
			0xC7,0x94,0x28,0xD0,0xA5,0x2A,0x50,0x35,0xA8,0xF9,0xA1,0x3E,0x15,0xFC,0xDD,0xE2,
			0x3F,0x3B,0xF9,0xA2,0xD6,0x2C,0xFE,0x5D,0x67,0xEB,0x75,0xEA,0x22,0xF4,0x3E,0x2B,
			0x4E,0x2E,0xB2,0x7D,0xF5,0xDC,0x34,0x48,0xD4,0xB4,0xBC,0xC5,0xC9,0xCD,0x78,0x55,
			0xBA,0xEC,0xA7,0x1F,0x20,0xE7,0xC6,0x0A,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelSplitter_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelSplitter_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x13,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0xD1,0x6E,0x83,0x20,0x18,0x46,0xF7,0x2E,
			0xED,0xA5,0x17,0x56,0x9D,0x6B,0xD3,0xB9,0xA4,0xB6,0x4B,0xE3,0xD2,0xB4,0xA6,0x55,
			0xB7,0x3B,0x82,0x4A,0x94,0x2C,0x82,0x01,0x8C,0x9A,0xF4,0xE1,0x87,0xC2,0xB6,0x72,
			0x43,0xFE,0x73,0x42,0x3E,0xE0,0xE3,0x78,0x8D,0x0E,0x20,0xC4,0x04,0xB2,0x31,0x19,
			0x5B,0xB4,0xC8,0x05,0x68,0x60,0x51,0x63,0x82,0xAE,0xB0,0x0F,0x6C,0x4B,0xF2,0xC7,
			0x2E,0xDB,0x81,0x70,0x14,0xA8,0xA0,0x25,0x0A,0x26,0xB3,0xBF,0xD5,0x90,0xB5,0x0F,
			0x6E,0x7B,0x9C,0x62,0xF6,0x71,0x3A,0x67,0x14,0x6D,0x07,0x86,0xB5,0x0F,0x06,0xDF,
			0x93,0x09,0x13,0x41,0xD6,0x04,0xF3,0xD0,0xE0,0x96,0x07,0x96,0xDA,0xFE,0x3E,0xA0,
			0xA2,0x13,0x28,0xC4,0x82,0x2F,0x50,0x0E,0x5C,0x27,0x97,0x53,0xE0,0x3A,0x96,0x04,
			0xDF,0x9B,0xC1,0xF7,0xF4,0xDE,0xCB,0x6D,0x4E,0xA6,0x5C,0x80,0x94,0x7C,0x13,0xDA,
			0x13,0x99,0x3C,0xD1,0x27,0x26,0xAE,0x23,0x23,0xF5,0x5C,0xD2,0x9E,0x83,0xCD,0x33,
			0xD8,0xAC,0x4D,0x75,0x4E,0x4C,0x76,0x6C,0xDB,0x36,0xCD,0x57,0x6C,0xF2,0x89,0x92,
			0xAA,0xA6,0x8C,0x98,0x36,0xC3,0x5C,0x40,0x53,0xBD,0x98,0xA8,0x0F,0x3E,0x61,0xD2,
			0x0D,0xC0,0x01,0x5E,0xB0,0xB2,0xD5,0x4D,0x7F,0x8D,0x2F,0xCD,0x4A,0x3F,0x2B,0x66,
			0xB4,0x62,0xB0,0x89,0x0E,0x4B,0x2E,0x18,0x26,0x95,0x35,0xF7,0x98,0x21,0xC6,0x31,
			0x25,0x0A,0x74,0xA9,0x0A,0x54,0x0D,0x6A,0x7E,0xA8,0x4F,0x89,0xFF,0x5F,0xD4,0xE9,
			0x3A,0x67,0x19,0x9D,0x13,0xD9,0xAA,0x5A,0xB7,0x0D,0x12,0x35,0x2D,0xEF,0x97,0x34,
			0xB9,0x43,0xC6,0xE0,0xF8,0x3A,0x1F,0xF9,0x77,0x91,0x37,0xEB,0xE9,0x07,0x6F,0xC8,
			0xB9,0xE4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListSplitters_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListSplitters_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x53,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0x5D,0x6F,0x83,0x20,0x18,0x85,0xF7,0x5F,
			0xDA,0x4B,0x2F,0xAC,0x3A,0xD7,0x8F,0xB9,0xA4,0xB6,0x4B,0xE3,0xD2,0xB4,0xA6,0xD5,
			0x6E,0x77,0x04,0x95,0x28,0xD9,0x04,0x03,0x98,0x6A,0xD2,0x1F,0x3F,0x14,0xBA,0x95,
			0x1B,0x78,0x9F,0x13,0x72,0x38,0x1C,0x76,0xA7,0x68,0x0B,0x42,0x4C,0x20,0xEB,0x93,
			0xBE,0x41,0x93,0x4C,0x80,0x1A,0xE6,0x15,0x26,0xE8,0x04,0xAF,0x81,0x6D,0x49,0xFE,
			0x58,0x5F,0xD6,0x20,0xEC,0x05,0xCA,0x69,0x81,0x82,0x41,0xD9,0x9C,0x2B,0xC8,0x9A,
			0x07,0x6D,0xB5,0x1B,0x6C,0x36,0x71,0x3A,0x7A,0xE4,0x4D,0x0B,0xBA,0xB9,0x0F,0x3A,
			0xDF,0x93,0x0E,0x03,0x41,0x56,0x07,0xE3,0x50,0xE3,0x86,0x07,0x96,0x3A,0xFE,0xDE,
			0xA1,0xBC,0x15,0x28,0xC4,0x82,0x4F,0x50,0x06,0x5C,0x27,0x93,0x53,0xE0,0x3A,0x96,
			0x04,0xDF,0x1B,0xC1,0xF7,0xF4,0xD9,0xE3,0x79,0x74,0xA6,0x5C,0x80,0x94,0x7C,0x13,
			0x7A,0x25,0xD2,0x79,0xA0,0x4F,0x4C,0x5C,0x47,0x5A,0xEA,0xB9,0xA0,0x57,0x0E,0x16,
			0xCF,0x60,0x31,0x37,0xA5,0x43,0x62,0xB2,0x63,0xDB,0xB6,0xA9,0x7C,0xC5,0x26,0xEF,
			0x29,0x29,0x2B,0xCA,0x88,0xA9,0x5E,0x30,0x17,0xD0,0x94,0x5E,0x4C,0xD4,0x17,0xEF,
			0x31,0x69,0x3B,0xE0,0x00,0x2F,0x98,0xD9,0x2A,0xE9,0x5D,0xF1,0xA5,0x32,0xD3,0xCF,
			0x8A,0x19,0x2D,0x19,0xAC,0xA3,0xED,0x94,0x0B,0x86,0x49,0x69,0x8D,0x3D,0x5E,0x10,
			0xE3,0x98,0x12,0x05,0xBA,0x54,0x05,0xAA,0x06,0x35,0x3F,0xD4,0xA7,0x84,0xFF,0x5F,
			0xD4,0xEE,0xDA,0x67,0x1A,0x1D,0x12,0xD9,0xAA,0x5A,0x55,0xF5,0x29,0x47,0x6C,0xB9,
			0x3C,0x37,0x3F,0x58,0x08,0xC4,0x62,0x28,0x33,0xDC,0x13,0xE8,0x20,0x7A,0x5B,0xD5,
			0x48,0x54,0xB4,0xB8,0x45,0x87,0xDB,0x18,0xED,0x2F,0xB0,0x75,0x4C,0x93,0x1B,0x64,
			0x0C,0xF6,0xAF,0x86,0xCD,0x9B,0xF5,0xF4,0x0B,0x0D,0x0C,0xD1,0xF3,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListSplitterParam_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListSplitterParam_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x77,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x72,0x2F,0xCA,0x4C,0x09,0x2E,0x49,0x2C,0xC9,0x2C,0x2E,0xC9,0x4C,0x2E,0x56,
			0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,0x81,0x52,0x9E,0x7E,0x21,0xC6,0x46,0x3A,0xB8,
			0x48,0x54,0xB5,0x50,0x9E,0x75,0x6E,0x6A,0x49,0x46,0x7E,0x4A,0x8D,0x7F,0x68,0x48,
			0x0D,0xAA,0xD9,0x3A,0x0C,0x00,0xAA,0x01,0x25,0x54,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetGridStatistics_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetGridStatistics_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x41,0xB0,0x18,0x00,0x91,0x83,0x08,0xDE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UDPLink__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UDPLink__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UDPLinkConfirm__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UDPLinkConfirm__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x23,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0xAB,0xF1,0x0C,0x08,0xD0,0x01,0x52,
			0x49,0x99,0x79,0x89,0x45,0x95,0x3A,0x0C,0x00,0xF8,0x4D,0x0C,0xF9,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetRedirect__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetRedirect__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetTimeout__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetTimeout__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetSessionBufferSize__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetSessionBufferSize__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0xC1,0x10,
			0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,0x9A,0x78,
			0x15,0x41,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_MethodCheck__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_MethodCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3C,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,
			0xC1,0x10,0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,
			0xC2,0xD8,0x15,0xED,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CallbackCheck__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CallbackCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x42,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x64,
			0x46,0x71,0x49,0x11,0x50,0x11,0x48,0x08,0xC1,0x4A,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,
			0x61,0x00,0x00,0x2B,0x3E,0x17,0xA5,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_Compute_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_Compute_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x2C,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x20,0x23,0xB5,
			0xA8,0x28,0xBF,0x08,0xC4,0x48,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0x61,0x00,0x00,0x7D,
			0x12,0x10,0x22,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_OnComputed_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_OnComputed_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xC6,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0xF7,0xCD,0x4F,0x29,0xCD,0x49,
			0x75,0xCB,0xCC,0x49,0x55,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x49,0xCA,0xCC,0x4B,
			0x2C,0xAA,0xD4,0xB1,0x0E,0x80,0xCB,0x7A,0xE6,0xA5,0xA4,0x56,0xA8,0x78,0xFA,0x85,
			0x98,0x99,0xE8,0x20,0x44,0x43,0x12,0xD3,0x91,0x15,0x01,0xB9,0x30,0x13,0x10,0x6A,
			0xC2,0x52,0x8B,0x8A,0x33,0xF3,0xF3,0x90,0xD5,0x41,0x85,0x40,0xC6,0x19,0x1B,0xE9,
			0x20,0x93,0xD6,0xC9,0x89,0x39,0x39,0x49,0x89,0xC9,0xD9,0x35,0x9E,0x7E,0x35,0x08,
			0x33,0xC0,0xB6,0x03,0x15,0xD6,0x24,0x16,0x15,0x25,0x56,0xDA,0x20,0x24,0x40,0x8E,
			0xB6,0xD3,0x61,0x00,0x00,0xDF,0xCF,0x44,0x8D,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_AddMoudle_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_AddMoudle_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x13,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x06,0x00,0x47,0x50,0x06,0x3E,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_RemoveModule_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_RemoveModule_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x97,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0xF7,0xCD,0x4F,0x29,0xCD,0x49,
			0xF5,0xCC,0x4B,0x49,0xAD,0x50,0xF1,0xF4,0x0B,0x31,0x33,0xD1,0x09,0x80,0x8B,0x86,
			0x24,0xA6,0xEB,0x58,0xA3,0x70,0x55,0x8A,0x4B,0x8A,0x32,0xF3,0xD2,0x91,0xD4,0x84,
			0xA5,0x16,0x15,0x67,0xE6,0xE7,0x21,0xAB,0x83,0x0A,0x81,0x8C,0x33,0x36,0xD2,0x41,
			0x26,0xAD,0x93,0x13,0x73,0x72,0x92,0x12,0x93,0xB3,0x6B,0xFC,0x43,0x43,0x6A,0x12,
			0x8B,0x8A,0x12,0x2B,0x6D,0x10,0xE6,0x83,0x1D,0x61,0xA7,0xC3,0x00,0x00,0x63,0xB7,
			0x33,0xA6,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_ListModules_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_ListModules_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x41,0xB0,0x18,0x00,0x91,0x83,0x08,0xDE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_UDPLink__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_UDPLink__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_UDPLinkConfirm__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_UDPLinkConfirm__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x23,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0xAB,0xF1,0x0C,0x08,0xD0,0x01,0x52,
			0x49,0x99,0x79,0x89,0x45,0x95,0x3A,0x0C,0x00,0xF8,0x4D,0x0C,0xF9,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_SetRedirect__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_SetRedirect__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_SetTimeout__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_SetTimeout__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_SetSessionBufferSize__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_SetSessionBufferSize__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0xC1,0x10,
			0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,0x9A,0x78,
			0x15,0x41,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_MethodCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_MethodCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3C,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,
			0xC1,0x10,0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,
			0xC2,0xD8,0x15,0xED,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Scatter_CallbackCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Scatter_CallbackCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x12,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x61,0x00,0x00,0x46,0x0C,0x06,0xD4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_Login_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_Login_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_Logout_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_Logout_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xBB,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0xF7,0xCD,0x4F,0x29,0xCD,0x49,
			0xF5,0xCC,0x4B,0xCB,0x57,0xF1,0xF4,0x0B,0x31,0x33,0xD1,0x09,0x80,0x0B,0x86,0x24,
			0xA6,0xEB,0x00,0xC5,0x8C,0x8D,0xA0,0x64,0x71,0x49,0x51,0x66,0x1E,0xAA,0x90,0x35,
			0x8A,0x6A,0x15,0xA8,0x0A,0x84,0x60,0x58,0x6A,0x51,0x71,0x66,0x7E,0x9E,0x0E,0x92,
			0x3A,0xA8,0x10,0xC8,0x36,0xB8,0xC9,0x10,0xB6,0x75,0x6E,0x6A,0x49,0x46,0x7E,0x4A,
			0x8D,0xA7,0x1F,0x10,0x81,0x24,0xFD,0x43,0x43,0x6A,0x12,0x8B,0x8A,0x12,0x2B,0x6D,
			0x10,0x26,0x82,0x5C,0x6A,0xA7,0xC3,0x00,0x00,0x64,0x87,0x3D,0x64,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_ListModules_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_ListModules_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x52,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0xF7,0xCD,0x4F,0x29,0xCD,0x49,
			0x75,0xCB,0xCC,0x49,0x55,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x49,0xCA,0xCC,0x4B,
			0x2C,0xAA,0xD4,0xB1,0xCE,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,
			0x33,0x13,0x1D,0x08,0xC3,0xD8,0x48,0xC7,0x3F,0x34,0xA4,0x26,0xB1,0xA8,0x28,0xB1,
			0xD2,0x26,0x00,0xC5,0x00,0x3B,0x1D,0x06,0x00,0xBB,0xF4,0x1C,0x63,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_GetModuleFile_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_GetModuleFile_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x39,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,
			0xA2,0x10,0x33,0x13,0x1D,0x20,0xA3,0xB8,0xA4,0x28,0x33,0x2F,0x1D,0x95,0x95,0x94,
			0x99,0x97,0x58,0x54,0xA9,0xE3,0x1F,0x1A,0x02,0x55,0xC7,0x00,0x00,0x5E,0xC4,0x13,
			0xF7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_Execute_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_Execute_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x2E,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x20,
			0x23,0xB5,0xA8,0x28,0xBF,0x08,0xC4,0x48,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0x61,0x00,
			0x00,0x9D,0x70,0x10,0xCE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_OnExecuted_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_OnExecuted_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x57,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0x0F,0x4E,0x2D,0x2A,0xCB,0x4C,
			0x4E,0xB5,0xB2,0x72,0xAD,0x48,0x4D,0x2E,0x2D,0x49,0x0D,0x2E,0x49,0x2C,0x49,0x55,
			0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x49,0x2D,0x2A,0xCA,0x2F,0xD2,0xB1,0xCE,0x4D,0x2D,
			0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x01,0x4A,0x9A,
			0x99,0xD8,0xE9,0xF8,0x87,0x86,0x40,0x05,0x90,0xB5,0xDA,0xE9,0x30,0x00,0x00,0x50,
			0x71,0x1E,0xB7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_QueryJobs_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_QueryJobs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x19,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x06,0x00,0x83,
			0xB1,0x08,0xF5,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_CancelJob_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_CancelJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x41,0xB0,0x18,0x00,0x91,0x83,0x08,0xDE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_UDPLink__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_UDPLink__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_UDPLinkConfirm__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_UDPLinkConfirm__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x23,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0xAB,0xF1,0x0C,0x08,0xD0,0x01,0x52,
			0x49,0x99,0x79,0x89,0x45,0x95,0x3A,0x0C,0x00,0xF8,0x4D,0x0C,0xF9,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_SetRedirect__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_SetRedirect__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_SetTimeout__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_SetTimeout__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_SetSessionBufferSize__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_SetSessionBufferSize__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0xC1,0x10,
			0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,0x9A,0x78,
			0x15,0x41,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_MethodCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_MethodCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3C,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,
			0xC1,0x10,0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,
			0xC2,0xD8,0x15,0xED,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Service_CallbackCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Service_CallbackCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x12,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x61,0x00,0x00,0x46,0x0C,0x06,0xD4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_Login_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_Login_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_Logout_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_Logout_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x9F,0x02,0x00,0x80,0x78,0x01,0x55,0x51,0xC1,0x6E,0x83,0x30,0x0C,0xDD,0xBF,0xAC,
			0xC7,0x1C,0x36,0x8A,0xD0,0xA4,0x8D,0x49,0x69,0x60,0x6D,0x26,0x20,0x28,0x84,0xB6,
			0x3B,0x45,0xAC,0x8D,0x3A,0xA4,0x0E,0x2A,0x4A,0xA5,0x21,0xF5,0xE3,0x17,0x4C,0x28,
			0xE9,0x25,0xB2,0x9F,0x1D,0xFB,0xBD,0xE7,0x94,0x10,0xB9,0x28,0xAB,0xA2,0xE9,0x44,
			0x77,0x52,0x8F,0xA9,0x4E,0x63,0x4C,0x56,0x34,0x09,0x25,0xC7,0x1B,0xFF,0x09,0xF5,
			0xC8,0x27,0x5E,0x63,0x49,0x58,0x10,0xFA,0x90,0x92,0x6C,0x85,0x79,0x6A,0x80,0xD7,
			0xBE,0x81,0xA4,0xF9,0xED,0xBB,0x8E,0xE5,0xF6,0xC5,0x93,0x5B,0xCF,0x35,0xDF,0x7B,
			0x04,0xF3,0xD8,0x47,0xD0,0x1B,0xFE,0xA9,0xDD,0xA5,0x55,0x8B,0xB2,0x3D,0xC3,0xBA,
			0xB9,0xB3,0xA0,0x22,0x33,0xAD,0x9E,0x0B,0xC9,0xD0,0x19,0xD7,0xFB,0xCB,0x51,0x7D,
			0x94,0x47,0x35,0x3B,0xB7,0x4D,0x59,0x1D,0xD0,0x37,0x30,0xB5,0xAB,0x51,0xD1,0xAA,
			0x6A,0xD7,0xC1,0xA4,0x08,0x8B,0x30,0x21,0x5F,0x32,0x8B,0x71,0x14,0x99,0x81,0x23,
			0x96,0x30,0xAE,0x51,0xFF,0x19,0x04,0x8C,0x60,0x84,0xF9,0x32,0xF4,0x1D,0x7B,0x5E,
			0xDA,0xD4,0x27,0xD5,0xB4,0xDD,0x0C,0x9C,0x00,0x02,0xA2,0x38,0x20,0x9A,0x88,0xB9,
			0x63,0x5E,0xAB,0xA2,0x2D,0x83,0x81,0x03,0x53,0xC3,0x05,0x19,0xB2,0x96,0x31,0xD0,
			0xC4,0xB2,0xDE,0x23,0x08,0x2D,0x0F,0x20,0x9F,0x2E,0x60,0x73,0xD1,0x8B,0x47,0xE1,
			0xD3,0xCE,0xB5,0x6A,0xCE,0x65,0x5D,0xDD,0xF5,0xDD,0x2E,0xC7,0x82,0x3C,0x0A,0x25,
			0x8D,0x97,0x29,0x67,0xC4,0x88,0x8D,0x07,0x70,0x4D,0x03,0x00,0x9D,0x81,0xF0,0x0D,
			0xCC,0x04,0xCF,0x89,0xC8,0x79,0xE8,0xBB,0xF6,0x4C,0xB3,0x66,0x66,0x0B,0x1F,0x62,
			0x38,0xE2,0xA0,0x05,0x5C,0x67,0x99,0xDC,0xD0,0x24,0x60,0x9B,0xF1,0x86,0x1A,0x88,
			0x68,0x92,0x6F,0xF5,0xBD,0x7F,0x55,0xFB,0x53,0xEF,0xAF,0x34,0xB9,0x4E,0x02,0x46,
			0x87,0xB5,0x9B,0xD7,0xA2,0x69,0x8A,0xEE,0x6D,0xAA,0xF5,0xB7,0x7E,0x47,0x2C,0x17,
			0xFA,0x8B,0xF0,0x5C,0xF4,0xF0,0x0F,0x66,0x47,0xD8,0x5D,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_AddModule_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_AddModule_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xB2,0x00,0x00,0x80,0x78,0x01,0x0B,0x70,0x76,0x8E,0xF7,0xCD,0x4F,0x29,0xCD,0x49,
			0x75,0xCB,0xCC,0x49,0x55,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x49,0xCA,0xCC,0x4B,
			0x2C,0xAA,0xD4,0xB1,0x0E,0x40,0x91,0x0D,0xA9,0x2C,0x48,0x55,0x06,0x0B,0xF9,0xBB,
			0x84,0xFA,0xB8,0xC6,0xFB,0x3A,0x3A,0x7B,0x78,0xFA,0xB9,0xC6,0x07,0x39,0x86,0xDB,
			0x1A,0xEA,0x20,0x49,0x38,0xFB,0xFB,0x85,0x04,0xF9,0xFB,0xD8,0x1A,0x21,0x0B,0x7A,
			0x84,0xF8,0xFA,0xD8,0x9A,0xE8,0x58,0xE7,0xA6,0x96,0x64,0xE4,0xA7,0xD4,0x78,0xFA,
			0x01,0x51,0x88,0x99,0x89,0x0E,0x90,0x81,0x69,0x0D,0x48,0x34,0xB1,0xA8,0x28,0xB1,
			0xD2,0x06,0x55,0xCE,0x4E,0x87,0x01,0x00,0xBD,0xD2,0x3A,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_AddModuleFile_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_AddModuleFile_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x11,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,
			0xA2,0x10,0x33,0x13,0x1D,0x06,0x00,0x3A,0x7C,0x05,0x92,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_RemoveModule_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_RemoveModule_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,
			0xA2,0x10,0x33,0x13,0x1D,0x08,0xC3,0xD8,0x48,0x87,0x01,0x00,0x7D,0x47,0x08,0x21,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x99,0x02,0x00,0x80,0x78,0x01,0x5D,0x52,0x5D,0x6B,0x83,0x30,0x14,0xDD,0x7F,0x59,
			0x1F,0x7D,0xD8,0xAC,0xC8,0x60,0x73,0x90,0x46,0x69,0xDD,0xA2,0x91,0x18,0x6B,0xF7,
			0x14,0xA4,0x0D,0xAD,0xB0,0x69,0xB1,0x16,0x26,0xEC,0xC7,0x2F,0xB9,0x49,0xDB,0x6C,
			0x2F,0xE1,0xDE,0x93,0x73,0x3F,0x72,0x4E,0x0A,0x8C,0xC5,0xA2,0xED,0x9A,0x61,0xE2,
			0xD3,0x51,0xDE,0x17,0x2A,0xCD,0x10,0x5E,0xA5,0x79,0x22,0x18,0xAA,0xA3,0x07,0x4F,
			0x23,0x6F,0x68,0x8D,0x04,0xA6,0x71,0x12,0x41,0x8A,0xCB,0x15,0x62,0x85,0x05,0x9E,
			0x35,0x01,0x17,0xD5,0xB5,0x5C,0xC5,0x62,0xF3,0x14,0x8A,0x4D,0x18,0xD8,0x72,0x8D,
			0x20,0x96,0x45,0x1E,0x70,0x93,0x6F,0xB9,0x3D,0x8F,0x72,0xD1,0x8E,0x27,0x18,0x37,
			0xF7,0x17,0x29,0x2F,0x2D,0x35,0x0C,0x20,0x31,0xCC,0xAC,0xDF,0x9D,0x3F,0x25,0x69,
			0x46,0xD9,0x6D,0x27,0xE0,0x12,0xC4,0x93,0x1C,0x7F,0x88,0x32,0x43,0x84,0xD8,0x92,
			0x0B,0x96,0x53,0xA6,0xD0,0xE8,0x11,0x56,0xBC,0x80,0x04,0xB1,0x65,0x12,0xF9,0x6E,
			0xBF,0x62,0xE8,0x8F,0x75,0x3B,0x1E,0xDE,0xE5,0x34,0x4B,0x73,0x1E,0x06,0x50,0x60,
			0x66,0xE9,0x3B,0x39,0x8C,0xD3,0x7F,0xBE,0xC6,0x66,0xA0,0x0D,0xAC,0xC4,0x9B,0xBD,
			0xA7,0x4A,0xE7,0xBE,0x3D,0x9D,0x1B,0x25,0xA2,0xD3,0xCF,0xEE,0xEE,0x9D,0xC6,0xA1,
			0xED,0xF6,0x46,0x3C,0x23,0x15,0xC4,0xB4,0xD4,0xAA,0x41,0xE8,0xA8,0x02,0xF9,0xCD,
			0x13,0x77,0x17,0x35,0x78,0xE6,0xF4,0x32,0x4B,0xAF,0xE5,0x70,0x6A,0xFB,0xEE,0x0F,
			0xEF,0xEA,0x25,0x8D,0x2B,0x92,0x88,0x34,0x5B,0x16,0x8C,0x62,0x2B,0x4E,0x66,0xC0,
			0x75,0x1A,0x03,0xE8,0xC3,0xC0,0x1B,0x58,0x72,0x56,0x61,0x5E,0xB1,0x24,0x0A,0xDC,
			0x9E,0x76,0x8C,0xD6,0xEC,0xFA,0x70,0x13,0x83,0xAD,0xE6,0x2D,0xE0,0x12,0x2D,0x45,
			0x9D,0xE6,0x31,0xAD,0x2F,0xAE,0x2A,0x80,0xA4,0x79,0xB5,0x51,0x3F,0xE0,0x4B,0x8E,
			0x87,0x7E,0xF7,0x43,0x2B,0xFE,0xD3,0x0C,0x43,0x33,0xBD,0xDC,0xB4,0x73,0x8C,0x79,
			0xF5,0xEE,0x7E,0x01,0xE4,0x01,0xD6,0x8F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_ListModules_TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_ListModules_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x23,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0xAB,0xF1,0x0C,0x08,0xD0,0x01,0x52,
			0x49,0x99,0x79,0x89,0x45,0x95,0x3A,0x0C,0x00,0xF8,0x4D,0x0C,0xF9,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_SetRedirect__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_SetRedirect__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_SetTimeout__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_SetTimeout__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0xC1,0x10,
			0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,0x9A,0x78,
			0x15,0x41,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_MethodCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_MethodCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3C,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,
			0xC1,0x10,0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,
			0xC2,0xD8,0x15,0xED,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_PCC_Toolkit_CallbackCheck__TypeInfo_ = (const char*)bin.Get();
		s_PCC_Toolkit_CallbackCheck__TypeInfo_len_ = bin.Length()-1;
	}

	END_LOCAL_SAFE_STATIC_OBJ(dummyVar);
}

struct NP_SCATTERED_AllMethodCallsTypeInfo_
{
	const char* typeInfos[44];
};
static NP_SCATTERED_AllMethodCallsTypeInfo_& NP_SCATTERED_GetAllMethodCallsTypeInfo_Impl_(
				OUT INT_PTR& methods
				)
{
	methods = 44;
	BEGIN_LOCAL_SAFE_STATIC_OBJ(NP_SCATTERED_AllMethodCallsTypeInfo_, infosObj);
	InitializeAllCallsTypeInfo_();
	infosObj.typeInfos[0] = s_GRID_User_AddJobProgram_TypeInfo_;
	infosObj.typeInfos[1] = s_GRID_User_DelJobProgram_TypeInfo_;
	infosObj.typeInfos[2] = s_GRID_User_FindJobProgram_TypeInfo_;
	infosObj.typeInfos[3] = s_GRID_User_ListJobPrograms_TypeInfo_;
	infosObj.typeInfos[4] = s_GRID_User_CommitJob_TypeInfo_;
	infosObj.typeInfos[5] = s_GRID_User_CancelJob_TypeInfo_;
	infosObj.typeInfos[6] = s_GRID_User_SetJobPriority_TypeInfo_;
	infosObj.typeInfos[7] = s_GRID_User_ListJobs_TypeInfo_;
	infosObj.typeInfos[8] = s_GRID_User_QueryJobs_TypeInfo_;
	infosObj.typeInfos[9] = s_GRID_User_UpdateGrid_TypeInfo_;
	infosObj.typeInfos[10] = s_GRID_User_ListModules_TypeInfo_;
	infosObj.typeInfos[11] = s_GRID_User_GetLogCount_TypeInfo_;
	infosObj.typeInfos[12] = s_GRID_User_LoadLogInfos_TypeInfo_;
	infosObj.typeInfos[13] = s_GRID_User_Login_TypeInfo_;
	infosObj.typeInfos[14] = s_GRID_User_Logout_TypeInfo_;
	infosObj.typeInfos[15] = s_GRID_User_AddUser_TypeInfo_;
	infosObj.typeInfos[16] = s_GRID_User_DelUser_TypeInfo_;
	infosObj.typeInfos[17] = s_GRID_User_UpdatePassword_TypeInfo_;
	infosObj.typeInfos[18] = s_GRID_User_ManageUser_TypeInfo_;
	infosObj.typeInfos[19] = s_GRID_User_ListAllUsers_TypeInfo_;
	infosObj.typeInfos[20] = s_GRID_User_GetUserInfo_TypeInfo_;
	infosObj.typeInfos[21] = s_GRID_User_ListJTMs_TypeInfo_;
	infosObj.typeInfos[22] = s_GRID_User_GetJTMInfo_TypeInfo_;
	infosObj.typeInfos[23] = s_GRID_User_GetGridProperty_TypeInfo_;
	infosObj.typeInfos[24] = s_GRID_User_AddSplitter_TypeInfo_;
	infosObj.typeInfos[25] = s_GRID_User_DelSplitter_TypeInfo_;
	infosObj.typeInfos[26] = s_GRID_User_ListSplitters_TypeInfo_;
	infosObj.typeInfos[27] = s_GRID_User_ListSplitterParam_TypeInfo_;
	infosObj.typeInfos[28] = s_GRID_User_GetGridStatistics_TypeInfo_;
	infosObj.typeInfos[29] = s_PCC_Scatter_OnComputed_TypeInfo_;
	infosObj.typeInfos[30] = s_PCC_Service_Login_TypeInfo_;
	infosObj.typeInfos[31] = s_PCC_Service_Logout_TypeInfo_;
	infosObj.typeInfos[32] = s_PCC_Service_ListModules_TypeInfo_;
	infosObj.typeInfos[33] = s_PCC_Service_GetModuleFile_TypeInfo_;
	infosObj.typeInfos[34] = s_PCC_Service_Execute_TypeInfo_;
	infosObj.typeInfos[35] = s_PCC_Service_QueryJobs_TypeInfo_;
	infosObj.typeInfos[36] = s_PCC_Service_CancelJob_TypeInfo_;
	infosObj.typeInfos[37] = s_PCC_Toolkit_Login_TypeInfo_;
	infosObj.typeInfos[38] = s_PCC_Toolkit_Logout_TypeInfo_;
	infosObj.typeInfos[39] = s_PCC_Toolkit_AddModule_TypeInfo_;
	infosObj.typeInfos[40] = s_PCC_Toolkit_AddModuleFile_TypeInfo_;
	infosObj.typeInfos[41] = s_PCC_Toolkit_RemoveModule_TypeInfo_;
	infosObj.typeInfos[42] = s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_;
	infosObj.typeInfos[43] = s_PCC_Toolkit_ListModules_TypeInfo_;
	END_LOCAL_SAFE_STATIC_OBJ(infosObj);
}
const char** NP_SCATTERED_GetAllMethodCallsTypeInfo_(
				OUT INT_PTR& methods
				)
{
	NP_SCATTERED_AllMethodCallsTypeInfo_& infosObj = NP_SCATTERED_GetAllMethodCallsTypeInfo_Impl_(methods);
	return infosObj.typeInfos;
}

const char* NP_SCATTERED_GetCallbackCallTypeInfo_(
				IN const char* faceName,
				IN const char* callbackName
				)
{
	if(NULL==faceName || NULL==callbackName)
	{
		ASSERT(false);
		return "";
	}
	InitializeAllCallsTypeInfo_();

	if(0 == strcmp("GRID_User", faceName))
	{
		if(0 == strcmp("ReplyCommitJob", callbackName))
			return s_GRID_User_ReplyCommitJob_TypeInfo_;
	}

	else if(0 == strcmp("PCC_Scatter", faceName))
	{
		if(0 == strcmp("Compute", callbackName))
			return s_PCC_Scatter_Compute_TypeInfo_;
		else if(0 == strcmp("AddMoudle", callbackName))
			return s_PCC_Scatter_AddMoudle_TypeInfo_;
		else if(0 == strcmp("RemoveModule", callbackName))
			return s_PCC_Scatter_RemoveModule_TypeInfo_;
		else if(0 == strcmp("ListModules", callbackName))
			return s_PCC_Scatter_ListModules_TypeInfo_;
	}

	else if(0 == strcmp("PCC_Service", faceName))
	{
		if(0 == strcmp("OnExecuted", callbackName))
			return s_PCC_Service_OnExecuted_TypeInfo_;
	}

	else if(0 == strcmp("PCC_Toolkit", faceName))
	{
	}

	return "";
}

TCPSError MakeLocalSession_GRID_User__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IGRID_User_LocalMethod*& methodHandler,
			IN IGRID_User_LocalCallback* callbackHandler
			);
TCPSError MakeLocalSession_PCC_Scatter__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Scatter_LocalMethod*& methodHandler,
			IN IPCC_Scatter_LocalCallback* callbackHandler
			);
TCPSError MakeLocalSession_PCC_Service__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Service_LocalMethod*& methodHandler,
			IN IPCC_Service_LocalCallback* callbackHandler
			);
TCPSError MakeLocalSession_PCC_Toolkit__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Toolkit_LocalMethod*& methodHandler,
			IN IPCC_Toolkit_LocalCallback* callbackHandler
			);

static void RegisterLocalSessionMakeFunction_(const IPP& serveIPP, NP_SCATTEREDSessionMaker& sessionMaker)
{
	NPR_ASSERT(TCPS_OK == iscm_RegisterFunction(serveIPP, "MakeLocalSession_GRID_User", (PROC)MakeLocalSession_GRID_User__, &sessionMaker));
	NPR_ASSERT(TCPS_OK == iscm_RegisterFunction(serveIPP, "MakeLocalSession_PCC_Scatter", (PROC)MakeLocalSession_PCC_Scatter__, &sessionMaker));
	NPR_ASSERT(TCPS_OK == iscm_RegisterFunction(serveIPP, "MakeLocalSession_PCC_Service", (PROC)MakeLocalSession_PCC_Service__, &sessionMaker));
	NPR_ASSERT(TCPS_OK == iscm_RegisterFunction(serveIPP, "MakeLocalSession_PCC_Toolkit", (PROC)MakeLocalSession_PCC_Toolkit__, &sessionMaker));
}

static void UnregisterLocalSessionMakeFunction_(const IPP& serveIPP)
{
	NPR_ASSERT(TCPS_OK == iscm_UnregisterFunction(serveIPP, "MakeLocalSession_GRID_User"));
	NPR_ASSERT(TCPS_OK == iscm_UnregisterFunction(serveIPP, "MakeLocalSession_PCC_Scatter"));
	NPR_ASSERT(TCPS_OK == iscm_UnregisterFunction(serveIPP, "MakeLocalSession_PCC_Service"));
	NPR_ASSERT(TCPS_OK == iscm_UnregisterFunction(serveIPP, "MakeLocalSession_PCC_Toolkit"));
}
//[[end_method_wrap_body]]

#if defined(_MSC_VER)
	#pragma warning(default: 4702) // unreachable code
#endif

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_AddJobProgram(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"AddJobProgram",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_ProgramInfo programInfo
	IN GRID_ProgramInfo programInfo_wrap;
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.ver);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.cpuType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.osType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.executeBits);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.programID.binaryType);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, programInfo_wrap.description);

	// IN tcps_Array<GRID_ProgramFile> files
	IN tcps_Array<GRID_ProgramFile> files_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	files_wrap.Resize(array_len);
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		GRID_ProgramFile& ref1 = files_wrap[idx1];
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.isExecutable);
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.name);
			GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.data);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::AddJobProgram() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->AddJobProgram(
			programInfo_wrap,
			files_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::AddJobProgram(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::AddJobProgram(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_DelJobProgram(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"DelJobProgram",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<INT64> programKeys
	IN tcps_Array<INT64> programKeys_wrap;
	GET_PODARRAY_EX_(thisObj, ptrInParams, ptrInParamsLen, programKeys_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::DelJobProgram() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->DelJobProgram(
			programKeys_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::DelJobProgram(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::DelJobProgram(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_FindJobProgram(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"FindJobProgram",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_ProgramID programID
	IN GRID_ProgramID programID_wrap;
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.name);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.ver);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.cpuType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.osType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.executeBits);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, programID_wrap.binaryType);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::FindJobProgram() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT BOOL found;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->FindJobProgram(
			programID_wrap,
			opWrapper->found
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::FindJobProgram(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::FindJobProgram(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT BOOL found
	OUT const BOOL& found_wrap = opWrapper->found;
	Set_BaseType_(outfiter, found_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListJobPrograms(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListJobPrograms",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListJobPrograms() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::JobProgram> jobPrograms;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListJobPrograms(
			opWrapper->jobPrograms
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListJobPrograms(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListJobPrograms(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<JobProgram> jobPrograms
	OUT const tcps_Array<GRID_User_T::JobProgram>& jobPrograms_wrap = opWrapper->jobPrograms;
	Set_BaseType_(outfiter, jobPrograms_wrap.LenRef());
	for(int idx1=0; idx1<jobPrograms_wrap.Length(); ++idx1)
	{
		const GRID_User_T::JobProgram& ref1 = jobPrograms_wrap[idx1];
			Set_BaseType_(outfiter, ref1.programKey);
					Set_String_(outfiter, ref1.programInfo.programID.name);
					Set_BaseType_(outfiter, ref1.programInfo.programID.ver);
					Set_BaseType_(outfiter, ref1.programInfo.programID.cpuType);
					Set_BaseType_(outfiter, ref1.programInfo.programID.osType);
					Set_BaseType_(outfiter, ref1.programInfo.programID.executeBits);
					Set_BaseType_(outfiter, ref1.programInfo.programID.binaryType);
				Set_String_(outfiter, ref1.programInfo.description);
			Set_BaseType_(outfiter, ref1.addTime);
			Set_BaseType_(outfiter, ref1.programSize);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_CommitJob(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"CommitJob",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_JobInfo jobInfo
	IN GRID_JobInfo jobInfo_wrap;
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.ver);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.cpuType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.osType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.executeBits);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programID.binaryType);
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.ver);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.cpuType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.osType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.executeBits);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitter.binaryType);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.dataInputUrl);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.dataOutputUrl);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.priority);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.jobTaskTimeout);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.jobTaskMaxAttempts);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.skipFailedJobTaskPercent);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.jobName);
		GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.splitterParam);
		GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, jobInfo_wrap.programParam);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::CommitJob() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT64 jobKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->CommitJob(
			opWrapper->jobKey,
			jobInfo_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::CommitJob(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::CommitJob(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT64 jobKey
	OUT const INT64& jobKey_wrap = opWrapper->jobKey;
	Set_BaseType_(outfiter, jobKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_CancelJob(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"CancelJob",
					ptrInParams,
					ptrInParamsLen,
					NULL,
					NULL
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<INT64> jobKeys
	IN tcps_Array<INT64> jobKeys_wrap;
	GET_PODARRAY_EX_(thisObj, ptrInParams, ptrInParamsLen, jobKeys_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::CancelJob() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->CancelJob(
			jobKeys_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::CancelJob(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::CancelJob(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_SetJobPriority(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"SetJobPriority",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 jobKey
	IN INT64 jobKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobKey_wrap);

	// IN GRID_JobPriority priority
	IN GRID_JobPriority priority_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, priority_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::SetJobPriority() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->SetJobPriority(
			jobKey_wrap,
			priority_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::SetJobPriority(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::SetJobPriority(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListJobs(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListJobs",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 jobState
	IN INT32 jobState_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobState_wrap);

	// IN LTMSEL beginTime
	IN LTMSEL beginTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, beginTime_wrap);

	// IN LTMSEL endTime
	IN LTMSEL endTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, endTime_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListJobs() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::JobReport> jobReports;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListJobs(
			opWrapper->jobReports,
			jobState_wrap,
			beginTime_wrap,
			endTime_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListJobs(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListJobs(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<JobReport> jobReports
	OUT const tcps_Array<GRID_User_T::JobReport>& jobReports_wrap = opWrapper->jobReports;
	Set_BaseType_(outfiter, jobReports_wrap.LenRef());
	for(int idx1=0; idx1<jobReports_wrap.Length(); ++idx1)
	{
		const GRID_User_T::JobReport& ref1 = jobReports_wrap[idx1];
			Set_BaseType_(outfiter, ref1.jobKey);
					Set_String_(outfiter, ref1.jobInfo.programID.name);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.ver);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.cpuType);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.osType);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.executeBits);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.binaryType);
					Set_String_(outfiter, ref1.jobInfo.splitter.name);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.ver);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.cpuType);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.osType);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.executeBits);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.binaryType);
				Set_String_(outfiter, ref1.jobInfo.dataInputUrl);
				Set_String_(outfiter, ref1.jobInfo.dataOutputUrl);
				Set_BaseType_(outfiter, ref1.jobInfo.priority);
				Set_BaseType_(outfiter, ref1.jobInfo.jobTaskTimeout);
				Set_BaseType_(outfiter, ref1.jobInfo.jobTaskMaxAttempts);
				Set_BaseType_(outfiter, ref1.jobInfo.skipFailedJobTaskPercent);
				Set_String_(outfiter, ref1.jobInfo.jobName);
				Set_Binary_(outfiter, ref1.jobInfo.splitterParam);
				Set_Binary_(outfiter, ref1.jobInfo.programParam);
			Set_BaseType_(outfiter, ref1.jobState);
			Set_BaseType_(outfiter, ref1.progress);
			Set_BaseType_(outfiter, ref1.costedMSELs);
			Set_BaseType_(outfiter, ref1.commitTime);
			Set_BaseType_(outfiter, ref1.errorCode);
			Set_String_(outfiter, ref1.causesOfError);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_QueryJobs(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"QueryJobs",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<INT64> jobKeys
	IN tcps_Array<INT64> jobKeys_wrap;
	GET_PODARRAY_EX_(thisObj, ptrInParams, ptrInParamsLen, jobKeys_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::QueryJobs() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::JobReport> jobReports;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->QueryJobs(
			jobKeys_wrap,
			opWrapper->jobReports
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::QueryJobs(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::QueryJobs(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<JobReport> jobReports
	OUT const tcps_Array<GRID_User_T::JobReport>& jobReports_wrap = opWrapper->jobReports;
	Set_BaseType_(outfiter, jobReports_wrap.LenRef());
	for(int idx1=0; idx1<jobReports_wrap.Length(); ++idx1)
	{
		const GRID_User_T::JobReport& ref1 = jobReports_wrap[idx1];
			Set_BaseType_(outfiter, ref1.jobKey);
					Set_String_(outfiter, ref1.jobInfo.programID.name);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.ver);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.cpuType);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.osType);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.executeBits);
					Set_BaseType_(outfiter, ref1.jobInfo.programID.binaryType);
					Set_String_(outfiter, ref1.jobInfo.splitter.name);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.ver);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.cpuType);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.osType);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.executeBits);
					Set_BaseType_(outfiter, ref1.jobInfo.splitter.binaryType);
				Set_String_(outfiter, ref1.jobInfo.dataInputUrl);
				Set_String_(outfiter, ref1.jobInfo.dataOutputUrl);
				Set_BaseType_(outfiter, ref1.jobInfo.priority);
				Set_BaseType_(outfiter, ref1.jobInfo.jobTaskTimeout);
				Set_BaseType_(outfiter, ref1.jobInfo.jobTaskMaxAttempts);
				Set_BaseType_(outfiter, ref1.jobInfo.skipFailedJobTaskPercent);
				Set_String_(outfiter, ref1.jobInfo.jobName);
				Set_Binary_(outfiter, ref1.jobInfo.splitterParam);
				Set_Binary_(outfiter, ref1.jobInfo.programParam);
			Set_BaseType_(outfiter, ref1.jobState);
			Set_BaseType_(outfiter, ref1.progress);
			Set_BaseType_(outfiter, ref1.costedMSELs);
			Set_BaseType_(outfiter, ref1.commitTime);
			Set_BaseType_(outfiter, ref1.errorCode);
			Set_String_(outfiter, ref1.causesOfError);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_UpdateGrid(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"UpdateGrid",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<GRID_ProgramFile> files
	IN tcps_Array<GRID_ProgramFile> files_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	files_wrap.Resize(array_len);
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		GRID_ProgramFile& ref1 = files_wrap[idx1];
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.isExecutable);
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.name);
			GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.data);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::UpdateGrid() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->UpdateGrid(
			files_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::UpdateGrid(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::UpdateGrid(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListModules(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListModules",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListModules() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_ProgramInfo> modules;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListModules(
			opWrapper->modules
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListModules(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListModules(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<GRID_ProgramInfo> modules
	OUT const tcps_Array<GRID_ProgramInfo>& modules_wrap = opWrapper->modules;
	Set_BaseType_(outfiter, modules_wrap.LenRef());
	for(int idx1=0; idx1<modules_wrap.Length(); ++idx1)
	{
		const GRID_ProgramInfo& ref1 = modules_wrap[idx1];
				Set_String_(outfiter, ref1.programID.name);
				Set_BaseType_(outfiter, ref1.programID.ver);
				Set_BaseType_(outfiter, ref1.programID.cpuType);
				Set_BaseType_(outfiter, ref1.programID.osType);
				Set_BaseType_(outfiter, ref1.programID.executeBits);
				Set_BaseType_(outfiter, ref1.programID.binaryType);
			Set_String_(outfiter, ref1.description);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_GetLogCount(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"GetLogCount",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN LTMSEL beginTime
	IN LTMSEL beginTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, beginTime_wrap);

	// IN LTMSEL endTime
	IN LTMSEL endTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, endTime_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::GetLogCount() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT32 logCount;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->GetLogCount(
			beginTime_wrap,
			endTime_wrap,
			opWrapper->logCount
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::GetLogCount(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::GetLogCount(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT32 logCount
	OUT const INT32& logCount_wrap = opWrapper->logCount;
	Set_BaseType_(outfiter, logCount_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_LoadLogInfos(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"LoadLogInfos",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN LTMSEL beginTime
	IN LTMSEL beginTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, beginTime_wrap);

	// IN LTMSEL endTime
	IN LTMSEL endTime_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, endTime_wrap);

	// IN INT32 startPos
	IN INT32 startPos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, startPos_wrap);

	// IN INT32 length
	IN INT32 length_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, length_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::LoadLogInfos() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::LogInfo> logInfos;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->LoadLogInfos(
			opWrapper->logInfos,
			beginTime_wrap,
			endTime_wrap,
			startPos_wrap,
			length_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::LoadLogInfos(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::LoadLogInfos(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<LogInfo> logInfos
	OUT const tcps_Array<GRID_User_T::LogInfo>& logInfos_wrap = opWrapper->logInfos;
	Set_BaseType_(outfiter, logInfos_wrap.LenRef());
	for(int idx1=0; idx1<logInfos_wrap.Length(); ++idx1)
	{
		const GRID_User_T::LogInfo& ref1 = logInfos_wrap[idx1];
			Set_BaseType_(outfiter, ref1.time);
			Set_String_(outfiter, ref1.info);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_Login(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"Login",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String userName
	IN tcps_String userName_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userName_wrap);

	// IN tcps_String password
	IN tcps_String password_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, password_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::Login() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->Login(
			userName_wrap,
			password_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::Login(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::Login(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_Logout(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"Logout",
					ptrInParams,
					ptrInParamsLen,
					NULL,
					NULL
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::Logout() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->Logout(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::Logout(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::Logout(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_AddUser(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"AddUser",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN UserInfo userInfo
	IN GRID_User_T::UserInfo userInfo_wrap;
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.userName);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.passWord);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.role);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::AddUser() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->AddUser(
			userInfo_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::AddUser(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::AddUser(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_DelUser(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"DelUser",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> userNames
	IN tcps_Array<tcps_String> userNames_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	userNames_wrap.Resize(array_len);
	for(int idx1=0; idx1<userNames_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = userNames_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::DelUser() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->DelUser(
			userNames_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::DelUser(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::DelUser(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_UpdatePassword(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"UpdatePassword",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String oldPassword
	IN tcps_String oldPassword_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, oldPassword_wrap);

	// IN tcps_String newPassword
	IN tcps_String newPassword_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, newPassword_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::UpdatePassword() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->UpdatePassword(
			oldPassword_wrap,
			newPassword_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::UpdatePassword(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::UpdatePassword(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ManageUser(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ManageUser",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN UserInfo userInfo
	IN GRID_User_T::UserInfo userInfo_wrap;
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.userName);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.passWord);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, userInfo_wrap.role);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ManageUser() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ManageUser(
			userInfo_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ManageUser(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ManageUser(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListAllUsers(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListAllUsers",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListAllUsers() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::UserInfo> userInfos;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListAllUsers(
			opWrapper->userInfos
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListAllUsers(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListAllUsers(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<UserInfo> userInfos
	OUT const tcps_Array<GRID_User_T::UserInfo>& userInfos_wrap = opWrapper->userInfos;
	Set_BaseType_(outfiter, userInfos_wrap.LenRef());
	for(int idx1=0; idx1<userInfos_wrap.Length(); ++idx1)
	{
		const GRID_User_T::UserInfo& ref1 = userInfos_wrap[idx1];
			Set_String_(outfiter, ref1.userName);
			Set_String_(outfiter, ref1.passWord);
			Set_BaseType_(outfiter, ref1.role);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_GetUserInfo(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"GetUserInfo",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String userName
	IN tcps_String userName_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, userName_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::GetUserInfo() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT GRID_User_T::UserInfo userInfo;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->GetUserInfo(
			userName_wrap,
			opWrapper->userInfo
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::GetUserInfo(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::GetUserInfo(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT UserInfo userInfo
	OUT const GRID_User_T::UserInfo& userInfo_wrap = opWrapper->userInfo;
		Set_String_(outfiter, userInfo_wrap.userName);
		Set_String_(outfiter, userInfo_wrap.passWord);
		Set_BaseType_(outfiter, userInfo_wrap.role);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListJTMs(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListJTMs",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListJTMs() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<tcps_String> jtms;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListJTMs(
			opWrapper->jtms
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListJTMs(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListJTMs(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<tcps_String> jtms
	OUT const tcps_Array<tcps_String>& jtms_wrap = opWrapper->jtms;
	Set_BaseType_(outfiter, jtms_wrap.LenRef());
	for(int idx1=0; idx1<jtms_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = jtms_wrap[idx1];
		Set_String_(outfiter, ref1);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_GetJTMInfo(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"GetJTMInfo",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String jtm
	IN tcps_String jtm_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, jtm_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::GetJTMInfo() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT GRID_User_T::JTMInfo jtmInfo;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->GetJTMInfo(
			jtm_wrap,
			opWrapper->jtmInfo
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::GetJTMInfo(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::GetJTMInfo(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT JTMInfo jtmInfo
	OUT const GRID_User_T::JTMInfo& jtmInfo_wrap = opWrapper->jtmInfo;
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.cpuType);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.cpuFreq);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.cpuProcessers);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.cpuThreads);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.totalMemoryBytes);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.networkBandwidth);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.osType);
			Set_String_(outfiter, jtmInfo_wrap.staticContext.osDetail);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.executeBits);
			Set_BaseType_(outfiter, jtmInfo_wrap.staticContext.binaryType);
			Set_String_(outfiter, jtmInfo_wrap.staticContext.hardwareInfo);
		Set_BaseType_(outfiter, jtmInfo_wrap.statisticInfo);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_GetGridProperty(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"GetGridProperty",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::GetGridProperty() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT GRID_User_T::GridProperty gridProperty;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->GetGridProperty(
			opWrapper->gridProperty
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::GetGridProperty(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::GetGridProperty(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT GridProperty gridProperty
	OUT const GRID_User_T::GridProperty& gridProperty_wrap = opWrapper->gridProperty;
		Set_BaseType_(outfiter, gridProperty_wrap.version);
		Set_BaseType_(outfiter, gridProperty_wrap.jtmNum);
		Set_BaseType_(outfiter, gridProperty_wrap.processorNum);
		Set_BaseType_(outfiter, gridProperty_wrap.threadNum);
		Set_String_(outfiter, gridProperty_wrap.authorizedID);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_AddSplitter(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"AddSplitter",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_ProgramInfo splitter
	IN GRID_ProgramInfo splitter_wrap;
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.ver);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.cpuType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.osType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.executeBits);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.binaryType);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.description);

	// IN tcps_Array<GRID_ProgramFile> files
	IN tcps_Array<GRID_ProgramFile> files_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	files_wrap.Resize(array_len);
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		GRID_ProgramFile& ref1 = files_wrap[idx1];
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.isExecutable);
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.name);
			GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.data);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::AddSplitter() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->AddSplitter(
			splitter_wrap,
			files_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::AddSplitter(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::AddSplitter(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_DelSplitter(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"DelSplitter",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_ProgramInfo splitter
	IN GRID_ProgramInfo splitter_wrap;
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.ver);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.cpuType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.osType);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.executeBits);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.programID.binaryType);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.description);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::DelSplitter() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->DelSplitter(
			splitter_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::DelSplitter(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::DelSplitter(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListSplitters(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListSplitters",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListSplitters() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_ProgramID> splitters;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListSplitters(
			opWrapper->splitters
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListSplitters(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListSplitters(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<GRID_ProgramID> splitters
	OUT const tcps_Array<GRID_ProgramID>& splitters_wrap = opWrapper->splitters;
	Set_BaseType_(outfiter, splitters_wrap.LenRef());
	for(int idx1=0; idx1<splitters_wrap.Length(); ++idx1)
	{
		const GRID_ProgramID& ref1 = splitters_wrap[idx1];
			Set_String_(outfiter, ref1.name);
			Set_BaseType_(outfiter, ref1.ver);
			Set_BaseType_(outfiter, ref1.cpuType);
			Set_BaseType_(outfiter, ref1.osType);
			Set_BaseType_(outfiter, ref1.executeBits);
			Set_BaseType_(outfiter, ref1.binaryType);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_ListSplitterParam(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"ListSplitterParam",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN GRID_ProgramID splitter
	IN GRID_ProgramID splitter_wrap;
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.name);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.ver);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.cpuType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.osType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.executeBits);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, splitter_wrap.binaryType);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::ListSplitterParam() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<GRID_User_T::SplitterParam> splitterParams;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->ListSplitterParam(
			splitter_wrap,
			opWrapper->splitterParams
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::ListSplitterParam(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::ListSplitterParam(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<SplitterParam> splitterParams
	OUT const tcps_Array<GRID_User_T::SplitterParam>& splitterParams_wrap = opWrapper->splitterParams;
	Set_BaseType_(outfiter, splitterParams_wrap.LenRef());
	for(int idx1=0; idx1<splitterParams_wrap.Length(); ++idx1)
	{
		const GRID_User_T::SplitterParam& ref1 = splitterParams_wrap[idx1];
			Set_String_(outfiter, ref1.name);
			Set_String_(outfiter, ref1.data);
			Set_String_(outfiter, ref1.description);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_GetGridStatistics(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"GRID_User",
					"GetGridStatistics",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::GetGridStatistics() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT GRID_User_T::GridStatistics stat;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->GetGridStatistics(
			opWrapper->stat
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::GetGridStatistics(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::GetGridStatistics(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT GridStatistics stat
	OUT const GRID_User_T::GridStatistics& stat_wrap = opWrapper->stat;
	Set_BaseType_(outfiter, stat_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_UDPLink_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::UDPLink_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT32 servePort;
		OUT INT32 linkKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->UDPLink_(
			opWrapper->servePort,
			opWrapper->linkKey
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::UDPLink_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::UDPLink_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT32 servePort
	OUT const INT32& servePort_wrap = opWrapper->servePort;
	Set_BaseType_(outfiter, servePort_wrap);

	// OUT INT32 linkKey
	OUT const INT32& linkKey_wrap = opWrapper->linkKey;
	Set_BaseType_(outfiter, linkKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_UDPLinkConfirm_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::UDPLinkConfirm_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->UDPLinkConfirm_(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::UDPLinkConfirm_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::UDPLinkConfirm_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_SetTimeout_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvTimeout
	IN INT32 recvTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvTimeout_wrap);

	// IN INT32 sendTimeout
	IN INT32 sendTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendTimeout_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::SetTimeout_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->SetTimeout_(
			recvTimeout_wrap,
			sendTimeout_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::SetTimeout_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::SetTimeout_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_SetSessionBufferSize_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvBufBytes
	IN INT32 recvBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvBufBytes_wrap);

	// IN INT32 sendBufBytes
	IN INT32 sendBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendBufBytes_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::SetSessionBufferSize_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->SetSessionBufferSize_(
			recvBufBytes_wrap,
			sendBufBytes_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::SetSessionBufferSize_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::SetSessionBufferSize_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_GRID_User_MethodCheck_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> methods
	IN tcps_Array<tcps_String> methods_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methods_wrap.Resize(array_len);
	for(int idx1=0; idx1<methods_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methods_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	// IN tcps_Array<tcps_String> methodTypeInfos
	IN tcps_Array<tcps_String> methodTypeInfos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methodTypeInfos_wrap.Resize(array_len);
	for(int idx1=0; idx1<methodTypeInfos_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methodTypeInfos_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("GRID_User_S::MethodCheck_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<BOOL> matchingFlags;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		GRID_User_S* gRID_UserObj_wrap;
		if(thisObj)
			gRID_UserObj_wrap = thisObj->m_gRID_User;
		else
			gRID_UserObj_wrap = (GRID_User_S*)faceObj;
		ASSERT(gRID_UserObj_wrap);
		errTcps = gRID_UserObj_wrap->MethodCheck_(
			methods_wrap,
			methodTypeInfos_wrap,
			opWrapper->matchingFlags
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("GRID_User_S::MethodCheck_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("GRID_User_S::MethodCheck_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<BOOL> matchingFlags
	OUT const tcps_Array<BOOL>& matchingFlags_wrap = opWrapper->matchingFlags;
	Set_PODArray_(outfiter, matchingFlags_wrap);

	return TCPS_OK;
}

GRID_User_S::CallbackMatchingFlag::CallbackMatchingFlag()
{
	Reset();
	cbmap_.insert(std::make_pair(CPtrStrA("ReplyCommitJob", 14), Info(&matching_ReplyCommitJob, true)));
	cbmap_.insert(std::make_pair(CPtrStrA("SetRedirect_", 12), Info(&matching_SetRedirect_, true)));
}

void GRID_User_S::CallbackMatchingFlag::Reset()
{
	matching_ReplyCommitJob = false;
	matching_SetRedirect_ = false;
}

TCPSError GRID_User_S::UpdateCallbackMatchingFlag_()
{
	if(!m_callbackMatchingUpdatedFlag.needUpdate)
		return TCPS_OK;
	InitializeAllCallsTypeInfo_();
	tcps_String callbacks_ar[2];
	IN tcps_Array<tcps_String> callbacks;
	callbacks.Attach(xat_bind, callbacks_ar, 2);
	tcps_String callbackTypeInfos_ar[2];
	IN tcps_Array<tcps_String> callbackTypeInfos;
	callbackTypeInfos.Attach(xat_bind, callbackTypeInfos_ar, 2);
	callbacks_ar[0].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ReplyCommitJob"));
	callbackTypeInfos_ar[0].Attach(xat_bind, (char*)s_GRID_User_ReplyCommitJob_TypeInfo_, s_GRID_User_ReplyCommitJob_TypeInfo_len_);
	callbacks_ar[1].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetRedirect_"));
	callbackTypeInfos_ar[1].Attach(xat_bind, (char*)s_GRID_User_SetRedirect__TypeInfo_, s_GRID_User_SetRedirect__TypeInfo_len_);
	OUT tcps_Array<BOOL> matchingFlags;
	TCPSError err = this->CallbackCheck_(callbacks, callbackTypeInfos, matchingFlags);
	if(TCPS_OK == err)
	{
		if(matchingFlags.Length() == callbacks.Length())
		{
			m_callbackMatchingUpdatedFlag.needUpdate = false;
			const BOOL* const matchingFlags_p = matchingFlags.Get();
			m_callbackMatchingFlag.matching_ReplyCommitJob = matchingFlags_p[0];
			m_callbackMatchingFlag.matching_SetRedirect_ = matchingFlags_p[1];
		}
		else
		{
			ASSERT(false);
			err = TCPS_ERR_MALFORMED_REPLY;
		}
	}
	if(m_callbackMatchingUpdatedFlag.needUpdate)
		m_callbackMatchingFlag.Reset();
	return err;
}

const GRID_User_S::CallbackMatchingFlag& GRID_User_S::GetCallbackMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	TCPSError ret = UpdateCallbackMatchingFlag_();
	if(err)
		*err = ret;
	return m_callbackMatchingFlag;
}

TCPSError GRID_User_S::OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	INT_PTR inParamsDataLen = sizeof(iscm_PeerCallFlags);
	inParamsDataLen += 11; // "GRID_User::"
	if(nameLen < 0)
		nameLen = strlen(methodName);
	inParamsDataLen += sizeof(INT32)+nameLen+1;
	inParamsDataLen += dataLen;
	BYTE* const inParamsData = (BYTE*)tcps_Alloc(inParamsDataLen);
	if(NULL == inParamsData)
		return TCPS_ERR_OUT_OF_MEMORY;

	BYTE* p = inParamsData;
	iscm_PeerCallFlags& peerCallFlags = *(iscm_PeerCallFlags*)p;
	peerCallFlags.sizeofBOOL_req = sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1; // ??
	peerCallFlags.dummy_15 = 0;
	p += sizeof(iscm_PeerCallFlags);
	*(INT32*)p = 11+(INT32)nameLen;
	p += sizeof(INT32);
	strncpy((char*)p, "GRID_User::", 11);
	p += 11;
	strncpy((char*)p, methodName, nameLen);
	p += nameLen;
	*(char*)p = 0;
	p += 1;
	memcpy(p, data, dataLen);
	p += dataLen;
	ASSERT(p-inParamsData == inParamsDataLen);

	BOOL requireReplyData = true;
	BOOL dataTransferred = false;
	iscm_RPCDataOutfiter outfiter;
	ASSERT(NULL==m_sessionR && m_sessionL);
	TCPSError err = NP_SCATTEREDSession::OnRPCCall_(NULL, this, requireReplyData, inParamsData, dataTransferred, inParamsDataLen, &outfiter);
	tcps_Free(inParamsData);
	if(TCPS_OK != err)
		return err;
	// outfiter.swbBufs_[0]总指向iscm_RPCDataOutfiter::reply_
	// outfiter.swbBufs_[1]为iscm_RPCReplyPrefix replyPrefix
	ASSERT(outfiter.swbBufs_.size()==1 || outfiter.swbBufs_.size()>=2);
	if(outfiter.swbBufs_.size() >= 2)
	{
		ASSERT(replyData && replyLen);
		*replyLen = SockTotalizeWriteBufVec(outfiter.swbBufs_.Get()+2, outfiter.swbBufs_.size()-2);
		*replyData = tcps_Alloc(*replyLen);
		BYTE* q = (BYTE*)*replyData;
		for(INT_PTR i=2; i<(INT_PTR)outfiter.swbBufs_.size(); ++i)
		{
			const SockWriteBuf& swb = outfiter.swbBufs_[i];
			memcpy(q, swb.data, swb.len);
			q += swb.len;
		}
		ASSERT(q-(BYTE*)*replyData == *replyLen);
	}
	return err;
}

TCPSError GRID_User_S::StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;
	if(NULL==callbackName || 0==nameLen)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if(nameLen < 0)
		nameLen = strlen(callbackName);

	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnOnStreamedCallback_L_;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("OnStreamedCallback_L_", -1, NULL, 0);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IGRID_User_LocalCallback::FN_OnStreamedCallback_L_)callbackFuncL)(
					m_sessionL,
					callbackName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}
	else if(m_sessionR)
	{
		iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
		if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
			return TCPS_ERR_CALLBACK_NOT_READY;
		DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;
		iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
		ASSERT(0 == m_dataOutfiter.bufs_.size());

		TCPSError errUpdate = UpdateCallbackMatchingFlag_();
		if(TCPS_OK != errUpdate)
			return errUpdate;
		CallbackMatchingFlag::CallbackMap::const_iterator itCallback = m_callbackMatchingFlag.cbmap_.find(callbackName, nameLen);
		if(m_callbackMatchingFlag.cbmap_.end() == itCallback)
			return TCPS_ERR_CALLBACK_NOT_MATCH;
		ASSERT(itCallback->second.pMatchingVar);
		if(!*(itCallback->second.pMatchingVar))
			return TCPS_ERR_CALLBACK_NOT_MATCH;

		DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

		// 填充基本类型数据到outfiter
		iscm_PeerCallFlags peerCallFlags;
		peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
		peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
		peerCallFlags.requireReply = !(itCallback->second.isPosting);
		peerCallFlags.dummy_15 = 0;
		m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

		INT32 call_id_len = (INT32)(11+nameLen);
		CSmartBuf<char, 256> call_id_buf(call_id_len+1);
		strncpy(call_id_buf.Get(), "GRID_User::", 11);
		StrAssign(call_id_buf.Get()+11, callbackName, nameLen);
		m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
		m_dataOutfiter.Push(call_id_buf.Get(), call_id_len+1);

		// 填充IN参数到outfiter
		if(dataLen > 0)
			m_dataOutfiter.Push(data, dataLen);

		// 调用PRCCall()
		if(peerCallFlags.requireReply) // callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			int replyBytes = 0;
			TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
			if(TCPS_OK != err)
				return err;

			iscm_RPCReplyPrefix replyPrefix;
			err = m_rpcRequester->RecvD(&replyPrefix, sizeof(replyPrefix));
			if(TCPS_OK != err)
			{
				this->OnNetworkMalformed_();
				return err;
			}

			INT_PTR leftReplyLen = replyBytes - sizeof(replyPrefix);
			ASSERT(leftReplyLen >= 0);
			if(leftReplyLen > 0)
			{
				ASSERT(replyData && replyLen);
				tcps_Binary replied_data;
				if(!replied_data.Resize(leftReplyLen))
				{
					this->OnNetworkMalformed_();
					return err;
				}
				err = m_rpcRequester->RecvD(replied_data.Get(), (int)leftReplyLen);
				if(TCPS_OK != err)
				{
					this->OnNetworkMalformed_();
					return err;
				}
				if(NULL==replyData || NULL==replyLen)
				{
					ASSERT(false);
					return TCPS_ERR_INVALID_PARAM;
				}
				INT32 bytes = 0;
				*replyData = replied_data.Drop(&bytes);
				*replyLen = bytes;
			}
		}
		else // posting callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			if(m_sessionR->m_udpSite.IsLinked())
			{
				int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
				BYTE* p = (BYTE*)tcps_Alloc(total);
				if(NULL == p)
				{
					ASSERT(false);
					return TCPS_ERR_OUT_OF_MEMORY;
				}
				BYTE* q = p;
				for(int i=0; i<reqBufVecCount; ++i)
				{
					const SockWriteBuf& swb = reqBufVec[i];
					memcpy(q, swb.data, swb.len);
					q += swb.len;
				}
				ASSERT((int)(q-p) == total);
				SockWriteBuf swb_udp;
				swb_udp.data = p;
				swb_udp.len = total;
				INT32 sessionID;
				m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.SendSessionData(sessionID, &swb_udp, 1);
			}
			else if(0 != m_sessionR->m_postingProxy.callerKey_)
			{
				INT_PTR queueFullIndexes = -1;
				INT_PTR queueFullCount = 0;
				TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
				if(TCPS_OK != err)
					return err;
				ASSERT(0==queueFullCount || 1==queueFullCount);
				if(1 == queueFullCount)
					return TCPS_ERR_POSTING_PENDING_FULL;
			}
			else
			{
				TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
				if(TCPS_OK != err)
					return err;
			}
		}
	}
	else
	{
		ASSERT(false);
		return TCPS_ERR_INTERNAL_BUG;
	}

	return TCPS_OK;
}

TCPSError GRID_User_S::ReplyCommitJob(
				IN INT64 jobKey_wrap,
				IN TCPSError replyCode_wrap,
				IN const void* context_wrap, IN INT32 context_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnReplyCommitJob;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("ReplyCommitJob", 14, s_GRID_User_ReplyCommitJob_TypeInfo_, s_GRID_User_ReplyCommitJob_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IGRID_User_LocalCallback::FN_ReplyCommitJob)callbackFuncL)(
					m_sessionL,
					jobKey_wrap,
					replyCode_wrap,
					tcps_Binary(xat_bind, (BYTE*)context_wrap, context_wrap_len)
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_ReplyCommitJob)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ReplyCommitJob", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT64 jobKey
	Put_BaseType_(&m_dataOutfiter, jobKey_wrap);

	// IN TCPSError replyCode
	Put_BaseType_(&m_dataOutfiter, replyCode_wrap);

	// IN tcps_Binary context
	if(context_wrap_len<0 || (context_wrap_len>0 && NULL==context_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, context_wrap, context_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_S::ReplyCommitJob_Batch(
				IN const PGRID_User_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN INT64 jobKey_wrap,
				IN TCPSError replyCode_wrap,
				IN const void* context_wrap, IN INT32 context_wrap_len,
				OUT PGRID_User_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PGRID_User_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
		{
			wrap_sessions[i]->ReplyCommitJob(
					jobKey_wrap,
					replyCode_wrap,
					context_wrap, context_wrap_len
					);
			continue;
		}
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_ReplyCommitJob)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'GRID_User::ReplyCommitJob()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 25;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "GRID_User::ReplyCommitJob";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN INT64 jobKey
	iscm_swb_.data = &jobKey_wrap;
	iscm_swb_.len = sizeof(jobKey_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN TCPSError replyCode
	iscm_swb_.data = &replyCode_wrap;
	iscm_swb_.len = sizeof(replyCode_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary context
	if(context_wrap_len<0 || (context_wrap_len>0 && NULL==context_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &context_wrap_len;
	iscm_swb_.len = sizeof(context_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(context_wrap_len > 0)
	{
		iscm_swb_.data = context_wrap;
		iscm_swb_.len = context_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError GRID_User_S::SetRedirect_(
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		// ASSERT(false); // ??
		return TCPS_ERR_REFUSED;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_SetRedirect_)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 23;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::SetRedirect_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN IPP redirectIPP
	Put_BaseType_(&m_dataOutfiter, redirectIPP_wrap);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, redirectParam_wrap, redirectParam_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_S::SetRedirect__Batch(
				IN const PGRID_User_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len,
				OUT PGRID_User_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PGRID_User_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
			continue;
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_SetRedirect_)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'GRID_User::SetRedirect_()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 23;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "GRID_User::SetRedirect_";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN IPP redirectIPP
	iscm_swb_.data = &redirectIPP_wrap;
	iscm_swb_.len = sizeof(redirectIPP_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &redirectParam_wrap_len;
	iscm_swb_.len = sizeof(redirectParam_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(redirectParam_wrap_len > 0)
	{
		iscm_swb_.data = redirectParam_wrap;
		iscm_swb_.len = redirectParam_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError GRID_User_S::CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks_wrap,
				IN const tcps_Array<tcps_String>& callbackTypeInfos_wrap,
				OUT tcps_Array<BOOL>& matchingFlags_wrap
				) callback
{
	if(m_sessionL)
	{
		InitializeAllCallsTypeInfo_();
		ASSERT(callbacks_wrap.Length() == callbackTypeInfos_wrap.Length());
		matchingFlags_wrap.Resize(callbacks_wrap.Length());
		for(int i=0; i<callbacks_wrap.Length(); ++i)
		{
			const tcps_String& name = callbacks_wrap[i];
			const tcps_String& typeInfo = callbackTypeInfos_wrap[i];
			matchingFlags_wrap[i] = (NULL != m_sessionL->FindCallback_(name.Get(), name.Length(), typeInfo.Get(), typeInfo.Length()));
		}
		return TCPS_OK;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::CallbackCheck_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> callbacks
	Put_BaseType_(&m_dataOutfiter, callbacks_wrap.LenRef());
	for(int idx1=0; idx1<callbacks_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbacks_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// IN tcps_Array<tcps_String> callbackTypeInfos
	Put_BaseType_(&m_dataOutfiter, callbackTypeInfos_wrap.LenRef());
	for(int idx1=0; idx1<callbackTypeInfos_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbackTypeInfos_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_IRequester* iscm_replied_picker = m_rpcRequester;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<BOOL> matchingFlags
		PICK_PODARRAY_(iscm_replied_picker, matchingFlags_wrap);
	}
	return TCPS_OK;
}

TCPSError GRID_User_S::UDPLink_(
			OUT INT32& servePort,
			OUT INT32& linkKey
			) method
{
	ASSERT(!m_sessionR->m_udpSite.IsOn());
	AutoSock udpSock;
	udpSock.sock_ = UDPNew();
	if(!SockValid(udpSock.sock_))
		return TCPS_ERR_SYSTEM_RESOURCE;

	INT32 port = 0;
	TCPSError err = tcps_AutoBindUDPPort(udpSock.sock_, m_sessionR->m_serveIPP.ip_, &port);
	if(TCPS_OK != err)
		return err;
	m_sessionR->m_udpSite.localPort_ = port;
	m_sessionR->m_udpSite.sock_ = udpSock.Drop();
	servePort = port;
	linkKey = m_sessionR->GetSessionKey();
	return TCPS_OK;
}

TCPSError GRID_User_S::UDPLinkConfirm_(
			) method
{
	if(!m_sessionR->m_udpSite.IsLinking())
	{
		ASSERT(false);
		return TCPS_ERR_CALL_WAS_IGNORED;
	}
	ASSERT(SockValid(m_sessionR->m_udpSite.sock_));

	DWORD recvTimeout = INFINITE;
	this->GetTimeout(NULL, &recvTimeout, NULL);
	if(INFINITE == recvTimeout)
		recvTimeout = iscm_GetDefaultRecvTimeout();
	recvTimeout = min(recvTimeout, (DWORD)2000);
	int r = SockCheckRead(m_sessionR->m_udpSite.sock_, recvTimeout);
	if(1 != r)
		return TCPS_ERR_RECV;
	IPP peerIPP;
	CSmartBuf<BYTE, 1024> smBuf(ISCM_MAX_UDP_FRAG_BYTES);
	BYTE* buf = smBuf.Get();
	r = SockReceiveFrom(m_sessionR->m_udpSite.sock_, &peerIPP.ip_, &peerIPP.port_, buf, ISCM_MAX_UDP_FRAG_BYTES);
	if((int)sizeof(iscm_UDPFrag) != r)
		return TCPS_ERR_RECV;
	if(!peerIPP.IsValid() || peerIPP.ip_!=m_sessionR->m_peerIPP.ip_)
		return TCPS_ERR_MALFORMED_REQ;

	const iscm_UDPFrag& frag = *(const iscm_UDPFrag*)buf;
	if(ISCM_UDP_FRAG_LINK != frag.fragType)
		return TCPS_ERR_MALFORMED_REQ;
	INT32 linkKey = m_sessionR->GetSessionKey();
	if(linkKey != frag.sendKey)
		return TCPS_ERR_INVALID_UDP_LINK_KEY;

	if(0 != UDPConnect(m_sessionR->m_udpSite.sock_, peerIPP.ip_, peerIPP.port_))
		return TCPS_ERR_SYSTEM;

	iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
	udpServer.AddSession(linkKey, m_sessionR->m_udpSite.sock_, m_sessionR);
	m_sessionR->m_udpSite.sock_ = INVALID_SOCKET;

	return TCPS_OK;
}

TCPSError GRID_User_S::SetTimeout_(
			IN INT32 recvTimeout,
			IN INT32 sendTimeout
			) posting_method
{
	this->SetTimeout(INFINITE, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError GRID_User_S::SetSessionBufferSize_(
			IN INT32 recvBufBytes,
			IN INT32 sendBufBytes
			) posting_method
{
	this->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	return TCPS_OK;
}

TCPSError GRID_User_S::MethodCheck_(
			IN const tcps_Array<tcps_String>& methods,
			IN const tcps_Array<tcps_String>& methodTypeInfos,
			OUT tcps_Array<BOOL>& matchingFlags
			) method
{
	if(0==methods.Length() || methods.Length()!=methodTypeInfos.Length())
		return TCPS_ERR_INVALID_PARAM;

	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<33> > MethodMap_;
	static MethodMap_* s_mdMap = NULL;
	if(NULL == s_mdMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_mdMap)
		{
			static MethodMap_ s_mdMapObj;
			MethodMap_& mdMap = s_mdMapObj;
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddJobProgram"), CPtrStrA(s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelJobProgram"), CPtrStrA(s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("FindJobProgram"), CPtrStrA(s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobPrograms"), CPtrStrA(s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CommitJob"), CPtrStrA(s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CancelJob"), CPtrStrA(s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetJobPriority"), CPtrStrA(s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobs"), CPtrStrA(s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("QueryJobs"), CPtrStrA(s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdateGrid"), CPtrStrA(s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), CPtrStrA(s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetLogCount"), CPtrStrA(s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("LoadLogInfos"), CPtrStrA(s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), CPtrStrA(s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), CPtrStrA(s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddUser"), CPtrStrA(s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelUser"), CPtrStrA(s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdatePassword"), CPtrStrA(s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ManageUser"), CPtrStrA(s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListAllUsers"), CPtrStrA(s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetUserInfo"), CPtrStrA(s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJTMs"), CPtrStrA(s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetJTMInfo"), CPtrStrA(s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridProperty"), CPtrStrA(s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddSplitter"), CPtrStrA(s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelSplitter"), CPtrStrA(s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitters"), CPtrStrA(s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitterParam"), CPtrStrA(s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridStatistics"), CPtrStrA(s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLink_"), CPtrStrA(s_GRID_User_UDPLink__TypeInfo_, s_GRID_User_UDPLink__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLinkConfirm_"), CPtrStrA(s_GRID_User_UDPLinkConfirm__TypeInfo_, s_GRID_User_UDPLinkConfirm__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetTimeout_"), CPtrStrA(s_GRID_User_SetTimeout__TypeInfo_, s_GRID_User_SetTimeout__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetSessionBufferSize_"), CPtrStrA(s_GRID_User_SetSessionBufferSize__TypeInfo_, s_GRID_User_SetSessionBufferSize__TypeInfo_len_))).second);
			s_mdMap = &mdMap;
		}
	}

	matchingFlags.Resize(methods.Length());
	for(int index=0; index<methods.Length(); ++index)
	{
		BOOL& flag = matchingFlags[index];
		const tcps_String& name = methods[index];
		const tcps_String& typeInfos = methodTypeInfos[index];
		MethodMap_::const_iterator cit = s_mdMap->find(name.Get(), name.Length());
		if(cit!=s_mdMap->end() && 0==typeInfos.Compare(cit->second.p, cit->second.len))
			flag = true;
		else
			flag = false;
	}
	return TCPS_OK;
}

void GRID_User_S::CloseSession(
				IN TCPSError cause /*= TCPS_OK*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->CloseSession_(cause);
	else if(m_sessionL)
	{
		AutoFlag<OSThreadID> autoClosingTID(m_closingTID_L.tid, OSThreadSelf());
		m_sessionL->CloseSession_();
	}
}

IPP GRID_User_S::GetPeerIPP(
				OUT IPP* peerID /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
	{
		if(peerID)
			*peerID = m_sessionR->m_peerID_IPP;
		return m_sessionR->m_peerIPP;
	}

	if(m_sessionL)
	{
		if(peerID)
			*peerID = IPP((DWORD)0, (int)0);
		return IPP((DWORD)0, (int)0);
	}

	if(peerID)
		*peerID = INVALID_IPP;
	return INVALID_IPP;
}

BOOL GRID_User_S::IsLocalPeer() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	return (NULL != m_sessionL);
}

void GRID_User_S::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->m_postingPendingParam.Set(maxPendingBytes, maxPendings);
}

void GRID_User_S::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxPendingBytes)
			*maxPendingBytes = 0;
		if(maxPendings)
			*maxPendings = 0;
		return;
	}
	m_sessionR->m_postingPendingParam.Get(maxPendingBytes, maxPendings);
}

BOOL GRID_User_S::CallbackIsReady() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		return (m_sessionR->m_callbackRequester.face_ && m_sessionR->m_callbackRequester.face_->IsConnected());
	return true;
}

TCPSError GRID_User_S::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return TCPS_OK;
	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError GRID_User_S::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(connectTimeout)
			*connectTimeout = INFINITE;
		if(recvTimeout)
			*recvTimeout = INFINITE;
		if(sendTimeout)
			*sendTimeout = INFINITE;
		return TCPS_OK;
	}

	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->GetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

void GRID_User_S::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return;

	if(recvBufBytes>=0 || sendBufBytes>=0)
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes >= 0)
			SockSetReceiveBufSize(sock, (0==recvBufBytes ? TCPS_DEFAULT_RECVBUF_SIZE : recvBufBytes));
		if(sendBufBytes >= 0)
			SockSetSendBufSize(sock, (0==sendBufBytes ? TCPS_DEFAULT_SENDBUF_SIZE : sendBufBytes));
	}
	if(m_sessionR->m_callbackRequester.face_)
		m_sessionR->m_callbackRequester.face_->SetSessionBufferSize(recvBufBytes, sendBufBytes);
}

void GRID_User_S::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(recvBufBytes)
			*recvBufBytes = 0;
		if(sendBufBytes)
			*sendBufBytes = 0;
		return;
	}

	if(NULL==recvBufBytes && NULL==sendBufBytes)
		return;
	if(m_sessionR->m_callbackRequester.face_)
	{
		m_sessionR->m_callbackRequester.face_->GetSessionBufferSize(recvBufBytes, sendBufBytes);
	}
	else
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes)
			SockGetReceiveBufSize(sock, recvBufBytes);
		if(sendBufBytes)
			SockGetSendBufSize(sock, sendBufBytes);
	}
}

void GRID_User_S::SetPostingSendParameters(
				IN INT32 maxBufferingSize,
				IN INT32 maxSendings
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR && 0!=m_sessionR->m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().SetBufferingParam(m_sessionR->m_postingProxy.callerKey_, maxBufferingSize, maxSendings);
	m_postingSendParam.Set(maxBufferingSize, maxSendings);
}

void GRID_User_S::GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxBufferingSize)
			*maxBufferingSize = 0;
		if(maxSendings)
			*maxSendings = 0;
		return;
	}
	m_postingSendParam.Get(maxBufferingSize, maxSendings);
}

TCPSError GRID_User_S::CleanPostingSendingQueue()
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(NULL==m_sessionR || 0==m_sessionR->m_postingProxy.callerKey_)
		return TCPS_ERR_CALL_WAS_IGNORED;
	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(&m_sessionR->m_postingProxy.callerKey_, 1);
}

TCPSError GRID_User_S::CleanPostingSendingQueue(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount
				)
{
	tcps_SmartArray<PGRID_User_S_, 256> sessions_ar_;
	for(INT_PTR i=0; i<sessionsCount; ++i)
	{
		if(NULL == sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(NULL==sessions[i]->m_sessionR || 0==sessions[i]->m_sessionR->m_postingProxy.callerKey_)
			continue;
		sessions_ar_.push_back(sessions[i]);
	}
	if(0 == sessions_ar_.size())
		return TCPS_OK;

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> callerKey_ar_;
	callerKey_ar_.resize(sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)sessions_ar_.size(); ++i)
		callerKey_ar_[i] = sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(callerKey_ar_.Get(), (int)callerKey_ar_.size());
}

class GRID_User_LS : public IGRID_User_LocalMethod
{
private:
	GRID_User_LS(const GRID_User_LS&);
	GRID_User_LS& operator= (const GRID_User_LS&);

private:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	IGRID_User_LocalCallback* const m_callback;
	GRID_User_S* const m_method;
	TCPSError m_connectError;
	INT32 m_sessionKey;

public:
	TCPSError GetConnectError() const
		{	return m_connectError;	}

public:
	GRID_User_LS(const IPP& clientID_IPP, NP_SCATTEREDSessionMaker& sessionMaker, IGRID_User_LocalCallback* callbackHandler)
		: m_sessionMaker(sessionMaker)
		, m_callback(callbackHandler)
		, m_method(tcps_NewEx(GRID_User_S, (m_sessionMaker, NULL, callbackHandler)))
		, m_connectError(TCPS_ERROR)
		, m_sessionKey(0)
	{
		INT32 sessionCount;
		m_sessionMaker.OnSessionConnect_(&m_sessionKey, sessionCount);
		m_connectError = m_method->OnConnected(m_sessionKey, clientID_IPP, sessionCount);
		if(TCPS_OK == m_connectError)
		{
			if(m_callback)
				m_method->OnCallbackReady();
			m_method->OnPostingCallReady();
			m_sessionMaker.RegisterLocalSession_(m_callback);
		}
	}
	virtual ~GRID_User_LS()
	{
		if(TCPS_OK == m_connectError)
			m_sessionMaker.UnregisterLocalSession_(m_callback);
		m_sessionMaker.Unregister(m_method);
		if(INVALID_OSTHREADID==m_method->m_closingTID_L.tid || m_method->m_closingTID_L.tid!=OSThreadSelf())
			m_method->OnPeerBroken(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_method->OnClose(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_sessionMaker.OnSessionClosed_();
		m_method->~GRID_User_S();
		tcps_Free(m_method);
	}
	virtual void DeleteThis()
		{	tcps_Delete(this);	}

	virtual PROC FindMethod_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				)
	{
		if(NULL == name)
		{
			ASSERT(false);
			return NULL;
		}

		// 对OnStreamedCall_L_()特殊处理
		if(nameLen<0 && 0==strcmp("OnStreamedCall_L_", name))
			return (PROC)&OnStreamedCall_L_;

		if(NULL == type)
		{
			ASSERT(false);
			return NULL;
		}

		InitializeAllCallsTypeInfo_();
		typedef TwoItems<CPtrStrA, PROC> FuncPair;
		typedef CQuickStringMap<CPtrStrA, FuncPair, QSS_Traits<29> > MethodMap_;
		static MethodMap_* s_mdMap = NULL;
		if(NULL == s_mdMap)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == s_mdMap)
			{
				static MethodMap_ s_mdMapObj;
				MethodMap_& mdMap = s_mdMapObj;
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddJobProgram"), FuncPair(CPtrStrA(s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_), (PROC)&AddJobProgram)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelJobProgram"), FuncPair(CPtrStrA(s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_), (PROC)&DelJobProgram)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("FindJobProgram"), FuncPair(CPtrStrA(s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_), (PROC)&FindJobProgram)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobPrograms"), FuncPair(CPtrStrA(s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_), (PROC)&ListJobPrograms)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CommitJob"), FuncPair(CPtrStrA(s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_), (PROC)&CommitJob)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CancelJob"), FuncPair(CPtrStrA(s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_), (PROC)&CancelJob)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetJobPriority"), FuncPair(CPtrStrA(s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_), (PROC)&SetJobPriority)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobs"), FuncPair(CPtrStrA(s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_), (PROC)&ListJobs)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("QueryJobs"), FuncPair(CPtrStrA(s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_), (PROC)&QueryJobs)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdateGrid"), FuncPair(CPtrStrA(s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_), (PROC)&UpdateGrid)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), FuncPair(CPtrStrA(s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_), (PROC)&ListModules)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetLogCount"), FuncPair(CPtrStrA(s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_), (PROC)&GetLogCount)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("LoadLogInfos"), FuncPair(CPtrStrA(s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_), (PROC)&LoadLogInfos)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), FuncPair(CPtrStrA(s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_), (PROC)&Login)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), FuncPair(CPtrStrA(s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_), (PROC)&Logout)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddUser"), FuncPair(CPtrStrA(s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_), (PROC)&AddUser)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelUser"), FuncPair(CPtrStrA(s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_), (PROC)&DelUser)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdatePassword"), FuncPair(CPtrStrA(s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_), (PROC)&UpdatePassword)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ManageUser"), FuncPair(CPtrStrA(s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_), (PROC)&ManageUser)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListAllUsers"), FuncPair(CPtrStrA(s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_), (PROC)&ListAllUsers)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetUserInfo"), FuncPair(CPtrStrA(s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_), (PROC)&GetUserInfo)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJTMs"), FuncPair(CPtrStrA(s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_), (PROC)&ListJTMs)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetJTMInfo"), FuncPair(CPtrStrA(s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_), (PROC)&GetJTMInfo)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridProperty"), FuncPair(CPtrStrA(s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_), (PROC)&GetGridProperty)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddSplitter"), FuncPair(CPtrStrA(s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_), (PROC)&AddSplitter)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelSplitter"), FuncPair(CPtrStrA(s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_), (PROC)&DelSplitter)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitters"), FuncPair(CPtrStrA(s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_), (PROC)&ListSplitters)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitterParam"), FuncPair(CPtrStrA(s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_), (PROC)&ListSplitterParam)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridStatistics"), FuncPair(CPtrStrA(s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_), (PROC)&GetGridStatistics)));
				s_mdMap = &mdMap;
			}
		}

		MethodMap_::iterator it = s_mdMap->find(name, nameLen);
		if(s_mdMap->end() == it)
			return NULL;
		const CPtrStrA& ps = it->second.first;
		if(0 != ps.Compare(type, typeLen))
			return NULL;
		return it->second.second;
	}

private:
	static TCPSError OnStreamedCall_L_(
				IN void* sessionObj,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
	{	return ((GRID_User_LS*)sessionObj)->m_method->OnStreamedCall_L_(
					methodName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}

private:
	static TCPSError AddJobProgram(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->AddJobProgram(
					programInfo,
					files
					);
	}

	static TCPSError DelJobProgram(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& programKeys
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->DelJobProgram(
					programKeys
					);
	}

	static TCPSError FindJobProgram(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->FindJobProgram(
					programID,
					found
					);
	}

	static TCPSError ListJobPrograms(
				IN void* sessionObj_wrap,
				OUT tcps_Array<JobProgram>& jobPrograms
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListJobPrograms(
					jobPrograms
					);
	}

	static TCPSError CommitJob(
				IN void* sessionObj_wrap,
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->CommitJob(
					jobKey,
					jobInfo
					);
	}

	static TCPSError CancelJob(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& jobKeys
				) posting_method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->CancelJob(
					jobKeys
					);
	}

	static TCPSError SetJobPriority(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->SetJobPriority(
					jobKey,
					priority
					);
	}

	static TCPSError ListJobs(
				IN void* sessionObj_wrap,
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListJobs(
					jobReports,
					jobState,
					beginTime,
					endTime
					);
	}

	static TCPSError QueryJobs(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->QueryJobs(
					jobKeys,
					jobReports
					);
	}

	static TCPSError UpdateGrid(
				IN void* sessionObj_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->UpdateGrid(
					files
					);
	}

	static TCPSError ListModules(
				IN void* sessionObj_wrap,
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListModules(
					modules
					);
	}

	static TCPSError GetLogCount(
				IN void* sessionObj_wrap,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->GetLogCount(
					beginTime,
					endTime,
					logCount
					);
	}

	static TCPSError LoadLogInfos(
				IN void* sessionObj_wrap,
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->LoadLogInfos(
					logInfos,
					beginTime,
					endTime,
					startPos,
					length
					);
	}

	static TCPSError Login(
				IN void* sessionObj_wrap,
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->Login(
					userName,
					password
					);
	}

	static TCPSError Logout(
				IN void* sessionObj_wrap
				) posting_method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->Logout(
					);
	}

	static TCPSError AddUser(
				IN void* sessionObj_wrap,
				IN const UserInfo& userInfo
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->AddUser(
					userInfo
					);
	}

	static TCPSError DelUser(
				IN void* sessionObj_wrap,
				IN const tcps_Array<tcps_String>& userNames
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->DelUser(
					userNames
					);
	}

	static TCPSError UpdatePassword(
				IN void* sessionObj_wrap,
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->UpdatePassword(
					oldPassword,
					newPassword
					);
	}

	static TCPSError ManageUser(
				IN void* sessionObj_wrap,
				IN const UserInfo& userInfo
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ManageUser(
					userInfo
					);
	}

	static TCPSError ListAllUsers(
				IN void* sessionObj_wrap,
				OUT tcps_Array<UserInfo>& userInfos
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListAllUsers(
					userInfos
					);
	}

	static TCPSError GetUserInfo(
				IN void* sessionObj_wrap,
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->GetUserInfo(
					userName,
					userInfo
					);
	}

	static TCPSError ListJTMs(
				IN void* sessionObj_wrap,
				OUT tcps_Array<tcps_String>& jtms
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListJTMs(
					jtms
					);
	}

	static TCPSError GetJTMInfo(
				IN void* sessionObj_wrap,
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->GetJTMInfo(
					jtm,
					jtmInfo
					);
	}

	static TCPSError GetGridProperty(
				IN void* sessionObj_wrap,
				OUT GridProperty& gridProperty
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->GetGridProperty(
					gridProperty
					);
	}

	static TCPSError AddSplitter(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->AddSplitter(
					splitter,
					files
					);
	}

	static TCPSError DelSplitter(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& splitter
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->DelSplitter(
					splitter
					);
	}

	static TCPSError ListSplitters(
				IN void* sessionObj_wrap,
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListSplitters(
					splitters
					);
	}

	static TCPSError ListSplitterParam(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->ListSplitterParam(
					splitter,
					splitterParams
					);
	}

	static TCPSError GetGridStatistics(
				IN void* sessionObj_wrap,
				OUT GridStatistics& stat
				) method
	{	return ((GRID_User_LS*)sessionObj_wrap)->m_method->GetGridStatistics(
					stat
					);
	}
};

TCPSError MakeLocalSession_GRID_User__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IGRID_User_LocalMethod*& methodHandler,
			IN IGRID_User_LocalCallback* callbackHandler
			)
{
	GRID_User_LS* session = tcps_NewEx(GRID_User_LS, (clientID_IPP, sessionMaker, callbackHandler));
	if(NULL == session)
		return TCPS_ERR_OUT_OF_MEMORY;
	TCPSError err = session->GetConnectError();
	if(TCPS_OK != err)
	{
		tcps_Delete(session);
		return err;
	}
	methodHandler = session;
	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_OnComputed(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Scatter",
					"OnComputed",
					ptrInParams,
					ptrInParamsLen,
					NULL,
					NULL
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 taskKey
	IN INT64 taskKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, taskKey_wrap);

	// IN TCPSError errorCode
	IN TCPSError errorCode_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, errorCode_wrap);

	// IN tcps_Binary context
	IN tcps_Binary context_wrap;
	GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, context_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::OnComputed() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->OnComputed(
			taskKey_wrap,
			errorCode_wrap,
			context_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::OnComputed(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::OnComputed(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_UDPLink_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::UDPLink_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT32 servePort;
		OUT INT32 linkKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->UDPLink_(
			opWrapper->servePort,
			opWrapper->linkKey
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::UDPLink_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::UDPLink_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT32 servePort
	OUT const INT32& servePort_wrap = opWrapper->servePort;
	Set_BaseType_(outfiter, servePort_wrap);

	// OUT INT32 linkKey
	OUT const INT32& linkKey_wrap = opWrapper->linkKey;
	Set_BaseType_(outfiter, linkKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_UDPLinkConfirm_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::UDPLinkConfirm_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->UDPLinkConfirm_(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::UDPLinkConfirm_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::UDPLinkConfirm_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_SetTimeout_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvTimeout
	IN INT32 recvTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvTimeout_wrap);

	// IN INT32 sendTimeout
	IN INT32 sendTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendTimeout_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::SetTimeout_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->SetTimeout_(
			recvTimeout_wrap,
			sendTimeout_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::SetTimeout_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::SetTimeout_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_SetSessionBufferSize_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvBufBytes
	IN INT32 recvBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvBufBytes_wrap);

	// IN INT32 sendBufBytes
	IN INT32 sendBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendBufBytes_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::SetSessionBufferSize_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->SetSessionBufferSize_(
			recvBufBytes_wrap,
			sendBufBytes_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::SetSessionBufferSize_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::SetSessionBufferSize_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Scatter_MethodCheck_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> methods
	IN tcps_Array<tcps_String> methods_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methods_wrap.Resize(array_len);
	for(int idx1=0; idx1<methods_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methods_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	// IN tcps_Array<tcps_String> methodTypeInfos
	IN tcps_Array<tcps_String> methodTypeInfos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methodTypeInfos_wrap.Resize(array_len);
	for(int idx1=0; idx1<methodTypeInfos_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methodTypeInfos_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Scatter_S::MethodCheck_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<BOOL> matchingFlags;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Scatter_S* pCC_ScatterObj_wrap;
		if(thisObj)
			pCC_ScatterObj_wrap = thisObj->m_pCC_Scatter;
		else
			pCC_ScatterObj_wrap = (PCC_Scatter_S*)faceObj;
		ASSERT(pCC_ScatterObj_wrap);
		errTcps = pCC_ScatterObj_wrap->MethodCheck_(
			methods_wrap,
			methodTypeInfos_wrap,
			opWrapper->matchingFlags
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Scatter_S::MethodCheck_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Scatter_S::MethodCheck_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<BOOL> matchingFlags
	OUT const tcps_Array<BOOL>& matchingFlags_wrap = opWrapper->matchingFlags;
	Set_PODArray_(outfiter, matchingFlags_wrap);

	return TCPS_OK;
}

PCC_Scatter_S::CallbackMatchingFlag::CallbackMatchingFlag()
{
	Reset();
	cbmap_.insert(std::make_pair(CPtrStrA("Compute", 7), Info(&matching_Compute, true)));
	cbmap_.insert(std::make_pair(CPtrStrA("AddMoudle", 9), Info(&matching_AddMoudle, false)));
	cbmap_.insert(std::make_pair(CPtrStrA("RemoveModule", 12), Info(&matching_RemoveModule, false)));
	cbmap_.insert(std::make_pair(CPtrStrA("ListModules", 11), Info(&matching_ListModules, false)));
	cbmap_.insert(std::make_pair(CPtrStrA("SetRedirect_", 12), Info(&matching_SetRedirect_, true)));
}

void PCC_Scatter_S::CallbackMatchingFlag::Reset()
{
	matching_Compute = false;
	matching_AddMoudle = false;
	matching_RemoveModule = false;
	matching_ListModules = false;
	matching_SetRedirect_ = false;
}

TCPSError PCC_Scatter_S::UpdateCallbackMatchingFlag_()
{
	if(!m_callbackMatchingUpdatedFlag.needUpdate)
		return TCPS_OK;
	InitializeAllCallsTypeInfo_();
	tcps_String callbacks_ar[5];
	IN tcps_Array<tcps_String> callbacks;
	callbacks.Attach(xat_bind, callbacks_ar, 5);
	tcps_String callbackTypeInfos_ar[5];
	IN tcps_Array<tcps_String> callbackTypeInfos;
	callbackTypeInfos.Attach(xat_bind, callbackTypeInfos_ar, 5);
	callbacks_ar[0].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("Compute"));
	callbackTypeInfos_ar[0].Attach(xat_bind, (char*)s_PCC_Scatter_Compute_TypeInfo_, s_PCC_Scatter_Compute_TypeInfo_len_);
	callbacks_ar[1].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("AddMoudle"));
	callbackTypeInfos_ar[1].Attach(xat_bind, (char*)s_PCC_Scatter_AddMoudle_TypeInfo_, s_PCC_Scatter_AddMoudle_TypeInfo_len_);
	callbacks_ar[2].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("RemoveModule"));
	callbackTypeInfos_ar[2].Attach(xat_bind, (char*)s_PCC_Scatter_RemoveModule_TypeInfo_, s_PCC_Scatter_RemoveModule_TypeInfo_len_);
	callbacks_ar[3].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListModules"));
	callbackTypeInfos_ar[3].Attach(xat_bind, (char*)s_PCC_Scatter_ListModules_TypeInfo_, s_PCC_Scatter_ListModules_TypeInfo_len_);
	callbacks_ar[4].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetRedirect_"));
	callbackTypeInfos_ar[4].Attach(xat_bind, (char*)s_PCC_Scatter_SetRedirect__TypeInfo_, s_PCC_Scatter_SetRedirect__TypeInfo_len_);
	OUT tcps_Array<BOOL> matchingFlags;
	TCPSError err = this->CallbackCheck_(callbacks, callbackTypeInfos, matchingFlags);
	if(TCPS_OK == err)
	{
		if(matchingFlags.Length() == callbacks.Length())
		{
			m_callbackMatchingUpdatedFlag.needUpdate = false;
			const BOOL* const matchingFlags_p = matchingFlags.Get();
			m_callbackMatchingFlag.matching_Compute = matchingFlags_p[0];
			m_callbackMatchingFlag.matching_AddMoudle = matchingFlags_p[1];
			m_callbackMatchingFlag.matching_RemoveModule = matchingFlags_p[2];
			m_callbackMatchingFlag.matching_ListModules = matchingFlags_p[3];
			m_callbackMatchingFlag.matching_SetRedirect_ = matchingFlags_p[4];
		}
		else
		{
			ASSERT(false);
			err = TCPS_ERR_MALFORMED_REPLY;
		}
	}
	if(m_callbackMatchingUpdatedFlag.needUpdate)
		m_callbackMatchingFlag.Reset();
	return err;
}

const PCC_Scatter_S::CallbackMatchingFlag& PCC_Scatter_S::GetCallbackMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	TCPSError ret = UpdateCallbackMatchingFlag_();
	if(err)
		*err = ret;
	return m_callbackMatchingFlag;
}

TCPSError PCC_Scatter_S::OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	INT_PTR inParamsDataLen = sizeof(iscm_PeerCallFlags);
	inParamsDataLen += 13; // "PCC_Scatter::"
	if(nameLen < 0)
		nameLen = strlen(methodName);
	inParamsDataLen += sizeof(INT32)+nameLen+1;
	inParamsDataLen += dataLen;
	BYTE* const inParamsData = (BYTE*)tcps_Alloc(inParamsDataLen);
	if(NULL == inParamsData)
		return TCPS_ERR_OUT_OF_MEMORY;

	BYTE* p = inParamsData;
	iscm_PeerCallFlags& peerCallFlags = *(iscm_PeerCallFlags*)p;
	peerCallFlags.sizeofBOOL_req = sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1; // ??
	peerCallFlags.dummy_15 = 0;
	p += sizeof(iscm_PeerCallFlags);
	*(INT32*)p = 13+(INT32)nameLen;
	p += sizeof(INT32);
	strncpy((char*)p, "PCC_Scatter::", 13);
	p += 13;
	strncpy((char*)p, methodName, nameLen);
	p += nameLen;
	*(char*)p = 0;
	p += 1;
	memcpy(p, data, dataLen);
	p += dataLen;
	ASSERT(p-inParamsData == inParamsDataLen);

	BOOL requireReplyData = true;
	BOOL dataTransferred = false;
	iscm_RPCDataOutfiter outfiter;
	ASSERT(NULL==m_sessionR && m_sessionL);
	TCPSError err = NP_SCATTEREDSession::OnRPCCall_(NULL, this, requireReplyData, inParamsData, dataTransferred, inParamsDataLen, &outfiter);
	tcps_Free(inParamsData);
	if(TCPS_OK != err)
		return err;
	// outfiter.swbBufs_[0]总指向iscm_RPCDataOutfiter::reply_
	// outfiter.swbBufs_[1]为iscm_RPCReplyPrefix replyPrefix
	ASSERT(outfiter.swbBufs_.size()==1 || outfiter.swbBufs_.size()>=2);
	if(outfiter.swbBufs_.size() >= 2)
	{
		ASSERT(replyData && replyLen);
		*replyLen = SockTotalizeWriteBufVec(outfiter.swbBufs_.Get()+2, outfiter.swbBufs_.size()-2);
		*replyData = tcps_Alloc(*replyLen);
		BYTE* q = (BYTE*)*replyData;
		for(INT_PTR i=2; i<(INT_PTR)outfiter.swbBufs_.size(); ++i)
		{
			const SockWriteBuf& swb = outfiter.swbBufs_[i];
			memcpy(q, swb.data, swb.len);
			q += swb.len;
		}
		ASSERT(q-(BYTE*)*replyData == *replyLen);
	}
	return err;
}

TCPSError PCC_Scatter_S::StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;
	if(NULL==callbackName || 0==nameLen)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if(nameLen < 0)
		nameLen = strlen(callbackName);

	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnOnStreamedCallback_L_;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("OnStreamedCallback_L_", -1, NULL, 0);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Scatter_LocalCallback::FN_OnStreamedCallback_L_)callbackFuncL)(
					m_sessionL,
					callbackName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}
	else if(m_sessionR)
	{
		iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
		if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
			return TCPS_ERR_CALLBACK_NOT_READY;
		DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;
		iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
		ASSERT(0 == m_dataOutfiter.bufs_.size());

		TCPSError errUpdate = UpdateCallbackMatchingFlag_();
		if(TCPS_OK != errUpdate)
			return errUpdate;
		CallbackMatchingFlag::CallbackMap::const_iterator itCallback = m_callbackMatchingFlag.cbmap_.find(callbackName, nameLen);
		if(m_callbackMatchingFlag.cbmap_.end() == itCallback)
			return TCPS_ERR_CALLBACK_NOT_MATCH;
		ASSERT(itCallback->second.pMatchingVar);
		if(!*(itCallback->second.pMatchingVar))
			return TCPS_ERR_CALLBACK_NOT_MATCH;

		DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

		// 填充基本类型数据到outfiter
		iscm_PeerCallFlags peerCallFlags;
		peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
		peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
		peerCallFlags.requireReply = !(itCallback->second.isPosting);
		peerCallFlags.dummy_15 = 0;
		m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

		INT32 call_id_len = (INT32)(13+nameLen);
		CSmartBuf<char, 256> call_id_buf(call_id_len+1);
		strncpy(call_id_buf.Get(), "PCC_Scatter::", 13);
		StrAssign(call_id_buf.Get()+13, callbackName, nameLen);
		m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
		m_dataOutfiter.Push(call_id_buf.Get(), call_id_len+1);

		// 填充IN参数到outfiter
		if(dataLen > 0)
			m_dataOutfiter.Push(data, dataLen);

		// 调用PRCCall()
		if(peerCallFlags.requireReply) // callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			int replyBytes = 0;
			TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
			if(TCPS_OK != err)
				return err;

			iscm_RPCReplyPrefix replyPrefix;
			err = m_rpcRequester->RecvD(&replyPrefix, sizeof(replyPrefix));
			if(TCPS_OK != err)
			{
				this->OnNetworkMalformed_();
				return err;
			}

			INT_PTR leftReplyLen = replyBytes - sizeof(replyPrefix);
			ASSERT(leftReplyLen >= 0);
			if(leftReplyLen > 0)
			{
				ASSERT(replyData && replyLen);
				tcps_Binary replied_data;
				if(!replied_data.Resize(leftReplyLen))
				{
					this->OnNetworkMalformed_();
					return err;
				}
				err = m_rpcRequester->RecvD(replied_data.Get(), (int)leftReplyLen);
				if(TCPS_OK != err)
				{
					this->OnNetworkMalformed_();
					return err;
				}
				if(NULL==replyData || NULL==replyLen)
				{
					ASSERT(false);
					return TCPS_ERR_INVALID_PARAM;
				}
				INT32 bytes = 0;
				*replyData = replied_data.Drop(&bytes);
				*replyLen = bytes;
			}
		}
		else // posting callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			if(m_sessionR->m_udpSite.IsLinked())
			{
				int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
				BYTE* p = (BYTE*)tcps_Alloc(total);
				if(NULL == p)
				{
					ASSERT(false);
					return TCPS_ERR_OUT_OF_MEMORY;
				}
				BYTE* q = p;
				for(int i=0; i<reqBufVecCount; ++i)
				{
					const SockWriteBuf& swb = reqBufVec[i];
					memcpy(q, swb.data, swb.len);
					q += swb.len;
				}
				ASSERT((int)(q-p) == total);
				SockWriteBuf swb_udp;
				swb_udp.data = p;
				swb_udp.len = total;
				INT32 sessionID;
				m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.SendSessionData(sessionID, &swb_udp, 1);
			}
			else if(0 != m_sessionR->m_postingProxy.callerKey_)
			{
				INT_PTR queueFullIndexes = -1;
				INT_PTR queueFullCount = 0;
				TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
				if(TCPS_OK != err)
					return err;
				ASSERT(0==queueFullCount || 1==queueFullCount);
				if(1 == queueFullCount)
					return TCPS_ERR_POSTING_PENDING_FULL;
			}
			else
			{
				TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
				if(TCPS_OK != err)
					return err;
			}
		}
	}
	else
	{
		ASSERT(false);
		return TCPS_ERR_INTERNAL_BUG;
	}

	return TCPS_OK;
}

TCPSError PCC_Scatter_S::Compute(
				IN INT64 moduleKey_wrap,
				IN INT64 taskKey_wrap,
				IN const char* inputUrl_wrap, IN INT32 inputUrl_wrap_len /*= -1*/,
				IN const char* outputUrl_wrap, IN INT32 outputUrl_wrap_len /*= -1*/,
				IN const void* moduleParams_wrap, IN INT32 moduleParams_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnCompute;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("Compute", 7, s_PCC_Scatter_Compute_TypeInfo_, s_PCC_Scatter_Compute_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Scatter_LocalCallback::FN_Compute)callbackFuncL)(
					m_sessionL,
					moduleKey_wrap,
					taskKey_wrap,
					tcps_String(xat_bind, (char*)inputUrl_wrap, inputUrl_wrap_len),
					tcps_String(xat_bind, (char*)outputUrl_wrap, outputUrl_wrap_len),
					tcps_Binary(xat_bind, (BYTE*)moduleParams_wrap, moduleParams_wrap_len)
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_Compute)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 20;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::Compute", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT64 moduleKey
	Put_BaseType_(&m_dataOutfiter, moduleKey_wrap);

	// IN INT64 taskKey
	Put_BaseType_(&m_dataOutfiter, taskKey_wrap);

	// IN tcps_String inputUrl
	if(NULL == inputUrl_wrap)
	{
		if(inputUrl_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		inputUrl_wrap = "";
		inputUrl_wrap_len = 0;
	}
	if(inputUrl_wrap_len < 0)
	{
		inputUrl_wrap_len = (INT32)strlen(inputUrl_wrap);
		Put_BaseType_(&m_dataOutfiter, inputUrl_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)inputUrl_wrap, inputUrl_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, inputUrl_wrap, inputUrl_wrap_len);
	}

	// IN tcps_String outputUrl
	if(NULL == outputUrl_wrap)
	{
		if(outputUrl_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		outputUrl_wrap = "";
		outputUrl_wrap_len = 0;
	}
	if(outputUrl_wrap_len < 0)
	{
		outputUrl_wrap_len = (INT32)strlen(outputUrl_wrap);
		Put_BaseType_(&m_dataOutfiter, outputUrl_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)outputUrl_wrap, outputUrl_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, outputUrl_wrap, outputUrl_wrap_len);
	}

	// IN tcps_Binary moduleParams
	if(moduleParams_wrap_len<0 || (moduleParams_wrap_len>0 && NULL==moduleParams_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, moduleParams_wrap, moduleParams_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::Compute_Batch(
				IN const PPCC_Scatter_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN INT64 moduleKey_wrap,
				IN INT64 taskKey_wrap,
				IN const char* inputUrl_wrap, IN INT32 inputUrl_wrap_len /*= -1*/,
				IN const char* outputUrl_wrap, IN INT32 outputUrl_wrap_len /*= -1*/,
				IN const void* moduleParams_wrap, IN INT32 moduleParams_wrap_len,
				OUT PPCC_Scatter_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PPCC_Scatter_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
		{
			wrap_sessions[i]->Compute(
					moduleKey_wrap,
					taskKey_wrap,
					inputUrl_wrap, inputUrl_wrap_len,
					outputUrl_wrap, outputUrl_wrap_len,
					moduleParams_wrap, moduleParams_wrap_len
					);
			continue;
		}
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_Compute)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'PCC_Scatter::Compute()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 20;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "PCC_Scatter::Compute";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN INT64 moduleKey
	iscm_swb_.data = &moduleKey_wrap;
	iscm_swb_.len = sizeof(moduleKey_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN INT64 taskKey
	iscm_swb_.data = &taskKey_wrap;
	iscm_swb_.len = sizeof(taskKey_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_String inputUrl
	if(NULL == inputUrl_wrap)
	{
		if(inputUrl_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		inputUrl_wrap = "";
		inputUrl_wrap_len = 0;
	}
	if(inputUrl_wrap_len < 0)
	{
		inputUrl_wrap_len = (INT32)strlen(inputUrl_wrap);
		iscm_swb_.data = &inputUrl_wrap_len;
		iscm_swb_.len = sizeof(inputUrl_wrap_len);
		iscm_swb_ar_.push_back(iscm_swb_);
		iscm_swb_.data = inputUrl_wrap;
		iscm_swb_.len = inputUrl_wrap_len+1;
		iscm_swb_ar_.push_back(iscm_swb_);
	}
	else
	{
		iscm_swb_.data = &inputUrl_wrap_len;
		iscm_swb_.len = sizeof(inputUrl_wrap_len);
		iscm_swb_ar_.push_back(iscm_swb_);
		if(inputUrl_wrap_len > 0)
		{
			iscm_swb_.data = inputUrl_wrap;
			iscm_swb_.len = inputUrl_wrap_len;
			iscm_swb_ar_.push_back(iscm_swb_);
		}
		iscm_swb_.data = "";
		iscm_swb_.len = 1;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// IN tcps_String outputUrl
	if(NULL == outputUrl_wrap)
	{
		if(outputUrl_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		outputUrl_wrap = "";
		outputUrl_wrap_len = 0;
	}
	if(outputUrl_wrap_len < 0)
	{
		outputUrl_wrap_len = (INT32)strlen(outputUrl_wrap);
		iscm_swb_.data = &outputUrl_wrap_len;
		iscm_swb_.len = sizeof(outputUrl_wrap_len);
		iscm_swb_ar_.push_back(iscm_swb_);
		iscm_swb_.data = outputUrl_wrap;
		iscm_swb_.len = outputUrl_wrap_len+1;
		iscm_swb_ar_.push_back(iscm_swb_);
	}
	else
	{
		iscm_swb_.data = &outputUrl_wrap_len;
		iscm_swb_.len = sizeof(outputUrl_wrap_len);
		iscm_swb_ar_.push_back(iscm_swb_);
		if(outputUrl_wrap_len > 0)
		{
			iscm_swb_.data = outputUrl_wrap;
			iscm_swb_.len = outputUrl_wrap_len;
			iscm_swb_ar_.push_back(iscm_swb_);
		}
		iscm_swb_.data = "";
		iscm_swb_.len = 1;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// IN tcps_Binary moduleParams
	if(moduleParams_wrap_len<0 || (moduleParams_wrap_len>0 && NULL==moduleParams_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &moduleParams_wrap_len;
	iscm_swb_.len = sizeof(moduleParams_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(moduleParams_wrap_len > 0)
	{
		iscm_swb_.data = moduleParams_wrap;
		iscm_swb_.len = moduleParams_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError PCC_Scatter_S::AddMoudle(
				IN const PCC_ModuleIndex& moduleIndex_wrap,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles_wrap
				) callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnAddMoudle;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("AddMoudle", 9, s_PCC_Scatter_AddMoudle_TypeInfo_, s_PCC_Scatter_AddMoudle_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Scatter_LocalCallback::FN_AddMoudle)callbackFuncL)(
					m_sessionL,
					moduleIndex_wrap,
					moudleFiles_wrap
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_AddMoudle)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::AddMoudle", call_id_len+1);

	// 填充IN参数到outfiter

	// IN PCC_ModuleIndex moduleIndex
		Put_BaseType_(&m_dataOutfiter, moduleIndex_wrap.moduleKey);
			Put_String_(&m_dataOutfiter, moduleIndex_wrap.moduleTag.name.Get(), moduleIndex_wrap.moduleTag.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, moduleIndex_wrap.moduleTag.version);

	// IN tcps_Array<PCC_ModuleFile> moudleFiles
	Put_BaseType_(&m_dataOutfiter, moudleFiles_wrap.LenRef());
	for(int idx1=0; idx1<moudleFiles_wrap.Length(); ++idx1)
	{
		const PCC_ModuleFile& ref1 = moudleFiles_wrap[idx1];
			Put_String_(&m_dataOutfiter, ref1.name.Get(), ref1.name.LenRef());
			Put_Binary_(&m_dataOutfiter, ref1.data.Get(), ref1.data.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			this->OnNetworkMalformed_();
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			NPLogError(("PCC_Scatter_S::AddMoudle() callback, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::RemoveModule(
				IN INT64 moduleKey_wrap
				) callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnRemoveModule;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("RemoveModule", 12, s_PCC_Scatter_RemoveModule_TypeInfo_, s_PCC_Scatter_RemoveModule_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Scatter_LocalCallback::FN_RemoveModule)callbackFuncL)(
					m_sessionL,
					moduleKey_wrap
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_RemoveModule)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::RemoveModule", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT64 moduleKey
	Put_BaseType_(&m_dataOutfiter, moduleKey_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			this->OnNetworkMalformed_();
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			NPLogError(("PCC_Scatter_S::RemoveModule() callback, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::ListModules(
				OUT tcps_Array<PCC_ModuleIndex>& modulesIndex_wrap
				) callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnListModules;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("ListModules", 11, s_PCC_Scatter_ListModules_TypeInfo_, s_PCC_Scatter_ListModules_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Scatter_LocalCallback::FN_ListModules)callbackFuncL)(
					m_sessionL,
					modulesIndex_wrap
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_ListModules)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 24;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::ListModules", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			this->OnNetworkMalformed_();
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<PCC_ModuleIndex> modulesIndex
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		modulesIndex_wrap.Resize(array_len);
		for(int idx2=0; idx2<modulesIndex_wrap.Length(); ++idx2)
		{
			PCC_ModuleIndex& ref2 = modulesIndex_wrap[idx2];
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.moduleKey);
					PICK_STRING_Q(iscm_replied_picker, ref2.moduleTag.name);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.moduleTag.version);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			NPLogError(("PCC_Scatter_S::ListModules() callback, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::SetRedirect_(
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		// ASSERT(false); // ??
		return TCPS_ERR_REFUSED;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_SetRedirect_)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::SetRedirect_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN IPP redirectIPP
	Put_BaseType_(&m_dataOutfiter, redirectIPP_wrap);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, redirectParam_wrap, redirectParam_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::SetRedirect__Batch(
				IN const PPCC_Scatter_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len,
				OUT PPCC_Scatter_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PPCC_Scatter_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
			continue;
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_SetRedirect_)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'PCC_Scatter::SetRedirect_()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 25;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "PCC_Scatter::SetRedirect_";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN IPP redirectIPP
	iscm_swb_.data = &redirectIPP_wrap;
	iscm_swb_.len = sizeof(redirectIPP_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &redirectParam_wrap_len;
	iscm_swb_.len = sizeof(redirectParam_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(redirectParam_wrap_len > 0)
	{
		iscm_swb_.data = redirectParam_wrap;
		iscm_swb_.len = redirectParam_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError PCC_Scatter_S::CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks_wrap,
				IN const tcps_Array<tcps_String>& callbackTypeInfos_wrap,
				OUT tcps_Array<BOOL>& matchingFlags_wrap
				) callback
{
	if(m_sessionL)
	{
		InitializeAllCallsTypeInfo_();
		ASSERT(callbacks_wrap.Length() == callbackTypeInfos_wrap.Length());
		matchingFlags_wrap.Resize(callbacks_wrap.Length());
		for(int i=0; i<callbacks_wrap.Length(); ++i)
		{
			const tcps_String& name = callbacks_wrap[i];
			const tcps_String& typeInfo = callbackTypeInfos_wrap[i];
			matchingFlags_wrap[i] = (NULL != m_sessionL->FindCallback_(name.Get(), name.Length(), typeInfo.Get(), typeInfo.Length()));
		}
		return TCPS_OK;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 27;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Scatter::CallbackCheck_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> callbacks
	Put_BaseType_(&m_dataOutfiter, callbacks_wrap.LenRef());
	for(int idx1=0; idx1<callbacks_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbacks_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// IN tcps_Array<tcps_String> callbackTypeInfos
	Put_BaseType_(&m_dataOutfiter, callbackTypeInfos_wrap.LenRef());
	for(int idx1=0; idx1<callbackTypeInfos_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbackTypeInfos_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_IRequester* iscm_replied_picker = m_rpcRequester;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<BOOL> matchingFlags
		PICK_PODARRAY_(iscm_replied_picker, matchingFlags_wrap);
	}
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::UDPLink_(
			OUT INT32& servePort,
			OUT INT32& linkKey
			) method
{
	ASSERT(!m_sessionR->m_udpSite.IsOn());
	AutoSock udpSock;
	udpSock.sock_ = UDPNew();
	if(!SockValid(udpSock.sock_))
		return TCPS_ERR_SYSTEM_RESOURCE;

	INT32 port = 0;
	TCPSError err = tcps_AutoBindUDPPort(udpSock.sock_, m_sessionR->m_serveIPP.ip_, &port);
	if(TCPS_OK != err)
		return err;
	m_sessionR->m_udpSite.localPort_ = port;
	m_sessionR->m_udpSite.sock_ = udpSock.Drop();
	servePort = port;
	linkKey = m_sessionR->GetSessionKey();
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::UDPLinkConfirm_(
			) method
{
	if(!m_sessionR->m_udpSite.IsLinking())
	{
		ASSERT(false);
		return TCPS_ERR_CALL_WAS_IGNORED;
	}
	ASSERT(SockValid(m_sessionR->m_udpSite.sock_));

	DWORD recvTimeout = INFINITE;
	this->GetTimeout(NULL, &recvTimeout, NULL);
	if(INFINITE == recvTimeout)
		recvTimeout = iscm_GetDefaultRecvTimeout();
	recvTimeout = min(recvTimeout, (DWORD)2000);
	int r = SockCheckRead(m_sessionR->m_udpSite.sock_, recvTimeout);
	if(1 != r)
		return TCPS_ERR_RECV;
	IPP peerIPP;
	CSmartBuf<BYTE, 1024> smBuf(ISCM_MAX_UDP_FRAG_BYTES);
	BYTE* buf = smBuf.Get();
	r = SockReceiveFrom(m_sessionR->m_udpSite.sock_, &peerIPP.ip_, &peerIPP.port_, buf, ISCM_MAX_UDP_FRAG_BYTES);
	if((int)sizeof(iscm_UDPFrag) != r)
		return TCPS_ERR_RECV;
	if(!peerIPP.IsValid() || peerIPP.ip_!=m_sessionR->m_peerIPP.ip_)
		return TCPS_ERR_MALFORMED_REQ;

	const iscm_UDPFrag& frag = *(const iscm_UDPFrag*)buf;
	if(ISCM_UDP_FRAG_LINK != frag.fragType)
		return TCPS_ERR_MALFORMED_REQ;
	INT32 linkKey = m_sessionR->GetSessionKey();
	if(linkKey != frag.sendKey)
		return TCPS_ERR_INVALID_UDP_LINK_KEY;

	if(0 != UDPConnect(m_sessionR->m_udpSite.sock_, peerIPP.ip_, peerIPP.port_))
		return TCPS_ERR_SYSTEM;

	iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
	udpServer.AddSession(linkKey, m_sessionR->m_udpSite.sock_, m_sessionR);
	m_sessionR->m_udpSite.sock_ = INVALID_SOCKET;

	return TCPS_OK;
}

TCPSError PCC_Scatter_S::SetTimeout_(
			IN INT32 recvTimeout,
			IN INT32 sendTimeout
			) posting_method
{
	this->SetTimeout(INFINITE, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::SetSessionBufferSize_(
			IN INT32 recvBufBytes,
			IN INT32 sendBufBytes
			) posting_method
{
	this->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::MethodCheck_(
			IN const tcps_Array<tcps_String>& methods,
			IN const tcps_Array<tcps_String>& methodTypeInfos,
			OUT tcps_Array<BOOL>& matchingFlags
			) method
{
	if(0==methods.Length() || methods.Length()!=methodTypeInfos.Length())
		return TCPS_ERR_INVALID_PARAM;

	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<5> > MethodMap_;
	static MethodMap_* s_mdMap = NULL;
	if(NULL == s_mdMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_mdMap)
		{
			static MethodMap_ s_mdMapObj;
			MethodMap_& mdMap = s_mdMapObj;
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("OnComputed"), CPtrStrA(s_PCC_Scatter_OnComputed_TypeInfo_, s_PCC_Scatter_OnComputed_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLink_"), CPtrStrA(s_PCC_Scatter_UDPLink__TypeInfo_, s_PCC_Scatter_UDPLink__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLinkConfirm_"), CPtrStrA(s_PCC_Scatter_UDPLinkConfirm__TypeInfo_, s_PCC_Scatter_UDPLinkConfirm__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetTimeout_"), CPtrStrA(s_PCC_Scatter_SetTimeout__TypeInfo_, s_PCC_Scatter_SetTimeout__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetSessionBufferSize_"), CPtrStrA(s_PCC_Scatter_SetSessionBufferSize__TypeInfo_, s_PCC_Scatter_SetSessionBufferSize__TypeInfo_len_))).second);
			s_mdMap = &mdMap;
		}
	}

	matchingFlags.Resize(methods.Length());
	for(int index=0; index<methods.Length(); ++index)
	{
		BOOL& flag = matchingFlags[index];
		const tcps_String& name = methods[index];
		const tcps_String& typeInfos = methodTypeInfos[index];
		MethodMap_::const_iterator cit = s_mdMap->find(name.Get(), name.Length());
		if(cit!=s_mdMap->end() && 0==typeInfos.Compare(cit->second.p, cit->second.len))
			flag = true;
		else
			flag = false;
	}
	return TCPS_OK;
}

void PCC_Scatter_S::CloseSession(
				IN TCPSError cause /*= TCPS_OK*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->CloseSession_(cause);
	else if(m_sessionL)
	{
		AutoFlag<OSThreadID> autoClosingTID(m_closingTID_L.tid, OSThreadSelf());
		m_sessionL->CloseSession_();
	}
}

IPP PCC_Scatter_S::GetPeerIPP(
				OUT IPP* peerID /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
	{
		if(peerID)
			*peerID = m_sessionR->m_peerID_IPP;
		return m_sessionR->m_peerIPP;
	}

	if(m_sessionL)
	{
		if(peerID)
			*peerID = IPP((DWORD)0, (int)0);
		return IPP((DWORD)0, (int)0);
	}

	if(peerID)
		*peerID = INVALID_IPP;
	return INVALID_IPP;
}

BOOL PCC_Scatter_S::IsLocalPeer() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	return (NULL != m_sessionL);
}

void PCC_Scatter_S::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->m_postingPendingParam.Set(maxPendingBytes, maxPendings);
}

void PCC_Scatter_S::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxPendingBytes)
			*maxPendingBytes = 0;
		if(maxPendings)
			*maxPendings = 0;
		return;
	}
	m_sessionR->m_postingPendingParam.Get(maxPendingBytes, maxPendings);
}

BOOL PCC_Scatter_S::CallbackIsReady() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		return (m_sessionR->m_callbackRequester.face_ && m_sessionR->m_callbackRequester.face_->IsConnected());
	return true;
}

TCPSError PCC_Scatter_S::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return TCPS_OK;
	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Scatter_S::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(connectTimeout)
			*connectTimeout = INFINITE;
		if(recvTimeout)
			*recvTimeout = INFINITE;
		if(sendTimeout)
			*sendTimeout = INFINITE;
		return TCPS_OK;
	}

	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->GetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

void PCC_Scatter_S::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return;

	if(recvBufBytes>=0 || sendBufBytes>=0)
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes >= 0)
			SockSetReceiveBufSize(sock, (0==recvBufBytes ? TCPS_DEFAULT_RECVBUF_SIZE : recvBufBytes));
		if(sendBufBytes >= 0)
			SockSetSendBufSize(sock, (0==sendBufBytes ? TCPS_DEFAULT_SENDBUF_SIZE : sendBufBytes));
	}
	if(m_sessionR->m_callbackRequester.face_)
		m_sessionR->m_callbackRequester.face_->SetSessionBufferSize(recvBufBytes, sendBufBytes);
}

void PCC_Scatter_S::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(recvBufBytes)
			*recvBufBytes = 0;
		if(sendBufBytes)
			*sendBufBytes = 0;
		return;
	}

	if(NULL==recvBufBytes && NULL==sendBufBytes)
		return;
	if(m_sessionR->m_callbackRequester.face_)
	{
		m_sessionR->m_callbackRequester.face_->GetSessionBufferSize(recvBufBytes, sendBufBytes);
	}
	else
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes)
			SockGetReceiveBufSize(sock, recvBufBytes);
		if(sendBufBytes)
			SockGetSendBufSize(sock, sendBufBytes);
	}
}

void PCC_Scatter_S::SetPostingSendParameters(
				IN INT32 maxBufferingSize,
				IN INT32 maxSendings
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR && 0!=m_sessionR->m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().SetBufferingParam(m_sessionR->m_postingProxy.callerKey_, maxBufferingSize, maxSendings);
	m_postingSendParam.Set(maxBufferingSize, maxSendings);
}

void PCC_Scatter_S::GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxBufferingSize)
			*maxBufferingSize = 0;
		if(maxSendings)
			*maxSendings = 0;
		return;
	}
	m_postingSendParam.Get(maxBufferingSize, maxSendings);
}

TCPSError PCC_Scatter_S::CleanPostingSendingQueue()
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(NULL==m_sessionR || 0==m_sessionR->m_postingProxy.callerKey_)
		return TCPS_ERR_CALL_WAS_IGNORED;
	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(&m_sessionR->m_postingProxy.callerKey_, 1);
}

TCPSError PCC_Scatter_S::CleanPostingSendingQueue(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount
				)
{
	tcps_SmartArray<PPCC_Scatter_S_, 256> sessions_ar_;
	for(INT_PTR i=0; i<sessionsCount; ++i)
	{
		if(NULL == sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(NULL==sessions[i]->m_sessionR || 0==sessions[i]->m_sessionR->m_postingProxy.callerKey_)
			continue;
		sessions_ar_.push_back(sessions[i]);
	}
	if(0 == sessions_ar_.size())
		return TCPS_OK;

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> callerKey_ar_;
	callerKey_ar_.resize(sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)sessions_ar_.size(); ++i)
		callerKey_ar_[i] = sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(callerKey_ar_.Get(), (int)callerKey_ar_.size());
}

class PCC_Scatter_LS : public IPCC_Scatter_LocalMethod
{
private:
	PCC_Scatter_LS(const PCC_Scatter_LS&);
	PCC_Scatter_LS& operator= (const PCC_Scatter_LS&);

private:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	IPCC_Scatter_LocalCallback* const m_callback;
	PCC_Scatter_S* const m_method;
	TCPSError m_connectError;
	INT32 m_sessionKey;

public:
	TCPSError GetConnectError() const
		{	return m_connectError;	}

public:
	PCC_Scatter_LS(const IPP& clientID_IPP, NP_SCATTEREDSessionMaker& sessionMaker, IPCC_Scatter_LocalCallback* callbackHandler)
		: m_sessionMaker(sessionMaker)
		, m_callback(callbackHandler)
		, m_method(tcps_NewEx(PCC_Scatter_S, (m_sessionMaker, NULL, callbackHandler)))
		, m_connectError(TCPS_ERROR)
		, m_sessionKey(0)
	{
		INT32 sessionCount;
		m_sessionMaker.OnSessionConnect_(&m_sessionKey, sessionCount);
		m_connectError = m_method->OnConnected(m_sessionKey, clientID_IPP, sessionCount);
		if(TCPS_OK == m_connectError)
		{
			if(m_callback)
				m_method->OnCallbackReady();
			m_method->OnPostingCallReady();
			m_sessionMaker.RegisterLocalSession_(m_callback);
		}
	}
	virtual ~PCC_Scatter_LS()
	{
		if(TCPS_OK == m_connectError)
			m_sessionMaker.UnregisterLocalSession_(m_callback);
		m_sessionMaker.Unregister(m_method);
		if(INVALID_OSTHREADID==m_method->m_closingTID_L.tid || m_method->m_closingTID_L.tid!=OSThreadSelf())
			m_method->OnPeerBroken(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_method->OnClose(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_sessionMaker.OnSessionClosed_();
		m_method->~PCC_Scatter_S();
		tcps_Free(m_method);
	}
	virtual void DeleteThis()
		{	tcps_Delete(this);	}

	virtual PROC FindMethod_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				)
	{
		if(NULL == name)
		{
			ASSERT(false);
			return NULL;
		}

		// 对OnStreamedCall_L_()特殊处理
		if(nameLen<0 && 0==strcmp("OnStreamedCall_L_", name))
			return (PROC)&OnStreamedCall_L_;

		if(NULL == type)
		{
			ASSERT(false);
			return NULL;
		}

		InitializeAllCallsTypeInfo_();
		typedef TwoItems<CPtrStrA, PROC> FuncPair;
		typedef CQuickStringMap<CPtrStrA, FuncPair, QSS_Traits<1> > MethodMap_;
		static MethodMap_* s_mdMap = NULL;
		if(NULL == s_mdMap)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == s_mdMap)
			{
				static MethodMap_ s_mdMapObj;
				MethodMap_& mdMap = s_mdMapObj;
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("OnComputed"), FuncPair(CPtrStrA(s_PCC_Scatter_OnComputed_TypeInfo_, s_PCC_Scatter_OnComputed_TypeInfo_len_), (PROC)&OnComputed)));
				s_mdMap = &mdMap;
			}
		}

		MethodMap_::iterator it = s_mdMap->find(name, nameLen);
		if(s_mdMap->end() == it)
			return NULL;
		const CPtrStrA& ps = it->second.first;
		if(0 != ps.Compare(type, typeLen))
			return NULL;
		return it->second.second;
	}

private:
	static TCPSError OnStreamedCall_L_(
				IN void* sessionObj,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
	{	return ((PCC_Scatter_LS*)sessionObj)->m_method->OnStreamedCall_L_(
					methodName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}

private:
	static TCPSError OnComputed(
				IN void* sessionObj_wrap,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_method
	{	return ((PCC_Scatter_LS*)sessionObj_wrap)->m_method->OnComputed(
					taskKey,
					errorCode,
					context
					);
	}
};

TCPSError MakeLocalSession_PCC_Scatter__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Scatter_LocalMethod*& methodHandler,
			IN IPCC_Scatter_LocalCallback* callbackHandler
			)
{
	PCC_Scatter_LS* session = tcps_NewEx(PCC_Scatter_LS, (clientID_IPP, sessionMaker, callbackHandler));
	if(NULL == session)
		return TCPS_ERR_OUT_OF_MEMORY;
	TCPSError err = session->GetConnectError();
	if(TCPS_OK != err)
	{
		tcps_Delete(session);
		return err;
	}
	methodHandler = session;
	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_Login(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"Login",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String ticket
	IN tcps_String ticket_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ticket_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::Login() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->Login(
			ticket_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::Login(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::Login(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_Logout(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"Logout",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::Logout() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->Logout(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::Logout(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::Logout(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_ListModules(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"ListModules",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 moduleType
	IN INT32 moduleType_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleType_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::ListModules() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<PCC_ModuleInfo> modulesInfo;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->ListModules(
			moduleType_wrap,
			opWrapper->modulesInfo
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::ListModules(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::ListModules(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<PCC_ModuleInfo> modulesInfo
	OUT const tcps_Array<PCC_ModuleInfo>& modulesInfo_wrap = opWrapper->modulesInfo;
	Set_BaseType_(outfiter, modulesInfo_wrap.LenRef());
	for(int idx1=0; idx1<modulesInfo_wrap.Length(); ++idx1)
	{
		const PCC_ModuleInfo& ref1 = modulesInfo_wrap[idx1];
			Set_BaseType_(outfiter, ref1.moduleKey);
				Set_String_(outfiter, ref1.moduleTag.name);
				Set_BaseType_(outfiter, ref1.moduleTag.version);
			Set_BaseType_(outfiter, ref1.modulePattern);
			Set_BaseType_(outfiter, ref1.moduleType);
			Set_String_(outfiter, ref1.description);
			Set_BaseType_(outfiter, ref1.moduleFileType);
			Set_BaseType_(outfiter, ref1.moduleLatency);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_GetModuleFile(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"GetModuleFile",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 moduleKey
	IN INT64 moduleKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleKey_wrap);

	// IN INT32 fileType
	IN INT32 fileType_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, fileType_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::GetModuleFile() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<PCC_ModuleFile> moduleFiles;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->GetModuleFile(
			moduleKey_wrap,
			fileType_wrap,
			opWrapper->moduleFiles
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::GetModuleFile(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::GetModuleFile(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<PCC_ModuleFile> moduleFiles
	OUT const tcps_Array<PCC_ModuleFile>& moduleFiles_wrap = opWrapper->moduleFiles;
	Set_BaseType_(outfiter, moduleFiles_wrap.LenRef());
	for(int idx1=0; idx1<moduleFiles_wrap.Length(); ++idx1)
	{
		const PCC_ModuleFile& ref1 = moduleFiles_wrap[idx1];
			Set_String_(outfiter, ref1.name);
			Set_Binary_(outfiter, ref1.data);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_Execute(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"Execute",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 moduleKey
	IN INT64 moduleKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleKey_wrap);

	// IN tcps_String inputUrl
	IN tcps_String inputUrl_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, inputUrl_wrap);

	// IN tcps_String outputUrl
	IN tcps_String outputUrl_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, outputUrl_wrap);

	// IN tcps_Binary moduleParams
	IN tcps_Binary moduleParams_wrap;
	GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleParams_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::Execute() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT64 jobKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->Execute(
			moduleKey_wrap,
			inputUrl_wrap,
			outputUrl_wrap,
			moduleParams_wrap,
			opWrapper->jobKey
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::Execute(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::Execute(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT64 jobKey
	OUT const INT64& jobKey_wrap = opWrapper->jobKey;
	Set_BaseType_(outfiter, jobKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_QueryJobs(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"QueryJobs",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<INT64> jobsKey
	IN tcps_Array<INT64> jobsKey_wrap;
	GET_PODARRAY_EX_(thisObj, ptrInParams, ptrInParamsLen, jobsKey_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::QueryJobs() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<PCC_Service_T::ExecuteState> jobsState;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->QueryJobs(
			jobsKey_wrap,
			opWrapper->jobsState
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::QueryJobs(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::QueryJobs(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<ExecuteState> jobsState
	OUT const tcps_Array<PCC_Service_T::ExecuteState>& jobsState_wrap = opWrapper->jobsState;
	Set_PODArray_(outfiter, jobsState_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_CancelJob(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Service",
					"CancelJob",
					ptrInParams,
					ptrInParamsLen,
					NULL,
					NULL
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 jobKey
	IN INT64 jobKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobKey_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::CancelJob() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->CancelJob(
			jobKey_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::CancelJob(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::CancelJob(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_UDPLink_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::UDPLink_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT32 servePort;
		OUT INT32 linkKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->UDPLink_(
			opWrapper->servePort,
			opWrapper->linkKey
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::UDPLink_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::UDPLink_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT32 servePort
	OUT const INT32& servePort_wrap = opWrapper->servePort;
	Set_BaseType_(outfiter, servePort_wrap);

	// OUT INT32 linkKey
	OUT const INT32& linkKey_wrap = opWrapper->linkKey;
	Set_BaseType_(outfiter, linkKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_UDPLinkConfirm_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::UDPLinkConfirm_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->UDPLinkConfirm_(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::UDPLinkConfirm_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::UDPLinkConfirm_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_SetTimeout_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvTimeout
	IN INT32 recvTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvTimeout_wrap);

	// IN INT32 sendTimeout
	IN INT32 sendTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendTimeout_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::SetTimeout_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->SetTimeout_(
			recvTimeout_wrap,
			sendTimeout_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::SetTimeout_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::SetTimeout_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_SetSessionBufferSize_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvBufBytes
	IN INT32 recvBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvBufBytes_wrap);

	// IN INT32 sendBufBytes
	IN INT32 sendBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendBufBytes_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::SetSessionBufferSize_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->SetSessionBufferSize_(
			recvBufBytes_wrap,
			sendBufBytes_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::SetSessionBufferSize_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::SetSessionBufferSize_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Service_MethodCheck_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> methods
	IN tcps_Array<tcps_String> methods_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methods_wrap.Resize(array_len);
	for(int idx1=0; idx1<methods_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methods_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	// IN tcps_Array<tcps_String> methodTypeInfos
	IN tcps_Array<tcps_String> methodTypeInfos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methodTypeInfos_wrap.Resize(array_len);
	for(int idx1=0; idx1<methodTypeInfos_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methodTypeInfos_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Service_S::MethodCheck_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<BOOL> matchingFlags;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Service_S* pCC_ServiceObj_wrap;
		if(thisObj)
			pCC_ServiceObj_wrap = thisObj->m_pCC_Service;
		else
			pCC_ServiceObj_wrap = (PCC_Service_S*)faceObj;
		ASSERT(pCC_ServiceObj_wrap);
		errTcps = pCC_ServiceObj_wrap->MethodCheck_(
			methods_wrap,
			methodTypeInfos_wrap,
			opWrapper->matchingFlags
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Service_S::MethodCheck_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Service_S::MethodCheck_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<BOOL> matchingFlags
	OUT const tcps_Array<BOOL>& matchingFlags_wrap = opWrapper->matchingFlags;
	Set_PODArray_(outfiter, matchingFlags_wrap);

	return TCPS_OK;
}

PCC_Service_S::CallbackMatchingFlag::CallbackMatchingFlag()
{
	Reset();
	cbmap_.insert(std::make_pair(CPtrStrA("OnExecuted", 10), Info(&matching_OnExecuted, true)));
	cbmap_.insert(std::make_pair(CPtrStrA("SetRedirect_", 12), Info(&matching_SetRedirect_, true)));
}

void PCC_Service_S::CallbackMatchingFlag::Reset()
{
	matching_OnExecuted = false;
	matching_SetRedirect_ = false;
}

TCPSError PCC_Service_S::UpdateCallbackMatchingFlag_()
{
	if(!m_callbackMatchingUpdatedFlag.needUpdate)
		return TCPS_OK;
	InitializeAllCallsTypeInfo_();
	tcps_String callbacks_ar[2];
	IN tcps_Array<tcps_String> callbacks;
	callbacks.Attach(xat_bind, callbacks_ar, 2);
	tcps_String callbackTypeInfos_ar[2];
	IN tcps_Array<tcps_String> callbackTypeInfos;
	callbackTypeInfos.Attach(xat_bind, callbackTypeInfos_ar, 2);
	callbacks_ar[0].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("OnExecuted"));
	callbackTypeInfos_ar[0].Attach(xat_bind, (char*)s_PCC_Service_OnExecuted_TypeInfo_, s_PCC_Service_OnExecuted_TypeInfo_len_);
	callbacks_ar[1].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetRedirect_"));
	callbackTypeInfos_ar[1].Attach(xat_bind, (char*)s_PCC_Service_SetRedirect__TypeInfo_, s_PCC_Service_SetRedirect__TypeInfo_len_);
	OUT tcps_Array<BOOL> matchingFlags;
	TCPSError err = this->CallbackCheck_(callbacks, callbackTypeInfos, matchingFlags);
	if(TCPS_OK == err)
	{
		if(matchingFlags.Length() == callbacks.Length())
		{
			m_callbackMatchingUpdatedFlag.needUpdate = false;
			const BOOL* const matchingFlags_p = matchingFlags.Get();
			m_callbackMatchingFlag.matching_OnExecuted = matchingFlags_p[0];
			m_callbackMatchingFlag.matching_SetRedirect_ = matchingFlags_p[1];
		}
		else
		{
			ASSERT(false);
			err = TCPS_ERR_MALFORMED_REPLY;
		}
	}
	if(m_callbackMatchingUpdatedFlag.needUpdate)
		m_callbackMatchingFlag.Reset();
	return err;
}

const PCC_Service_S::CallbackMatchingFlag& PCC_Service_S::GetCallbackMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	TCPSError ret = UpdateCallbackMatchingFlag_();
	if(err)
		*err = ret;
	return m_callbackMatchingFlag;
}

TCPSError PCC_Service_S::OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	INT_PTR inParamsDataLen = sizeof(iscm_PeerCallFlags);
	inParamsDataLen += 13; // "PCC_Service::"
	if(nameLen < 0)
		nameLen = strlen(methodName);
	inParamsDataLen += sizeof(INT32)+nameLen+1;
	inParamsDataLen += dataLen;
	BYTE* const inParamsData = (BYTE*)tcps_Alloc(inParamsDataLen);
	if(NULL == inParamsData)
		return TCPS_ERR_OUT_OF_MEMORY;

	BYTE* p = inParamsData;
	iscm_PeerCallFlags& peerCallFlags = *(iscm_PeerCallFlags*)p;
	peerCallFlags.sizeofBOOL_req = sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1; // ??
	peerCallFlags.dummy_15 = 0;
	p += sizeof(iscm_PeerCallFlags);
	*(INT32*)p = 13+(INT32)nameLen;
	p += sizeof(INT32);
	strncpy((char*)p, "PCC_Service::", 13);
	p += 13;
	strncpy((char*)p, methodName, nameLen);
	p += nameLen;
	*(char*)p = 0;
	p += 1;
	memcpy(p, data, dataLen);
	p += dataLen;
	ASSERT(p-inParamsData == inParamsDataLen);

	BOOL requireReplyData = true;
	BOOL dataTransferred = false;
	iscm_RPCDataOutfiter outfiter;
	ASSERT(NULL==m_sessionR && m_sessionL);
	TCPSError err = NP_SCATTEREDSession::OnRPCCall_(NULL, this, requireReplyData, inParamsData, dataTransferred, inParamsDataLen, &outfiter);
	tcps_Free(inParamsData);
	if(TCPS_OK != err)
		return err;
	// outfiter.swbBufs_[0]总指向iscm_RPCDataOutfiter::reply_
	// outfiter.swbBufs_[1]为iscm_RPCReplyPrefix replyPrefix
	ASSERT(outfiter.swbBufs_.size()==1 || outfiter.swbBufs_.size()>=2);
	if(outfiter.swbBufs_.size() >= 2)
	{
		ASSERT(replyData && replyLen);
		*replyLen = SockTotalizeWriteBufVec(outfiter.swbBufs_.Get()+2, outfiter.swbBufs_.size()-2);
		*replyData = tcps_Alloc(*replyLen);
		BYTE* q = (BYTE*)*replyData;
		for(INT_PTR i=2; i<(INT_PTR)outfiter.swbBufs_.size(); ++i)
		{
			const SockWriteBuf& swb = outfiter.swbBufs_[i];
			memcpy(q, swb.data, swb.len);
			q += swb.len;
		}
		ASSERT(q-(BYTE*)*replyData == *replyLen);
	}
	return err;
}

TCPSError PCC_Service_S::StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;
	if(NULL==callbackName || 0==nameLen)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if(nameLen < 0)
		nameLen = strlen(callbackName);

	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnOnStreamedCallback_L_;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("OnStreamedCallback_L_", -1, NULL, 0);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Service_LocalCallback::FN_OnStreamedCallback_L_)callbackFuncL)(
					m_sessionL,
					callbackName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}
	else if(m_sessionR)
	{
		iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
		if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
			return TCPS_ERR_CALLBACK_NOT_READY;
		DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;
		iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
		ASSERT(0 == m_dataOutfiter.bufs_.size());

		TCPSError errUpdate = UpdateCallbackMatchingFlag_();
		if(TCPS_OK != errUpdate)
			return errUpdate;
		CallbackMatchingFlag::CallbackMap::const_iterator itCallback = m_callbackMatchingFlag.cbmap_.find(callbackName, nameLen);
		if(m_callbackMatchingFlag.cbmap_.end() == itCallback)
			return TCPS_ERR_CALLBACK_NOT_MATCH;
		ASSERT(itCallback->second.pMatchingVar);
		if(!*(itCallback->second.pMatchingVar))
			return TCPS_ERR_CALLBACK_NOT_MATCH;

		DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

		// 填充基本类型数据到outfiter
		iscm_PeerCallFlags peerCallFlags;
		peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
		peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
		peerCallFlags.requireReply = !(itCallback->second.isPosting);
		peerCallFlags.dummy_15 = 0;
		m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

		INT32 call_id_len = (INT32)(13+nameLen);
		CSmartBuf<char, 256> call_id_buf(call_id_len+1);
		strncpy(call_id_buf.Get(), "PCC_Service::", 13);
		StrAssign(call_id_buf.Get()+13, callbackName, nameLen);
		m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
		m_dataOutfiter.Push(call_id_buf.Get(), call_id_len+1);

		// 填充IN参数到outfiter
		if(dataLen > 0)
			m_dataOutfiter.Push(data, dataLen);

		// 调用PRCCall()
		if(peerCallFlags.requireReply) // callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			int replyBytes = 0;
			TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
			if(TCPS_OK != err)
				return err;

			iscm_RPCReplyPrefix replyPrefix;
			err = m_rpcRequester->RecvD(&replyPrefix, sizeof(replyPrefix));
			if(TCPS_OK != err)
			{
				this->OnNetworkMalformed_();
				return err;
			}

			INT_PTR leftReplyLen = replyBytes - sizeof(replyPrefix);
			ASSERT(leftReplyLen >= 0);
			if(leftReplyLen > 0)
			{
				ASSERT(replyData && replyLen);
				tcps_Binary replied_data;
				if(!replied_data.Resize(leftReplyLen))
				{
					this->OnNetworkMalformed_();
					return err;
				}
				err = m_rpcRequester->RecvD(replied_data.Get(), (int)leftReplyLen);
				if(TCPS_OK != err)
				{
					this->OnNetworkMalformed_();
					return err;
				}
				if(NULL==replyData || NULL==replyLen)
				{
					ASSERT(false);
					return TCPS_ERR_INVALID_PARAM;
				}
				INT32 bytes = 0;
				*replyData = replied_data.Drop(&bytes);
				*replyLen = bytes;
			}
		}
		else // posting callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			if(m_sessionR->m_udpSite.IsLinked())
			{
				int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
				BYTE* p = (BYTE*)tcps_Alloc(total);
				if(NULL == p)
				{
					ASSERT(false);
					return TCPS_ERR_OUT_OF_MEMORY;
				}
				BYTE* q = p;
				for(int i=0; i<reqBufVecCount; ++i)
				{
					const SockWriteBuf& swb = reqBufVec[i];
					memcpy(q, swb.data, swb.len);
					q += swb.len;
				}
				ASSERT((int)(q-p) == total);
				SockWriteBuf swb_udp;
				swb_udp.data = p;
				swb_udp.len = total;
				INT32 sessionID;
				m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.SendSessionData(sessionID, &swb_udp, 1);
			}
			else if(0 != m_sessionR->m_postingProxy.callerKey_)
			{
				INT_PTR queueFullIndexes = -1;
				INT_PTR queueFullCount = 0;
				TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
				if(TCPS_OK != err)
					return err;
				ASSERT(0==queueFullCount || 1==queueFullCount);
				if(1 == queueFullCount)
					return TCPS_ERR_POSTING_PENDING_FULL;
			}
			else
			{
				TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
				if(TCPS_OK != err)
					return err;
			}
		}
	}
	else
	{
		ASSERT(false);
		return TCPS_ERR_INTERNAL_BUG;
	}

	return TCPS_OK;
}

TCPSError PCC_Service_S::OnExecuted(
				IN INT64 jobKey_wrap,
				IN TCPSError errorCode_wrap,
				IN const void* context_wrap, IN INT32 context_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnOnExecuted;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("OnExecuted", 10, s_PCC_Service_OnExecuted_TypeInfo_, s_PCC_Service_OnExecuted_TypeInfo_len_);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Service_LocalCallback::FN_OnExecuted)callbackFuncL)(
					m_sessionL,
					jobKey_wrap,
					errorCode_wrap,
					tcps_Binary(xat_bind, (BYTE*)context_wrap, context_wrap_len)
					);
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_OnExecuted)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 23;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Service::OnExecuted", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT64 jobKey
	Put_BaseType_(&m_dataOutfiter, jobKey_wrap);

	// IN TCPSError errorCode
	Put_BaseType_(&m_dataOutfiter, errorCode_wrap);

	// IN tcps_Binary context
	if(context_wrap_len<0 || (context_wrap_len>0 && NULL==context_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, context_wrap, context_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Service_S::OnExecuted_Batch(
				IN const PPCC_Service_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN INT64 jobKey_wrap,
				IN TCPSError errorCode_wrap,
				IN const void* context_wrap, IN INT32 context_wrap_len,
				OUT PPCC_Service_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PPCC_Service_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
		{
			wrap_sessions[i]->OnExecuted(
					jobKey_wrap,
					errorCode_wrap,
					context_wrap, context_wrap_len
					);
			continue;
		}
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_OnExecuted)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'PCC_Service::OnExecuted()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 23;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "PCC_Service::OnExecuted";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN INT64 jobKey
	iscm_swb_.data = &jobKey_wrap;
	iscm_swb_.len = sizeof(jobKey_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN TCPSError errorCode
	iscm_swb_.data = &errorCode_wrap;
	iscm_swb_.len = sizeof(errorCode_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary context
	if(context_wrap_len<0 || (context_wrap_len>0 && NULL==context_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &context_wrap_len;
	iscm_swb_.len = sizeof(context_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(context_wrap_len > 0)
	{
		iscm_swb_.data = context_wrap;
		iscm_swb_.len = context_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError PCC_Service_S::SetRedirect_(
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		// ASSERT(false); // ??
		return TCPS_ERR_REFUSED;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_SetRedirect_)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Service::SetRedirect_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN IPP redirectIPP
	Put_BaseType_(&m_dataOutfiter, redirectIPP_wrap);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, redirectParam_wrap, redirectParam_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_sessionR->m_udpSite.IsLinked())
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID = m_sessionR->GetSessionKey();
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_sessionR->m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError PCC_Service_S::SetRedirect__Batch(
				IN const PPCC_Service_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len,
				OUT PPCC_Service_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PPCC_Service_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
			continue;
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_SetRedirect_)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'PCC_Service::SetRedirect_()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 25;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "PCC_Service::SetRedirect_";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN IPP redirectIPP
	iscm_swb_.data = &redirectIPP_wrap;
	iscm_swb_.len = sizeof(redirectIPP_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &redirectParam_wrap_len;
	iscm_swb_.len = sizeof(redirectParam_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(redirectParam_wrap_len > 0)
	{
		iscm_swb_.data = redirectParam_wrap;
		iscm_swb_.len = redirectParam_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError PCC_Service_S::CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks_wrap,
				IN const tcps_Array<tcps_String>& callbackTypeInfos_wrap,
				OUT tcps_Array<BOOL>& matchingFlags_wrap
				) callback
{
	if(m_sessionL)
	{
		InitializeAllCallsTypeInfo_();
		ASSERT(callbacks_wrap.Length() == callbackTypeInfos_wrap.Length());
		matchingFlags_wrap.Resize(callbacks_wrap.Length());
		for(int i=0; i<callbacks_wrap.Length(); ++i)
		{
			const tcps_String& name = callbacks_wrap[i];
			const tcps_String& typeInfo = callbackTypeInfos_wrap[i];
			matchingFlags_wrap[i] = (NULL != m_sessionL->FindCallback_(name.Get(), name.Length(), typeInfo.Get(), typeInfo.Length()));
		}
		return TCPS_OK;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 27;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Service::CallbackCheck_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> callbacks
	Put_BaseType_(&m_dataOutfiter, callbacks_wrap.LenRef());
	for(int idx1=0; idx1<callbacks_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbacks_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// IN tcps_Array<tcps_String> callbackTypeInfos
	Put_BaseType_(&m_dataOutfiter, callbackTypeInfos_wrap.LenRef());
	for(int idx1=0; idx1<callbackTypeInfos_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbackTypeInfos_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_IRequester* iscm_replied_picker = m_rpcRequester;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<BOOL> matchingFlags
		PICK_PODARRAY_(iscm_replied_picker, matchingFlags_wrap);
	}
	return TCPS_OK;
}

TCPSError PCC_Service_S::UDPLink_(
			OUT INT32& servePort,
			OUT INT32& linkKey
			) method
{
	ASSERT(!m_sessionR->m_udpSite.IsOn());
	AutoSock udpSock;
	udpSock.sock_ = UDPNew();
	if(!SockValid(udpSock.sock_))
		return TCPS_ERR_SYSTEM_RESOURCE;

	INT32 port = 0;
	TCPSError err = tcps_AutoBindUDPPort(udpSock.sock_, m_sessionR->m_serveIPP.ip_, &port);
	if(TCPS_OK != err)
		return err;
	m_sessionR->m_udpSite.localPort_ = port;
	m_sessionR->m_udpSite.sock_ = udpSock.Drop();
	servePort = port;
	linkKey = m_sessionR->GetSessionKey();
	return TCPS_OK;
}

TCPSError PCC_Service_S::UDPLinkConfirm_(
			) method
{
	if(!m_sessionR->m_udpSite.IsLinking())
	{
		ASSERT(false);
		return TCPS_ERR_CALL_WAS_IGNORED;
	}
	ASSERT(SockValid(m_sessionR->m_udpSite.sock_));

	DWORD recvTimeout = INFINITE;
	this->GetTimeout(NULL, &recvTimeout, NULL);
	if(INFINITE == recvTimeout)
		recvTimeout = iscm_GetDefaultRecvTimeout();
	recvTimeout = min(recvTimeout, (DWORD)2000);
	int r = SockCheckRead(m_sessionR->m_udpSite.sock_, recvTimeout);
	if(1 != r)
		return TCPS_ERR_RECV;
	IPP peerIPP;
	CSmartBuf<BYTE, 1024> smBuf(ISCM_MAX_UDP_FRAG_BYTES);
	BYTE* buf = smBuf.Get();
	r = SockReceiveFrom(m_sessionR->m_udpSite.sock_, &peerIPP.ip_, &peerIPP.port_, buf, ISCM_MAX_UDP_FRAG_BYTES);
	if((int)sizeof(iscm_UDPFrag) != r)
		return TCPS_ERR_RECV;
	if(!peerIPP.IsValid() || peerIPP.ip_!=m_sessionR->m_peerIPP.ip_)
		return TCPS_ERR_MALFORMED_REQ;

	const iscm_UDPFrag& frag = *(const iscm_UDPFrag*)buf;
	if(ISCM_UDP_FRAG_LINK != frag.fragType)
		return TCPS_ERR_MALFORMED_REQ;
	INT32 linkKey = m_sessionR->GetSessionKey();
	if(linkKey != frag.sendKey)
		return TCPS_ERR_INVALID_UDP_LINK_KEY;

	if(0 != UDPConnect(m_sessionR->m_udpSite.sock_, peerIPP.ip_, peerIPP.port_))
		return TCPS_ERR_SYSTEM;

	iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
	udpServer.AddSession(linkKey, m_sessionR->m_udpSite.sock_, m_sessionR);
	m_sessionR->m_udpSite.sock_ = INVALID_SOCKET;

	return TCPS_OK;
}

TCPSError PCC_Service_S::SetTimeout_(
			IN INT32 recvTimeout,
			IN INT32 sendTimeout
			) posting_method
{
	this->SetTimeout(INFINITE, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Service_S::SetSessionBufferSize_(
			IN INT32 recvBufBytes,
			IN INT32 sendBufBytes
			) posting_method
{
	this->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	return TCPS_OK;
}

TCPSError PCC_Service_S::MethodCheck_(
			IN const tcps_Array<tcps_String>& methods,
			IN const tcps_Array<tcps_String>& methodTypeInfos,
			OUT tcps_Array<BOOL>& matchingFlags
			) method
{
	if(0==methods.Length() || methods.Length()!=methodTypeInfos.Length())
		return TCPS_ERR_INVALID_PARAM;

	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<11> > MethodMap_;
	static MethodMap_* s_mdMap = NULL;
	if(NULL == s_mdMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_mdMap)
		{
			static MethodMap_ s_mdMapObj;
			MethodMap_& mdMap = s_mdMapObj;
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), CPtrStrA(s_PCC_Service_Login_TypeInfo_, s_PCC_Service_Login_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), CPtrStrA(s_PCC_Service_Logout_TypeInfo_, s_PCC_Service_Logout_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), CPtrStrA(s_PCC_Service_ListModules_TypeInfo_, s_PCC_Service_ListModules_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetModuleFile"), CPtrStrA(s_PCC_Service_GetModuleFile_TypeInfo_, s_PCC_Service_GetModuleFile_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Execute"), CPtrStrA(s_PCC_Service_Execute_TypeInfo_, s_PCC_Service_Execute_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("QueryJobs"), CPtrStrA(s_PCC_Service_QueryJobs_TypeInfo_, s_PCC_Service_QueryJobs_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CancelJob"), CPtrStrA(s_PCC_Service_CancelJob_TypeInfo_, s_PCC_Service_CancelJob_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLink_"), CPtrStrA(s_PCC_Service_UDPLink__TypeInfo_, s_PCC_Service_UDPLink__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UDPLinkConfirm_"), CPtrStrA(s_PCC_Service_UDPLinkConfirm__TypeInfo_, s_PCC_Service_UDPLinkConfirm__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetTimeout_"), CPtrStrA(s_PCC_Service_SetTimeout__TypeInfo_, s_PCC_Service_SetTimeout__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetSessionBufferSize_"), CPtrStrA(s_PCC_Service_SetSessionBufferSize__TypeInfo_, s_PCC_Service_SetSessionBufferSize__TypeInfo_len_))).second);
			s_mdMap = &mdMap;
		}
	}

	matchingFlags.Resize(methods.Length());
	for(int index=0; index<methods.Length(); ++index)
	{
		BOOL& flag = matchingFlags[index];
		const tcps_String& name = methods[index];
		const tcps_String& typeInfos = methodTypeInfos[index];
		MethodMap_::const_iterator cit = s_mdMap->find(name.Get(), name.Length());
		if(cit!=s_mdMap->end() && 0==typeInfos.Compare(cit->second.p, cit->second.len))
			flag = true;
		else
			flag = false;
	}
	return TCPS_OK;
}

void PCC_Service_S::CloseSession(
				IN TCPSError cause /*= TCPS_OK*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->CloseSession_(cause);
	else if(m_sessionL)
	{
		AutoFlag<OSThreadID> autoClosingTID(m_closingTID_L.tid, OSThreadSelf());
		m_sessionL->CloseSession_();
	}
}

IPP PCC_Service_S::GetPeerIPP(
				OUT IPP* peerID /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
	{
		if(peerID)
			*peerID = m_sessionR->m_peerID_IPP;
		return m_sessionR->m_peerIPP;
	}

	if(m_sessionL)
	{
		if(peerID)
			*peerID = IPP((DWORD)0, (int)0);
		return IPP((DWORD)0, (int)0);
	}

	if(peerID)
		*peerID = INVALID_IPP;
	return INVALID_IPP;
}

BOOL PCC_Service_S::IsLocalPeer() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	return (NULL != m_sessionL);
}

void PCC_Service_S::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->m_postingPendingParam.Set(maxPendingBytes, maxPendings);
}

void PCC_Service_S::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxPendingBytes)
			*maxPendingBytes = 0;
		if(maxPendings)
			*maxPendings = 0;
		return;
	}
	m_sessionR->m_postingPendingParam.Get(maxPendingBytes, maxPendings);
}

BOOL PCC_Service_S::CallbackIsReady() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		return (m_sessionR->m_callbackRequester.face_ && m_sessionR->m_callbackRequester.face_->IsConnected());
	return true;
}

TCPSError PCC_Service_S::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return TCPS_OK;
	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Service_S::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(connectTimeout)
			*connectTimeout = INFINITE;
		if(recvTimeout)
			*recvTimeout = INFINITE;
		if(sendTimeout)
			*sendTimeout = INFINITE;
		return TCPS_OK;
	}

	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->GetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

void PCC_Service_S::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return;

	if(recvBufBytes>=0 || sendBufBytes>=0)
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes >= 0)
			SockSetReceiveBufSize(sock, (0==recvBufBytes ? TCPS_DEFAULT_RECVBUF_SIZE : recvBufBytes));
		if(sendBufBytes >= 0)
			SockSetSendBufSize(sock, (0==sendBufBytes ? TCPS_DEFAULT_SENDBUF_SIZE : sendBufBytes));
	}
	if(m_sessionR->m_callbackRequester.face_)
		m_sessionR->m_callbackRequester.face_->SetSessionBufferSize(recvBufBytes, sendBufBytes);
}

void PCC_Service_S::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(recvBufBytes)
			*recvBufBytes = 0;
		if(sendBufBytes)
			*sendBufBytes = 0;
		return;
	}

	if(NULL==recvBufBytes && NULL==sendBufBytes)
		return;
	if(m_sessionR->m_callbackRequester.face_)
	{
		m_sessionR->m_callbackRequester.face_->GetSessionBufferSize(recvBufBytes, sendBufBytes);
	}
	else
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes)
			SockGetReceiveBufSize(sock, recvBufBytes);
		if(sendBufBytes)
			SockGetSendBufSize(sock, sendBufBytes);
	}
}

void PCC_Service_S::SetPostingSendParameters(
				IN INT32 maxBufferingSize,
				IN INT32 maxSendings
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR && 0!=m_sessionR->m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().SetBufferingParam(m_sessionR->m_postingProxy.callerKey_, maxBufferingSize, maxSendings);
	m_postingSendParam.Set(maxBufferingSize, maxSendings);
}

void PCC_Service_S::GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxBufferingSize)
			*maxBufferingSize = 0;
		if(maxSendings)
			*maxSendings = 0;
		return;
	}
	m_postingSendParam.Get(maxBufferingSize, maxSendings);
}

TCPSError PCC_Service_S::CleanPostingSendingQueue()
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(NULL==m_sessionR || 0==m_sessionR->m_postingProxy.callerKey_)
		return TCPS_ERR_CALL_WAS_IGNORED;
	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(&m_sessionR->m_postingProxy.callerKey_, 1);
}

TCPSError PCC_Service_S::CleanPostingSendingQueue(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount
				)
{
	tcps_SmartArray<PPCC_Service_S_, 256> sessions_ar_;
	for(INT_PTR i=0; i<sessionsCount; ++i)
	{
		if(NULL == sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(NULL==sessions[i]->m_sessionR || 0==sessions[i]->m_sessionR->m_postingProxy.callerKey_)
			continue;
		sessions_ar_.push_back(sessions[i]);
	}
	if(0 == sessions_ar_.size())
		return TCPS_OK;

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> callerKey_ar_;
	callerKey_ar_.resize(sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)sessions_ar_.size(); ++i)
		callerKey_ar_[i] = sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(callerKey_ar_.Get(), (int)callerKey_ar_.size());
}

class PCC_Service_LS : public IPCC_Service_LocalMethod
{
private:
	PCC_Service_LS(const PCC_Service_LS&);
	PCC_Service_LS& operator= (const PCC_Service_LS&);

private:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	IPCC_Service_LocalCallback* const m_callback;
	PCC_Service_S* const m_method;
	TCPSError m_connectError;
	INT32 m_sessionKey;

public:
	TCPSError GetConnectError() const
		{	return m_connectError;	}

public:
	PCC_Service_LS(const IPP& clientID_IPP, NP_SCATTEREDSessionMaker& sessionMaker, IPCC_Service_LocalCallback* callbackHandler)
		: m_sessionMaker(sessionMaker)
		, m_callback(callbackHandler)
		, m_method(tcps_NewEx(PCC_Service_S, (m_sessionMaker, NULL, callbackHandler)))
		, m_connectError(TCPS_ERROR)
		, m_sessionKey(0)
	{
		INT32 sessionCount;
		m_sessionMaker.OnSessionConnect_(&m_sessionKey, sessionCount);
		m_connectError = m_method->OnConnected(m_sessionKey, clientID_IPP, sessionCount);
		if(TCPS_OK == m_connectError)
		{
			if(m_callback)
				m_method->OnCallbackReady();
			m_method->OnPostingCallReady();
			m_sessionMaker.RegisterLocalSession_(m_callback);
		}
	}
	virtual ~PCC_Service_LS()
	{
		if(TCPS_OK == m_connectError)
			m_sessionMaker.UnregisterLocalSession_(m_callback);
		m_sessionMaker.Unregister(m_method);
		if(INVALID_OSTHREADID==m_method->m_closingTID_L.tid || m_method->m_closingTID_L.tid!=OSThreadSelf())
			m_method->OnPeerBroken(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_method->OnClose(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_sessionMaker.OnSessionClosed_();
		m_method->~PCC_Service_S();
		tcps_Free(m_method);
	}
	virtual void DeleteThis()
		{	tcps_Delete(this);	}

	virtual PROC FindMethod_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				)
	{
		if(NULL == name)
		{
			ASSERT(false);
			return NULL;
		}

		// 对OnStreamedCall_L_()特殊处理
		if(nameLen<0 && 0==strcmp("OnStreamedCall_L_", name))
			return (PROC)&OnStreamedCall_L_;

		if(NULL == type)
		{
			ASSERT(false);
			return NULL;
		}

		InitializeAllCallsTypeInfo_();
		typedef TwoItems<CPtrStrA, PROC> FuncPair;
		typedef CQuickStringMap<CPtrStrA, FuncPair, QSS_Traits<7> > MethodMap_;
		static MethodMap_* s_mdMap = NULL;
		if(NULL == s_mdMap)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == s_mdMap)
			{
				static MethodMap_ s_mdMapObj;
				MethodMap_& mdMap = s_mdMapObj;
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), FuncPair(CPtrStrA(s_PCC_Service_Login_TypeInfo_, s_PCC_Service_Login_TypeInfo_len_), (PROC)&Login)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), FuncPair(CPtrStrA(s_PCC_Service_Logout_TypeInfo_, s_PCC_Service_Logout_TypeInfo_len_), (PROC)&Logout)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), FuncPair(CPtrStrA(s_PCC_Service_ListModules_TypeInfo_, s_PCC_Service_ListModules_TypeInfo_len_), (PROC)&ListModules)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetModuleFile"), FuncPair(CPtrStrA(s_PCC_Service_GetModuleFile_TypeInfo_, s_PCC_Service_GetModuleFile_TypeInfo_len_), (PROC)&GetModuleFile)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Execute"), FuncPair(CPtrStrA(s_PCC_Service_Execute_TypeInfo_, s_PCC_Service_Execute_TypeInfo_len_), (PROC)&Execute)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("QueryJobs"), FuncPair(CPtrStrA(s_PCC_Service_QueryJobs_TypeInfo_, s_PCC_Service_QueryJobs_TypeInfo_len_), (PROC)&QueryJobs)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CancelJob"), FuncPair(CPtrStrA(s_PCC_Service_CancelJob_TypeInfo_, s_PCC_Service_CancelJob_TypeInfo_len_), (PROC)&CancelJob)));
				s_mdMap = &mdMap;
			}
		}

		MethodMap_::iterator it = s_mdMap->find(name, nameLen);
		if(s_mdMap->end() == it)
			return NULL;
		const CPtrStrA& ps = it->second.first;
		if(0 != ps.Compare(type, typeLen))
			return NULL;
		return it->second.second;
	}

private:
	static TCPSError OnStreamedCall_L_(
				IN void* sessionObj,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
	{	return ((PCC_Service_LS*)sessionObj)->m_method->OnStreamedCall_L_(
					methodName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}

private:
	static TCPSError Login(
				IN void* sessionObj_wrap,
				IN const tcps_String& ticket
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->Login(
					ticket
					);
	}

	static TCPSError Logout(
				IN void* sessionObj_wrap
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->Logout(
					);
	}

	static TCPSError ListModules(
				IN void* sessionObj_wrap,
				IN INT32 moduleType,
				OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->ListModules(
					moduleType,
					modulesInfo
					);
	}

	static TCPSError GetModuleFile(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN INT32 fileType,
				OUT tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->GetModuleFile(
					moduleKey,
					fileType,
					moduleFiles
					);
	}

	static TCPSError Execute(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams,
				OUT INT64& jobKey
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->Execute(
					moduleKey,
					inputUrl,
					outputUrl,
					moduleParams,
					jobKey
					);
	}

	static TCPSError QueryJobs(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& jobsKey,
				OUT tcps_Array<ExecuteState>& jobsState
				) method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->QueryJobs(
					jobsKey,
					jobsState
					);
	}

	static TCPSError CancelJob(
				IN void* sessionObj_wrap,
				IN INT64 jobKey
				) posting_method
	{	return ((PCC_Service_LS*)sessionObj_wrap)->m_method->CancelJob(
					jobKey
					);
	}
};

TCPSError MakeLocalSession_PCC_Service__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Service_LocalMethod*& methodHandler,
			IN IPCC_Service_LocalCallback* callbackHandler
			)
{
	PCC_Service_LS* session = tcps_NewEx(PCC_Service_LS, (clientID_IPP, sessionMaker, callbackHandler));
	if(NULL == session)
		return TCPS_ERR_OUT_OF_MEMORY;
	TCPSError err = session->GetConnectError();
	if(TCPS_OK != err)
	{
		tcps_Delete(session);
		return err;
	}
	methodHandler = session;
	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_Login(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"Login",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_String ticket
	IN tcps_String ticket_wrap;
	GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ticket_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::Login() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->Login(
			ticket_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::Login(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::Login(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_Logout(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"Logout",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::Logout() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->Logout(
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::Logout(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::Logout(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_AddModule(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"AddModule",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN PCC_ModuleProperty moduleProperty
	IN PCC_ModuleProperty moduleProperty_wrap;
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.moduleTag.name);
			GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.moduleTag.version);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.modulePattern);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.moduleFileType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.moduleType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.moduleLatency);
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.description);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.cpuType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.osType);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.executeBits);
		GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleProperty_wrap.binaryType);

	// IN tcps_Array<PCC_ModuleFile> moudleFiles
	IN tcps_Array<PCC_ModuleFile> moudleFiles_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	moudleFiles_wrap.Resize(array_len);
	for(int idx1=0; idx1<moudleFiles_wrap.Length(); ++idx1)
	{
		PCC_ModuleFile& ref1 = moudleFiles_wrap[idx1];
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.name);
			GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.data);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::AddModule() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT INT64 moduleKey;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->AddModule(
			moduleProperty_wrap,
			moudleFiles_wrap,
			opWrapper->moduleKey
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::AddModule(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::AddModule(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT INT64 moduleKey
	OUT const INT64& moduleKey_wrap = opWrapper->moduleKey;
	Set_BaseType_(outfiter, moduleKey_wrap);

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_AddModuleFile(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"AddModuleFile",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 moduleKey
	IN INT64 moduleKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleKey_wrap);

	// IN PCC_ModuleFileType fileType
	IN PCC_ModuleFileType fileType_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, fileType_wrap);

	// IN tcps_Array<PCC_ModuleFile> moudleFiles
	IN tcps_Array<PCC_ModuleFile> moudleFiles_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	moudleFiles_wrap.Resize(array_len);
	for(int idx1=0; idx1<moudleFiles_wrap.Length(); ++idx1)
	{
		PCC_ModuleFile& ref1 = moudleFiles_wrap[idx1];
			GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.name);
			GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1.data);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::AddModuleFile() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->AddModuleFile(
			moduleKey_wrap,
			fileType_wrap,
			moudleFiles_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::AddModuleFile(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::AddModuleFile(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_RemoveModule(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"RemoveModule",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 moduleKey
	IN INT64 moduleKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleKey_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::RemoveModule() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->RemoveModule(
			moduleKey_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::RemoveModule(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::RemoveModule(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_RemoveModuleFiles(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"RemoveModuleFiles",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 moduleKey
	IN INT64 moduleKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, moduleKey_wrap);

	// IN INT32 fileType
	IN INT32 fileType_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, fileType_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::RemoveModuleFiles() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->RemoveModuleFiles(
			moduleKey_wrap,
			fileType_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::RemoveModuleFiles(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::RemoveModuleFiles(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_ListModules(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	if(thisObj && thisObj->m_streamedCallSite.func)
	{
		void* replyData = NULL;
		INT_PTR replyLen = 0;
		errTcps = thisObj->m_streamedCallSite.func(
					thisObj->m_streamedCallSite.serverObj,
					thisObj->m_streamedCallSite.sessionObj,
					"PCC_Toolkit",
					"ListModules",
					ptrInParams,
					ptrInParamsLen,
					&replyData,
					&replyLen
					);
		if(TCPS_OK == errTcps)
		{
			ptrInParams += ptrInParamsLen;
			ptrInParamsLen = 0;
		}
		ASSERT(outfiter);
		iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
		replyPrefix->Init();
		outfiter->Push(replyPrefix, sizeof(*replyPrefix), true, NULL);
		if(replyLen > 0)
			outfiter->Push(replyData, replyLen, true, NULL);
		return errTcps;
	}

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::ListModules() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<PCC_ModulePropWithKey> modulesInfo;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->ListModules(
			opWrapper->modulesInfo
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::ListModules(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::ListModules(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<PCC_ModulePropWithKey> modulesInfo
	OUT const tcps_Array<PCC_ModulePropWithKey>& modulesInfo_wrap = opWrapper->modulesInfo;
	Set_BaseType_(outfiter, modulesInfo_wrap.LenRef());
	for(int idx1=0; idx1<modulesInfo_wrap.Length(); ++idx1)
	{
		const PCC_ModulePropWithKey& ref1 = modulesInfo_wrap[idx1];
			Set_BaseType_(outfiter, ref1.key);
					Set_String_(outfiter, ref1.prop.moduleTag.name);
					Set_BaseType_(outfiter, ref1.prop.moduleTag.version);
				Set_BaseType_(outfiter, ref1.prop.modulePattern);
				Set_BaseType_(outfiter, ref1.prop.moduleFileType);
				Set_BaseType_(outfiter, ref1.prop.moduleType);
				Set_BaseType_(outfiter, ref1.prop.moduleLatency);
				Set_String_(outfiter, ref1.prop.description);
				Set_BaseType_(outfiter, ref1.prop.cpuType);
				Set_BaseType_(outfiter, ref1.prop.osType);
				Set_BaseType_(outfiter, ref1.prop.executeBits);
				Set_BaseType_(outfiter, ref1.prop.binaryType);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_SetTimeout_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvTimeout
	IN INT32 recvTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvTimeout_wrap);

	// IN INT32 sendTimeout
	IN INT32 sendTimeout_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendTimeout_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::SetTimeout_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->SetTimeout_(
			recvTimeout_wrap,
			sendTimeout_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::SetTimeout_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::SetTimeout_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_SetSessionBufferSize_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT32 recvBufBytes
	IN INT32 recvBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, recvBufBytes_wrap);

	// IN INT32 sendBufBytes
	IN INT32 sendBufBytes_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, sendBufBytes_wrap);

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::SetSessionBufferSize_() posting_method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->SetSessionBufferSize_(
			recvBufBytes_wrap,
			sendBufBytes_wrap
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::SetSessionBufferSize_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::SetSessionBufferSize_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

TCPSError NP_SCATTEREDSession::Wrap_PCC_Toolkit_MethodCheck_(
				IN NP_SCATTEREDSession* thisObj,
				IN void* faceObj,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) method
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> methods
	IN tcps_Array<tcps_String> methods_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methods_wrap.Resize(array_len);
	for(int idx1=0; idx1<methods_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methods_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	// IN tcps_Array<tcps_String> methodTypeInfos
	IN tcps_Array<tcps_String> methodTypeInfos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	methodTypeInfos_wrap.Resize(array_len);
	for(int idx1=0; idx1<methodTypeInfos_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = methodTypeInfos_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		NPLogError(("PCC_Toolkit_S::MethodCheck_() method, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<BOOL> matchingFlags;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的方法函数
	try
	{
		PCC_Toolkit_S* pCC_ToolkitObj_wrap;
		if(thisObj)
			pCC_ToolkitObj_wrap = thisObj->m_pCC_Toolkit;
		else
			pCC_ToolkitObj_wrap = (PCC_Toolkit_S*)faceObj;
		ASSERT(pCC_ToolkitObj_wrap);
		errTcps = pCC_ToolkitObj_wrap->MethodCheck_(
			methods_wrap,
			methodTypeInfos_wrap,
			opWrapper->matchingFlags
			);
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		NPLogError(("PCC_Toolkit_S::MethodCheck_(), Out of memory"));
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		NPLogError(("PCC_Toolkit_S::MethodCheck_(), Unknown exception"));
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<BOOL> matchingFlags
	OUT const tcps_Array<BOOL>& matchingFlags_wrap = opWrapper->matchingFlags;
	Set_PODArray_(outfiter, matchingFlags_wrap);

	return TCPS_OK;
}

PCC_Toolkit_S::CallbackMatchingFlag::CallbackMatchingFlag()
{
	Reset();
	cbmap_.insert(std::make_pair(CPtrStrA("SetRedirect_", 12), Info(&matching_SetRedirect_, true)));
}

void PCC_Toolkit_S::CallbackMatchingFlag::Reset()
{
	matching_SetRedirect_ = false;
}

TCPSError PCC_Toolkit_S::UpdateCallbackMatchingFlag_()
{
	if(!m_callbackMatchingUpdatedFlag.needUpdate)
		return TCPS_OK;
	InitializeAllCallsTypeInfo_();
	tcps_String callbacks_ar[1];
	IN tcps_Array<tcps_String> callbacks;
	callbacks.Attach(xat_bind, callbacks_ar, 1);
	tcps_String callbackTypeInfos_ar[1];
	IN tcps_Array<tcps_String> callbackTypeInfos;
	callbackTypeInfos.Attach(xat_bind, callbackTypeInfos_ar, 1);
	callbacks_ar[0].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetRedirect_"));
	callbackTypeInfos_ar[0].Attach(xat_bind, (char*)s_PCC_Toolkit_SetRedirect__TypeInfo_, s_PCC_Toolkit_SetRedirect__TypeInfo_len_);
	OUT tcps_Array<BOOL> matchingFlags;
	TCPSError err = this->CallbackCheck_(callbacks, callbackTypeInfos, matchingFlags);
	if(TCPS_OK == err)
	{
		if(matchingFlags.Length() == callbacks.Length())
		{
			m_callbackMatchingUpdatedFlag.needUpdate = false;
			const BOOL* const matchingFlags_p = matchingFlags.Get();
			m_callbackMatchingFlag.matching_SetRedirect_ = matchingFlags_p[0];
		}
		else
		{
			ASSERT(false);
			err = TCPS_ERR_MALFORMED_REPLY;
		}
	}
	if(m_callbackMatchingUpdatedFlag.needUpdate)
		m_callbackMatchingFlag.Reset();
	return err;
}

const PCC_Toolkit_S::CallbackMatchingFlag& PCC_Toolkit_S::GetCallbackMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	TCPSError ret = UpdateCallbackMatchingFlag_();
	if(err)
		*err = ret;
	return m_callbackMatchingFlag;
}

TCPSError PCC_Toolkit_S::OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	INT_PTR inParamsDataLen = sizeof(iscm_PeerCallFlags);
	inParamsDataLen += 13; // "PCC_Toolkit::"
	if(nameLen < 0)
		nameLen = strlen(methodName);
	inParamsDataLen += sizeof(INT32)+nameLen+1;
	inParamsDataLen += dataLen;
	BYTE* const inParamsData = (BYTE*)tcps_Alloc(inParamsDataLen);
	if(NULL == inParamsData)
		return TCPS_ERR_OUT_OF_MEMORY;

	BYTE* p = inParamsData;
	iscm_PeerCallFlags& peerCallFlags = *(iscm_PeerCallFlags*)p;
	peerCallFlags.sizeofBOOL_req = sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1; // ??
	peerCallFlags.dummy_15 = 0;
	p += sizeof(iscm_PeerCallFlags);
	*(INT32*)p = 13+(INT32)nameLen;
	p += sizeof(INT32);
	strncpy((char*)p, "PCC_Toolkit::", 13);
	p += 13;
	strncpy((char*)p, methodName, nameLen);
	p += nameLen;
	*(char*)p = 0;
	p += 1;
	memcpy(p, data, dataLen);
	p += dataLen;
	ASSERT(p-inParamsData == inParamsDataLen);

	BOOL requireReplyData = true;
	BOOL dataTransferred = false;
	iscm_RPCDataOutfiter outfiter;
	ASSERT(NULL==m_sessionR && m_sessionL);
	TCPSError err = NP_SCATTEREDSession::OnRPCCall_(NULL, this, requireReplyData, inParamsData, dataTransferred, inParamsDataLen, &outfiter);
	tcps_Free(inParamsData);
	if(TCPS_OK != err)
		return err;
	// outfiter.swbBufs_[0]总指向iscm_RPCDataOutfiter::reply_
	// outfiter.swbBufs_[1]为iscm_RPCReplyPrefix replyPrefix
	ASSERT(outfiter.swbBufs_.size()==1 || outfiter.swbBufs_.size()>=2);
	if(outfiter.swbBufs_.size() >= 2)
	{
		ASSERT(replyData && replyLen);
		*replyLen = SockTotalizeWriteBufVec(outfiter.swbBufs_.Get()+2, outfiter.swbBufs_.size()-2);
		*replyData = tcps_Alloc(*replyLen);
		BYTE* q = (BYTE*)*replyData;
		for(INT_PTR i=2; i<(INT_PTR)outfiter.swbBufs_.size(); ++i)
		{
			const SockWriteBuf& swb = outfiter.swbBufs_[i];
			memcpy(q, swb.data, swb.len);
			q += swb.len;
		}
		ASSERT(q-(BYTE*)*replyData == *replyLen);
	}
	return err;
}

TCPSError PCC_Toolkit_S::StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;
	if(NULL==callbackName || 0==nameLen)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if(nameLen < 0)
		nameLen = strlen(callbackName);

	if(m_sessionL)
	{
		if(NULL == m_callSiteL.func_)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == m_callSiteL.func_)
				m_callSiteL.func_ = tcps_New(CallSiteL_::TFunc);
		}
		PROC& callbackFuncL = m_callSiteL.func_->fnOnStreamedCallback_L_;
		if(NULL == callbackFuncL)
		{
			InitializeAllCallsTypeInfo_();
			callbackFuncL = m_sessionL->FindCallback_("OnStreamedCallback_L_", -1, NULL, 0);
			if(NULL == callbackFuncL)
				return TCPS_ERR_CALLBACK_NOT_MATCH;
		}
		return ((IPCC_Toolkit_LocalCallback::FN_OnStreamedCallback_L_)callbackFuncL)(
					m_sessionL,
					callbackName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}
	else if(m_sessionR)
	{
		iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
		if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
			return TCPS_ERR_CALLBACK_NOT_READY;
		DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;
		iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
		ASSERT(0 == m_dataOutfiter.bufs_.size());

		TCPSError errUpdate = UpdateCallbackMatchingFlag_();
		if(TCPS_OK != errUpdate)
			return errUpdate;
		CallbackMatchingFlag::CallbackMap::const_iterator itCallback = m_callbackMatchingFlag.cbmap_.find(callbackName, nameLen);
		if(m_callbackMatchingFlag.cbmap_.end() == itCallback)
			return TCPS_ERR_CALLBACK_NOT_MATCH;
		ASSERT(itCallback->second.pMatchingVar);
		if(!*(itCallback->second.pMatchingVar))
			return TCPS_ERR_CALLBACK_NOT_MATCH;

		DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

		// 填充基本类型数据到outfiter
		iscm_PeerCallFlags peerCallFlags;
		peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
		peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
		peerCallFlags.requireReply = !(itCallback->second.isPosting);
		peerCallFlags.dummy_15 = 0;
		m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

		INT32 call_id_len = (INT32)(13+nameLen);
		CSmartBuf<char, 256> call_id_buf(call_id_len+1);
		strncpy(call_id_buf.Get(), "PCC_Toolkit::", 13);
		StrAssign(call_id_buf.Get()+13, callbackName, nameLen);
		m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
		m_dataOutfiter.Push(call_id_buf.Get(), call_id_len+1);

		// 填充IN参数到outfiter
		if(dataLen > 0)
			m_dataOutfiter.Push(data, dataLen);

		// 调用PRCCall()
		if(peerCallFlags.requireReply) // callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			int replyBytes = 0;
			TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
			if(TCPS_OK != err)
				return err;

			iscm_RPCReplyPrefix replyPrefix;
			err = m_rpcRequester->RecvD(&replyPrefix, sizeof(replyPrefix));
			if(TCPS_OK != err)
			{
				this->OnNetworkMalformed_();
				return err;
			}

			INT_PTR leftReplyLen = replyBytes - sizeof(replyPrefix);
			ASSERT(leftReplyLen >= 0);
			if(leftReplyLen > 0)
			{
				ASSERT(replyData && replyLen);
				tcps_Binary replied_data;
				if(!replied_data.Resize(leftReplyLen))
				{
					this->OnNetworkMalformed_();
					return err;
				}
				err = m_rpcRequester->RecvD(replied_data.Get(), (int)leftReplyLen);
				if(TCPS_OK != err)
				{
					this->OnNetworkMalformed_();
					return err;
				}
				if(NULL==replyData || NULL==replyLen)
				{
					ASSERT(false);
					return TCPS_ERR_INVALID_PARAM;
				}
				INT32 bytes = 0;
				*replyData = replied_data.Drop(&bytes);
				*replyLen = bytes;
			}
		}
		else // posting callback
		{
			const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
			int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
			if(m_sessionR->m_udpSite.IsLinked())
			{
				int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
				BYTE* p = (BYTE*)tcps_Alloc(total);
				if(NULL == p)
				{
					ASSERT(false);
					return TCPS_ERR_OUT_OF_MEMORY;
				}
				BYTE* q = p;
				for(int i=0; i<reqBufVecCount; ++i)
				{
					const SockWriteBuf& swb = reqBufVec[i];
					memcpy(q, swb.data, swb.len);
					q += swb.len;
				}
				ASSERT((int)(q-p) == total);
				SockWriteBuf swb_udp;
				swb_udp.data = p;
				swb_udp.len = total;
				INT32 sessionID;
				m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.SendSessionData(sessionID, &swb_udp, 1);
			}
			else if(0 != m_sessionR->m_postingProxy.callerKey_)
			{
				INT_PTR queueFullIndexes = -1;
				INT_PTR queueFullCount = 0;
				TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_sessionR->m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
				if(TCPS_OK != err)
					return err;
				ASSERT(0==queueFullCount || 1==queueFullCount);
				if(1 == queueFullCount)
					return TCPS_ERR_POSTING_PENDING_FULL;
			}
			else
			{
				TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
				if(TCPS_OK != err)
					return err;
			}
		}
	}
	else
	{
		ASSERT(false);
		return TCPS_ERR_INTERNAL_BUG;
	}

	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::SetRedirect_(
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len
				) posting_callback
{
	if(m_sessionL)
	{
		// ASSERT(false); // ??
		return TCPS_ERR_REFUSED;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateCallbackMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_callbackMatchingFlag.matching_SetRedirect_)
		return TCPS_ERR_CALLBACK_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Toolkit::SetRedirect_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN IPP redirectIPP
	Put_BaseType_(&m_dataOutfiter, redirectIPP_wrap);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	Put_Binary_(&m_dataOutfiter, redirectParam_wrap, redirectParam_wrap_len);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
		if(TCPS_OK != err)
			return err;
	}
	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::SetRedirect__Batch(
				IN const PPCC_Toolkit_S_* wrap_sessions,
				IN INT_PTR wrap_sessionsCount,
				IN const IPP& redirectIPP_wrap,
				IN const void* redirectParam_wrap, IN INT32 redirectParam_wrap_len,
				OUT PPCC_Toolkit_S_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_callback
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_sessions || wrap_sessionsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PPCC_Toolkit_S_, 256> iscm_sessions_ar_;
	for(INT_PTR i=0; i<wrap_sessionsCount; ++i)
	{
		if(NULL == wrap_sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_sessions[i]->m_sessionL)
			continue;
		if(TCPS_OK != wrap_sessions[i]->UpdateCallbackMatchingFlag_())
			continue;
		if(!wrap_sessions[i]->m_callbackMatchingFlag.matching_SetRedirect_)
		{
			IPP peerIPP = wrap_sessions[i]->m_sessionR->m_peerIPP;
			NPLogWarning(("The 'PCC_Toolkit::SetRedirect_()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_sessions[i]->m_sessionR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_sessions[i];
				++notReadies;
			}
			continue;
		}
		iscm_sessions_ar_.push_back(wrap_sessions[i]);
	}
	if(0 == iscm_sessions_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 25;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "PCC_Toolkit::SetRedirect_";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN IPP redirectIPP
	iscm_swb_.data = &redirectIPP_wrap;
	iscm_swb_.len = sizeof(redirectIPP_wrap);
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Binary redirectParam
	if(redirectParam_wrap_len<0 || (redirectParam_wrap_len>0 && NULL==redirectParam_wrap))
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	iscm_swb_.data = &redirectParam_wrap_len;
	iscm_swb_.len = sizeof(redirectParam_wrap_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	if(redirectParam_wrap_len > 0)
	{
		iscm_swb_.data = redirectParam_wrap;
		iscm_swb_.len = redirectParam_wrap_len;
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_sessions_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_sessions_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError PCC_Toolkit_S::CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks_wrap,
				IN const tcps_Array<tcps_String>& callbackTypeInfos_wrap,
				OUT tcps_Array<BOOL>& matchingFlags_wrap
				) callback
{
	if(m_sessionL)
	{
		InitializeAllCallsTypeInfo_();
		ASSERT(callbacks_wrap.Length() == callbackTypeInfos_wrap.Length());
		matchingFlags_wrap.Resize(callbacks_wrap.Length());
		for(int i=0; i<callbacks_wrap.Length(); ++i)
		{
			const tcps_String& name = callbacks_wrap[i];
			const tcps_String& typeInfo = callbackTypeInfos_wrap[i];
			matchingFlags_wrap[i] = (NULL != m_sessionL->FindCallback_(name.Get(), name.Length(), typeInfo.Get(), typeInfo.Length()));
		}
		return TCPS_OK;
	}

	// 准备回调相关变量
	iscm_IRequester* const m_rpcRequester = m_sessionR->m_callbackRequester.face_;
	if(NULL==m_rpcRequester || !m_rpcRequester->IsConnected())
		return TCPS_ERR_CALLBACK_NOT_READY;
	DataOutfiter& m_dataOutfiter = m_sessionR->m_callbackOutfiter;

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 27;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("PCC_Toolkit::CallbackCheck_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> callbacks
	Put_BaseType_(&m_dataOutfiter, callbacks_wrap.LenRef());
	for(int idx1=0; idx1<callbacks_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbacks_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// IN tcps_Array<tcps_String> callbackTypeInfos
	Put_BaseType_(&m_dataOutfiter, callbackTypeInfos_wrap.LenRef());
	for(int idx1=0; idx1<callbackTypeInfos_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = callbackTypeInfos_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_IRequester* iscm_replied_picker = m_rpcRequester;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<BOOL> matchingFlags
		PICK_PODARRAY_(iscm_replied_picker, matchingFlags_wrap);
	}
	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::SetTimeout_(
			IN INT32 recvTimeout,
			IN INT32 sendTimeout
			) posting_method
{
	this->SetTimeout(INFINITE, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::SetSessionBufferSize_(
			IN INT32 recvBufBytes,
			IN INT32 sendBufBytes
			) posting_method
{
	this->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::MethodCheck_(
			IN const tcps_Array<tcps_String>& methods,
			IN const tcps_Array<tcps_String>& methodTypeInfos,
			OUT tcps_Array<BOOL>& matchingFlags
			) method
{
	if(0==methods.Length() || methods.Length()!=methodTypeInfos.Length())
		return TCPS_ERR_INVALID_PARAM;

	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<9> > MethodMap_;
	static MethodMap_* s_mdMap = NULL;
	if(NULL == s_mdMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_mdMap)
		{
			static MethodMap_ s_mdMapObj;
			MethodMap_& mdMap = s_mdMapObj;
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), CPtrStrA(s_PCC_Toolkit_Login_TypeInfo_, s_PCC_Toolkit_Login_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), CPtrStrA(s_PCC_Toolkit_Logout_TypeInfo_, s_PCC_Toolkit_Logout_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddModule"), CPtrStrA(s_PCC_Toolkit_AddModule_TypeInfo_, s_PCC_Toolkit_AddModule_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddModuleFile"), CPtrStrA(s_PCC_Toolkit_AddModuleFile_TypeInfo_, s_PCC_Toolkit_AddModuleFile_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("RemoveModule"), CPtrStrA(s_PCC_Toolkit_RemoveModule_TypeInfo_, s_PCC_Toolkit_RemoveModule_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("RemoveModuleFiles"), CPtrStrA(s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_, s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), CPtrStrA(s_PCC_Toolkit_ListModules_TypeInfo_, s_PCC_Toolkit_ListModules_TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetTimeout_"), CPtrStrA(s_PCC_Toolkit_SetTimeout__TypeInfo_, s_PCC_Toolkit_SetTimeout__TypeInfo_len_))).second);
			VERIFY(mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetSessionBufferSize_"), CPtrStrA(s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_, s_PCC_Toolkit_SetSessionBufferSize__TypeInfo_len_))).second);
			s_mdMap = &mdMap;
		}
	}

	matchingFlags.Resize(methods.Length());
	for(int index=0; index<methods.Length(); ++index)
	{
		BOOL& flag = matchingFlags[index];
		const tcps_String& name = methods[index];
		const tcps_String& typeInfos = methodTypeInfos[index];
		MethodMap_::const_iterator cit = s_mdMap->find(name.Get(), name.Length());
		if(cit!=s_mdMap->end() && 0==typeInfos.Compare(cit->second.p, cit->second.len))
			flag = true;
		else
			flag = false;
	}
	return TCPS_OK;
}

void PCC_Toolkit_S::CloseSession(
				IN TCPSError cause /*= TCPS_OK*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->CloseSession_(cause);
	else if(m_sessionL)
	{
		AutoFlag<OSThreadID> autoClosingTID(m_closingTID_L.tid, OSThreadSelf());
		m_sessionL->CloseSession_();
	}
}

IPP PCC_Toolkit_S::GetPeerIPP(
				OUT IPP* peerID /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
	{
		if(peerID)
			*peerID = m_sessionR->m_peerID_IPP;
		return m_sessionR->m_peerIPP;
	}

	if(m_sessionL)
	{
		if(peerID)
			*peerID = IPP((DWORD)0, (int)0);
		return IPP((DWORD)0, (int)0);
	}

	if(peerID)
		*peerID = INVALID_IPP;
	return INVALID_IPP;
}

BOOL PCC_Toolkit_S::IsLocalPeer() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	return (NULL != m_sessionL);
}

void PCC_Toolkit_S::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		m_sessionR->m_postingPendingParam.Set(maxPendingBytes, maxPendings);
}

void PCC_Toolkit_S::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxPendingBytes)
			*maxPendingBytes = 0;
		if(maxPendings)
			*maxPendings = 0;
		return;
	}
	m_sessionR->m_postingPendingParam.Get(maxPendingBytes, maxPendings);
}

BOOL PCC_Toolkit_S::CallbackIsReady() const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR)
		return (m_sessionR->m_callbackRequester.face_ && m_sessionR->m_callbackRequester.face_->IsConnected());
	return true;
}

TCPSError PCC_Toolkit_S::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return TCPS_OK;
	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

TCPSError PCC_Toolkit_S::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(connectTimeout)
			*connectTimeout = INFINITE;
		if(recvTimeout)
			*recvTimeout = INFINITE;
		if(sendTimeout)
			*sendTimeout = INFINITE;
		return TCPS_OK;
	}

	if(NULL == m_sessionR->m_callbackRequester.face_)
		return TCPS_ERR_CALLBACK_NOT_READY;
	m_sessionR->m_callbackRequester.face_->GetTimeout(connectTimeout, recvTimeout, sendTimeout);
	return TCPS_OK;
}

void PCC_Toolkit_S::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
		return;

	if(recvBufBytes>=0 || sendBufBytes>=0)
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes >= 0)
			SockSetReceiveBufSize(sock, (0==recvBufBytes ? TCPS_DEFAULT_RECVBUF_SIZE : recvBufBytes));
		if(sendBufBytes >= 0)
			SockSetSendBufSize(sock, (0==sendBufBytes ? TCPS_DEFAULT_SENDBUF_SIZE : sendBufBytes));
	}
	if(m_sessionR->m_callbackRequester.face_)
		m_sessionR->m_callbackRequester.face_->SetSessionBufferSize(recvBufBytes, sendBufBytes);
}

void PCC_Toolkit_S::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(recvBufBytes)
			*recvBufBytes = 0;
		if(sendBufBytes)
			*sendBufBytes = 0;
		return;
	}

	if(NULL==recvBufBytes && NULL==sendBufBytes)
		return;
	if(m_sessionR->m_callbackRequester.face_)
	{
		m_sessionR->m_callbackRequester.face_->GetSessionBufferSize(recvBufBytes, sendBufBytes);
	}
	else
	{
		SOCKET sock = INVALID_SOCKET;
		m_sessionR->m_bindedSession->GetInfos(NULL, &sock, NULL, NULL, NULL);
		ASSERT(SockValid(sock));
		if(recvBufBytes)
			SockGetReceiveBufSize(sock, recvBufBytes);
		if(sendBufBytes)
			SockGetSendBufSize(sock, sendBufBytes);
	}
}

void PCC_Toolkit_S::SetPostingSendParameters(
				IN INT32 maxBufferingSize,
				IN INT32 maxSendings
				)
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionR && 0!=m_sessionR->m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().SetBufferingParam(m_sessionR->m_postingProxy.callerKey_, maxBufferingSize, maxSendings);
	m_postingSendParam.Set(maxBufferingSize, maxSendings);
}

void PCC_Toolkit_S::GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(m_sessionL)
	{
		if(maxBufferingSize)
			*maxBufferingSize = 0;
		if(maxSendings)
			*maxSendings = 0;
		return;
	}
	m_postingSendParam.Get(maxBufferingSize, maxSendings);
}

TCPSError PCC_Toolkit_S::CleanPostingSendingQueue()
{
	ASSERT((NULL==m_sessionR) != (NULL==m_sessionL));
	if(NULL==m_sessionR || 0==m_sessionR->m_postingProxy.callerKey_)
		return TCPS_ERR_CALL_WAS_IGNORED;
	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(&m_sessionR->m_postingProxy.callerKey_, 1);
}

TCPSError PCC_Toolkit_S::CleanPostingSendingQueue(
				IN const PPCC_Toolkit_S_* sessions,
				IN INT_PTR sessionsCount
				)
{
	tcps_SmartArray<PPCC_Toolkit_S_, 256> sessions_ar_;
	for(INT_PTR i=0; i<sessionsCount; ++i)
	{
		if(NULL == sessions[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(NULL==sessions[i]->m_sessionR || 0==sessions[i]->m_sessionR->m_postingProxy.callerKey_)
			continue;
		sessions_ar_.push_back(sessions[i]);
	}
	if(0 == sessions_ar_.size())
		return TCPS_OK;

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> callerKey_ar_;
	callerKey_ar_.resize(sessions_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)sessions_ar_.size(); ++i)
		callerKey_ar_[i] = sessions_ar_[i]->m_sessionR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(callerKey_ar_.Get(), (int)callerKey_ar_.size());
}

class PCC_Toolkit_LS : public IPCC_Toolkit_LocalMethod
{
private:
	PCC_Toolkit_LS(const PCC_Toolkit_LS&);
	PCC_Toolkit_LS& operator= (const PCC_Toolkit_LS&);

private:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	IPCC_Toolkit_LocalCallback* const m_callback;
	PCC_Toolkit_S* const m_method;
	TCPSError m_connectError;
	INT32 m_sessionKey;

public:
	TCPSError GetConnectError() const
		{	return m_connectError;	}

public:
	PCC_Toolkit_LS(const IPP& clientID_IPP, NP_SCATTEREDSessionMaker& sessionMaker, IPCC_Toolkit_LocalCallback* callbackHandler)
		: m_sessionMaker(sessionMaker)
		, m_callback(callbackHandler)
		, m_method(tcps_NewEx(PCC_Toolkit_S, (m_sessionMaker, NULL, callbackHandler)))
		, m_connectError(TCPS_ERROR)
		, m_sessionKey(0)
	{
		INT32 sessionCount;
		m_sessionMaker.OnSessionConnect_(&m_sessionKey, sessionCount);
		m_connectError = m_method->OnConnected(m_sessionKey, clientID_IPP, sessionCount);
		if(TCPS_OK == m_connectError)
		{
			if(m_callback)
				m_method->OnCallbackReady();
			m_method->OnPostingCallReady();
			m_sessionMaker.RegisterLocalSession_(m_callback);
		}
	}
	virtual ~PCC_Toolkit_LS()
	{
		if(TCPS_OK == m_connectError)
			m_sessionMaker.UnregisterLocalSession_(m_callback);
		m_sessionMaker.Unregister(m_method);
		if(INVALID_OSTHREADID==m_method->m_closingTID_L.tid || m_method->m_closingTID_L.tid!=OSThreadSelf())
			m_method->OnPeerBroken(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_method->OnClose(m_sessionKey, TCPS_ANY_IPP, m_connectError);
		m_sessionMaker.OnSessionClosed_();
		m_method->~PCC_Toolkit_S();
		tcps_Free(m_method);
	}
	virtual void DeleteThis()
		{	tcps_Delete(this);	}

	virtual PROC FindMethod_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				)
	{
		if(NULL == name)
		{
			ASSERT(false);
			return NULL;
		}

		// 对OnStreamedCall_L_()特殊处理
		if(nameLen<0 && 0==strcmp("OnStreamedCall_L_", name))
			return (PROC)&OnStreamedCall_L_;

		if(NULL == type)
		{
			ASSERT(false);
			return NULL;
		}

		InitializeAllCallsTypeInfo_();
		typedef TwoItems<CPtrStrA, PROC> FuncPair;
		typedef CQuickStringMap<CPtrStrA, FuncPair, QSS_Traits<7> > MethodMap_;
		static MethodMap_* s_mdMap = NULL;
		if(NULL == s_mdMap)
		{
			IscmRPCGlobalLocker locker;
			if(NULL == s_mdMap)
			{
				static MethodMap_ s_mdMapObj;
				MethodMap_& mdMap = s_mdMapObj;
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), FuncPair(CPtrStrA(s_PCC_Toolkit_Login_TypeInfo_, s_PCC_Toolkit_Login_TypeInfo_len_), (PROC)&Login)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), FuncPair(CPtrStrA(s_PCC_Toolkit_Logout_TypeInfo_, s_PCC_Toolkit_Logout_TypeInfo_len_), (PROC)&Logout)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddModule"), FuncPair(CPtrStrA(s_PCC_Toolkit_AddModule_TypeInfo_, s_PCC_Toolkit_AddModule_TypeInfo_len_), (PROC)&AddModule)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddModuleFile"), FuncPair(CPtrStrA(s_PCC_Toolkit_AddModuleFile_TypeInfo_, s_PCC_Toolkit_AddModuleFile_TypeInfo_len_), (PROC)&AddModuleFile)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("RemoveModule"), FuncPair(CPtrStrA(s_PCC_Toolkit_RemoveModule_TypeInfo_, s_PCC_Toolkit_RemoveModule_TypeInfo_len_), (PROC)&RemoveModule)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("RemoveModuleFiles"), FuncPair(CPtrStrA(s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_, s_PCC_Toolkit_RemoveModuleFiles_TypeInfo_len_), (PROC)&RemoveModuleFiles)));
				mdMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), FuncPair(CPtrStrA(s_PCC_Toolkit_ListModules_TypeInfo_, s_PCC_Toolkit_ListModules_TypeInfo_len_), (PROC)&ListModules)));
				s_mdMap = &mdMap;
			}
		}

		MethodMap_::iterator it = s_mdMap->find(name, nameLen);
		if(s_mdMap->end() == it)
			return NULL;
		const CPtrStrA& ps = it->second.first;
		if(0 != ps.Compare(type, typeLen))
			return NULL;
		return it->second.second;
	}

private:
	static TCPSError OnStreamedCall_L_(
				IN void* sessionObj,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
	{	return ((PCC_Toolkit_LS*)sessionObj)->m_method->OnStreamedCall_L_(
					methodName,
					nameLen,
					data,
					dataLen,
					replyData,
					replyLen
					);
	}

private:
	static TCPSError Login(
				IN void* sessionObj_wrap,
				IN const tcps_String& ticket
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->Login(
					ticket
					);
	}

	static TCPSError Logout(
				IN void* sessionObj_wrap
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->Logout(
					);
	}

	static TCPSError AddModule(
				IN void* sessionObj_wrap,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->AddModule(
					moduleProperty,
					moudleFiles,
					moduleKey
					);
	}

	static TCPSError AddModuleFile(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->AddModuleFile(
					moduleKey,
					fileType,
					moudleFiles
					);
	}

	static TCPSError RemoveModule(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->RemoveModule(
					moduleKey
					);
	}

	static TCPSError RemoveModuleFiles(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->RemoveModuleFiles(
					moduleKey,
					fileType
					);
	}

	static TCPSError ListModules(
				IN void* sessionObj_wrap,
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
	{	return ((PCC_Toolkit_LS*)sessionObj_wrap)->m_method->ListModules(
					modulesInfo
					);
	}
};

TCPSError MakeLocalSession_PCC_Toolkit__(
			IN const IPP& clientID_IPP,
			IN NP_SCATTEREDSessionMaker& sessionMaker,
			OUT IPCC_Toolkit_LocalMethod*& methodHandler,
			IN IPCC_Toolkit_LocalCallback* callbackHandler
			)
{
	PCC_Toolkit_LS* session = tcps_NewEx(PCC_Toolkit_LS, (clientID_IPP, sessionMaker, callbackHandler));
	if(NULL == session)
		return TCPS_ERR_OUT_OF_MEMORY;
	TCPSError err = session->GetConnectError();
	if(TCPS_OK != err)
	{
		tcps_Delete(session);
		return err;
	}
	methodHandler = session;
	return TCPS_OK;
}
