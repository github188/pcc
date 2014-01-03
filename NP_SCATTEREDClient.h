// NP_SCATTEREDClient.h
//
// 注意: 此文件为工具生成，请不要修改

#ifndef _NP_SCATTEREDClient_h_
#define _NP_SCATTEREDClient_h_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
	#pragma once
#endif

#include "iscm_helper.h"
#include "iscm_smart_container.h"
#include "NP_SCATTEREDTypesDefine.h"

//[[begin_iscm]]

#ifndef GRID_User_defined
#define GRID_User_defined
class GRID_User_S;
class GRID_User_RC;
class GRID_User : public IGRID_User_LocalCallback
{
	friend class GRID_User_S;
	friend class GRID_User_RC;
private:
	GRID_User(const GRID_User&);
	GRID_User& operator= (const GRID_User&);

	typedef GRID_User* PGRID_User_;

public:
	// initNetworkSingletons，是否初始化网络相关单件（若只用于连接进程内服务，可传false）
	explicit GRID_User(BOOL initNetworkSingletons = true);
	virtual ~GRID_User();

protected:
	// 析构RPC请求对象
	// 注意：若有派生类，必须且只能在派生类的析构函数的最后调用，
	//		 以保证派生类对象继承的通知虚函数总被正确调用。
	void DestroyRequester();

	// 连接后通知虚函数
	// 若返回失败，则立即触发OnClose()，且继续异步尝试连接
	// TODO: 若需处理连接动作，请在派生类中重载OnConnected()
	virtual TCPSError OnConnected()
		{	return TCPS_OK;	}

	// 对端断线通知虚函数（在OnClose()之前被调用）
	// 注: 只有对端关闭或网络断线才会触发，Client对象被析构或设置INVALID_IPP不会触发
	// TODO: 若需处理此动作，请在派生类中重载OnPeerBroken()
	virtual void OnPeerBroken()
		{	}

	// 连接关闭通知虚函数（在OnPeerBroken()之后被调用）
	// 注: 所有关闭情况都会触发
	// TODO: 若需处理关闭动作，请在派生类中重载OnClose()
	virtual void OnClose()
		{	}

	// 连接超时通知虚函数
	// TODO: 若需处理连接超时动作，请在派生类中重载OnConnectTimeout()
	virtual void OnConnectTimeout(IN TCPSError cause)
		{	(void)cause;	}

public:
	// 设置正向服务参数
	// @serveIPP[in] 第一备选服务器IPP，INVALID_IPP表示取消连接
	// @sessionFlags[in] 会话标志，为以下值的位组合
	//		ISCMC_NO_CALLBACK、ISCMC_ASYNC_CONNECT、ISCMC_UDP_POSTING、ISCMC_NO_ASYNC_RECONNECT、ISCMC_NO_POSTING_CHANNEL
	// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP使用自动值（标识端口使用0）
	// @secondaryServeIPP[in] 第二备选服务IPP（取消连接时本参数被忽略）
	// @tertiaryServeIPP[in] 第三备选服务IPP（取消连接时本参数被忽略）
	// @reconnectIntervalMSELs[in] 网络重建间隔时间
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	// 设置连接保持标志
	//		注：调用SetServeIPP()时，若指定了ISCMC_NO_ASYNC_RECONNECT，
	//			则连接保持标志为false；否则为true
	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	// 对端是否为进程内服务
	BOOL IsLocalPeer() const;

	// 准备断开网络，在有巨量会话对象需要关闭网络时，先批量调用一次PrepareDisconnect()，
	// 调用PrepareDisconnect()后，再调用SetServeIPP(INVALID_IPP)，或析构接口对象
	void PrepareDisconnect();

	// 设置网络超时，INFINITE表示使用默认值（网络对端也会被设置）
	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值（网络对端也会被设置）
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
	// @maxSendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理客户端对象的posting方法发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PGRID_User_* clients,
				IN INT_PTR clientsCount
				);

	// 方法匹配检查支持
public:
	struct MethodMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 33> MethodMap;
		MethodMap mmap_;
		BOOL matching_AddJobProgram;
		BOOL matching_DelJobProgram;
		BOOL matching_FindJobProgram;
		BOOL matching_ListJobPrograms;
		BOOL matching_CommitJob;
		BOOL matching_CancelJob;
		BOOL matching_SetJobPriority;
		BOOL matching_ListJobs;
		BOOL matching_QueryJobs;
		BOOL matching_UpdateGrid;
		BOOL matching_ListModules;
		BOOL matching_GetLogCount;
		BOOL matching_LoadLogInfos;
		BOOL matching_Login;
		BOOL matching_Logout;
		BOOL matching_AddUser;
		BOOL matching_DelUser;
		BOOL matching_UpdatePassword;
		BOOL matching_ManageUser;
		BOOL matching_ListAllUsers;
		BOOL matching_GetUserInfo;
		BOOL matching_ListJTMs;
		BOOL matching_GetJTMInfo;
		BOOL matching_GetGridProperty;
		BOOL matching_AddSplitter;
		BOOL matching_DelSplitter;
		BOOL matching_ListSplitters;
		BOOL matching_ListSplitterParam;
		BOOL matching_GetGridStatistics;
		BOOL matching_UDPLink_;
		BOOL matching_UDPLinkConfirm_;
		BOOL matching_SetTimeout_;
		BOOL matching_SetSessionBufferSize_;
		void Reset();
		MethodMatchingFlag();
	};
public:
	// 获取本地与对端方法匹配信息
	const MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
public:
	TCPSError GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				) const;

	virtual TCPSError GetStreamedCallbackTypeInfo_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)typeInfo; (void)infoLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

	/// 流式方法调用
	/// @methodName[in] 方法名，必须提供
	/// @nameLen[in] 方法名长度，-1表示方法名是一'\0'结束符字符串
	/// @data[in] 方法参数数据（dataLen==0时可以为NULL）
	/// @dataLen[in] 方法参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据（无OUT参数的方法可以传NULL）
	///			注：若*replyData返回非NULL，则必须使用tcps_Free()释放
	/// @replyLen[out] 返回OUT型参数数据长度（无OUT参数的方法可以传NULL）
	/// @return TCPSError
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	/// 流式回调调用通知（在调用普通的回调函数之前被调用）
	/// @callbackName[in] 回调名，必须提供
	///		注：callbackName==NULL时，用于检测StreamedCallback_()是否被派生，
	///			派生类的StreamedCallback_()中，最前面总这样实现：
	///			{
	///				if(NULL == callbackName)
	///					return TCPS_ERR_INVALID_PARAM; // 必须返回非TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED
	///				...
	///			}
	/// @nameLen[in] 回调名长度，-1表示回调名为'\0'结束符字符串
	/// @data[in] 回调IN型参数数据（dataLen==0时可能为NULL）
	/// @dataLen[in] 回调IN型参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据，传入时已置为NULL，
	///			若返回非NULL，则必须使用tcps_Alloc()申请内存；无OUT参数的回调时可返回NULL
	/// @replyLen[out] 返回OUT型参数数据长度，传入时已置为0
	/// @return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED表示忽略此次流式回调调用，
	///			框架若收到此返回值，则会继续调用普通的回调函数；
	///			其它值，直接返回给服务器端。
	///			<<<注：若遇到IN型参数数据无法被正确解析，应该返回TCPS_ERR_MALFORMED_REQ>>>
	virtual TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT void*& replyData /*= NULL*/,
				OUT INT_PTR& replyLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)data; (void)dataLen;
		(void)replyData; (void)replyLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;
	TCPSError AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const GRID_ProgramFile* files, IN INT32 files_count
				) method
		{	return this->AddJobProgram(
							programInfo,
							tcps_Array<GRID_ProgramFile>(xat_bind, (GRID_ProgramFile*)files, files_count)
							);
		}

public:
	TCPSError DelJobProgram(
				IN const tcps_Array<INT64>& programKeys
				) method;
	TCPSError DelJobProgram(
				IN const INT64* programKeys, IN INT32 programKeys_count
				) method
		{	return this->DelJobProgram(
							tcps_Array<INT64>(xat_bind, (INT64*)programKeys, programKeys_count)
							);
		}

public:
	TCPSError FindJobProgram(
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method;

public:
	TCPSError ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms
				) method;

public:
	TCPSError CommitJob(
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method;

protected:
	virtual TCPSError ReplyCommitJob(
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)jobKey; (void)replyCode; (void)context;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError CancelJob(
				IN const tcps_Array<INT64>& jobKeys
				) posting_method;
	TCPSError CancelJob(
				IN const INT64* jobKeys, IN INT32 jobKeys_count
				) posting_method
		{	return this->CancelJob(
							tcps_Array<INT64>(xat_bind, (INT64*)jobKeys, jobKeys_count)
							);
		}

public:
	static TCPSError CancelJob_Batch(
				IN const PGRID_User_* clients,
				IN INT_PTR clientsCount,
				IN const tcps_Array<INT64>& jobKeys,
				OUT PGRID_User_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method;
	static inline TCPSError CancelJob_Batch(
				IN const PGRID_User_* clients,
				IN INT_PTR clientsCount,
				IN const INT64* jobKeys, IN INT32 jobKeys_count,
				OUT PGRID_User_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method
		{	return GRID_User::CancelJob_Batch(
							clients, clientsCount,
							tcps_Array<INT64>(xat_bind, (INT64*)jobKeys, jobKeys_count),
							sendFaileds, failedCount
							);
		}

public:
	TCPSError SetJobPriority(
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method;

public:
	TCPSError ListJobs(
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method;

public:
	TCPSError QueryJobs(
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method;
	TCPSError QueryJobs(
				IN const INT64* jobKeys, IN INT32 jobKeys_count,
				OUT tcps_Array<JobReport>& jobReports
				) method
		{	return this->QueryJobs(
							tcps_Array<INT64>(xat_bind, (INT64*)jobKeys, jobKeys_count),
							jobReports
							);
		}

public:
	TCPSError UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;
	TCPSError UpdateGrid(
				IN const GRID_ProgramFile* files, IN INT32 files_count
				) method
		{	return this->UpdateGrid(
							tcps_Array<GRID_ProgramFile>(xat_bind, (GRID_ProgramFile*)files, files_count)
							);
		}

public:
	TCPSError ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method;

public:
	TCPSError GetLogCount(
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method;

public:
	TCPSError LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method;

public:
	TCPSError Login(
				IN const char* userName, IN INT32 userName_len /*= -1*/,
				IN const char* password, IN INT32 password_len /*= -1*/
				) method;
	TCPSError Login(
				IN const char* userName,
				IN const char* password
				) method
		{	return this->Login(
							userName, -1,
							password, -1
							);
		}
	TCPSError Login(
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method
		{	return this->Login(
							userName.Get(), userName.Length(),
							password.Get(), password.Length()
							);
		}

public:
	TCPSError Logout(
				) posting_method;

public:
	static TCPSError Logout_Batch(
				IN const PGRID_User_* clients,
				IN INT_PTR clientsCount,
				OUT PGRID_User_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method;

public:
	TCPSError AddUser(
				IN const UserInfo& userInfo
				) method;

public:
	TCPSError DelUser(
				IN const tcps_Array<tcps_String>& userNames
				) method;
	TCPSError DelUser(
				IN const tcps_String* userNames, IN INT32 userNames_count
				) method
		{	return this->DelUser(
							tcps_Array<tcps_String>(xat_bind, (tcps_String*)userNames, userNames_count)
							);
		}

public:
	TCPSError UpdatePassword(
				IN const char* oldPassword, IN INT32 oldPassword_len /*= -1*/,
				IN const char* newPassword, IN INT32 newPassword_len /*= -1*/
				) method;
	TCPSError UpdatePassword(
				IN const char* oldPassword,
				IN const char* newPassword
				) method
		{	return this->UpdatePassword(
							oldPassword, -1,
							newPassword, -1
							);
		}
	TCPSError UpdatePassword(
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method
		{	return this->UpdatePassword(
							oldPassword.Get(), oldPassword.Length(),
							newPassword.Get(), newPassword.Length()
							);
		}

public:
	TCPSError ManageUser(
				IN const UserInfo& userInfo
				) method;

public:
	TCPSError ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos
				) method;

public:
	TCPSError GetUserInfo(
				IN const char* userName, IN INT32 userName_len /*= -1*/,
				OUT UserInfo& userInfo
				) method;
	TCPSError GetUserInfo(
				IN const char* userName,
				OUT UserInfo& userInfo
				) method
		{	return this->GetUserInfo(
							userName, -1,
							userInfo
							);
		}
	TCPSError GetUserInfo(
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method
		{	return this->GetUserInfo(
							userName.Get(), userName.Length(),
							userInfo
							);
		}

public:
	TCPSError ListJTMs(
				OUT tcps_Array<tcps_String>& jtms
				) method;

public:
	TCPSError GetJTMInfo(
				IN const char* jtm, IN INT32 jtm_len /*= -1*/,
				OUT JTMInfo& jtmInfo
				) method;
	TCPSError GetJTMInfo(
				IN const char* jtm,
				OUT JTMInfo& jtmInfo
				) method
		{	return this->GetJTMInfo(
							jtm, -1,
							jtmInfo
							);
		}
	TCPSError GetJTMInfo(
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method
		{	return this->GetJTMInfo(
							jtm.Get(), jtm.Length(),
							jtmInfo
							);
		}

public:
	TCPSError GetGridProperty(
				OUT GridProperty& gridProperty
				) method;

public:
	TCPSError AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;
	TCPSError AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const GRID_ProgramFile* files, IN INT32 files_count
				) method
		{	return this->AddSplitter(
							splitter,
							tcps_Array<GRID_ProgramFile>(xat_bind, (GRID_ProgramFile*)files, files_count)
							);
		}

public:
	TCPSError DelSplitter(
				IN const GRID_ProgramInfo& splitter
				) method;

public:
	TCPSError ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method;

public:
	TCPSError ListSplitterParam(
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method;

public:
	TCPSError GetGridStatistics(
				OUT GridStatistics& stat
				) method;

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)redirectIPP; (void)redirectParam;
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	void* m_base_v_tab_ptr;
	void* m_derived_v_tab_ptr;
	GRID_User_RC* m_faceR;

private:
	struct CallSiteL_
	{
		BOOL needUpdateFlags;
		MethodMatchingFlag matchingFlags;
		PROC fnOnStreamedCall_L_;
		PROC fnAddJobProgram;
		PROC fnDelJobProgram;
		PROC fnFindJobProgram;
		PROC fnListJobPrograms;
		PROC fnCommitJob;
		PROC fnCancelJob;
		PROC fnSetJobPriority;
		PROC fnListJobs;
		PROC fnQueryJobs;
		PROC fnUpdateGrid;
		PROC fnListModules;
		PROC fnGetLogCount;
		PROC fnLoadLogInfos;
		PROC fnLogin;
		PROC fnLogout;
		PROC fnAddUser;
		PROC fnDelUser;
		PROC fnUpdatePassword;
		PROC fnManageUser;
		PROC fnListAllUsers;
		PROC fnGetUserInfo;
		PROC fnListJTMs;
		PROC fnGetJTMInfo;
		PROC fnGetGridProperty;
		PROC fnAddSplitter;
		PROC fnDelSplitter;
		PROC fnListSplitters;
		PROC fnListSplitterParam;
		PROC fnGetGridStatistics;
		CallSiteL_()
			: needUpdateFlags(true)
			, fnOnStreamedCall_L_(NULL)
			, fnAddJobProgram(NULL)
			, fnDelJobProgram(NULL)
			, fnFindJobProgram(NULL)
			, fnListJobPrograms(NULL)
			, fnCommitJob(NULL)
			, fnCancelJob(NULL)
			, fnSetJobPriority(NULL)
			, fnListJobs(NULL)
			, fnQueryJobs(NULL)
			, fnUpdateGrid(NULL)
			, fnListModules(NULL)
			, fnGetLogCount(NULL)
			, fnLoadLogInfos(NULL)
			, fnLogin(NULL)
			, fnLogout(NULL)
			, fnAddUser(NULL)
			, fnDelUser(NULL)
			, fnUpdatePassword(NULL)
			, fnManageUser(NULL)
			, fnListAllUsers(NULL)
			, fnGetUserInfo(NULL)
			, fnListJTMs(NULL)
			, fnGetJTMInfo(NULL)
			, fnGetGridProperty(NULL)
			, fnAddSplitter(NULL)
			, fnDelSplitter(NULL)
			, fnListSplitters(NULL)
			, fnListSplitterParam(NULL)
			, fnGetGridStatistics(NULL)
			{}
		void Reset()
		{
			needUpdateFlags = true;
			matchingFlags.Reset();
			fnOnStreamedCall_L_ = NULL;
			fnAddJobProgram = NULL;
			fnDelJobProgram = NULL;
			fnFindJobProgram = NULL;
			fnListJobPrograms = NULL;
			fnCommitJob = NULL;
			fnCancelJob = NULL;
			fnSetJobPriority = NULL;
			fnListJobs = NULL;
			fnQueryJobs = NULL;
			fnUpdateGrid = NULL;
			fnListModules = NULL;
			fnGetLogCount = NULL;
			fnLoadLogInfos = NULL;
			fnLogin = NULL;
			fnLogout = NULL;
			fnAddUser = NULL;
			fnDelUser = NULL;
			fnUpdatePassword = NULL;
			fnManageUser = NULL;
			fnListAllUsers = NULL;
			fnGetUserInfo = NULL;
			fnListJTMs = NULL;
			fnGetJTMInfo = NULL;
			fnGetGridProperty = NULL;
			fnAddSplitter = NULL;
			fnDelSplitter = NULL;
			fnListSplitters = NULL;
			fnListSplitterParam = NULL;
			fnGetGridStatistics = NULL;
		}
	};
	struct LocalMakingFlag_
	{
		BOOL making;
		BOOL failed;
		LocalMakingFlag_() : making(false), failed(false) {}
	};
	LocalMakingFlag_ m_localMakingFlag;	// 用于本地服务递归调用处理
	IGRID_User_LocalMethod* m_faceL;
	CallSiteL_* m_callSiteL;	// m_faceL!=NULL时有效
	IPP m_localServeIPP;		// m_faceL!=NULL时有效

private:
	OSThreadID m_closingTID;	// 正在关闭会话的用户线程（即正析构Client对象或调用SetServeIPP(NEW_IPP/INVALID_IPP)的线程）

public:
	// 不要使用此函数，仅供框架使用
	OSThreadID GetClosingTID_() const
		{	return m_closingTID;	}

private:
	DWORD m_connectTimeout;
	DWORD m_recvTimeout;
	DWORD m_sendTimeout;
	tcps_SockRecvSendBufParam m_sockRecvSendBufParam;
	iscm_PostingPendingParam m_postingPendingParam;
	iscm_PostingSendParam m_postingSendParam;

private:
	CLocker m_lock;
	BOOL m_streamedCallbackMap_IsInited;
	typedef tcps_QuickStringMap<
				CPtrStrA/*callback_name*/,
				BOOL /*callback_is_matched*/,
				1> StreamedCallbackMap;
	StreamedCallbackMap m_streamedCallbackMap;

public:
	/// 判断流式回调方的回调版本是否匹配（本函数内部会调用this->GetStreamedCallbackTypeInfo_()）
	BOOL IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				);

private:
	TCPSError SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	void DestroyRequester_(
				IN BOOL byLocalPeer
				);

	void OnNetworkMalformed_();
	virtual void CloseSession_();

	virtual PROC FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				);

	static TCPSError OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	static TCPSError Local_ReplyCommitJob(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback;
};
#endif // #ifndef GRID_User_defined

#ifndef PCC_Deploy_defined
#define PCC_Deploy_defined
class PCC_Deploy_S;
class PCC_Deploy_RC;
class PCC_Deploy : public IPCC_Deploy_LocalCallback
{
	friend class PCC_Deploy_S;
	friend class PCC_Deploy_RC;
private:
	PCC_Deploy(const PCC_Deploy&);
	PCC_Deploy& operator= (const PCC_Deploy&);

	typedef PCC_Deploy* PPCC_Deploy_;

public:
	// initNetworkSingletons，是否初始化网络相关单件（若只用于连接进程内服务，可传false）
	explicit PCC_Deploy(BOOL initNetworkSingletons = true);
	virtual ~PCC_Deploy();

protected:
	// 析构RPC请求对象
	// 注意：若有派生类，必须且只能在派生类的析构函数的最后调用，
	//		 以保证派生类对象继承的通知虚函数总被正确调用。
	void DestroyRequester();

	// 连接后通知虚函数
	// 若返回失败，则立即触发OnClose()，且继续异步尝试连接
	// TODO: 若需处理连接动作，请在派生类中重载OnConnected()
	virtual TCPSError OnConnected()
		{	return TCPS_OK;	}

	// 对端断线通知虚函数（在OnClose()之前被调用）
	// 注: 只有对端关闭或网络断线才会触发，Client对象被析构或设置INVALID_IPP不会触发
	// TODO: 若需处理此动作，请在派生类中重载OnPeerBroken()
	virtual void OnPeerBroken()
		{	}

	// 连接关闭通知虚函数（在OnPeerBroken()之后被调用）
	// 注: 所有关闭情况都会触发
	// TODO: 若需处理关闭动作，请在派生类中重载OnClose()
	virtual void OnClose()
		{	}

	// 连接超时通知虚函数
	// TODO: 若需处理连接超时动作，请在派生类中重载OnConnectTimeout()
	virtual void OnConnectTimeout(IN TCPSError cause)
		{	(void)cause;	}

public:
	// 设置正向服务参数
	// @serveIPP[in] 第一备选服务器IPP，INVALID_IPP表示取消连接
	// @sessionFlags[in] 会话标志，为以下值的位组合
	//		ISCMC_NO_CALLBACK、ISCMC_ASYNC_CONNECT、ISCMC_UDP_POSTING、ISCMC_NO_ASYNC_RECONNECT、ISCMC_NO_POSTING_CHANNEL
	// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP使用自动值（标识端口使用0）
	// @secondaryServeIPP[in] 第二备选服务IPP（取消连接时本参数被忽略）
	// @tertiaryServeIPP[in] 第三备选服务IPP（取消连接时本参数被忽略）
	// @reconnectIntervalMSELs[in] 网络重建间隔时间
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	// 设置连接保持标志
	//		注：调用SetServeIPP()时，若指定了ISCMC_NO_ASYNC_RECONNECT，
	//			则连接保持标志为false；否则为true
	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	// 对端是否为进程内服务
	BOOL IsLocalPeer() const;

	// 准备断开网络，在有巨量会话对象需要关闭网络时，先批量调用一次PrepareDisconnect()，
	// 调用PrepareDisconnect()后，再调用SetServeIPP(INVALID_IPP)，或析构接口对象
	void PrepareDisconnect();

	// 设置网络超时，INFINITE表示使用默认值（网络对端也会被设置）
	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值（网络对端也会被设置）
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
	// @maxSendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理客户端对象的posting方法发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Deploy_* clients,
				IN INT_PTR clientsCount
				);

	// 方法匹配检查支持
public:
	struct MethodMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 16> MethodMap;
		MethodMap mmap_;
		BOOL matching_Login;
		BOOL matching_Logout;
		BOOL matching_CreateTrunk;
		BOOL matching_RemoveTrunk;
		BOOL matching_ListTrunk;
		BOOL matching_AddAuthCenter;
		BOOL matching_RemoveAuthCenter;
		BOOL matching_ListAuthCenter;
		BOOL matching_FindAuthCenter;
		BOOL matching_AddModule;
		BOOL matching_AddModuleFile;
		BOOL matching_RemoveModule;
		BOOL matching_RemoveModuleFiles;
		BOOL matching_ListModules;
		BOOL matching_SetTimeout_;
		BOOL matching_SetSessionBufferSize_;
		void Reset();
		MethodMatchingFlag();
	};
public:
	// 获取本地与对端方法匹配信息
	const MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
public:
	TCPSError GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				) const;

	virtual TCPSError GetStreamedCallbackTypeInfo_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)typeInfo; (void)infoLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

	/// 流式方法调用
	/// @methodName[in] 方法名，必须提供
	/// @nameLen[in] 方法名长度，-1表示方法名是一'\0'结束符字符串
	/// @data[in] 方法参数数据（dataLen==0时可以为NULL）
	/// @dataLen[in] 方法参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据（无OUT参数的方法可以传NULL）
	///			注：若*replyData返回非NULL，则必须使用tcps_Free()释放
	/// @replyLen[out] 返回OUT型参数数据长度（无OUT参数的方法可以传NULL）
	/// @return TCPSError
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	/// 流式回调调用通知（在调用普通的回调函数之前被调用）
	/// @callbackName[in] 回调名，必须提供
	///		注：callbackName==NULL时，用于检测StreamedCallback_()是否被派生，
	///			派生类的StreamedCallback_()中，最前面总这样实现：
	///			{
	///				if(NULL == callbackName)
	///					return TCPS_ERR_INVALID_PARAM; // 必须返回非TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED
	///				...
	///			}
	/// @nameLen[in] 回调名长度，-1表示回调名为'\0'结束符字符串
	/// @data[in] 回调IN型参数数据（dataLen==0时可能为NULL）
	/// @dataLen[in] 回调IN型参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据，传入时已置为NULL，
	///			若返回非NULL，则必须使用tcps_Alloc()申请内存；无OUT参数的回调时可返回NULL
	/// @replyLen[out] 返回OUT型参数数据长度，传入时已置为0
	/// @return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED表示忽略此次流式回调调用，
	///			框架若收到此返回值，则会继续调用普通的回调函数；
	///			其它值，直接返回给服务器端。
	///			<<<注：若遇到IN型参数数据无法被正确解析，应该返回TCPS_ERR_MALFORMED_REQ>>>
	virtual TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT void*& replyData /*= NULL*/,
				OUT INT_PTR& replyLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)data; (void)dataLen;
		(void)replyData; (void)replyLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError Login(
				IN const char* ticket, IN INT32 ticket_len /*= -1*/
				) method;
	TCPSError Login(
				IN const char* ticket
				) method
		{	return this->Login(
							ticket, -1
							);
		}
	TCPSError Login(
				IN const tcps_String& ticket
				) method
		{	return this->Login(
							ticket.Get(), ticket.Length()
							);
		}

public:
	TCPSError Logout(
				) method;

public:
	TCPSError CreateTrunk(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/
				) method;
	TCPSError CreateTrunk(
				IN const char* trunk
				) method
		{	return this->CreateTrunk(
							trunk, -1
							);
		}
	TCPSError CreateTrunk(
				IN const tcps_String& trunk
				) method
		{	return this->CreateTrunk(
							trunk.Get(), trunk.Length()
							);
		}

public:
	TCPSError RemoveTrunk(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/
				) method;
	TCPSError RemoveTrunk(
				IN const char* trunk
				) method
		{	return this->RemoveTrunk(
							trunk, -1
							);
		}
	TCPSError RemoveTrunk(
				IN const tcps_String& trunk
				) method
		{	return this->RemoveTrunk(
							trunk.Get(), trunk.Length()
							);
		}

public:
	TCPSError ListTrunk(
				OUT tcps_Array<tcps_String>& trunks
				) method;

public:
	TCPSError AddAuthCenter(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleTag& authTag,
				IN const tcps_Array<PCC_ModuleFile>& files
				) method;
	TCPSError AddAuthCenter(
				IN const char* trunk,
				IN const PCC_ModuleTag& authTag,
				IN const tcps_Array<PCC_ModuleFile>& files
				) method
		{	return this->AddAuthCenter(
							trunk, -1,
							authTag,
							files
							);
		}
	TCPSError AddAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag,
				IN const tcps_Array<PCC_ModuleFile>& files
				) method
		{	return this->AddAuthCenter(
							trunk.Get(), trunk.Length(),
							authTag,
							files
							);
		}
	TCPSError AddAuthCenter(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleTag& authTag,
				IN const PCC_ModuleFile* files, IN INT32 files_count
				) method
		{	return this->AddAuthCenter(
							trunk, trunk_len,
							authTag,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)files, files_count)
							);
		}

public:
	TCPSError RemoveAuthCenter(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleTag& authTag
				) method;
	TCPSError RemoveAuthCenter(
				IN const char* trunk,
				IN const PCC_ModuleTag& authTag
				) method
		{	return this->RemoveAuthCenter(
							trunk, -1,
							authTag
							);
		}
	TCPSError RemoveAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag
				) method
		{	return this->RemoveAuthCenter(
							trunk.Get(), trunk.Length(),
							authTag
							);
		}

public:
	TCPSError ListAuthCenter(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				OUT tcps_Array<PCC_ModuleTag>& authTags
				) method;
	TCPSError ListAuthCenter(
				IN const char* trunk,
				OUT tcps_Array<PCC_ModuleTag>& authTags
				) method
		{	return this->ListAuthCenter(
							trunk, -1,
							authTags
							);
		}
	TCPSError ListAuthCenter(
				IN const tcps_String& trunk,
				OUT tcps_Array<PCC_ModuleTag>& authTags
				) method
		{	return this->ListAuthCenter(
							trunk.Get(), trunk.Length(),
							authTags
							);
		}

public:
	TCPSError FindAuthCenter(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleTag& authTag
				) method;
	TCPSError FindAuthCenter(
				IN const char* trunk,
				IN const PCC_ModuleTag& authTag
				) method
		{	return this->FindAuthCenter(
							trunk, -1,
							authTag
							);
		}
	TCPSError FindAuthCenter(
				IN const tcps_String& trunk,
				IN const PCC_ModuleTag& authTag
				) method
		{	return this->FindAuthCenter(
							trunk.Get(), trunk.Length(),
							authTag
							);
		}

public:
	TCPSError AddModule(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method;
	TCPSError AddModule(
				IN const char* trunk,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
		{	return this->AddModule(
							trunk, -1,
							moduleProperty,
							moudleFiles,
							moduleKey
							);
		}
	TCPSError AddModule(
				IN const tcps_String& trunk,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method
		{	return this->AddModule(
							trunk.Get(), trunk.Length(),
							moduleProperty,
							moudleFiles,
							moduleKey
							);
		}
	TCPSError AddModule(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN const PCC_ModuleProperty& moduleProperty,
				IN const PCC_ModuleFile* moudleFiles, IN INT32 moudleFiles_count,
				OUT INT64& moduleKey
				) method
		{	return this->AddModule(
							trunk, trunk_len,
							moduleProperty,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)moudleFiles, moudleFiles_count),
							moduleKey
							);
		}

public:
	TCPSError AddModuleFile(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method;
	TCPSError AddModuleFile(
				IN const char* trunk,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
		{	return this->AddModuleFile(
							trunk, -1,
							moduleKey,
							fileType,
							moduleFiles
							);
		}
	TCPSError AddModuleFile(
				IN const tcps_String& trunk,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method
		{	return this->AddModuleFile(
							trunk.Get(), trunk.Length(),
							moduleKey,
							fileType,
							moduleFiles
							);
		}
	TCPSError AddModuleFile(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const PCC_ModuleFile* moduleFiles, IN INT32 moduleFiles_count
				) method
		{	return this->AddModuleFile(
							trunk, trunk_len,
							moduleKey,
							fileType,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)moduleFiles, moduleFiles_count)
							);
		}

public:
	TCPSError RemoveModule(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN INT64 moduleKey
				) method;
	TCPSError RemoveModule(
				IN const char* trunk,
				IN INT64 moduleKey
				) method
		{	return this->RemoveModule(
							trunk, -1,
							moduleKey
							);
		}
	TCPSError RemoveModule(
				IN const tcps_String& trunk,
				IN INT64 moduleKey
				) method
		{	return this->RemoveModule(
							trunk.Get(), trunk.Length(),
							moduleKey
							);
		}

public:
	TCPSError RemoveModuleFiles(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				IN INT64 moduleKey,
				IN INT32 fileType
				) method;
	TCPSError RemoveModuleFiles(
				IN const char* trunk,
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
		{	return this->RemoveModuleFiles(
							trunk, -1,
							moduleKey,
							fileType
							);
		}
	TCPSError RemoveModuleFiles(
				IN const tcps_String& trunk,
				IN INT64 moduleKey,
				IN INT32 fileType
				) method
		{	return this->RemoveModuleFiles(
							trunk.Get(), trunk.Length(),
							moduleKey,
							fileType
							);
		}

public:
	TCPSError ListModules(
				IN const char* trunk, IN INT32 trunk_len /*= -1*/,
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method;
	TCPSError ListModules(
				IN const char* trunk,
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
		{	return this->ListModules(
							trunk, -1,
							modulesInfo
							);
		}
	TCPSError ListModules(
				IN const tcps_String& trunk,
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method
		{	return this->ListModules(
							trunk.Get(), trunk.Length(),
							modulesInfo
							);
		}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)redirectIPP; (void)redirectParam;
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	void* m_base_v_tab_ptr;
	void* m_derived_v_tab_ptr;
	PCC_Deploy_RC* m_faceR;

private:
	struct CallSiteL_
	{
		BOOL needUpdateFlags;
		MethodMatchingFlag matchingFlags;
		PROC fnOnStreamedCall_L_;
		PROC fnLogin;
		PROC fnLogout;
		PROC fnCreateTrunk;
		PROC fnRemoveTrunk;
		PROC fnListTrunk;
		PROC fnAddAuthCenter;
		PROC fnRemoveAuthCenter;
		PROC fnListAuthCenter;
		PROC fnFindAuthCenter;
		PROC fnAddModule;
		PROC fnAddModuleFile;
		PROC fnRemoveModule;
		PROC fnRemoveModuleFiles;
		PROC fnListModules;
		CallSiteL_()
			: needUpdateFlags(true)
			, fnOnStreamedCall_L_(NULL)
			, fnLogin(NULL)
			, fnLogout(NULL)
			, fnCreateTrunk(NULL)
			, fnRemoveTrunk(NULL)
			, fnListTrunk(NULL)
			, fnAddAuthCenter(NULL)
			, fnRemoveAuthCenter(NULL)
			, fnListAuthCenter(NULL)
			, fnFindAuthCenter(NULL)
			, fnAddModule(NULL)
			, fnAddModuleFile(NULL)
			, fnRemoveModule(NULL)
			, fnRemoveModuleFiles(NULL)
			, fnListModules(NULL)
			{}
		void Reset()
		{
			needUpdateFlags = true;
			matchingFlags.Reset();
			fnOnStreamedCall_L_ = NULL;
			fnLogin = NULL;
			fnLogout = NULL;
			fnCreateTrunk = NULL;
			fnRemoveTrunk = NULL;
			fnListTrunk = NULL;
			fnAddAuthCenter = NULL;
			fnRemoveAuthCenter = NULL;
			fnListAuthCenter = NULL;
			fnFindAuthCenter = NULL;
			fnAddModule = NULL;
			fnAddModuleFile = NULL;
			fnRemoveModule = NULL;
			fnRemoveModuleFiles = NULL;
			fnListModules = NULL;
		}
	};
	struct LocalMakingFlag_
	{
		BOOL making;
		BOOL failed;
		LocalMakingFlag_() : making(false), failed(false) {}
	};
	LocalMakingFlag_ m_localMakingFlag;	// 用于本地服务递归调用处理
	IPCC_Deploy_LocalMethod* m_faceL;
	CallSiteL_* m_callSiteL;	// m_faceL!=NULL时有效
	IPP m_localServeIPP;		// m_faceL!=NULL时有效

private:
	OSThreadID m_closingTID;	// 正在关闭会话的用户线程（即正析构Client对象或调用SetServeIPP(NEW_IPP/INVALID_IPP)的线程）

public:
	// 不要使用此函数，仅供框架使用
	OSThreadID GetClosingTID_() const
		{	return m_closingTID;	}

private:
	DWORD m_connectTimeout;
	DWORD m_recvTimeout;
	DWORD m_sendTimeout;
	tcps_SockRecvSendBufParam m_sockRecvSendBufParam;
	iscm_PostingPendingParam m_postingPendingParam;
	iscm_PostingSendParam m_postingSendParam;

private:
	CLocker m_lock;
	BOOL m_streamedCallbackMap_IsInited;
	typedef tcps_QuickStringMap<
				CPtrStrA/*callback_name*/,
				BOOL /*callback_is_matched*/,
				0> StreamedCallbackMap;
	StreamedCallbackMap m_streamedCallbackMap;

public:
	/// 判断流式回调方的回调版本是否匹配（本函数内部会调用this->GetStreamedCallbackTypeInfo_()）
	BOOL IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				);

private:
	TCPSError SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	void DestroyRequester_(
				IN BOOL byLocalPeer
				);

	void OnNetworkMalformed_();
	virtual void CloseSession_();

	virtual PROC FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				);

	static TCPSError OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);
};
#endif // #ifndef PCC_Deploy_defined

#ifndef PCC_Scatter_defined
#define PCC_Scatter_defined
class PCC_Scatter_S;
class PCC_Scatter_RC;
class PCC_Scatter : public IPCC_Scatter_LocalCallback
{
	friend class PCC_Scatter_S;
	friend class PCC_Scatter_RC;
private:
	PCC_Scatter(const PCC_Scatter&);
	PCC_Scatter& operator= (const PCC_Scatter&);

	typedef PCC_Scatter* PPCC_Scatter_;

public:
	// initNetworkSingletons，是否初始化网络相关单件（若只用于连接进程内服务，可传false）
	explicit PCC_Scatter(BOOL initNetworkSingletons = true);
	virtual ~PCC_Scatter();

protected:
	// 析构RPC请求对象
	// 注意：若有派生类，必须且只能在派生类的析构函数的最后调用，
	//		 以保证派生类对象继承的通知虚函数总被正确调用。
	void DestroyRequester();

	// 连接后通知虚函数
	// 若返回失败，则立即触发OnClose()，且继续异步尝试连接
	// TODO: 若需处理连接动作，请在派生类中重载OnConnected()
	virtual TCPSError OnConnected()
		{	return TCPS_OK;	}

	// 对端断线通知虚函数（在OnClose()之前被调用）
	// 注: 只有对端关闭或网络断线才会触发，Client对象被析构或设置INVALID_IPP不会触发
	// TODO: 若需处理此动作，请在派生类中重载OnPeerBroken()
	virtual void OnPeerBroken()
		{	}

	// 连接关闭通知虚函数（在OnPeerBroken()之后被调用）
	// 注: 所有关闭情况都会触发
	// TODO: 若需处理关闭动作，请在派生类中重载OnClose()
	virtual void OnClose()
		{	}

	// 连接超时通知虚函数
	// TODO: 若需处理连接超时动作，请在派生类中重载OnConnectTimeout()
	virtual void OnConnectTimeout(IN TCPSError cause)
		{	(void)cause;	}

public:
	// 设置正向服务参数
	// @serveIPP[in] 第一备选服务器IPP，INVALID_IPP表示取消连接
	// @sessionFlags[in] 会话标志，为以下值的位组合
	//		ISCMC_NO_CALLBACK、ISCMC_ASYNC_CONNECT、ISCMC_UDP_POSTING、ISCMC_NO_ASYNC_RECONNECT、ISCMC_NO_POSTING_CHANNEL
	// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP使用自动值（标识端口使用0）
	// @secondaryServeIPP[in] 第二备选服务IPP（取消连接时本参数被忽略）
	// @tertiaryServeIPP[in] 第三备选服务IPP（取消连接时本参数被忽略）
	// @reconnectIntervalMSELs[in] 网络重建间隔时间
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	// 设置连接保持标志
	//		注：调用SetServeIPP()时，若指定了ISCMC_NO_ASYNC_RECONNECT，
	//			则连接保持标志为false；否则为true
	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	// 对端是否为进程内服务
	BOOL IsLocalPeer() const;

	// 准备断开网络，在有巨量会话对象需要关闭网络时，先批量调用一次PrepareDisconnect()，
	// 调用PrepareDisconnect()后，再调用SetServeIPP(INVALID_IPP)，或析构接口对象
	void PrepareDisconnect();

	// 设置网络超时，INFINITE表示使用默认值（网络对端也会被设置）
	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值（网络对端也会被设置）
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
	// @maxSendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理客户端对象的posting方法发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount
				);

	// 方法匹配检查支持
public:
	struct MethodMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 6> MethodMap;
		MethodMap mmap_;
		BOOL matching_OnComputed;
		BOOL matching_OnComputed1;
		BOOL matching_UDPLink_;
		BOOL matching_UDPLinkConfirm_;
		BOOL matching_SetTimeout_;
		BOOL matching_SetSessionBufferSize_;
		void Reset();
		MethodMatchingFlag();
	};
public:
	// 获取本地与对端方法匹配信息
	const MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
public:
	TCPSError GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				) const;

	virtual TCPSError GetStreamedCallbackTypeInfo_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)typeInfo; (void)infoLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

	/// 流式方法调用
	/// @methodName[in] 方法名，必须提供
	/// @nameLen[in] 方法名长度，-1表示方法名是一'\0'结束符字符串
	/// @data[in] 方法参数数据（dataLen==0时可以为NULL）
	/// @dataLen[in] 方法参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据（无OUT参数的方法可以传NULL）
	///			注：若*replyData返回非NULL，则必须使用tcps_Free()释放
	/// @replyLen[out] 返回OUT型参数数据长度（无OUT参数的方法可以传NULL）
	/// @return TCPSError
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	/// 流式回调调用通知（在调用普通的回调函数之前被调用）
	/// @callbackName[in] 回调名，必须提供
	///		注：callbackName==NULL时，用于检测StreamedCallback_()是否被派生，
	///			派生类的StreamedCallback_()中，最前面总这样实现：
	///			{
	///				if(NULL == callbackName)
	///					return TCPS_ERR_INVALID_PARAM; // 必须返回非TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED
	///				...
	///			}
	/// @nameLen[in] 回调名长度，-1表示回调名为'\0'结束符字符串
	/// @data[in] 回调IN型参数数据（dataLen==0时可能为NULL）
	/// @dataLen[in] 回调IN型参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据，传入时已置为NULL，
	///			若返回非NULL，则必须使用tcps_Alloc()申请内存；无OUT参数的回调时可返回NULL
	/// @replyLen[out] 返回OUT型参数数据长度，传入时已置为0
	/// @return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED表示忽略此次流式回调调用，
	///			框架若收到此返回值，则会继续调用普通的回调函数；
	///			其它值，直接返回给服务器端。
	///			<<<注：若遇到IN型参数数据无法被正确解析，应该返回TCPS_ERR_MALFORMED_REQ>>>
	virtual TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT void*& replyData /*= NULL*/,
				OUT INT_PTR& replyLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)data; (void)dataLen;
		(void)replyData; (void)replyLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

protected:
	virtual TCPSError Compute(
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)moduleKey; (void)taskKey; (void)inputUrl; (void)outputUrl;
		(void)moduleParams;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError OnComputed(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len
				) posting_method;
	TCPSError OnComputed(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_method
		{	return this->OnComputed(
							taskKey,
							errorCode,
							context.Get(), context.Length()
							);
		}

public:
	static TCPSError OnComputed_Batch(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				OUT PPCC_Scatter_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method;
	static inline TCPSError OnComputed_Batch(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				OUT PPCC_Scatter_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method
		{	return PCC_Scatter::OnComputed_Batch(
							clients, clientsCount,
							taskKey,
							errorCode,
							context.Get(), context.Length(),
							sendFaileds, failedCount
							);
		}

public:
	TCPSError OnComputed1(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				IN const tcps_Array<tcps_Binary>& outArgs
				) posting_method;
	TCPSError OnComputed1(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs
				) posting_method
		{	return this->OnComputed1(
							taskKey,
							errorCode,
							context.Get(), context.Length(),
							outArgs
							);
		}
	TCPSError OnComputed1(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				IN const tcps_Binary* outArgs, IN INT32 outArgs_count
				) posting_method
		{	return this->OnComputed1(
							taskKey,
							errorCode,
							context, context_len,
							tcps_Array<tcps_Binary>(xat_bind, (tcps_Binary*)outArgs, outArgs_count)
							);
		}

public:
	static TCPSError OnComputed1_Batch(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				IN const tcps_Array<tcps_Binary>& outArgs,
				OUT PPCC_Scatter_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method;
	static inline TCPSError OnComputed1_Batch(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs,
				OUT PPCC_Scatter_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method
		{	return PCC_Scatter::OnComputed1_Batch(
							clients, clientsCount,
							taskKey,
							errorCode,
							context.Get(), context.Length(),
							outArgs,
							sendFaileds, failedCount
							);
		}
	static inline TCPSError OnComputed1_Batch(
				IN const PPCC_Scatter_* clients,
				IN INT_PTR clientsCount,
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const void* context, IN INT32 context_len,
				IN const tcps_Binary* outArgs, IN INT32 outArgs_count,
				OUT PPCC_Scatter_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method
		{	return PCC_Scatter::OnComputed1_Batch(
							clients, clientsCount,
							taskKey,
							errorCode,
							context, context_len,
							tcps_Array<tcps_Binary>(xat_bind, (tcps_Binary*)outArgs, outArgs_count),
							sendFaileds, failedCount
							);
		}

protected:
	virtual TCPSError AddModule(
				IN INT64 moduleKey,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) callback
	{	// TODO: 请在派生类中重载此函数
		(void)moduleKey; (void)moudleFiles;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

protected:
	virtual TCPSError RemoveModule(
				IN INT64 moduleKey
				) callback
	{	// TODO: 请在派生类中重载此函数
		(void)moduleKey;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

protected:
	virtual TCPSError FindModule(
				IN INT64 moduleKey,
				OUT BOOL& found
				) callback
	{	// TODO: 请在派生类中重载此函数
		(void)moduleKey; (void)found;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)redirectIPP; (void)redirectParam;
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	void* m_base_v_tab_ptr;
	void* m_derived_v_tab_ptr;
	PCC_Scatter_RC* m_faceR;

private:
	struct CallSiteL_
	{
		BOOL needUpdateFlags;
		MethodMatchingFlag matchingFlags;
		PROC fnOnStreamedCall_L_;
		PROC fnOnComputed;
		PROC fnOnComputed1;
		CallSiteL_()
			: needUpdateFlags(true)
			, fnOnStreamedCall_L_(NULL)
			, fnOnComputed(NULL)
			, fnOnComputed1(NULL)
			{}
		void Reset()
		{
			needUpdateFlags = true;
			matchingFlags.Reset();
			fnOnStreamedCall_L_ = NULL;
			fnOnComputed = NULL;
			fnOnComputed1 = NULL;
		}
	};
	struct LocalMakingFlag_
	{
		BOOL making;
		BOOL failed;
		LocalMakingFlag_() : making(false), failed(false) {}
	};
	LocalMakingFlag_ m_localMakingFlag;	// 用于本地服务递归调用处理
	IPCC_Scatter_LocalMethod* m_faceL;
	CallSiteL_* m_callSiteL;	// m_faceL!=NULL时有效
	IPP m_localServeIPP;		// m_faceL!=NULL时有效

private:
	OSThreadID m_closingTID;	// 正在关闭会话的用户线程（即正析构Client对象或调用SetServeIPP(NEW_IPP/INVALID_IPP)的线程）

public:
	// 不要使用此函数，仅供框架使用
	OSThreadID GetClosingTID_() const
		{	return m_closingTID;	}

private:
	DWORD m_connectTimeout;
	DWORD m_recvTimeout;
	DWORD m_sendTimeout;
	tcps_SockRecvSendBufParam m_sockRecvSendBufParam;
	iscm_PostingPendingParam m_postingPendingParam;
	iscm_PostingSendParam m_postingSendParam;

private:
	CLocker m_lock;
	BOOL m_streamedCallbackMap_IsInited;
	typedef tcps_QuickStringMap<
				CPtrStrA/*callback_name*/,
				BOOL /*callback_is_matched*/,
				4> StreamedCallbackMap;
	StreamedCallbackMap m_streamedCallbackMap;

public:
	/// 判断流式回调方的回调版本是否匹配（本函数内部会调用this->GetStreamedCallbackTypeInfo_()）
	BOOL IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				);

private:
	TCPSError SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	void DestroyRequester_(
				IN BOOL byLocalPeer
				);

	void OnNetworkMalformed_();
	virtual void CloseSession_();

	virtual PROC FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				);

	static TCPSError OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	static TCPSError Local_Compute(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams
				) posting_callback;

	static TCPSError Local_AddModule(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) callback;

	static TCPSError Local_RemoveModule(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey
				) callback;

	static TCPSError Local_FindModule(
				IN void* sessionObj_wrap,
				IN INT64 moduleKey,
				OUT BOOL& found
				) callback;
};
#endif // #ifndef PCC_Scatter_defined

#ifndef PCC_Service_defined
#define PCC_Service_defined
class PCC_Service_S;
class PCC_Service_RC;
class PCC_Service : public IPCC_Service_LocalCallback
{
	friend class PCC_Service_S;
	friend class PCC_Service_RC;
private:
	PCC_Service(const PCC_Service&);
	PCC_Service& operator= (const PCC_Service&);

	typedef PCC_Service* PPCC_Service_;

public:
	// initNetworkSingletons，是否初始化网络相关单件（若只用于连接进程内服务，可传false）
	explicit PCC_Service(BOOL initNetworkSingletons = true);
	virtual ~PCC_Service();

protected:
	// 析构RPC请求对象
	// 注意：若有派生类，必须且只能在派生类的析构函数的最后调用，
	//		 以保证派生类对象继承的通知虚函数总被正确调用。
	void DestroyRequester();

	// 连接后通知虚函数
	// 若返回失败，则立即触发OnClose()，且继续异步尝试连接
	// TODO: 若需处理连接动作，请在派生类中重载OnConnected()
	virtual TCPSError OnConnected()
		{	return TCPS_OK;	}

	// 对端断线通知虚函数（在OnClose()之前被调用）
	// 注: 只有对端关闭或网络断线才会触发，Client对象被析构或设置INVALID_IPP不会触发
	// TODO: 若需处理此动作，请在派生类中重载OnPeerBroken()
	virtual void OnPeerBroken()
		{	}

	// 连接关闭通知虚函数（在OnPeerBroken()之后被调用）
	// 注: 所有关闭情况都会触发
	// TODO: 若需处理关闭动作，请在派生类中重载OnClose()
	virtual void OnClose()
		{	}

	// 连接超时通知虚函数
	// TODO: 若需处理连接超时动作，请在派生类中重载OnConnectTimeout()
	virtual void OnConnectTimeout(IN TCPSError cause)
		{	(void)cause;	}

public:
	// 设置正向服务参数
	// @serveIPP[in] 第一备选服务器IPP，INVALID_IPP表示取消连接
	// @sessionFlags[in] 会话标志，为以下值的位组合
	//		ISCMC_NO_CALLBACK、ISCMC_ASYNC_CONNECT、ISCMC_UDP_POSTING、ISCMC_NO_ASYNC_RECONNECT、ISCMC_NO_POSTING_CHANNEL
	// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP使用自动值（标识端口使用0）
	// @secondaryServeIPP[in] 第二备选服务IPP（取消连接时本参数被忽略）
	// @tertiaryServeIPP[in] 第三备选服务IPP（取消连接时本参数被忽略）
	// @reconnectIntervalMSELs[in] 网络重建间隔时间
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	// 设置连接保持标志
	//		注：调用SetServeIPP()时，若指定了ISCMC_NO_ASYNC_RECONNECT，
	//			则连接保持标志为false；否则为true
	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	// 对端是否为进程内服务
	BOOL IsLocalPeer() const;

	// 准备断开网络，在有巨量会话对象需要关闭网络时，先批量调用一次PrepareDisconnect()，
	// 调用PrepareDisconnect()后，再调用SetServeIPP(INVALID_IPP)，或析构接口对象
	void PrepareDisconnect();

	// 设置网络超时，INFINITE表示使用默认值（网络对端也会被设置）
	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值（网络对端也会被设置）
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
	// @maxSendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理客户端对象的posting方法发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Service_* clients,
				IN INT_PTR clientsCount
				);

	// 方法匹配检查支持
public:
	struct MethodMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 11> MethodMap;
		MethodMap mmap_;
		BOOL matching_Login;
		BOOL matching_Logout;
		BOOL matching_ListModules;
		BOOL matching_GetModuleFile;
		BOOL matching_Execute;
		BOOL matching_QueryJobs;
		BOOL matching_CancelJob;
		BOOL matching_UDPLink_;
		BOOL matching_UDPLinkConfirm_;
		BOOL matching_SetTimeout_;
		BOOL matching_SetSessionBufferSize_;
		void Reset();
		MethodMatchingFlag();
	};
public:
	// 获取本地与对端方法匹配信息
	const MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
public:
	TCPSError GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				) const;

	virtual TCPSError GetStreamedCallbackTypeInfo_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)typeInfo; (void)infoLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

	/// 流式方法调用
	/// @methodName[in] 方法名，必须提供
	/// @nameLen[in] 方法名长度，-1表示方法名是一'\0'结束符字符串
	/// @data[in] 方法参数数据（dataLen==0时可以为NULL）
	/// @dataLen[in] 方法参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据（无OUT参数的方法可以传NULL）
	///			注：若*replyData返回非NULL，则必须使用tcps_Free()释放
	/// @replyLen[out] 返回OUT型参数数据长度（无OUT参数的方法可以传NULL）
	/// @return TCPSError
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	/// 流式回调调用通知（在调用普通的回调函数之前被调用）
	/// @callbackName[in] 回调名，必须提供
	///		注：callbackName==NULL时，用于检测StreamedCallback_()是否被派生，
	///			派生类的StreamedCallback_()中，最前面总这样实现：
	///			{
	///				if(NULL == callbackName)
	///					return TCPS_ERR_INVALID_PARAM; // 必须返回非TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED
	///				...
	///			}
	/// @nameLen[in] 回调名长度，-1表示回调名为'\0'结束符字符串
	/// @data[in] 回调IN型参数数据（dataLen==0时可能为NULL）
	/// @dataLen[in] 回调IN型参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据，传入时已置为NULL，
	///			若返回非NULL，则必须使用tcps_Alloc()申请内存；无OUT参数的回调时可返回NULL
	/// @replyLen[out] 返回OUT型参数数据长度，传入时已置为0
	/// @return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED表示忽略此次流式回调调用，
	///			框架若收到此返回值，则会继续调用普通的回调函数；
	///			其它值，直接返回给服务器端。
	///			<<<注：若遇到IN型参数数据无法被正确解析，应该返回TCPS_ERR_MALFORMED_REQ>>>
	virtual TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT void*& replyData /*= NULL*/,
				OUT INT_PTR& replyLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)data; (void)dataLen;
		(void)replyData; (void)replyLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError Login(
				IN const char* ticket, IN INT32 ticket_len /*= -1*/
				) method;
	TCPSError Login(
				IN const char* ticket
				) method
		{	return this->Login(
							ticket, -1
							);
		}
	TCPSError Login(
				IN const tcps_String& ticket
				) method
		{	return this->Login(
							ticket.Get(), ticket.Length()
							);
		}

public:
	TCPSError Logout(
				) method;

public:
	TCPSError ListModules(
				IN INT32 moduleType,
				OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
				) method;

public:
	TCPSError GetModuleFile(
				IN INT64 moduleKey,
				IN INT32 fileType,
				OUT tcps_Array<PCC_ModuleFile>& moduleFiles
				) method;

public:
	TCPSError Execute(
				IN INT64 moduleKey,
				IN const char* inputUrl, IN INT32 inputUrl_len /*= -1*/,
				IN const char* outputUrl, IN INT32 outputUrl_len /*= -1*/,
				IN const void* moduleParams, IN INT32 moduleParams_len,
				OUT INT64& jobKey
				) method;
	TCPSError Execute(
				IN INT64 moduleKey,
				IN const char* inputUrl,
				IN const char* outputUrl,
				IN const void* moduleParams, IN INT32 moduleParams_len,
				OUT INT64& jobKey
				) method
		{	return this->Execute(
							moduleKey,
							inputUrl, -1,
							outputUrl, -1,
							moduleParams, moduleParams_len,
							jobKey
							);
		}
	TCPSError Execute(
				IN INT64 moduleKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams,
				OUT INT64& jobKey
				) method
		{	return this->Execute(
							moduleKey,
							inputUrl.Get(), inputUrl.Length(),
							outputUrl.Get(), outputUrl.Length(),
							moduleParams.Get(), moduleParams.Length(),
							jobKey
							);
		}

protected:
	virtual TCPSError OnExecuted(
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)jobKey; (void)errorCode; (void)context;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

protected:
	virtual TCPSError OnExecuted1(
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)jobKey; (void)errorCode; (void)context; (void)outArgs;
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError QueryJobs(
				IN const tcps_Array<INT64>& jobsKey,
				OUT tcps_Array<ExecuteState>& jobsState
				) method;
	TCPSError QueryJobs(
				IN const INT64* jobsKey, IN INT32 jobsKey_count,
				OUT tcps_Array<ExecuteState>& jobsState
				) method
		{	return this->QueryJobs(
							tcps_Array<INT64>(xat_bind, (INT64*)jobsKey, jobsKey_count),
							jobsState
							);
		}

public:
	TCPSError CancelJob(
				IN INT64 jobKey
				) posting_method;

public:
	static TCPSError CancelJob_Batch(
				IN const PPCC_Service_* clients,
				IN INT_PTR clientsCount,
				IN INT64 jobKey,
				OUT PPCC_Service_* sendFaileds = NULL,
				OUT INT_PTR* failedCount = NULL
				) posting_method;

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)redirectIPP; (void)redirectParam;
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	void* m_base_v_tab_ptr;
	void* m_derived_v_tab_ptr;
	PCC_Service_RC* m_faceR;

private:
	struct CallSiteL_
	{
		BOOL needUpdateFlags;
		MethodMatchingFlag matchingFlags;
		PROC fnOnStreamedCall_L_;
		PROC fnLogin;
		PROC fnLogout;
		PROC fnListModules;
		PROC fnGetModuleFile;
		PROC fnExecute;
		PROC fnQueryJobs;
		PROC fnCancelJob;
		CallSiteL_()
			: needUpdateFlags(true)
			, fnOnStreamedCall_L_(NULL)
			, fnLogin(NULL)
			, fnLogout(NULL)
			, fnListModules(NULL)
			, fnGetModuleFile(NULL)
			, fnExecute(NULL)
			, fnQueryJobs(NULL)
			, fnCancelJob(NULL)
			{}
		void Reset()
		{
			needUpdateFlags = true;
			matchingFlags.Reset();
			fnOnStreamedCall_L_ = NULL;
			fnLogin = NULL;
			fnLogout = NULL;
			fnListModules = NULL;
			fnGetModuleFile = NULL;
			fnExecute = NULL;
			fnQueryJobs = NULL;
			fnCancelJob = NULL;
		}
	};
	struct LocalMakingFlag_
	{
		BOOL making;
		BOOL failed;
		LocalMakingFlag_() : making(false), failed(false) {}
	};
	LocalMakingFlag_ m_localMakingFlag;	// 用于本地服务递归调用处理
	IPCC_Service_LocalMethod* m_faceL;
	CallSiteL_* m_callSiteL;	// m_faceL!=NULL时有效
	IPP m_localServeIPP;		// m_faceL!=NULL时有效

private:
	OSThreadID m_closingTID;	// 正在关闭会话的用户线程（即正析构Client对象或调用SetServeIPP(NEW_IPP/INVALID_IPP)的线程）

public:
	// 不要使用此函数，仅供框架使用
	OSThreadID GetClosingTID_() const
		{	return m_closingTID;	}

private:
	DWORD m_connectTimeout;
	DWORD m_recvTimeout;
	DWORD m_sendTimeout;
	tcps_SockRecvSendBufParam m_sockRecvSendBufParam;
	iscm_PostingPendingParam m_postingPendingParam;
	iscm_PostingSendParam m_postingSendParam;

private:
	CLocker m_lock;
	BOOL m_streamedCallbackMap_IsInited;
	typedef tcps_QuickStringMap<
				CPtrStrA/*callback_name*/,
				BOOL /*callback_is_matched*/,
				2> StreamedCallbackMap;
	StreamedCallbackMap m_streamedCallbackMap;

public:
	/// 判断流式回调方的回调版本是否匹配（本函数内部会调用this->GetStreamedCallbackTypeInfo_()）
	BOOL IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				);

private:
	TCPSError SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	void DestroyRequester_(
				IN BOOL byLocalPeer
				);

	void OnNetworkMalformed_();
	virtual void CloseSession_();

	virtual PROC FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				);

	static TCPSError OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	static TCPSError Local_OnExecuted(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_callback;

	static TCPSError Local_OnExecuted1(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs
				) posting_callback;
};
#endif // #ifndef PCC_Service_defined

#ifndef PCC_Toolkit_defined
#define PCC_Toolkit_defined
class PCC_Toolkit_S;
class PCC_Toolkit_RC;
class PCC_Toolkit : public IPCC_Toolkit_LocalCallback
{
	friend class PCC_Toolkit_S;
	friend class PCC_Toolkit_RC;
private:
	PCC_Toolkit(const PCC_Toolkit&);
	PCC_Toolkit& operator= (const PCC_Toolkit&);

	typedef PCC_Toolkit* PPCC_Toolkit_;

public:
	// initNetworkSingletons，是否初始化网络相关单件（若只用于连接进程内服务，可传false）
	explicit PCC_Toolkit(BOOL initNetworkSingletons = true);
	virtual ~PCC_Toolkit();

protected:
	// 析构RPC请求对象
	// 注意：若有派生类，必须且只能在派生类的析构函数的最后调用，
	//		 以保证派生类对象继承的通知虚函数总被正确调用。
	void DestroyRequester();

	// 连接后通知虚函数
	// 若返回失败，则立即触发OnClose()，且继续异步尝试连接
	// TODO: 若需处理连接动作，请在派生类中重载OnConnected()
	virtual TCPSError OnConnected()
		{	return TCPS_OK;	}

	// 对端断线通知虚函数（在OnClose()之前被调用）
	// 注: 只有对端关闭或网络断线才会触发，Client对象被析构或设置INVALID_IPP不会触发
	// TODO: 若需处理此动作，请在派生类中重载OnPeerBroken()
	virtual void OnPeerBroken()
		{	}

	// 连接关闭通知虚函数（在OnPeerBroken()之后被调用）
	// 注: 所有关闭情况都会触发
	// TODO: 若需处理关闭动作，请在派生类中重载OnClose()
	virtual void OnClose()
		{	}

	// 连接超时通知虚函数
	// TODO: 若需处理连接超时动作，请在派生类中重载OnConnectTimeout()
	virtual void OnConnectTimeout(IN TCPSError cause)
		{	(void)cause;	}

public:
	// 设置正向服务参数
	// @serveIPP[in] 第一备选服务器IPP，INVALID_IPP表示取消连接
	// @sessionFlags[in] 会话标志，为以下值的位组合
	//		ISCMC_NO_CALLBACK、ISCMC_ASYNC_CONNECT、ISCMC_UDP_POSTING、ISCMC_NO_ASYNC_RECONNECT、ISCMC_NO_POSTING_CHANNEL
	// @clientID_IPP[in] 客户端标识IPP，INVALID_IPP使用自动值（标识端口使用0）
	// @secondaryServeIPP[in] 第二备选服务IPP（取消连接时本参数被忽略）
	// @tertiaryServeIPP[in] 第三备选服务IPP（取消连接时本参数被忽略）
	// @reconnectIntervalMSELs[in] 网络重建间隔时间
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	// 设置连接保持标志
	//		注：调用SetServeIPP()时，若指定了ISCMC_NO_ASYNC_RECONNECT，
	//			则连接保持标志为false；否则为true
	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	// 对端是否为进程内服务
	BOOL IsLocalPeer() const;

	// 准备断开网络，在有巨量会话对象需要关闭网络时，先批量调用一次PrepareDisconnect()，
	// 调用PrepareDisconnect()后，再调用SetServeIPP(INVALID_IPP)，或析构接口对象
	void PrepareDisconnect();

	// 设置网络超时，INFINITE表示使用默认值（网络对端也会被设置）
	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	// 设置同步调用网络缓冲大小，<0表示不改变此项，==0表示恢复默认值（网络对端也会被设置）
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
	// @maxSendingBytes[in] 最大缓冲大小（字节，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_SEND_BUFFER_SIZE）
	// @maxSendings[in] 最大缓冲调用数量（个，<0表示不改变此项，==0表示恢复默认值，默认ISCM_POSTING_MAX_SENDINGS）
	void SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				);
	void GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const;

	// 清理客户端对象的posting方法发送队列
	TCPSError CleanPostingSendingQueue();
	static TCPSError CleanPostingSendingQueue(
				IN const PPCC_Toolkit_* clients,
				IN INT_PTR clientsCount
				);

	// 方法匹配检查支持
public:
	struct MethodMatchingFlag
	{
		struct Info
		{
			BOOL* pMatchingVar;
			BOOL isPosting;
			Info(BOOL* p, BOOL is) : pMatchingVar(p), isPosting(is) {}
		};
		typedef tcps_QuickStringMap<CPtrStrA, Info, 9> MethodMap;
		MethodMap mmap_;
		BOOL matching_Login;
		BOOL matching_Logout;
		BOOL matching_AddModule;
		BOOL matching_AddModuleFile;
		BOOL matching_RemoveModule;
		BOOL matching_RemoveModuleFiles;
		BOOL matching_ListModules;
		BOOL matching_SetTimeout_;
		BOOL matching_SetSessionBufferSize_;
		void Reset();
		MethodMatchingFlag();
	};
public:
	// 获取本地与对端方法匹配信息
	const MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

	// 下面流式调用定义仅用于非C++语言的辅助实现
public:
	TCPSError GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				) const;

	virtual TCPSError GetStreamedCallbackTypeInfo_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo /*= NULL*/,
				OUT INT_PTR& infoLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)typeInfo; (void)infoLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

	/// 流式方法调用
	/// @methodName[in] 方法名，必须提供
	/// @nameLen[in] 方法名长度，-1表示方法名是一'\0'结束符字符串
	/// @data[in] 方法参数数据（dataLen==0时可以为NULL）
	/// @dataLen[in] 方法参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据（无OUT参数的方法可以传NULL）
	///			注：若*replyData返回非NULL，则必须使用tcps_Free()释放
	/// @replyLen[out] 返回OUT型参数数据长度（无OUT参数的方法可以传NULL）
	/// @return TCPSError
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	/// 流式回调调用通知（在调用普通的回调函数之前被调用）
	/// @callbackName[in] 回调名，必须提供
	///		注：callbackName==NULL时，用于检测StreamedCallback_()是否被派生，
	///			派生类的StreamedCallback_()中，最前面总这样实现：
	///			{
	///				if(NULL == callbackName)
	///					return TCPS_ERR_INVALID_PARAM; // 必须返回非TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED
	///				...
	///			}
	/// @nameLen[in] 回调名长度，-1表示回调名为'\0'结束符字符串
	/// @data[in] 回调IN型参数数据（dataLen==0时可能为NULL）
	/// @dataLen[in] 回调IN型参数数据长度（>=0）
	/// @replyData[out] 返回OUT型参数数据，传入时已置为NULL，
	///			若返回非NULL，则必须使用tcps_Alloc()申请内存；无OUT参数的回调时可返回NULL
	/// @replyLen[out] 返回OUT型参数数据长度，传入时已置为0
	/// @return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED表示忽略此次流式回调调用，
	///			框架若收到此返回值，则会继续调用普通的回调函数；
	///			其它值，直接返回给服务器端。
	///			<<<注：若遇到IN型参数数据无法被正确解析，应该返回TCPS_ERR_MALFORMED_REQ>>>
	virtual TCPSError StreamedCallback_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT void*& replyData /*= NULL*/,
				OUT INT_PTR& replyLen /*= 0*/
				)
	{	// TODO: 请在派生类中重载此函数
		(void)callbackName; (void)nameLen;
		(void)data; (void)dataLen;
		(void)replyData; (void)replyLen;
		return TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError Login(
				IN const char* ticket, IN INT32 ticket_len /*= -1*/
				) method;
	TCPSError Login(
				IN const char* ticket
				) method
		{	return this->Login(
							ticket, -1
							);
		}
	TCPSError Login(
				IN const tcps_String& ticket
				) method
		{	return this->Login(
							ticket.Get(), ticket.Length()
							);
		}

public:
	TCPSError Logout(
				) method;

public:
	TCPSError AddModule(
				IN const PCC_ModuleProperty& moduleProperty,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
				OUT INT64& moduleKey
				) method;
	TCPSError AddModule(
				IN const PCC_ModuleProperty& moduleProperty,
				IN const PCC_ModuleFile* moudleFiles, IN INT32 moudleFiles_count,
				OUT INT64& moduleKey
				) method
		{	return this->AddModule(
							moduleProperty,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)moudleFiles, moudleFiles_count),
							moduleKey
							);
		}

public:
	TCPSError AddModuleFile(
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const tcps_Array<PCC_ModuleFile>& moduleFiles
				) method;
	TCPSError AddModuleFile(
				IN INT64 moduleKey,
				IN PCC_ModuleFileType fileType,
				IN const PCC_ModuleFile* moduleFiles, IN INT32 moduleFiles_count
				) method
		{	return this->AddModuleFile(
							moduleKey,
							fileType,
							tcps_Array<PCC_ModuleFile>(xat_bind, (PCC_ModuleFile*)moduleFiles, moduleFiles_count)
							);
		}

public:
	TCPSError RemoveModule(
				IN INT64 moduleKey
				) method;

public:
	TCPSError RemoveModuleFiles(
				IN INT64 moduleKey,
				IN INT32 fileType
				) method;

public:
	TCPSError ListModules(
				OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
				) method;

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{	// TODO: 请在派生类中重载此函数
		(void)redirectIPP; (void)redirectParam;
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	void* m_base_v_tab_ptr;
	void* m_derived_v_tab_ptr;
	PCC_Toolkit_RC* m_faceR;

private:
	struct CallSiteL_
	{
		BOOL needUpdateFlags;
		MethodMatchingFlag matchingFlags;
		PROC fnOnStreamedCall_L_;
		PROC fnLogin;
		PROC fnLogout;
		PROC fnAddModule;
		PROC fnAddModuleFile;
		PROC fnRemoveModule;
		PROC fnRemoveModuleFiles;
		PROC fnListModules;
		CallSiteL_()
			: needUpdateFlags(true)
			, fnOnStreamedCall_L_(NULL)
			, fnLogin(NULL)
			, fnLogout(NULL)
			, fnAddModule(NULL)
			, fnAddModuleFile(NULL)
			, fnRemoveModule(NULL)
			, fnRemoveModuleFiles(NULL)
			, fnListModules(NULL)
			{}
		void Reset()
		{
			needUpdateFlags = true;
			matchingFlags.Reset();
			fnOnStreamedCall_L_ = NULL;
			fnLogin = NULL;
			fnLogout = NULL;
			fnAddModule = NULL;
			fnAddModuleFile = NULL;
			fnRemoveModule = NULL;
			fnRemoveModuleFiles = NULL;
			fnListModules = NULL;
		}
	};
	struct LocalMakingFlag_
	{
		BOOL making;
		BOOL failed;
		LocalMakingFlag_() : making(false), failed(false) {}
	};
	LocalMakingFlag_ m_localMakingFlag;	// 用于本地服务递归调用处理
	IPCC_Toolkit_LocalMethod* m_faceL;
	CallSiteL_* m_callSiteL;	// m_faceL!=NULL时有效
	IPP m_localServeIPP;		// m_faceL!=NULL时有效

private:
	OSThreadID m_closingTID;	// 正在关闭会话的用户线程（即正析构Client对象或调用SetServeIPP(NEW_IPP/INVALID_IPP)的线程）

public:
	// 不要使用此函数，仅供框架使用
	OSThreadID GetClosingTID_() const
		{	return m_closingTID;	}

private:
	DWORD m_connectTimeout;
	DWORD m_recvTimeout;
	DWORD m_sendTimeout;
	tcps_SockRecvSendBufParam m_sockRecvSendBufParam;
	iscm_PostingPendingParam m_postingPendingParam;
	iscm_PostingSendParam m_postingSendParam;

private:
	CLocker m_lock;
	BOOL m_streamedCallbackMap_IsInited;
	typedef tcps_QuickStringMap<
				CPtrStrA/*callback_name*/,
				BOOL /*callback_is_matched*/,
				0> StreamedCallbackMap;
	StreamedCallbackMap m_streamedCallbackMap;

public:
	/// 判断流式回调方的回调版本是否匹配（本函数内部会调用this->GetStreamedCallbackTypeInfo_()）
	BOOL IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				);

private:
	TCPSError SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	void DestroyRequester_(
				IN BOOL byLocalPeer
				);

	void OnNetworkMalformed_();
	virtual void CloseSession_();

	virtual PROC FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				);

	static TCPSError OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);
};
#endif // #ifndef PCC_Toolkit_defined

//[[end_iscm]]

#endif	// #ifndef _NP_SCATTEREDClient_h_
