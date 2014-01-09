// NP_SCATTEREDSession.cpp
//
// ע��: ���ļ�Ϊ�������ɣ���С���޸�

#include "StdAfx.h"
#include "NP_SCATTEREDSession.h"
#include "ipcvt.h"
#include "nplog.h"
#include "singleton.h"
#include "ipcvt.h"
#include "TrunkManage.h"
/////////////////////////////////////////////////////////////////////
// interface GRID_User

GRID_User_S::GRID_User_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IGRID_User_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // ����ֻ��Ϊһ��ģʽ
	// TODO: �����GRID_User�Ĺ��촦��
}

GRID_User_S::~GRID_User_S()
{
	// TODO: �����GRID_User����������
}

TCPSError GRID_User_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: ����ӽӿ�GRID_User�����Ӻ���

	NPLogInfo(("GRID_User_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	return TCPS_OK;
}

void GRID_User_S::OnCallbackReady()
{
	// TODO: ����ӽӿ�GRID_User�Ļص���������

	NPLogInfo(("GRID_User_S::OnCallbackReady()"));
}

void GRID_User_S::OnPostingCallReady()
{
	// TODO: ����ӽӿ�GRID_User��posting�ص���������

	NPLogInfo(("GRID_User_S::OnPostingCallReady()"));
}

void GRID_User_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("GRID_User_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�GRID_User�ĶԶ˶��ߴ���
}

void GRID_User_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("GRID_User_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�GRID_User�����ӹرմ���
}

TCPSError GRID_User_S::AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelJobProgram(
				IN const tcps_Array<INT64>& programKeys
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::FindJobProgram(
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::CommitJob(
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::CancelJob(
				IN const tcps_Array<INT64>& jobKeys
				) posting_method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::SetJobPriority(
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJobs(
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::QueryJobs(
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetLogCount(
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method
{
	// TODO: ��ʵ�ִ˺���
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
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::Login(
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method
{
	
	IPP ipp(GetLocalIP(),9012);//127.0.0.1
	TCPSError ret = this->SetRedirect_(ipp, NULL, 0);
	ret;
	return TCPS_OK;	
}

TCPSError GRID_User_S::Logout(
				) posting_method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::AddUser(
				IN const UserInfo& userInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelUser(
				IN const tcps_Array<tcps_String>& userNames
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::UpdatePassword(
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ManageUser(
				IN const UserInfo& userInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetUserInfo(
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListJTMs(
				OUT tcps_Array<tcps_String>& jtms
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetJTMInfo(
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetGridProperty(
				OUT GridProperty& gridProperty
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::DelSplitter(
				IN const GRID_ProgramInfo& splitter
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::ListSplitterParam(
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError GRID_User_S::GetGridStatistics(
				OUT GridStatistics& stat
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Deploy

PCC_Deploy_S::PCC_Deploy_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Deploy_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // ����ֻ��Ϊһ��ģʽ
	// TODO: �����PCC_Deploy�Ĺ��촦��
}

PCC_Deploy_S::~PCC_Deploy_S()
{
	// TODO: �����PCC_Deploy����������
}

TCPSError PCC_Deploy_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: ����ӽӿ�PCC_Deploy�����Ӻ���

	NPLogInfo(("PCC_Deploy_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	
	return m_deploy.OnConnected(sessionKey,peerID_IPP,sessionCount);
}

void PCC_Deploy_S::OnCallbackReady()
{
	// TODO: ����ӽӿ�PCC_Deploy�Ļص���������

	NPLogInfo(("PCC_Deploy_S::OnCallbackReady()"));
}

void PCC_Deploy_S::OnPostingCallReady()
{
	// TODO: ����ӽӿ�PCC_Deploy��posting�ص���������

	NPLogInfo(("PCC_Deploy_S::OnPostingCallReady()"));
}

void PCC_Deploy_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Deploy_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Deploy�ĶԶ˶��ߴ���
}

void PCC_Deploy_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Deploy_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Deploy�����ӹرմ���
	m_deploy.OnClose(sessionKey,peerID_IPP,cause);
}

TCPSError PCC_Deploy_S::Login(
				IN const tcps_String& ticket
				) method
{
	return m_deploy.Login(ticket);	
	
}

TCPSError PCC_Deploy_S::Logout(
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.Logout();
	
}

TCPSError PCC_Deploy_S::CreateTrunk(
				IN const tcps_String& trunk
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.CreateTrunk(trunk);
}

TCPSError PCC_Deploy_S::RemoveTrunk(
				IN const tcps_String& trunk
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.RemoveTrunk(trunk);
}

TCPSError PCC_Deploy_S::ListTrunk(
				OUT tcps_Array<tcps_String>& trunks
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.ListTrunk(trunks);
}

TCPSError PCC_Deploy_S::AddAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag,
				IN const tcps_Array<PCC_ModuleFile>& files
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.AddAuthCenter(trunk,authTag,files);
}

TCPSError PCC_Deploy_S::RemoveAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.RemoveAuthCenter(trunk,authTag);
}

TCPSError PCC_Deploy_S::ListAuthCenter(
				IN const tcps_String& trunk,
				OUT tcps_Array<PCC_ModuleTag>& authTags
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.ListAuthCenter(trunk,authTags);
}

TCPSError PCC_Deploy_S::FindAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.FindAuthCenter(trunk,authTag);
}

TCPSError PCC_Deploy_S::AddModule(
				IN const tcps_String& trunk,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
{
	// TODO: ��ʵ�ִ˺�'��
	return m_deploy.AddModule(trunk,moduleProperty,moudleFiles,moduleKey);
}

TCPSError PCC_Deploy_S::AddModuleFile(
				IN const tcps_String& trunk,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.AddModuleFile(trunk,moduleKey,fileType,moduleFiles);
}

TCPSError PCC_Deploy_S::RemoveModule(
				IN const tcps_String& trunk,
				IN INT64 moduleKey
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.RemoveModule(trunk,moduleKey);
}

TCPSError PCC_Deploy_S::RemoveModuleFiles(
				IN const tcps_String& trunk,
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.RemoveModuleFiles(trunk,moduleKey,fileType);
}

TCPSError PCC_Deploy_S::ListModules(
				IN const tcps_String& trunk,
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_deploy.ListModules(trunk,modulesInfo);
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Scatter

PCC_Scatter_S::PCC_Scatter_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Scatter_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // ����ֻ��Ϊһ��ģʽ
	// TODO: �����PCC_Scatter�Ĺ��촦��
}

PCC_Scatter_S::~PCC_Scatter_S()
{
	// TODO: �����PCC_Scatter����������
}

TCPSError PCC_Scatter_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: ����ӽӿ�PCC_Scatter�����Ӻ���

	NPLogInfo(("PCC_Scatter_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	m_skey = sessionKey;
	m_jobkey = -1;
	return TCPS_OK;
}

void PCC_Scatter_S::OnCallbackReady()
{
	// TODO: ����ӽӿ�PCC_Scatter�Ļص���������

	NPLogInfo(("PCC_Scatter_S::OnCallbackReady()"));
}

void PCC_Scatter_S::OnPostingCallReady()
{
	// TODO: ����ӽӿ�PCC_Scatter��posting�ص���������

	printf("�����½ڵ�%d\n",m_skey);
	//������ڴ�����ҵ����Ҫ�ص�֪ͨ
	pgrid_util::Singleton<CScatteredManage>::instance().pushNode(m_skey,this);
	NPLogInfo(("PCC_Scatter_S::OnPostingCallReady()"));
}

void PCC_Scatter_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Scatter_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Scatter�ĶԶ˶��ߴ���
}

void PCC_Scatter_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Scatter_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Scatter�����ӹرմ���
	if(m_jobkey > -1)//�ڵ����ڴ�����ҵ��������ҵ����δ����˳�
	{
		tcps_Binary context;
		pgrid_util::Singleton<CScatteredManage>::instance().callbackSS(m_jobkey,m_jobkey,TCPS_ERROR,context);
	}
	pgrid_util::Singleton<CScatteredManage>::instance().diableNode(sessionKey);
}

TCPSError PCC_Scatter_S::OnComputed(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_method
{
	//�ص�sevice�ӿڵ�OnExecuted()	
	printf("�ص���ҵ��%d\n",m_jobkey);
	ASSERT(taskKey == m_jobkey);
	TCPSError rt = pgrid_util::Singleton<CScatteredManage>::instance().callbackSS(m_jobkey,taskKey,errorCode,context);
	
	//�Żؿɵ��ȶ���
	printf("�Żؾɽڵ�%d\n",m_skey);
	this->m_jobkey = -1;
	pgrid_util::Singleton<CScatteredManage>::instance().pushNode(m_skey,this);
	return rt;
}

TCPSError PCC_Scatter_S::OnComputed1(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs
				) posting_method
{
	//�ص�sevice�ӿڵ�OnExecuted()	
	printf("�ص���ҵ��%d\n",m_jobkey);
	TCPSError rt = pgrid_util::Singleton<CScatteredManage>::instance().callbackSS1(taskKey,errorCode,context,outArgs);
	
	//�Żؿɵ��ȶ���
	printf("�Żؾɽڵ�%d\n",m_skey);
	this->m_jobkey = -1;
	pgrid_util::Singleton<CScatteredManage>::instance().pushNode(m_skey,this);
	return rt;
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Service

PCC_Service_S::PCC_Service_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Service_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // ����ֻ��Ϊһ��ģʽ
	// TODO: �����PCC_Service�Ĺ��촦��
}

PCC_Service_S::~PCC_Service_S()
{
	// TODO: �����PCC_Service����������
}

TCPSError PCC_Service_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: ����ӽӿ�PCC_Service�����Ӻ���

	NPLogInfo(("PCC_Service_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	m_handler.SetSenssion(this);
	return TCPS_OK;
}

void PCC_Service_S::OnCallbackReady()
{
	// TODO: ����ӽӿ�PCC_Service�Ļص���������

	NPLogInfo(("PCC_Service_S::OnCallbackReady()"));
}

void PCC_Service_S::OnPostingCallReady()
{
	// TODO: ����ӽӿ�PCC_Service��posting�ص���������

	NPLogInfo(("PCC_Service_S::OnPostingCallReady()"));
}

void PCC_Service_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Service_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Service�ĶԶ˶��ߴ���
}

void PCC_Service_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Service_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Service�����ӹرմ���
	m_handler.OnClose(sessionKey,peerID_IPP,cause);
	//����ύ��job
	pgrid_util::Singleton<CScatteredManage>::instance().clearInvalidJobs(m_que_jobkeys);

}

TCPSError PCC_Service_S::Login(
				IN const tcps_String& ticket
				) method
{
	return m_handler.Login(ticket);
}

TCPSError PCC_Service_S::Logout(
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.Logout();
}

TCPSError PCC_Service_S::ListModules(
				IN INT32 moduleType,
				OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.ListModules(moduleType,modulesInfo);
}

TCPSError PCC_Service_S::GetModuleFile(
				IN INT64 moduleKey,
				IN INT32 fileType,
				OUT tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
{
	// TODO: ��ʵ�ִ˺���
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
	// TODO: ��ʵ�ִ˺���
	
	TCPSError rt = m_handler.Execute(moduleKey,inputUrl,outputUrl,moduleParams,jobKey);
	return rt;
}

TCPSError PCC_Service_S::QueryJobs(
				IN const tcps_Array<INT64>& jobsKey,
				OUT tcps_Array<ExecuteState>& jobsState
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.QueryJobs(jobsKey,jobsState);
}

TCPSError PCC_Service_S::CancelJob(
				IN INT64 jobKey
				) posting_method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.CancelJob(jobKey);
}

/////////////////////////////////////////////////////////////////////
// interface PCC_Toolkit

PCC_Toolkit_S::PCC_Toolkit_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Toolkit_LocalCallback* sessionL)
	: m_sessionMaker(sessionMaker), m_sessionR(sessionR), m_sessionL(sessionL)
{
	NPR_ASSERT((NULL==sessionR) != (NULL==sessionL)); // ����ֻ��Ϊһ��ģʽ
	// TODO: �����PCC_Toolkit�Ĺ��촦��
}

PCC_Toolkit_S::~PCC_Toolkit_S()
{
	// TODO: �����PCC_Toolkit����������
}

TCPSError PCC_Toolkit_S::OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				)
{
	// TODO: ����ӽӿ�PCC_Toolkit�����Ӻ���

	NPLogInfo(("PCC_Toolkit_S::OnConnected(%d, %s, %d)", sessionKey, IPP_TO_STR_A(peerID_IPP), sessionCount));
	m_handler.SetSenssion(NULL);//ע������ΪNULL

	return TCPS_OK;
}

void PCC_Toolkit_S::OnCallbackReady()
{
	// TODO: ����ӽӿ�PCC_Toolkit�Ļص���������

	NPLogInfo(("PCC_Toolkit_S::OnCallbackReady()"));
}

void PCC_Toolkit_S::OnPostingCallReady()
{
	// TODO: ����ӽӿ�PCC_Toolkit��posting�ص���������

	NPLogInfo(("PCC_Toolkit_S::OnPostingCallReady()"));
}

void PCC_Toolkit_S::OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Toolkit_S::OnPeerBroken(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Toolkit�ĶԶ˶��ߴ���
}

void PCC_Toolkit_S::OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				)
{
	NPLogInfo(("PCC_Toolkit_S::OnClose(%d, %s, %s(%d))", sessionKey, IPP_TO_STR_A(peerID_IPP), tcps_GetErrTxt(cause), cause));
	// TODO: ����ӽӿ�PCC_Toolkit�����ӹرմ���
	m_handler.OnClose(sessionKey,peerID_IPP,cause);
}

TCPSError PCC_Toolkit_S::Login(
				IN const tcps_String& ticket
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.Login(ticket);
}

TCPSError PCC_Toolkit_S::Logout(
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.Logout();
}

TCPSError PCC_Toolkit_S::AddModule(
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.AddModule(moduleProperty,moudleFiles,moduleKey);
}

TCPSError PCC_Toolkit_S::AddModuleFile(
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::RemoveModule(
				IN INT64 moduleKey
				) method
{
	// TODO: ��ʵ�ִ˺���
	return m_handler.RemoveModule(moduleKey);	
}

TCPSError PCC_Toolkit_S::RemoveModuleFiles(
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
{
	// TODO: ��ʵ�ִ˺���
	return TCPS_ERR_NOT_IMPLEMENTED;
}

TCPSError PCC_Toolkit_S::ListModules(
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
{
	// TODO: ��ʵ�ִ˺���
	TCPSError rt =  
		m_handler.ListModules(modulesInfo);
	return rt;
}
