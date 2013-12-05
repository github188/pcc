// NP_GridUserTypesDefine.h
//
// 注意: 此文件为工具生成，请不要修改

#ifndef _NP_GridUserTypesDefine_h_
#define _NP_GridUserTypesDefine_h_

#if defined(_MSC_VER) && (_MSC_VER > 1000)
	#pragma once
#endif

#include "iscm_helper_base.h"

class NP_GridUserSessionMaker;

#ifndef GRID_Version_defined
#define GRID_Version_defined
#pragma pack(push, 1)
struct GRID_Version
{
	INT32 majorVer;
	INT32 minorVer;

	GRID_Version()
		: majorVer(0)
		, minorVer(0)
	{
	}
	int Compare(const GRID_Version& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_Version)
};
#pragma pack(pop) // #pragma pack(push, 1)
#endif	// #ifndef GRID_Version_defined

#ifndef GRID_CPUType_defined
#define GRID_CPUType_defined
enum GRID_CPUType
{
	cpu_x86_x64 = 0,
	cpu_arm = 1,
	cpu_mips = 2,
};
#endif	// #ifndef GRID_CPUType_defined

#ifndef GRID_OSType_defined
#define GRID_OSType_defined
enum GRID_OSType
{
	ost_Unknown = 0,
	ost_Win32s = 1,
	ost_Windows_95_98 = 2,
	ost_Windows_NT = 3,
	ost_Windows_2000 = 4,
	ost_Windows_XP = 5,
	ost_Windows_Longhorn = 6,
	ost_Windows_Vista = 7,
	ost_Windows_7 = 8,
	ost_Windows_8 = 9,
	ost_Linux_2_4 = 100,
	ost_Linux_2_6 = 101,
};
#endif	// #ifndef GRID_OSType_defined

#ifndef GRID_ExecuteBits_defined
#define GRID_ExecuteBits_defined
enum GRID_ExecuteBits
{
	eb_32bits = 32,
	eb_64bits = 64,
};
#endif	// #ifndef GRID_ExecuteBits_defined

#ifndef GRID_BinaryType_defined
#define GRID_BinaryType_defined
enum GRID_BinaryType
{
	bt_machineRaw = 0,
	bt_JAVA_Bytecode = 1,
	bt_CSharp_Bytecode = 2,
};
#endif	// #ifndef GRID_BinaryType_defined

#ifndef GRID_ProgramID_defined
#define GRID_ProgramID_defined
struct GRID_ProgramID
{
	tcps_String name;
	GRID_Version ver;
	GRID_CPUType cpuType;
	GRID_OSType osType;
	GRID_ExecuteBits executeBits;
	GRID_BinaryType binaryType;

	GRID_ProgramID()
	{
		this->cpuType = cpu_x86_x64;
		this->osType = ost_Unknown;
		this->executeBits = eb_32bits;
		this->binaryType = bt_machineRaw;
	}

	int Compare(const GRID_ProgramID& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_ProgramID)
};
#endif	// #ifndef GRID_ProgramID_defined

#ifndef GRID_ProgramInfo_defined
#define GRID_ProgramInfo_defined
struct GRID_ProgramInfo
{
	GRID_ProgramID programID;
	tcps_String description;

	int Compare(const GRID_ProgramInfo& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_ProgramInfo)
};
#endif	// #ifndef GRID_ProgramInfo_defined

#ifndef GRID_JobPriority_defined
#define GRID_JobPriority_defined
enum GRID_JobPriority
{
	GRID_JOBPRIO_IDLE = 0,
	GRID_JOBPRIO_LOW = 1,
	GRID_JOBPRIO_NORMAL = 2,
	GRID_JOBPRIO_HIGH = 3,
	GRID_JOBPRIO_PREEMPTIVE = 4,
};
#endif	// #ifndef GRID_JobPriority_defined

#ifndef GRID_JobInfo_defined
#define GRID_JobInfo_defined
struct GRID_JobInfo
{
	GRID_ProgramID programID;
	GRID_ProgramID splitter;
	tcps_String dataInputUrl;
	tcps_String dataOutputUrl;
	GRID_JobPriority priority;
	LTMSEL jobTaskTimeout;
	INT32 jobTaskMaxAttempts;
	INT32 skipFailedJobTaskPercent;
	tcps_String jobName;
	tcps_Binary splitterParam;
	tcps_Binary programParam;

	GRID_JobInfo()
	{
		this->priority = GRID_JOBPRIO_IDLE;
		this->jobTaskTimeout = INVALID_UTC_MSEL;
		this->jobTaskMaxAttempts = 0;
		this->skipFailedJobTaskPercent = 0;
	}

	int Compare(const GRID_JobInfo& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_JobInfo)
};
#endif	// #ifndef GRID_JobInfo_defined

#ifndef GRID_JobTaskInfo_defined
#define GRID_JobTaskInfo_defined
struct GRID_JobTaskInfo
{
	GRID_ProgramID programID;
	tcps_String dataInputUrl;
	tcps_String dataOutputUrl;
	tcps_Binary programParam;

	int Compare(const GRID_JobTaskInfo& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_JobTaskInfo)
};
#endif	// #ifndef GRID_JobTaskInfo_defined

#ifndef GRID_ProgramFile_defined
#define GRID_ProgramFile_defined
struct GRID_ProgramFile
{
	BOOL isExecutable;
	tcps_String name;
	tcps_Binary data;

	GRID_ProgramFile()
		: isExecutable(false)
	{
	}
	int Compare(const GRID_ProgramFile& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_ProgramFile)
};
#endif	// #ifndef GRID_ProgramFile_defined

#ifndef GRID_JobTaskReport_defined
#define GRID_JobTaskReport_defined
struct GRID_JobTaskReport
{
	INT64 jobTaskKey;
	GRID_JobTaskInfo jobTaskInfo;
	INT32 progress;
	LTMSEL costedMSELs;

	GRID_JobTaskReport()
	{
		this->jobTaskKey = 0;
		this->progress = 0;
		this->costedMSELs = INVALID_UTC_MSEL;
	}

	int Compare(const GRID_JobTaskReport& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_JobTaskReport)
};
#endif	// #ifndef GRID_JobTaskReport_defined

#ifndef GRID_MachineStaticContext_defined
#define GRID_MachineStaticContext_defined
struct GRID_MachineStaticContext
{
	GRID_CPUType cpuType;
	INT64 cpuFreq;
	INT32 cpuProcessers;
	INT32 cpuThreads;
	INT64 totalMemoryBytes;
	INT64 networkBandwidth;
	GRID_OSType osType;
	tcps_String osDetail;
	GRID_ExecuteBits executeBits;
	GRID_BinaryType binaryType;
	tcps_String hardwareInfo;

	GRID_MachineStaticContext()
	{
		this->cpuType = cpu_x86_x64;
		this->cpuFreq = 0;
		this->cpuProcessers = 0;
		this->cpuThreads = 0;
		this->totalMemoryBytes = 0;
		this->networkBandwidth = 0;
		this->osType = ost_Unknown;
		this->executeBits = eb_32bits;
		this->binaryType = bt_machineRaw;
	}

	int Compare(const GRID_MachineStaticContext& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_MachineStaticContext)
};
#endif	// #ifndef GRID_MachineStaticContext_defined

#ifndef GRID_MachineDynamicContext_defined
#define GRID_MachineDynamicContext_defined
#pragma pack(push, 1)
struct GRID_MachineDynamicContext
{
	INT32 cpuUsage;
	INT32 memoryUsage;
	INT32 gridMemoryUsage;
	INT32 networkUsage;
	INT32 jobbingTaskCount;

	GRID_MachineDynamicContext()
	{
		this->cpuUsage = 0;
		this->memoryUsage = 0;
		this->gridMemoryUsage = 0;
		this->networkUsage = 0;
		this->jobbingTaskCount = 0;
	}

	int Compare(const GRID_MachineDynamicContext& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_MachineDynamicContext)
};
#pragma pack(pop) // #pragma pack(push, 1)
#endif	// #ifndef GRID_MachineDynamicContext_defined

#ifndef GRID_MachineStatistic_defined
#define GRID_MachineStatistic_defined
#pragma pack(push, 1)
struct GRID_MachineStatistic
{
	INT32 tasksOK;
	INT32 tasksFailed;
	INT32 tasksProcessing;
	INT32 cpuPeakUsage;
	INT32 cpuAvgUsage;
	INT32 memoryPeakUsage;
	INT32 memoryAvgUsage;
	INT32 networkPeakUsage;
	INT32 networkAvgUsage;

	GRID_MachineStatistic()
	{
		this->tasksOK = 0;
		this->tasksFailed = 0;
		this->tasksProcessing = 0;
		this->cpuPeakUsage = 0;
		this->cpuAvgUsage = 0;
		this->memoryPeakUsage = 0;
		this->memoryAvgUsage = 0;
		this->networkPeakUsage = 0;
		this->networkAvgUsage = 0;
	}

	int Compare(const GRID_MachineStatistic& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_MachineStatistic)
};
#pragma pack(pop) // #pragma pack(push, 1)
#endif	// #ifndef GRID_MachineStatistic_defined

#ifndef GRID_ColorSpace_defined
#define GRID_ColorSpace_defined
enum GRID_ColorSpace
{
	GRID_IMAGE_GRAY = 0,
	GRID_IMAGE_RBG = 1,
	GRID_IMAGE_BGR = 2,
	GRID_IMAGE_YUV420P = 3,
	GRID_IMAGE_YUV422P = 4,
};
#endif	// #ifndef GRID_ColorSpace_defined

#ifndef GRID_ImageProperty_defined
#define GRID_ImageProperty_defined
struct GRID_ImageProperty
{
	INT32 width;
	INT32 height;
	INT32 step[4];
	INT64 timeStamp;
	GRID_ColorSpace colorSpace;
	tcps_Binary imageData;

	GRID_ImageProperty()
	{
		this->width = 0;
		this->height = 0;
		for(int i=0; i<4; ++i)
			this->step[i] = 0;
		this->timeStamp = 0;
		this->colorSpace = GRID_IMAGE_GRAY;
	}

	int Compare(const GRID_ImageProperty& r) const;
	ISCM_STRUCT_COMPARE_OPERATORS(GRID_ImageProperty)
};
#endif	// #ifndef GRID_ImageProperty_defined

#ifndef GRID_DefaultListenPort_defined
#define GRID_DefaultListenPort_defined
enum GRID_DefaultListenPort
{
	GRID_JOB_CENTER_SERVER_PORT = 9011,
	GRID_JOB_TERMINAL_MANAGER_SERVER_PORT = 9031,
	GRID_RECORD_COLLECTOR_SERVER_PORT = 9051,
	GRID_DB_SERVER_PORT = 9071,
};
#endif	// #ifndef GRID_DefaultListenPort_defined

// 'class GRID_User_T'用于定义接口'GRID_User'的局部类型
#ifndef GRID_User_T_defined
#define GRID_User_T_defined
class GRID_User_T
{
public:
	struct JobProgram
	{
		INT64 programKey;
		GRID_ProgramInfo programInfo;
		LTMSEL addTime;
		INT64 programSize;

		JobProgram()
		{
			this->programKey = 0;
			this->addTime = INVALID_UTC_MSEL;
			this->programSize = 0;
		}

		int Compare(const JobProgram& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(JobProgram)
	};

public:
	enum JobState
	{
		js_All = -1,
		js_Waiting = 1,
		js_Processing = 2,
		js_Processed_OK = 4,
		js_Processed_Failed = 8,
	};

public:
	struct JobReport
	{
		INT64 jobKey;
		GRID_JobInfo jobInfo;
		JobState jobState;
		INT32 progress;
		LTMSEL costedMSELs;
		LTMSEL commitTime;
		TCPSError errorCode;
		tcps_String causesOfError;

		JobReport()
		{
			this->jobKey = 0;
			this->jobState = js_All;
			this->progress = 0;
			this->costedMSELs = INVALID_UTC_MSEL;
			this->commitTime = INVALID_UTC_MSEL;
			this->errorCode = TCPS_ERROR;
		}

		int Compare(const JobReport& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(JobReport)
	};

public:
	struct LogInfo
	{
		LTMSEL time;
		tcps_String info;

		LogInfo()
		{
			this->time = INVALID_UTC_MSEL;
		}

		int Compare(const LogInfo& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(LogInfo)
	};

public:
	enum UserRole
	{
		USER_GUEST = 0,
		USER_CLIENT = 1,
		USER_ADMIN = 2,
	};

public:
	struct UserInfo
	{
		tcps_String userName;
		tcps_String passWord;
		UserRole role;

		UserInfo()
		{
			this->role = USER_GUEST;
		}

		int Compare(const UserInfo& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(UserInfo)
	};

public:
	struct JTMInfo
	{
		GRID_MachineStaticContext staticContext;
		GRID_MachineStatistic statisticInfo;

		int Compare(const JTMInfo& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(JTMInfo)
	};

public:
	struct GridProperty
	{
		GRID_Version version;
		INT32 jtmNum;
		INT32 processorNum;
		INT32 threadNum;
		tcps_String authorizedID;

		GridProperty()
		{
			this->jtmNum = 0;
			this->processorNum = 0;
			this->threadNum = 0;
		}

		int Compare(const GridProperty& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(GridProperty)
	};

public:
	struct SplitterParam
	{
		tcps_String name;
		tcps_String data;
		tcps_String description;

		int Compare(const SplitterParam& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(SplitterParam)
	};

public:
	#pragma pack(push, 1)
	struct GridStatistics
	{
		LTMSEL startupTime;
		LTMSEL runTime;
		INT32 errorCount;
		INT32 recoveredErrorCount;
		INT32 unrecoverErrorCount;
		INT32 maxJTM;
		INT32 avgJTM;
		LTMSEL maxJobTime;
		LTMSEL minJobTime;
		LTMSEL avgJobTime;

		GridStatistics()
		{
			this->startupTime = INVALID_UTC_MSEL;
			this->runTime = INVALID_UTC_MSEL;
			this->errorCount = 0;
			this->recoveredErrorCount = 0;
			this->unrecoverErrorCount = 0;
			this->maxJTM = 0;
			this->avgJTM = 0;
			this->maxJobTime = INVALID_UTC_MSEL;
			this->minJobTime = INVALID_UTC_MSEL;
			this->avgJobTime = INVALID_UTC_MSEL;
		}

		int Compare(const GridStatistics& r) const;
		ISCM_STRUCT_COMPARE_OPERATORS(GridStatistics)
	};
	#pragma pack(pop) // #pragma pack(push, 1)
};

struct IGRID_User_LocalMethod : public iscm_ILocalMethodBase, public GRID_User_T
{
	typedef TCPSError (*FN_OnStreamedCall_L_)(
				IN void* sessionObj,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	typedef TCPSError (*FN_AddJobProgram)(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

	typedef TCPSError (*FN_DelJobProgram)(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& programKeys
				) method;

	typedef TCPSError (*FN_FindJobProgram)(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method;

	typedef TCPSError (*FN_ListJobPrograms)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<JobProgram>& jobPrograms
				) method;

	typedef TCPSError (*FN_CommitJob)(
				IN void* sessionObj_wrap,
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method;

	typedef TCPSError (*FN_CancelJob)(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& jobKeys
				) posting_method;

	typedef TCPSError (*FN_SetJobPriority)(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method;

	typedef TCPSError (*FN_ListJobs)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method;

	typedef TCPSError (*FN_QueryJobs)(
				IN void* sessionObj_wrap,
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method;

	typedef TCPSError (*FN_UpdateGrid)(
				IN void* sessionObj_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

	typedef TCPSError (*FN_ListModules)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method;

	typedef TCPSError (*FN_GetLogCount)(
				IN void* sessionObj_wrap,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method;

	typedef TCPSError (*FN_LoadLogInfos)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method;

	typedef TCPSError (*FN_Login)(
				IN void* sessionObj_wrap,
				IN const tcps_String& userName,
				IN const tcps_String& password
				) method;

	typedef TCPSError (*FN_Logout)(
				IN void* sessionObj_wrap
				) posting_method;

	typedef TCPSError (*FN_AddUser)(
				IN void* sessionObj_wrap,
				IN const UserInfo& userInfo
				) method;

	typedef TCPSError (*FN_DelUser)(
				IN void* sessionObj_wrap,
				IN const tcps_Array<tcps_String>& userNames
				) method;

	typedef TCPSError (*FN_UpdatePassword)(
				IN void* sessionObj_wrap,
				IN const tcps_String& oldPassword,
				IN const tcps_String& newPassword
				) method;

	typedef TCPSError (*FN_ManageUser)(
				IN void* sessionObj_wrap,
				IN const UserInfo& userInfo
				) method;

	typedef TCPSError (*FN_ListAllUsers)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<UserInfo>& userInfos
				) method;

	typedef TCPSError (*FN_GetUserInfo)(
				IN void* sessionObj_wrap,
				IN const tcps_String& userName,
				OUT UserInfo& userInfo
				) method;

	typedef TCPSError (*FN_ListJTMs)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<tcps_String>& jtms
				) method;

	typedef TCPSError (*FN_GetJTMInfo)(
				IN void* sessionObj_wrap,
				IN const tcps_String& jtm,
				OUT JTMInfo& jtmInfo
				) method;

	typedef TCPSError (*FN_GetGridProperty)(
				IN void* sessionObj_wrap,
				OUT GridProperty& gridProperty
				) method;

	typedef TCPSError (*FN_AddSplitter)(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

	typedef TCPSError (*FN_DelSplitter)(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramInfo& splitter
				) method;

	typedef TCPSError (*FN_ListSplitters)(
				IN void* sessionObj_wrap,
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method;

	typedef TCPSError (*FN_ListSplitterParam)(
				IN void* sessionObj_wrap,
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method;

	typedef TCPSError (*FN_GetGridStatistics)(
				IN void* sessionObj_wrap,
				OUT GridStatistics& stat
				) method;
};

struct IGRID_User_LocalCallback : public iscm_ILocalCallbackBase, public GRID_User_T
{
	typedef TCPSError (*FN_OnStreamedCallback_L_)(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

	typedef TCPSError (*FN_ReplyCommitJob)(
				IN void* sessionObj_wrap,
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback;
};

typedef TCPSError (*FNMakeLocalSession_GRID_User)(
			IN const IPP& clientID_IPP,
			IN NP_GridUserSessionMaker& sessionMaker,
			OUT IGRID_User_LocalMethod*& methodHandler,
			IN IGRID_User_LocalCallback* callbackHandler
			);
#endif	// #ifndef GRID_User_T_defined


/////////////////////////////////////////////////////////////////////
// 结构体的Compare()函数内联实现

#ifndef GRID_Version_Compare_defined
	#define GRID_Version_Compare_defined
	inline int GRID_Version::Compare(const GRID_Version& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->majorVer, r.majorVer);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->minorVer, r.minorVer);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_Version_Compare_defined

#ifndef GRID_ProgramID_Compare_defined
	#define GRID_ProgramID_Compare_defined
	inline int GRID_ProgramID::Compare(const GRID_ProgramID& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->name, r.name);
		if(0 != cmp)
			return cmp;
		cmp = this->ver.Compare(r.ver);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuType, r.cpuType);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->osType, r.osType);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->executeBits, r.executeBits);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->binaryType, r.binaryType);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_ProgramID_Compare_defined

#ifndef GRID_ProgramInfo_Compare_defined
	#define GRID_ProgramInfo_Compare_defined
	inline int GRID_ProgramInfo::Compare(const GRID_ProgramInfo& r) const
	{
		int cmp = 0;
		cmp = this->programID.Compare(r.programID);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->description, r.description);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_ProgramInfo_Compare_defined

#ifndef GRID_JobInfo_Compare_defined
	#define GRID_JobInfo_Compare_defined
	inline int GRID_JobInfo::Compare(const GRID_JobInfo& r) const
	{
		int cmp = 0;
		cmp = this->programID.Compare(r.programID);
		if(0 != cmp)
			return cmp;
		cmp = this->splitter.Compare(r.splitter);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->dataInputUrl, r.dataInputUrl);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->dataOutputUrl, r.dataOutputUrl);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->priority, r.priority);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jobTaskTimeout, r.jobTaskTimeout);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jobTaskMaxAttempts, r.jobTaskMaxAttempts);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->skipFailedJobTaskPercent, r.skipFailedJobTaskPercent);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jobName, r.jobName);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->splitterParam, r.splitterParam);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->programParam, r.programParam);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_JobInfo_Compare_defined

#ifndef GRID_JobTaskInfo_Compare_defined
	#define GRID_JobTaskInfo_Compare_defined
	inline int GRID_JobTaskInfo::Compare(const GRID_JobTaskInfo& r) const
	{
		int cmp = 0;
		cmp = this->programID.Compare(r.programID);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->dataInputUrl, r.dataInputUrl);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->dataOutputUrl, r.dataOutputUrl);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->programParam, r.programParam);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_JobTaskInfo_Compare_defined

#ifndef GRID_ProgramFile_Compare_defined
	#define GRID_ProgramFile_Compare_defined
	inline int GRID_ProgramFile::Compare(const GRID_ProgramFile& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->isExecutable, r.isExecutable);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->name, r.name);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->data, r.data);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_ProgramFile_Compare_defined

#ifndef GRID_JobTaskReport_Compare_defined
	#define GRID_JobTaskReport_Compare_defined
	inline int GRID_JobTaskReport::Compare(const GRID_JobTaskReport& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->jobTaskKey, r.jobTaskKey);
		if(0 != cmp)
			return cmp;
		cmp = this->jobTaskInfo.Compare(r.jobTaskInfo);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->progress, r.progress);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->costedMSELs, r.costedMSELs);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_JobTaskReport_Compare_defined

#ifndef GRID_MachineStaticContext_Compare_defined
	#define GRID_MachineStaticContext_Compare_defined
	inline int GRID_MachineStaticContext::Compare(const GRID_MachineStaticContext& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->cpuType, r.cpuType);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuFreq, r.cpuFreq);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuProcessers, r.cpuProcessers);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuThreads, r.cpuThreads);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->totalMemoryBytes, r.totalMemoryBytes);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->networkBandwidth, r.networkBandwidth);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->osType, r.osType);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->osDetail, r.osDetail);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->executeBits, r.executeBits);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->binaryType, r.binaryType);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->hardwareInfo, r.hardwareInfo);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_MachineStaticContext_Compare_defined

#ifndef GRID_MachineDynamicContext_Compare_defined
	#define GRID_MachineDynamicContext_Compare_defined
	inline int GRID_MachineDynamicContext::Compare(const GRID_MachineDynamicContext& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->cpuUsage, r.cpuUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->memoryUsage, r.memoryUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->gridMemoryUsage, r.gridMemoryUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->networkUsage, r.networkUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jobbingTaskCount, r.jobbingTaskCount);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_MachineDynamicContext_Compare_defined

#ifndef GRID_MachineStatistic_Compare_defined
	#define GRID_MachineStatistic_Compare_defined
	inline int GRID_MachineStatistic::Compare(const GRID_MachineStatistic& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->tasksOK, r.tasksOK);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->tasksFailed, r.tasksFailed);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->tasksProcessing, r.tasksProcessing);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuPeakUsage, r.cpuPeakUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->cpuAvgUsage, r.cpuAvgUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->memoryPeakUsage, r.memoryPeakUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->memoryAvgUsage, r.memoryAvgUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->networkPeakUsage, r.networkPeakUsage);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->networkAvgUsage, r.networkAvgUsage);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_MachineStatistic_Compare_defined

#ifndef GRID_ImageProperty_Compare_defined
	#define GRID_ImageProperty_Compare_defined
	inline int GRID_ImageProperty::Compare(const GRID_ImageProperty& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->width, r.width);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->height, r.height);
		if(0 != cmp)
			return cmp;
		cmp = memcmp(this->step, r.step, sizeof(this->step));
		if(0 != cmp)
			return (cmp>0 ? 1 : -1);
		cmp = SimpleTypeCompare_(this->timeStamp, r.timeStamp);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->colorSpace, r.colorSpace);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->imageData, r.imageData);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_ImageProperty_Compare_defined

#ifndef GRID_User_JobProgram_Compare_defined
	#define GRID_User_JobProgram_Compare_defined
	inline int GRID_User_T::JobProgram::Compare(const JobProgram& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->programKey, r.programKey);
		if(0 != cmp)
			return cmp;
		cmp = this->programInfo.Compare(r.programInfo);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->addTime, r.addTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->programSize, r.programSize);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_JobProgram_Compare_defined

#ifndef GRID_User_JobReport_Compare_defined
	#define GRID_User_JobReport_Compare_defined
	inline int GRID_User_T::JobReport::Compare(const JobReport& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->jobKey, r.jobKey);
		if(0 != cmp)
			return cmp;
		cmp = this->jobInfo.Compare(r.jobInfo);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jobState, r.jobState);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->progress, r.progress);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->costedMSELs, r.costedMSELs);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->commitTime, r.commitTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->errorCode, r.errorCode);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->causesOfError, r.causesOfError);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_JobReport_Compare_defined

#ifndef GRID_User_LogInfo_Compare_defined
	#define GRID_User_LogInfo_Compare_defined
	inline int GRID_User_T::LogInfo::Compare(const LogInfo& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->time, r.time);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->info, r.info);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_LogInfo_Compare_defined

#ifndef GRID_User_UserInfo_Compare_defined
	#define GRID_User_UserInfo_Compare_defined
	inline int GRID_User_T::UserInfo::Compare(const UserInfo& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->userName, r.userName);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->passWord, r.passWord);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->role, r.role);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_UserInfo_Compare_defined

#ifndef GRID_User_JTMInfo_Compare_defined
	#define GRID_User_JTMInfo_Compare_defined
	inline int GRID_User_T::JTMInfo::Compare(const JTMInfo& r) const
	{
		int cmp = 0;
		cmp = this->staticContext.Compare(r.staticContext);
		if(0 != cmp)
			return cmp;
		cmp = this->statisticInfo.Compare(r.statisticInfo);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_JTMInfo_Compare_defined

#ifndef GRID_User_GridProperty_Compare_defined
	#define GRID_User_GridProperty_Compare_defined
	inline int GRID_User_T::GridProperty::Compare(const GridProperty& r) const
	{
		int cmp = 0;
		cmp = this->version.Compare(r.version);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->jtmNum, r.jtmNum);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->processorNum, r.processorNum);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->threadNum, r.threadNum);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->authorizedID, r.authorizedID);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_GridProperty_Compare_defined

#ifndef GRID_User_SplitterParam_Compare_defined
	#define GRID_User_SplitterParam_Compare_defined
	inline int GRID_User_T::SplitterParam::Compare(const SplitterParam& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->name, r.name);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->data, r.data);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->description, r.description);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_SplitterParam_Compare_defined

#ifndef GRID_User_GridStatistics_Compare_defined
	#define GRID_User_GridStatistics_Compare_defined
	inline int GRID_User_T::GridStatistics::Compare(const GridStatistics& r) const
	{
		int cmp = 0;
		cmp = SimpleTypeCompare_(this->startupTime, r.startupTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->runTime, r.runTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->errorCount, r.errorCount);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->recoveredErrorCount, r.recoveredErrorCount);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->unrecoverErrorCount, r.unrecoverErrorCount);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->maxJTM, r.maxJTM);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->avgJTM, r.avgJTM);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->maxJobTime, r.maxJobTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->minJobTime, r.minJobTime);
		if(0 != cmp)
			return cmp;
		cmp = SimpleTypeCompare_(this->avgJobTime, r.avgJobTime);
		if(0 != cmp)
			return cmp;
		return 0;
	}
#endif	// #ifndef GRID_User_GridStatistics_Compare_defined


/////////////////////////////////////////////////////////////////////
// 结构体的流式支持内联函数

#ifndef GRID_ProgramID_STREAMED_FUNCTIONS_defined
	#define GRID_ProgramID_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_ProgramID& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.name);
		size += (int)sizeof(val.ver);
		size += (int)sizeof(val.cpuType);
		size += (int)sizeof(val.osType);
		size += (int)sizeof(val.executeBits);
		size += (int)sizeof(val.binaryType);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_ProgramID& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.name, data);
		iscm_StreamedLoad(val.ver, data);
		iscm_StreamedLoad(val.cpuType, data);
		iscm_StreamedLoad(val.osType, data);
		iscm_StreamedLoad(val.executeBits, data);
		iscm_StreamedLoad(val.binaryType, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_ProgramID& val)
	{
		iscm_StreamedStore(buf, val.name);
		iscm_StreamedStore(buf, val.ver);
		iscm_StreamedStore(buf, val.cpuType);
		iscm_StreamedStore(buf, val.osType);
		iscm_StreamedStore(buf, val.executeBits);
		iscm_StreamedStore(buf, val.binaryType);
	}
#endif	// #ifndef GRID_ProgramID_STREAMED_FUNCTIONS_defined

#ifndef GRID_ProgramInfo_STREAMED_FUNCTIONS_defined
	#define GRID_ProgramInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_ProgramInfo& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.programID);
		size += iscm_GetStreamedSize(val.description);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_ProgramInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.programID, data);
		iscm_StreamedLoad(val.description, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_ProgramInfo& val)
	{
		iscm_StreamedStore(buf, val.programID);
		iscm_StreamedStore(buf, val.description);
	}
#endif	// #ifndef GRID_ProgramInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_JobInfo_STREAMED_FUNCTIONS_defined
	#define GRID_JobInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_JobInfo& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.programID);
		size += iscm_GetStreamedSize(val.splitter);
		size += iscm_GetStreamedSize(val.dataInputUrl);
		size += iscm_GetStreamedSize(val.dataOutputUrl);
		size += (int)sizeof(val.priority);
		size += (int)sizeof(val.jobTaskTimeout);
		size += (int)sizeof(val.jobTaskMaxAttempts);
		size += (int)sizeof(val.skipFailedJobTaskPercent);
		size += iscm_GetStreamedSize(val.jobName);
		size += iscm_GetStreamedSize(val.splitterParam);
		size += iscm_GetStreamedSize(val.programParam);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_JobInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.programID, data);
		iscm_StreamedLoad(val.splitter, data);
		iscm_StreamedLoad(val.dataInputUrl, data);
		iscm_StreamedLoad(val.dataOutputUrl, data);
		iscm_StreamedLoad(val.priority, data);
		iscm_StreamedLoad(val.jobTaskTimeout, data);
		iscm_StreamedLoad(val.jobTaskMaxAttempts, data);
		iscm_StreamedLoad(val.skipFailedJobTaskPercent, data);
		iscm_StreamedLoad(val.jobName, data);
		iscm_StreamedLoad(val.splitterParam, data);
		iscm_StreamedLoad(val.programParam, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_JobInfo& val)
	{
		iscm_StreamedStore(buf, val.programID);
		iscm_StreamedStore(buf, val.splitter);
		iscm_StreamedStore(buf, val.dataInputUrl);
		iscm_StreamedStore(buf, val.dataOutputUrl);
		iscm_StreamedStore(buf, val.priority);
		iscm_StreamedStore(buf, val.jobTaskTimeout);
		iscm_StreamedStore(buf, val.jobTaskMaxAttempts);
		iscm_StreamedStore(buf, val.skipFailedJobTaskPercent);
		iscm_StreamedStore(buf, val.jobName);
		iscm_StreamedStore(buf, val.splitterParam);
		iscm_StreamedStore(buf, val.programParam);
	}
#endif	// #ifndef GRID_JobInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_JobTaskInfo_STREAMED_FUNCTIONS_defined
	#define GRID_JobTaskInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_JobTaskInfo& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.programID);
		size += iscm_GetStreamedSize(val.dataInputUrl);
		size += iscm_GetStreamedSize(val.dataOutputUrl);
		size += iscm_GetStreamedSize(val.programParam);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_JobTaskInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.programID, data);
		iscm_StreamedLoad(val.dataInputUrl, data);
		iscm_StreamedLoad(val.dataOutputUrl, data);
		iscm_StreamedLoad(val.programParam, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_JobTaskInfo& val)
	{
		iscm_StreamedStore(buf, val.programID);
		iscm_StreamedStore(buf, val.dataInputUrl);
		iscm_StreamedStore(buf, val.dataOutputUrl);
		iscm_StreamedStore(buf, val.programParam);
	}
#endif	// #ifndef GRID_JobTaskInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_ProgramFile_STREAMED_FUNCTIONS_defined
	#define GRID_ProgramFile_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_ProgramFile& val)
	{
		int size = 0;
		size += (int)sizeof(val.isExecutable);
		size += iscm_GetStreamedSize(val.name);
		size += iscm_GetStreamedSize(val.data);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_ProgramFile& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.isExecutable, data);
		iscm_StreamedLoad(val.name, data);
		iscm_StreamedLoad(val.data, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_ProgramFile& val)
	{
		iscm_StreamedStore(buf, val.isExecutable);
		iscm_StreamedStore(buf, val.name);
		iscm_StreamedStore(buf, val.data);
	}
#endif	// #ifndef GRID_ProgramFile_STREAMED_FUNCTIONS_defined

#ifndef GRID_JobTaskReport_STREAMED_FUNCTIONS_defined
	#define GRID_JobTaskReport_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_JobTaskReport& val)
	{
		int size = 0;
		size += (int)sizeof(val.jobTaskKey);
		size += iscm_GetStreamedSize(val.jobTaskInfo);
		size += (int)sizeof(val.progress);
		size += (int)sizeof(val.costedMSELs);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_JobTaskReport& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.jobTaskKey, data);
		iscm_StreamedLoad(val.jobTaskInfo, data);
		iscm_StreamedLoad(val.progress, data);
		iscm_StreamedLoad(val.costedMSELs, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_JobTaskReport& val)
	{
		iscm_StreamedStore(buf, val.jobTaskKey);
		iscm_StreamedStore(buf, val.jobTaskInfo);
		iscm_StreamedStore(buf, val.progress);
		iscm_StreamedStore(buf, val.costedMSELs);
	}
#endif	// #ifndef GRID_JobTaskReport_STREAMED_FUNCTIONS_defined

#ifndef GRID_MachineStaticContext_STREAMED_FUNCTIONS_defined
	#define GRID_MachineStaticContext_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_MachineStaticContext& val)
	{
		int size = 0;
		size += (int)sizeof(val.cpuType);
		size += (int)sizeof(val.cpuFreq);
		size += (int)sizeof(val.cpuProcessers);
		size += (int)sizeof(val.cpuThreads);
		size += (int)sizeof(val.totalMemoryBytes);
		size += (int)sizeof(val.networkBandwidth);
		size += (int)sizeof(val.osType);
		size += iscm_GetStreamedSize(val.osDetail);
		size += (int)sizeof(val.executeBits);
		size += (int)sizeof(val.binaryType);
		size += iscm_GetStreamedSize(val.hardwareInfo);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_MachineStaticContext& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.cpuType, data);
		iscm_StreamedLoad(val.cpuFreq, data);
		iscm_StreamedLoad(val.cpuProcessers, data);
		iscm_StreamedLoad(val.cpuThreads, data);
		iscm_StreamedLoad(val.totalMemoryBytes, data);
		iscm_StreamedLoad(val.networkBandwidth, data);
		iscm_StreamedLoad(val.osType, data);
		iscm_StreamedLoad(val.osDetail, data);
		iscm_StreamedLoad(val.executeBits, data);
		iscm_StreamedLoad(val.binaryType, data);
		iscm_StreamedLoad(val.hardwareInfo, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_MachineStaticContext& val)
	{
		iscm_StreamedStore(buf, val.cpuType);
		iscm_StreamedStore(buf, val.cpuFreq);
		iscm_StreamedStore(buf, val.cpuProcessers);
		iscm_StreamedStore(buf, val.cpuThreads);
		iscm_StreamedStore(buf, val.totalMemoryBytes);
		iscm_StreamedStore(buf, val.networkBandwidth);
		iscm_StreamedStore(buf, val.osType);
		iscm_StreamedStore(buf, val.osDetail);
		iscm_StreamedStore(buf, val.executeBits);
		iscm_StreamedStore(buf, val.binaryType);
		iscm_StreamedStore(buf, val.hardwareInfo);
	}
#endif	// #ifndef GRID_MachineStaticContext_STREAMED_FUNCTIONS_defined

#ifndef GRID_ImageProperty_STREAMED_FUNCTIONS_defined
	#define GRID_ImageProperty_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_ImageProperty& val)
	{
		int size = 0;
		size += (int)sizeof(val.width);
		size += (int)sizeof(val.height);
		size += (int)sizeof(val.step);
		size += (int)sizeof(val.timeStamp);
		size += (int)sizeof(val.colorSpace);
		size += iscm_GetStreamedSize(val.imageData);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_ImageProperty& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.width, data);
		iscm_StreamedLoad(val.height, data);
		memcpy(val.step, data, sizeof(val.step)); data += sizeof(val.step);
		iscm_StreamedLoad(val.timeStamp, data);
		iscm_StreamedLoad(val.colorSpace, data);
		iscm_StreamedLoad(val.imageData, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_ImageProperty& val)
	{
		iscm_StreamedStore(buf, val.width);
		iscm_StreamedStore(buf, val.height);
		memcpy(buf, val.step, sizeof(val.step)); buf += sizeof(val.step);
		iscm_StreamedStore(buf, val.timeStamp);
		iscm_StreamedStore(buf, val.colorSpace);
		iscm_StreamedStore(buf, val.imageData);
	}
#endif	// #ifndef GRID_ImageProperty_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_JobProgram_STREAMED_FUNCTIONS_defined
	#define GRID_User_JobProgram_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::JobProgram& val)
	{
		int size = 0;
		size += (int)sizeof(val.programKey);
		size += iscm_GetStreamedSize(val.programInfo);
		size += (int)sizeof(val.addTime);
		size += (int)sizeof(val.programSize);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::JobProgram& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.programKey, data);
		iscm_StreamedLoad(val.programInfo, data);
		iscm_StreamedLoad(val.addTime, data);
		iscm_StreamedLoad(val.programSize, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::JobProgram& val)
	{
		iscm_StreamedStore(buf, val.programKey);
		iscm_StreamedStore(buf, val.programInfo);
		iscm_StreamedStore(buf, val.addTime);
		iscm_StreamedStore(buf, val.programSize);
	}
#endif	// #ifndef GRID_User_JobProgram_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_JobReport_STREAMED_FUNCTIONS_defined
	#define GRID_User_JobReport_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::JobReport& val)
	{
		int size = 0;
		size += (int)sizeof(val.jobKey);
		size += iscm_GetStreamedSize(val.jobInfo);
		size += (int)sizeof(val.jobState);
		size += (int)sizeof(val.progress);
		size += (int)sizeof(val.costedMSELs);
		size += (int)sizeof(val.commitTime);
		size += (int)sizeof(val.errorCode);
		size += iscm_GetStreamedSize(val.causesOfError);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::JobReport& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.jobKey, data);
		iscm_StreamedLoad(val.jobInfo, data);
		iscm_StreamedLoad(val.jobState, data);
		iscm_StreamedLoad(val.progress, data);
		iscm_StreamedLoad(val.costedMSELs, data);
		iscm_StreamedLoad(val.commitTime, data);
		iscm_StreamedLoad(val.errorCode, data);
		iscm_StreamedLoad(val.causesOfError, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::JobReport& val)
	{
		iscm_StreamedStore(buf, val.jobKey);
		iscm_StreamedStore(buf, val.jobInfo);
		iscm_StreamedStore(buf, val.jobState);
		iscm_StreamedStore(buf, val.progress);
		iscm_StreamedStore(buf, val.costedMSELs);
		iscm_StreamedStore(buf, val.commitTime);
		iscm_StreamedStore(buf, val.errorCode);
		iscm_StreamedStore(buf, val.causesOfError);
	}
#endif	// #ifndef GRID_User_JobReport_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_LogInfo_STREAMED_FUNCTIONS_defined
	#define GRID_User_LogInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::LogInfo& val)
	{
		int size = 0;
		size += (int)sizeof(val.time);
		size += iscm_GetStreamedSize(val.info);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::LogInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.time, data);
		iscm_StreamedLoad(val.info, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::LogInfo& val)
	{
		iscm_StreamedStore(buf, val.time);
		iscm_StreamedStore(buf, val.info);
	}
#endif	// #ifndef GRID_User_LogInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_UserInfo_STREAMED_FUNCTIONS_defined
	#define GRID_User_UserInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::UserInfo& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.userName);
		size += iscm_GetStreamedSize(val.passWord);
		size += (int)sizeof(val.role);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::UserInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.userName, data);
		iscm_StreamedLoad(val.passWord, data);
		iscm_StreamedLoad(val.role, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::UserInfo& val)
	{
		iscm_StreamedStore(buf, val.userName);
		iscm_StreamedStore(buf, val.passWord);
		iscm_StreamedStore(buf, val.role);
	}
#endif	// #ifndef GRID_User_UserInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_JTMInfo_STREAMED_FUNCTIONS_defined
	#define GRID_User_JTMInfo_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::JTMInfo& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.staticContext);
		size += (int)sizeof(val.statisticInfo);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::JTMInfo& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.staticContext, data);
		iscm_StreamedLoad(val.statisticInfo, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::JTMInfo& val)
	{
		iscm_StreamedStore(buf, val.staticContext);
		iscm_StreamedStore(buf, val.statisticInfo);
	}
#endif	// #ifndef GRID_User_JTMInfo_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_GridProperty_STREAMED_FUNCTIONS_defined
	#define GRID_User_GridProperty_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::GridProperty& val)
	{
		int size = 0;
		size += (int)sizeof(val.version);
		size += (int)sizeof(val.jtmNum);
		size += (int)sizeof(val.processorNum);
		size += (int)sizeof(val.threadNum);
		size += iscm_GetStreamedSize(val.authorizedID);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::GridProperty& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.version, data);
		iscm_StreamedLoad(val.jtmNum, data);
		iscm_StreamedLoad(val.processorNum, data);
		iscm_StreamedLoad(val.threadNum, data);
		iscm_StreamedLoad(val.authorizedID, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::GridProperty& val)
	{
		iscm_StreamedStore(buf, val.version);
		iscm_StreamedStore(buf, val.jtmNum);
		iscm_StreamedStore(buf, val.processorNum);
		iscm_StreamedStore(buf, val.threadNum);
		iscm_StreamedStore(buf, val.authorizedID);
	}
#endif	// #ifndef GRID_User_GridProperty_STREAMED_FUNCTIONS_defined

#ifndef GRID_User_SplitterParam_STREAMED_FUNCTIONS_defined
	#define GRID_User_SplitterParam_STREAMED_FUNCTIONS_defined
	inline int iscm_GetStreamedSize(const GRID_User_T::SplitterParam& val)
	{
		int size = 0;
		size += iscm_GetStreamedSize(val.name);
		size += iscm_GetStreamedSize(val.data);
		size += iscm_GetStreamedSize(val.description);
		return size;
	}
	inline void iscm_StreamedLoad(GRID_User_T::SplitterParam& val, const BYTE*& data)
	{
		iscm_StreamedLoad(val.name, data);
		iscm_StreamedLoad(val.data, data);
		iscm_StreamedLoad(val.description, data);
	}
	inline void iscm_StreamedStore(BYTE*& buf, const GRID_User_T::SplitterParam& val)
	{
		iscm_StreamedStore(buf, val.name);
		iscm_StreamedStore(buf, val.data);
		iscm_StreamedStore(buf, val.description);
	}
#endif	// #ifndef GRID_User_SplitterParam_STREAMED_FUNCTIONS_defined

#endif	// #ifndef _NP_GridUserTypesDefine_h_
