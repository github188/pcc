// NP_SCATTEREDSession.h
//
// 注意: 此文件为工具生成，请小心修改

#ifndef _NP_SCATTEREDSession_h_
#define _NP_SCATTEREDSession_h_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
	#pragma once
#endif

#include "ILocker.h"
#include "npmap.h"
#include "QuickStringMap.h"
#include "AutoInterface.h"
#include "rpc_serve.h"
#include "iscm_requester.h"
#include "iscm_posting_caller.h"
#include "iscm_udp_serve.h"
#include "iscm_helper.h"
#include "iscm_smart_container.h"
#include "NP_SCATTEREDTypesDefine.h"

class NP_SCATTEREDSession;

//[[begin_iscm]]

class GRID_User;
class GRID_User_LS;
class GRID_User_S : public GRID_User_T
{
	friend class GRID_User;
	friend class NP_SCATTEREDSession;
	friend class GRID_User_LS;
private:
	GRID_User_S(const GRID_User_S&);
	GRID_User_S& operator= (const GRID_User_S&);

	typedef GRID_User_S* PGRID_User_S_;

public:
	// TODO: 可以在此处添加GRID_User的自定义成员

private:
	GRID_User_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IGRID_User_LocalCallback* sessionL);
	~GRID_User_S();
	TCPSError OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				);
	void OnCallbackReady();
	void OnPostingCallReady();
	void OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);
	void OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);

public:
	// 主动关闭会话
	void CloseSession(
				IN TCPSError cause = TCPS_OK
				);

	// 获取对端IPP
	// peerID用于返回对端的peerID_IPP
	IPP GetPeerIPP(
				OUT IPP* peerID = NULL
				) const;

	// 对端是否为进程内客户端
	BOOL IsLocalPeer() const;

	// 判断回调连接是否就绪
	BOOL CallbackIsReady() const;

	// 设置回调调用网络超时，INFINITE表示使用默认值，回调未就绪时返回失败
	TCPSError SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	TCPSError GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				);

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值
	void SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				);
	void GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const;

	// 设置posting执行端参数
	// @maxPendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_PENDING_BUFFER_SIZE）
	// @maxPendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_PENDINGS）
	void SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				);
	void GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const;

	// 设置posting调用端参数
	// @maxBufferingSize[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxBufferingSize /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理会话的posting回调发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount
				);

	// 回调匹配检查支持
public:
	struct CallbackMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 2> CallbackMap;
		CallbackMap cbmap_;
		BOOL matching_ReplyCommitJob;
		BOOL matching_SetRedirect_;
		void Reset();
		CallbackMatchingFlag();
	};
public:
	// 获取本地与对端回调匹配信息
	const CallbackMatchingFlag& GetCallbackMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
private:
	TCPSError OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

private:
	TCPSError AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

private:
	TCPSError DelJobProgram(
				IN const tcps_Array<INT64>& programKeys
				) method;

private:
	TCPSError FindJobProgram(
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method;

private:
	TCPSError ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms
				) method;

private:
	TCPSError CommitJob(
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method;

public:
	TCPSError ReplyCommitJob(
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const void* context, IN INT32 context_len
				) posting_callback;
	TCPSError ReplyCommitJob(
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback
		{	return this->ReplyCommitJob(
							jobKey,
							replyCode,
							context.Get(), context.Length()
							);
		}

public:
	static TCPSError ReplyCommitJob_Batch(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const void* context, IN INT32 context_len,
				OUT PGRID_User_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError ReplyCommitJob_Batch(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context,
				OUT PGRID_User_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return GRID_User_S::ReplyCommitJob_Batch(
							sessions, sessionsCount,
							jobKey,
							replyCode,
							context.Get(), context.Length(),
							sendFaileds, failedCount
							);
		}

private:
	TCPSError CancelJob(
				IN const tcps_Array<INT64>& jobKeys
				) posting_method;

private:
	TCPSError SetJobPriority(
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method;

private:
	TCPSError ListJobs(
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method;

private:
	TCPSError QueryJobs(
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method;

private:
	TCPSError UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

private:
	TCPSError ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method;

private:
	TCPSError GetLogCount(
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method;

private:
	TCPSError LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method;

private:
	TCPSError Login(
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method;

private:
	TCPSError Logout(
				) posting_method;

private:
	TCPSError AddUser(
				IN const UserInfo& userInfo
				) method;

private:
	TCPSError DelUser(
				IN const tcps_Array<tcps_String>& userNames
				) method;

private:
	TCPSError UpdatePassword(
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method;

private:
	TCPSError ManageUser(
				IN const UserInfo& userInfo
				) method;

private:
	TCPSError ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos
				) method;

private:
	TCPSError GetUserInfo(
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method;

private:
	TCPSError ListJTMs(
				OUT tcps_Array<tcps_String>& jtms
				) method;

private:
	TCPSError GetJTMInfo(
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method;

private:
	TCPSError GetGridProperty(
				OUT GridProperty& gridProperty
				) method;

private:
	TCPSError AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

private:
	TCPSError DelSplitter(
				IN const GRID_ProgramInfo& splitter
				) method;

private:
	TCPSError ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method;

private:
	TCPSError ListSplitterParam(
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method;

private:
	TCPSError GetGridStatistics(
				OUT GridStatistics& stat
				) method;

public:
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len
				) posting_callback;
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
		{	return this->SetRedirect_(
							redirectIPP,
							redirectParam.Get(), redirectParam.Length()
							);
		}

public:
	static TCPSError SetRedirect__Batch(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len,
				OUT PGRID_User_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError SetRedirect__Batch(
				IN const PGRID_User_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam,
				OUT PGRID_User_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return GRID_User_S::SetRedirect__Batch(
							sessions, sessionsCount,
							redirectIPP,
							redirectParam.Get(), redirectParam.Length(),
							sendFaileds, failedCount
							);
		}

	/////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 /////
private:
	void OnNetworkMalformed_()
		{	this->CloseSession();	}

public:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	NP_SCATTEREDSession* const m_sessionR;
	IGRID_User_LocalCallback* const m_sessionL;
	struct TIDFlag_
	{
		OSThreadID tid;
		TIDFlag_() : tid(INVALID_OSTHREADID) {}
	};
	TIDFlag_ m_closingTID_L;

private:
	iscm_PostingSendParam m_postingSendParam;

private:
	struct CallSiteL_
	{
		struct TFunc
		{
			PROC fnOnStreamedCallback_L_;
			PROC fnReplyCommitJob;
			TFunc()
				: fnOnStreamedCallback_L_(NULL)
				, fnReplyCommitJob(NULL)
				{}
		};
		TFunc* func_;
		CallSiteL_() : func_(NULL) {}
		~CallSiteL_() { if(func_) tcps_Delete(func_); }
	};
	CallSiteL_ m_callSiteL; // m_sessionL!=NULL时有效

private:
	iscm_MatchingUpdateFlag m_callbackMatchingUpdatedFlag;
	CallbackMatchingFlag m_callbackMatchingFlag;
	TCPSError UpdateCallbackMatchingFlag_();

private:
	TCPSError UDPLink_(
				OUT INT32& servePort,
				OUT INT32& linkKey
				) method;

private:
	TCPSError UDPLinkConfirm_(
				) method;

private:
	TCPSError SetTimeout_(
				IN INT32 recvTimeout,
				IN INT32 sendTimeout
				) posting_method;

private:
	TCPSError SetSessionBufferSize_(
				IN INT32 recvBufBytes,
				IN INT32 sendBufBytes
				) posting_method;

private:
	TCPSError MethodCheck_(
				IN const tcps_Array<tcps_String>& methods,
				IN const tcps_Array<tcps_String>& methodTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) method;

private:
	TCPSError CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback;
};

class PCC_Scatter;
class PCC_Scatter_LS;
class PCC_Scatter_S : public PCC_Scatter_T
{
	friend class PCC_Scatter;
	friend class NP_SCATTEREDSession;
	friend class PCC_Scatter_LS;
private:
	PCC_Scatter_S(const PCC_Scatter_S&);
	PCC_Scatter_S& operator= (const PCC_Scatter_S&);

	typedef PCC_Scatter_S* PPCC_Scatter_S_;

public:
	// TODO: 可以在此处添加PCC_Scatter的自定义成员

private:
	PCC_Scatter_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Scatter_LocalCallback* sessionL);
	~PCC_Scatter_S();
	TCPSError OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				);
	void OnCallbackReady();
	void OnPostingCallReady();
	void OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);
	void OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);

public:
	// 主动关闭会话
	void CloseSession(
				IN TCPSError cause = TCPS_OK
				);

	// 获取对端IPP
	// peerID用于返回对端的peerID_IPP
	IPP GetPeerIPP(
				OUT IPP* peerID = NULL
				) const;

	// 对端是否为进程内客户端
	BOOL IsLocalPeer() const;

	// 判断回调连接是否就绪
	BOOL CallbackIsReady() const;

	// 设置回调调用网络超时，INFINITE表示使用默认值，回调未就绪时返回失败
	TCPSError SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	TCPSError GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				);

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值
	void SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				);
	void GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const;

	// 设置posting执行端参数
	// @maxPendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_PENDING_BUFFER_SIZE）
	// @maxPendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_PENDINGS）
	void SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				);
	void GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const;

	// 设置posting调用端参数
	// @maxBufferingSize[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxBufferingSize /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理会话的posting回调发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount
				);

	// 回调匹配检查支持
public:
	struct CallbackMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 5> CallbackMap;
		CallbackMap cbmap_;
		BOOL matching_Compute;
		BOOL matching_AddMoudle;
		BOOL matching_RemoveModule;
		BOOL matching_ListModules;
		BOOL matching_SetRedirect_;
		void Reset();
		CallbackMatchingFlag();
	};
public:
	// 获取本地与对端回调匹配信息
	const CallbackMatchingFlag& GetCallbackMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
private:
	TCPSError OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError Compute(
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const char* inputUrl, IN INT32 inputUrl_len /*= -1*/,
				IN const char* outputUrl, IN INT32 outputUrl_len /*= -1*/,
				IN const void* moduleParams, IN INT32 moduleParams_len
				) posting_callback;
	TCPSError Compute(
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const char* inputUrl,
				IN const char* outputUrl,
				IN const void* moduleParams, IN INT32 moduleParams_len
				) posting_callback
		{	return this->Compute(
							moduleKey,
							taskKey,
							inputUrl, -1,
							outputUrl, -1,
							moduleParams, moduleParams_len
							);
		}
	TCPSError Compute(
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams
				) posting_callback
		{	return this->Compute(
							moduleKey,
							taskKey,
							inputUrl.Get(), inputUrl.Length(),
							outputUrl.Get(), outputUrl.Length(),
							moduleParams.Get(), moduleParams.Length()
							);
		}

public:
	static TCPSError Compute_Batch(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const char* inputUrl, IN INT32 inputUrl_len /*= -1*/,
				IN const char* outputUrl, IN INT32 outputUrl_len /*= -1*/,
				IN const void* moduleParams, IN INT32 moduleParams_len,
				OUT PPCC_Scatter_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError Compute_Batch(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const char* inputUrl,
				IN const char* outputUrl,
				IN const void* moduleParams, IN INT32 moduleParams_len,
				OUT PPCC_Scatter_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Scatter_S::Compute_Batch(
							sessions, sessionsCount,
							moduleKey,
							taskKey,
							inputUrl, -1,
							outputUrl, -1,
							moduleParams, moduleParams_len,
							sendFaileds, failedCount
							);
		}
	static inline TCPSError Compute_Batch(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams,
				OUT PPCC_Scatter_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Scatter_S::Compute_Batch(
							sessions, sessionsCount,
							moduleKey,
							taskKey,
							inputUrl.Get(), inputUrl.Length(),
							outputUrl.Get(), outputUrl.Length(),
							moduleParams.Get(), moduleParams.Length(),
							sendFaileds, failedCount
							);
		}

private:
	TCPSError OnComputed(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_method;

public:
	TCPSError AddMoudle(
				IN const PCC_ModuleIndex& moduleIndex,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) callback;
	TCPSError AddMoudle(
				IN const PCC_ModuleIndex& moduleIndex,
				IN const PCC_ModuleFile* moudleFiles, IN INT32 moudleFiles_count
				) callback
		{	return this->AddMoudle(
							moduleIndex,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)moudleFiles, moudleFiles_count)
							);
		}

public:
	TCPSError RemoveModule(
				IN INT64 moduleKey
				) callback;

public:
	TCPSError ListModules(
				OUT tcps_Array<PCC_ModuleIndex>& modulesIndex
				) callback;

public:
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len
				) posting_callback;
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
		{	return this->SetRedirect_(
							redirectIPP,
							redirectParam.Get(), redirectParam.Length()
							);
		}

public:
	static TCPSError SetRedirect__Batch(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len,
				OUT PPCC_Scatter_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError SetRedirect__Batch(
				IN const PPCC_Scatter_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam,
				OUT PPCC_Scatter_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Scatter_S::SetRedirect__Batch(
							sessions, sessionsCount,
							redirectIPP,
							redirectParam.Get(), redirectParam.Length(),
							sendFaileds, failedCount
							);
		}

	/////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 /////
private:
	void OnNetworkMalformed_()
		{	this->CloseSession();	}

public:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	NP_SCATTEREDSession* const m_sessionR;
	IPCC_Scatter_LocalCallback* const m_sessionL;
	struct TIDFlag_
	{
		OSThreadID tid;
		TIDFlag_() : tid(INVALID_OSTHREADID) {}
	};
	TIDFlag_ m_closingTID_L;

private:
	iscm_PostingSendParam m_postingSendParam;

private:
	struct CallSiteL_
	{
		struct TFunc
		{
			PROC fnOnStreamedCallback_L_;
			PROC fnCompute;
			PROC fnAddMoudle;
			PROC fnRemoveModule;
			PROC fnListModules;
			TFunc()
				: fnOnStreamedCallback_L_(NULL)
				, fnCompute(NULL)
				, fnAddMoudle(NULL)
				, fnRemoveModule(NULL)
				, fnListModules(NULL)
				{}
		};
		TFunc* func_;
		CallSiteL_() : func_(NULL) {}
		~CallSiteL_() { if(func_) tcps_Delete(func_); }
	};
	CallSiteL_ m_callSiteL; // m_sessionL!=NULL时有效

private:
	iscm_MatchingUpdateFlag m_callbackMatchingUpdatedFlag;
	CallbackMatchingFlag m_callbackMatchingFlag;
	TCPSError UpdateCallbackMatchingFlag_();

private:
	TCPSError UDPLink_(
				OUT INT32& servePort,
				OUT INT32& linkKey
				) method;

private:
	TCPSError UDPLinkConfirm_(
				) method;

private:
	TCPSError SetTimeout_(
				IN INT32 recvTimeout,
				IN INT32 sendTimeout
				) posting_method;

private:
	TCPSError SetSessionBufferSize_(
				IN INT32 recvBufBytes,
				IN INT32 sendBufBytes
				) posting_method;

private:
	TCPSError MethodCheck_(
				IN const tcps_Array<tcps_String>& methods,
				IN const tcps_Array<tcps_String>& methodTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) method;

private:
	TCPSError CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback;
};

class PCC_Service;
class PCC_Service_LS;
class PCC_Service_S : public PCC_Service_T
{
	friend class PCC_Service;
	friend class NP_SCATTEREDSession;
	friend class PCC_Service_LS;
private:
	PCC_Service_S(const PCC_Service_S&);
	PCC_Service_S& operator= (const PCC_Service_S&);

	typedef PCC_Service_S* PPCC_Service_S_;

public:
	// TODO: 可以在此处添加PCC_Service的自定义成员

private:
	PCC_Service_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Service_LocalCallback* sessionL);
	~PCC_Service_S();
	TCPSError OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				);
	void OnCallbackReady();
	void OnPostingCallReady();
	void OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);
	void OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);

public:
	// 主动关闭会话
	void CloseSession(
				IN TCPSError cause = TCPS_OK
				);

	// 获取对端IPP
	// peerID用于返回对端的peerID_IPP
	IPP GetPeerIPP(
				OUT IPP* peerID = NULL
				) const;

	// 对端是否为进程内客户端
	BOOL IsLocalPeer() const;

	// 判断回调连接是否就绪
	BOOL CallbackIsReady() const;

	// 设置回调调用网络超时，INFINITE表示使用默认值，回调未就绪时返回失败
	TCPSError SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	TCPSError GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				);

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值
	void SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				);
	void GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const;

	// 设置posting执行端参数
	// @maxPendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_PENDING_BUFFER_SIZE）
	// @maxPendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_PENDINGS）
	void SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				);
	void GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const;

	// 设置posting调用端参数
	// @maxBufferingSize[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxBufferingSize /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理会话的posting回调发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount
				);

	// 回调匹配检查支持
public:
	struct CallbackMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 2> CallbackMap;
		CallbackMap cbmap_;
		BOOL matching_OnExecuted;
		BOOL matching_SetRedirect_;
		void Reset();
		CallbackMatchingFlag();
	};
public:
	// 获取本地与对端回调匹配信息
	const CallbackMatchingFlag& GetCallbackMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
private:
	TCPSError OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

private:
	TCPSError Login(
				IN const tcps_String& ticket
				) method;

private:
	TCPSError Logout(
				) method;

private:
	TCPSError ListModules(
				IN INT32 moduleType,
				OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
				) method;

private:
	TCPSError GetModuleFile(
				IN INT64 moduleKey,
				IN INT32 fileType,
				OUT tcps_Array<PCC_ModuleFile>& moduleFiles
				) method;

private:
	TCPSError Execute(
				IN INT64 moduleKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams,
				OUT INT64& jobKey
				) method;

public:
	TCPSError OnExecuted(
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len
				) posting_callback;
	TCPSError OnExecuted(
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_callback
		{	return this->OnExecuted(
							jobKey,
							errorCode,
							context.Get(), context.Length()
							);
		}

public:
	static TCPSError OnExecuted_Batch(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				OUT PPCC_Service_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError OnExecuted_Batch(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount,
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				OUT PPCC_Service_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Service_S::OnExecuted_Batch(
							sessions, sessionsCount,
							jobKey,
							errorCode,
							context.Get(), context.Length(),
							sendFaileds, failedCount
							);
		}

private:
	TCPSError QueryJobs(
				IN const tcps_Array<INT64>& jobsKey,
				OUT tcps_Array<ExecuteState>& jobsState
				) method;

private:
	TCPSError CancelJob(
				IN INT64 jobKey
				) posting_method;

public:
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len
				) posting_callback;
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
		{	return this->SetRedirect_(
							redirectIPP,
							redirectParam.Get(), redirectParam.Length()
							);
		}

public:
	static TCPSError SetRedirect__Batch(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len,
				OUT PPCC_Service_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError SetRedirect__Batch(
				IN const PPCC_Service_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam,
				OUT PPCC_Service_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Service_S::SetRedirect__Batch(
							sessions, sessionsCount,
							redirectIPP,
							redirectParam.Get(), redirectParam.Length(),
							sendFaileds, failedCount
							);
		}

	/////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 /////
private:
	void OnNetworkMalformed_()
		{	this->CloseSession();	}

public:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	NP_SCATTEREDSession* const m_sessionR;
	IPCC_Service_LocalCallback* const m_sessionL;
	struct TIDFlag_
	{
		OSThreadID tid;
		TIDFlag_() : tid(INVALID_OSTHREADID) {}
	};
	TIDFlag_ m_closingTID_L;

private:
	iscm_PostingSendParam m_postingSendParam;

private:
	struct CallSiteL_
	{
		struct TFunc
		{
			PROC fnOnStreamedCallback_L_;
			PROC fnOnExecuted;
			TFunc()
				: fnOnStreamedCallback_L_(NULL)
				, fnOnExecuted(NULL)
				{}
		};
		TFunc* func_;
		CallSiteL_() : func_(NULL) {}
		~CallSiteL_() { if(func_) tcps_Delete(func_); }
	};
	CallSiteL_ m_callSiteL; // m_sessionL!=NULL时有效

private:
	iscm_MatchingUpdateFlag m_callbackMatchingUpdatedFlag;
	CallbackMatchingFlag m_callbackMatchingFlag;
	TCPSError UpdateCallbackMatchingFlag_();

private:
	TCPSError UDPLink_(
				OUT INT32& servePort,
				OUT INT32& linkKey
				) method;

private:
	TCPSError UDPLinkConfirm_(
				) method;

private:
	TCPSError SetTimeout_(
				IN INT32 recvTimeout,
				IN INT32 sendTimeout
				) posting_method;

private:
	TCPSError SetSessionBufferSize_(
				IN INT32 recvBufBytes,
				IN INT32 sendBufBytes
				) posting_method;

private:
	TCPSError MethodCheck_(
				IN const tcps_Array<tcps_String>& methods,
				IN const tcps_Array<tcps_String>& methodTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) method;

private:
	TCPSError CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback;
};

class PCC_Toolkit;
class PCC_Toolkit_LS;
class PCC_Toolkit_S : public PCC_Toolkit_T
{
	friend class PCC_Toolkit;
	friend class NP_SCATTEREDSession;
	friend class PCC_Toolkit_LS;
private:
	PCC_Toolkit_S(const PCC_Toolkit_S&);
	PCC_Toolkit_S& operator= (const PCC_Toolkit_S&);

	typedef PCC_Toolkit_S* PPCC_Toolkit_S_;

public:
	// TODO: 可以在此处添加PCC_Toolkit的自定义成员

private:
	PCC_Toolkit_S(NP_SCATTEREDSessionMaker& sessionMaker, NP_SCATTEREDSession* sessionR, IPCC_Toolkit_LocalCallback* sessionL);
	~PCC_Toolkit_S();
	TCPSError OnConnected(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN INT32 sessionCount
				);
	void OnCallbackReady();
	void OnPostingCallReady();
	void OnPeerBroken(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);
	void OnClose(
				IN INT32 sessionKey,
				IN const IPP& peerID_IPP,
				IN TCPSError cause
				);

public:
	// 主动关闭会话
	void CloseSession(
				IN TCPSError cause = TCPS_OK
				);

	// 获取对端IPP
	// peerID用于返回对端的peerID_IPP
	IPP GetPeerIPP(
				OUT IPP* peerID = NULL
				) const;

	// 对端是否为进程内客户端
	BOOL IsLocalPeer() const;

	// 判断回调连接是否就绪
	BOOL CallbackIsReady() const;

	// 设置回调调用网络超时，INFINITE表示使用默认值，回调未就绪时返回失败
	TCPSError SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	TCPSError GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				);

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值
	void SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				);
	void GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const;

	// 设置posting执行端参数
	// @maxPendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_PENDING_BUFFER_SIZE）
	// @maxPendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_PENDINGS）
	void SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				);
	void GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const;

	// 设置posting调用端参数
	// @maxBufferingSize[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxBufferingSize /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxBufferingSize /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理会话的posting回调发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Toolkit_S_* sessions,
				IN INT_PTR sessionsCount
				);

	// 回调匹配检查支持
public:
	struct CallbackMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 1> CallbackMap;
		CallbackMap cbmap_;
		BOOL matching_SetRedirect_;
		void Reset();
		CallbackMatchingFlag();
	};
public:
	// 获取本地与对端回调匹配信息
	const CallbackMatchingFlag& GetCallbackMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
private:
	TCPSError OnStreamedCall_L_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

private:
	TCPSError Login(
				IN const tcps_String& ticket
				) method;

private:
	TCPSError Logout(
				) method;

private:
	TCPSError AddModule(
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method;

private:
	TCPSError AddModuleFile(
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) method;

private:
	TCPSError RemoveModule(
				IN INT64 moduleKey
				) method;

private:
	TCPSError RemoveModuleFiles(
				IN INT64 moduleKey,
				IN INT32 fileType
				) method;

private:
	TCPSError ListModules(
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method;

public:
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len
				) posting_callback;
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
		{	return this->SetRedirect_(
							redirectIPP,
							redirectParam.Get(), redirectParam.Length()
							);
		}

public:
	static TCPSError SetRedirect__Batch(
				IN const PPCC_Toolkit_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const void* redirectParam, IN INT32 redirectParam_len,
				OUT PPCC_Toolkit_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback;
	static inline TCPSError SetRedirect__Batch(
				IN const PPCC_Toolkit_S_* sessions,
				IN INT_PTR sessionsCount,
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam,
				OUT PPCC_Toolkit_S_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_callback
		{	return PCC_Toolkit_S::SetRedirect__Batch(
							sessions, sessionsCount,
							redirectIPP,
							redirectParam.Get(), redirectParam.Length(),
							sendFaileds, failedCount
							);
		}

	/////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 /////
private:
	void OnNetworkMalformed_()
		{	this->CloseSession();	}

public:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	NP_SCATTEREDSession* const m_sessionR;
	IPCC_Toolkit_LocalCallback* const m_sessionL;
	struct TIDFlag_
	{
		OSThreadID tid;
		TIDFlag_() : tid(INVALID_OSTHREADID) {}
	};
	TIDFlag_ m_closingTID_L;

private:
	iscm_PostingSendParam m_postingSendParam;

private:
	struct CallSiteL_
	{
		struct TFunc
		{
			PROC fnOnStreamedCallback_L_;
			TFunc()
				: fnOnStreamedCallback_L_(NULL)
				{}
		};
		TFunc* func_;
		CallSiteL_() : func_(NULL) {}
		~CallSiteL_() { if(func_) tcps_Delete(func_); }
	};
	CallSiteL_ m_callSiteL; // m_sessionL!=NULL时有效

private:
	iscm_MatchingUpdateFlag m_callbackMatchingUpdatedFlag;
	CallbackMatchingFlag m_callbackMatchingFlag;
	TCPSError UpdateCallbackMatchingFlag_();

private:
	TCPSError SetTimeout_(
				IN INT32 recvTimeout,
				IN INT32 sendTimeout
				) posting_method;

private:
	TCPSError SetSessionBufferSize_(
				IN INT32 recvBufBytes,
				IN INT32 sendBufBytes
				) posting_method;

private:
	TCPSError MethodCheck_(
				IN const tcps_Array<tcps_String>& methods,
				IN const tcps_Array<tcps_String>& methodTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) method;

private:
	TCPSError CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback;
};

//[[end_iscm]]

//[[begin_session_wrap]]

//////////////////////////////////////////////////////////////
// NP_SCATTEREDSessionMaker
class BaseFacet_S;
class NP_SCATTEREDSessionMaker : public iscm_IRPCSessionMaker
{
	friend class iscm_SessionRegister;
	friend class NP_SCATTEREDSession;
	friend class BaseFacet_S;
private:
	NP_SCATTEREDSessionMaker(const NP_SCATTEREDSessionMaker&);
	NP_SCATTEREDSessionMaker& operator= (const NP_SCATTEREDSessionMaker&);

public:
	void* const m_userParameter;

public:
	explicit NP_SCATTEREDSessionMaker(void* userParameter = NULL);
	virtual ~NP_SCATTEREDSessionMaker();

public:
	/// 获取某客户端，某接口对象的当前连接数量
	///	注：进程内连接在此未做统计
	/// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP表示查询所有连接
	/// @faceName[in] 接口对象名，NULL表示查询所有接口对象
	/// @return 接口对象数量
	int GetSessionCount(const IPP& clientID_IPP = INVALID_IPP, const char* faceName = NULL)
		{	return m_sessionRegister.GetFaceObjsCount_(clientID_IPP, faceName);	}

public:
	/// 注册会话
	template<typename SESSION>
	TCPSError Register(SESSION* session)
		{	return m_sessionRegister.Register(session);	}

	/// 撤销注册（ISCM框架在会话关闭前会自动撤销已注册的会话）
	template<typename SESSION>
	TCPSError Unregister(SESSION* session)
		{	return m_sessionRegister.Unregister(session);	}

	///////// 下面这些公共成员函数为框架程序使用，请勿直接使用 ////////
public:
	virtual int GetSessionObjSize() const;
	virtual void MakeSessionObj(void* session);
	virtual void OnServiceCreated(
				IN const IPP& serveIPP,
				IN iscm_IRPCServeMan* rpcMan
				);
	virtual void OnServiceClose(
				IN const IPP& serveIPP,
				IN iscm_IRPCServeMan* rpcMan
				);

public:
	void RegisterLocalSession_(iscm_ILocalCallbackBase* session);
	void UnregisterLocalSession_(iscm_ILocalCallbackBase* session);

public:
	CLocker& GetUserRegisteredLock__()
		{	return m_sessionRegister.GetLock__();	}
	iscm_SessionRegister::SessionMap& GetUserRegisteredMap__()
		{	return m_sessionRegister.GetUserRegisteredMap__();	}

public:
	void OnSessionConnect_(INT32* nextSessionKey /*= NULL*/, INT32& sessionCount);
	void OnSessionClosed_();

private:
	mutable CLocker m_lock;
	typedef tcps_set<iscm_ILocalCallbackBase*> LocalSessionsSet;
	LocalSessionsSet m_localSessionsSet; /// 进程内服务会话对象指针表
	iscm_SessionRegister m_sessionRegister;

	tcps_IServeMan* m_serveMan;
	IntSerialKeyGenerater<CNullLocker, INT32> m_sessionKeyGenerater;
	INT32 m_sessionCount;
};

/////////////////////////////////////////////////////////////////////
///////// 本头文件在此之后的代码为框架程序使用，请勿直接使用 ////////
class NP_SCATTEREDSession : public iscm_IRPCSession, public iscm_IUDPSession
{
	friend class iscm_SessionRegister;
	//[[begin_face_friend]]
	friend class GRID_User_S;
	friend class PCC_Scatter_S;
	friend class PCC_Service_S;
	friend class PCC_Toolkit_S;
	//[[end_face_friend]]

private:
	NP_SCATTEREDSession(const NP_SCATTEREDSession&);
	NP_SCATTEREDSession& operator= (const NP_SCATTEREDSession&);

public:
	explicit NP_SCATTEREDSession(NP_SCATTEREDSessionMaker& sessionMaker);
	virtual ~NP_SCATTEREDSession();

	// 下面一组函数用于实现接口iscm_IRPCSession
	// ISCM内部使用
private:
	virtual TCPSError OnConnected(
				IN iscm_IRPCServeMan* serveMan,
				IN tcps_ISession* bindedSession,
				IN const IPP& serveIPP,
				IN const IPP& peerIPP,
				IN INT32 sessionKey,
				IN INT32 sessionCount,
				IN tcps_ITrusteeParameter* trusteeParam /*= NULL*/
				);
	virtual void OnPrepareClose(
				IN TCPSError cause
				);
	virtual BOOL IsSessionBusying() const;
	virtual void OnClose(
				IN TCPSError cause
				);
	virtual void OnPrepareCall(
				IN const iscm_RPCReq& req,
				IN const void* data,
				OUT BOOL& notifyByTaskPool /*= true*/
				);
	virtual TCPSError OnCall(
				OUT BOOL& requireReplyData /*= true*/,
				OUT BOOL& destroySession /*= false*/,
				IN const iscm_RPCReq& req,
				IN const void* data,
				OUT BOOL& dataTransferred /*= false*/,
				IN iscm_IRPCOutfiter* outfiter
				);
	virtual iscm_IRPCServeMan* GetServeMan() const;

private:
	virtual void OnUDPCall(
				IN tcps_Binary& frame
				);

private:
	TCPSError BindCallbackSocket_(
				IN SOCKET sock,
				IN const char* peerIPPTxt
				);
	TCPSError BindPostingSocket_(
				IN SOCKET sock,
				IN const char* peerIPPTxt
				);
	static TCPSError OnRPCCall_(
				IN NP_SCATTEREDSession* thisObj /*= NULL*/,
				IN void* faceObj /*= NULL*/,
				OUT BOOL& requireReplyData /*= true*/,
				IN const void* inParamsData,
				OUT BOOL& dataTransferred /*= false*/,
				IN INT_PTR inParamsDataLen,
				OUT iscm_IRPCOutfiter* outfiter
				);

private:
	//[[begin_method_wrap]]
	static TCPSError Wrap_GRID_User_AddJobProgram(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_DelJobProgram(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_FindJobProgram(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListJobPrograms(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_CommitJob(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_CancelJob(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_GRID_User_SetJobPriority(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListJobs(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_QueryJobs(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_UpdateGrid(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListModules(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_GetLogCount(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_LoadLogInfos(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_Login(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_Logout(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_GRID_User_AddUser(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_DelUser(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_UpdatePassword(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ManageUser(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListAllUsers(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_GetUserInfo(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListJTMs(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_GetJTMInfo(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_GetGridProperty(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_AddSplitter(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_DelSplitter(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListSplitters(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_ListSplitterParam(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_GetGridStatistics(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_UDPLink_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_UDPLinkConfirm_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_GRID_User_SetTimeout_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_GRID_User_SetSessionBufferSize_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_GRID_User_MethodCheck_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Scatter_OnComputed(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Scatter_UDPLink_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Scatter_UDPLinkConfirm_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Scatter_SetTimeout_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Scatter_SetSessionBufferSize_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Scatter_MethodCheck_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_Login(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_Logout(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_ListModules(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_GetModuleFile(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_Execute(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_QueryJobs(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_CancelJob(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Service_UDPLink_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_UDPLinkConfirm_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Service_SetTimeout_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Service_SetSessionBufferSize_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Service_MethodCheck_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_Login(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_Logout(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_AddModule(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_AddModuleFile(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_RemoveModule(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_RemoveModuleFiles(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_ListModules(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	static TCPSError Wrap_PCC_Toolkit_SetTimeout_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Toolkit_SetSessionBufferSize_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_method;
	static TCPSError Wrap_PCC_Toolkit_MethodCheck_(NP_SCATTEREDSession*, void*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) method;
	//[[end_method_wrap]]

private:
	NP_SCATTEREDSessionMaker& m_sessionMaker;
	iscm_IRPCServeMan* m_serveMan;	///< 本会话所属的服务管理器（NULL表示本会话为未连接态，或为待关闭态）
	tcps_ISession* m_bindedSession;
	IPP m_serveIPP;
	IPP m_peerIPP;
	INT32 m_sessionCount;			///< 记录初始化本会话对象时，已经成功连接的会话数量（包括本会话）
	IPP m_peerID_IPP;				///< 对端标识IPP，为对方的ISCM服务IPP（若为普通ISCM客户端程序，则端口为0）
	tcps_String m_bindedInterface;	///< 与本会话绑定的接口类型名

	// 下面一组变量用于支持反向回调（callback）
private:
	DataOutfiter m_callbackOutfiter;
	// 用于向client端进行callback调用
	// m_callbackRequester.face_为NULL或处于未连接状态，表示无callback调用，或者client还未启动callback
	AutoDeleteInterface<iscm_IRequester> m_callbackRequester;

	struct UDPSite_
	{
		INT32 localPort_;
		SOCKET sock_;		///< listen UDP socket，临时记录，握手完成后由iscm_IUDPServeMan管理
		UDPSite_() : localPort_(-1), sock_(INVALID_SOCKET) {}
		BOOL IsOn() const
		{
			BOOL is = localPort_>0;
			ASSERT(is || !SockValid(sock_));
			return is;
		}
		BOOL IsLinking() const
			{	return localPort_>0 && SockValid(sock_);	}
		BOOL IsLinked() const
			{	return localPort_>0 && !SockValid(sock_);	}
	};
	UDPSite_ m_udpSite;

private:
	struct PostingProxy_ : public iscm_IPostingCallerMan::INotifier
	{
		NP_SCATTEREDSession& session_;
		INT32 callerKey_; // 为iscm_IPostingCallerMan中的会话标识

		explicit PostingProxy_(NP_SCATTEREDSession& session)
			: session_(session), callerKey_(0)
			{}

		virtual void OnSessionClose(
					IN INT32 callerKey,
					IN TCPSError cause
					)
		{
			(void)callerKey;
			ASSERT(callerKey_ == callerKey);
			callerKey_ = 0;
			session_.CloseSession_(cause);
		}

		virtual void OnSessionPost(
					IN INT32 callerKey,
					IN const void* data,
					IN INT_PTR dataLen,
					OUT BOOL& dataTransferred /*= false*/
					)
		{
			(void)callerKey;
			ASSERT(callerKey_ == callerKey);
			while(true)
			{
				BOOL requireReplyData = true;
				TCPSError err = OnRPCCall_(&session_, NULL, requireReplyData, data, dataTransferred, dataLen, NULL);
				ASSERT(TCPS_OK!=err || !requireReplyData);
				if(TCPS_ERR_POSTING_PENDING_FULL == err)
				{
					ASSERT(!dataTransferred);
					Sleep(1);
					continue;
				}
				else
					break;
			}
		}
	};
	friend struct PostingProxy_;
	PostingProxy_ m_postingProxy;

private:
	//[[begin_face_member]]
	enum FaceTypeValue
	{
		ftv_InvalidFace,
		ftv_GRID_User,
		ftv_PCC_Scatter,
		ftv_PCC_Service,
		ftv_PCC_Toolkit,
	};
	union
	{
		void* m_sessionDummyPtr;
		GRID_User_S* m_gRID_User;
		PCC_Scatter_S* m_pCC_Scatter;
		PCC_Service_S* m_pCC_Service;
		PCC_Toolkit_S* m_pCC_Toolkit;
	};
public:
	GRID_User_S* GetGRID_User()
		{	return ("GRID_User"==m_bindedInterface ? m_gRID_User : NULL);	}
	PCC_Scatter_S* GetPCC_Scatter()
		{	return ("PCC_Scatter"==m_bindedInterface ? m_pCC_Scatter : NULL);	}
	PCC_Service_S* GetPCC_Service()
		{	return ("PCC_Service"==m_bindedInterface ? m_pCC_Service : NULL);	}
	PCC_Toolkit_S* GetPCC_Toolkit()
		{	return ("PCC_Toolkit"==m_bindedInterface ? m_pCC_Toolkit : NULL);	}
	//[[end_face_member]]

public:
	// 获取会话流水标识号（0为无效号）
	INT32 GetSessionKey() const;

private:
	void CloseSession_(
				IN TCPSError cause /*= TCPS_OK*/
				);
	void OnNetworkMalformed_()
		{	this->CloseSession_(TCPS_ERR_NETWORK_MALFORMED);	}

	// 下面流式调用定义仅用于非C++语言的辅助实现
private:
	iscm_ServerStreamedCallSite m_streamedCallSite;

private:
	typedef tcps_QuickStringMap<CPtrStrA, FaceTypeValue> FTVMap_;
	static FTVMap_& GetFTVMap_();
	static FTVMap_* sm_FTVMap;
	static void InitFTVMap_();

private:
	typedef TCPSError (*FMethodHandler_)(IN NP_SCATTEREDSession* thisObj, IN void* faceObj, IN iscm_PeerCallFlags peerCallFlags, IN OUT const BYTE*& ptrInParams, IN OUT INT_PTR& ptrInParamsLen, OUT iscm_IRPCOutfiter* outfiter);
	struct MethodSite_
	{
		FMethodHandler_ handler;
		BOOL isPosting; // 是否为 posting 修饰的方法/回调
		explicit MethodSite_(FMethodHandler_ hd = NULL, BOOL pst = false)
			: handler(hd), isPosting(pst)
			{}
	};
	typedef tcps_QuickStringMap<CPtrStrA, MethodSite_> MethodCallMap_;
	static MethodCallMap_& GetMethodCallMap_();
	static MethodCallMap_* sm_methodCallMap;

private:
	mutable CLocker m_methodCallLock;
	struct PostingTask_ : public NPBaseTask
	{
		NP_SCATTEREDSession& session_;
		iscm_PeerCallFlags peerCallFlags_;
		void* baseInParamsData_;
		const BYTE* inParamsData_;
		INT_PTR inParamsDataLen_;
		FMethodHandler_ handler_;

		explicit PostingTask_(NP_SCATTEREDSession& session) : session_(session) {}
		virtual ~PostingTask_() {}
		virtual void OnPoolTask();
	};
	friend struct PostingTask_;
	struct PostingTaskSite_
	{
		mutable CLocker lock;
		INT_PTR callingsDataBytes;
		INT32 callingCount;
		NPBaseTask* head;
		NPBaseTask* tail;
		PostingTaskSite_() : callingsDataBytes(0), callingCount(0), head(NULL), tail(NULL) {}
	};
	PostingTaskSite_ m_postingMethods;

// 下面一组变量用于查错和调试
#ifdef _DEBUG
private:
	LTMSEL mutable m_closingPromptTime;	///< 上一次关闭警告打印时间（INVALID_UTC_MSEL表示不处于正关闭态）
	union
	{
	OSThreadID m_callingPostingTaskTID;	///< 正在调用本会话posting method的线程ID
	void* m_callingPostingTaskTID_hex;
	INT_PTR m_callingPostingTaskTID_int;
	};
#endif

private:
	iscm_PostingPendingParam m_postingPendingParam;
};
//[[end_session_wrap]]

#endif	// #ifndef _NP_SCATTEREDSession_h_
