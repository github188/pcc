// NP_SCATTEREDSession.cpp
//
// 注意: 此文件为工具生成，请小心修改

#include "StdAfx.h"
#include "NP_SCATTEREDSession.h"
#include "ipcvt.h"
#include "nplog.h"

/////////////////////////////////////////////////////////////////////
// interface GRID_User

GRID_User_S::GRID_User_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IGRID_User_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // 有且只能为一种模式
	// TODO: 请添加GRID_User的构造处理
}

GRID_User_S::~GRID_User_S()
{
	// TODO: 请添加GRID_User的析构处理
}

TCPSError GRID_User_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: 请添加接口GRID_User的连接后处理

	NPLogInfo(("GRID_User_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	return TCPS_OK;
}

void GRID_User_S::OnCallbackReady()
{
	// TODO: 请添加接口GRID_User的回调就绪处理

	NPLogInfo(("GRID_User_S::OnCallbackReady()"));
}

void GRID_User_S::OnPostingCallReady()
{
	// TODO: 请添加接口GRID_User的posting回调就绪处理

	NPLogInfo(("GRID_User_S::OnPostingCallReady()"));
}

void GRID_User_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("GRID_User_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口GRID_User的对端断线处理
}

void GRID_User_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("GRID_User_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口GRID_User的连接关闭处理
}

TCPSError GRID_User_S::AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelJobProgram(
				IN const tcps_Array<INT64>& programKeys
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::FindJobProgram(
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::CommitJob(
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::CancelJob(
				IN const tcps_Array<INT64>& jobKeys
				) posting_method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::SetJobPriority(
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJobs(
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::QueryJobs(
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetLogCount(
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::Login(
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::Logout(
				) posting_method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::AddUser(
				IN const UserInfo& userInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelUser(
				IN const tcps_Array<tcps_String>& userNames
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::UpdatePassword(
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ManageUser(
				IN const UserInfo& userInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetUserInfo(
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJTMs(
				OUT tcps_Array<tcps_String>& jtms
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetJTMInfo(
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetGridProperty(
				OUT GridProperty& gridProperty
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelSplitter(
				IN const GRID_ProgramInfo& splitter
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListSplitterParam(
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetGridStatistics(
				OUT GridStatistics& stat
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Scatter

PCC_Scatter_S::PCC_Scatter_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Scatter_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // 有且只能为一种模式
	// TODO: 请添加PCC_Scatter的构造处理
}

PCC_Scatter_S::~PCC_Scatter_S()
{
	// TODO: 请添加PCC_Scatter的析构处理
}

TCPSError PCC_Scatter_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: 请添加接口PCC_Scatter的连接后处理

	NPLogInfo(("PCC_Scatter_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	return TCPS_OK;
}

void PCC_Scatter_S::OnCallbackReady()
{
	// TODO: 请添加接口PCC_Scatter的回调就绪处理

	NPLogInfo(("PCC_Scatter_S::OnCallbackReady()"));
}

void PCC_Scatter_S::OnPostingCallReady()
{
	// TODO: 请添加接口PCC_Scatter的posting回调就绪处理

	NPLogInfo(("PCC_Scatter_S::OnPostingCallReady()"));
}

void PCC_Scatter_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Scatter_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Scatter的对端断线处理
}

void PCC_Scatter_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Scatter_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Scatter的连接关闭处理
}

TCPSError PCC_Scatter_S::OnComputed(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Service

PCC_Service_S::PCC_Service_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Service_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // 有且只能为一种模式
	// TODO: 请添加PCC_Service的构造处理
}

PCC_Service_S::~PCC_Service_S()
{
	// TODO: 请添加PCC_Service的析构处理
}

TCPSError PCC_Service_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: 请添加接口PCC_Service的连接后处理

	NPLogInfo(("PCC_Service_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	return TCPS_OK;
}

void PCC_Service_S::OnCallbackReady()
{
	// TODO: 请添加接口PCC_Service的回调就绪处理

	NPLogInfo(("PCC_Service_S::OnCallbackReady()"));
}

void PCC_Service_S::OnPostingCallReady()
{
	// TODO: 请添加接口PCC_Service的posting回调就绪处理

	NPLogInfo(("PCC_Service_S::OnPostingCallReady()"));
}

void PCC_Service_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Service_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Service的对端断线处理
}

void PCC_Service_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Service_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Service的连接关闭处理
}

TCPSError PCC_Service_S::Login(
				IN const tcps_String& ticket
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::Logout(
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::ListModules(
				IN INT32 moduleType,
				OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::GetModuleFile(
				IN INT64 moduleKey,
				IN INT32 fileType,
				OUT tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::Execute(
				IN INT64 moduleKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams,
				OUT INT64& jobKey
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::QueryJobs(
				IN const tcps_Array<INT64>& jobsKey,
				OUT tcps_Array<ExecuteState>& jobsState
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Service_S::CancelJob(
				IN INT64 jobKey
				) posting_method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Toolkit

PCC_Toolkit_S::PCC_Toolkit_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Toolkit_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // 有且只能为一种模式
	// TODO: 请添加PCC_Toolkit的构造处理
}

PCC_Toolkit_S::~PCC_Toolkit_S()
{
	// TODO: 请添加PCC_Toolkit的析构处理
}

TCPSError PCC_Toolkit_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: 请添加接口PCC_Toolkit的连接后处理

	NPLogInfo(("PCC_Toolkit_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	return TCPS_OK;
}

void PCC_Toolkit_S::OnCallbackReady()
{
	// TODO: 请添加接口PCC_Toolkit的回调就绪处理

	NPLogInfo(("PCC_Toolkit_S::OnCallbackReady()"));
}

void PCC_Toolkit_S::OnPostingCallReady()
{
	// TODO: 请添加接口PCC_Toolkit的posting回调就绪处理

	NPLogInfo(("PCC_Toolkit_S::OnPostingCallReady()"));
}

void PCC_Toolkit_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Toolkit_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Toolkit的对端断线处理
}

void PCC_Toolkit_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Toolkit_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: 请添加接口PCC_Toolkit的连接关闭处理
}

TCPSError PCC_Toolkit_S::Login(
				IN const tcps_String& ticket
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::Logout(
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::AddModule(
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::AddModuleFile(
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::RemoveModule(
				IN INT64 moduleKey
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::RemoveModuleFiles(
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::ListModules(
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
{
	// TODO: 请实现此函数
	return TCPS_ERR_NOT_IMPLEMENTED;
}
