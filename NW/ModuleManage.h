#pragma once

#include "NP_SCATTEREDTypesDefine.h"
#include "Ilocker.h"
#include <map>
#include <queue>
//#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include "my_np_griduserclient.h"
#include "npfdk.h"
#include "strtmpl.h"
#include "nplog.h"
//#include "np_scatteredsession.h"
#define BASE_ID 1000000
class PCC_Service_S;
class PCC_Scatter_S;
//模块管理

//应为全局对象
class CScatteredManage//:public boost::enable_shared_from_this<CModuleManage>
{
public:
typedef std::map<INT64,/*PCC_ModuleInfo*/PCC_ModuleProperty>::const_iterator MNG_IT;


public:
	CScatteredManage(void);
	~CScatteredManage(void);


	bool find(INT64 key,PCC_ModuleProperty &info)
	{
		CNPAutoLock lok(m_lock_map);//必须加锁，否则it可能失效
		MNG_IT it = m_modules.find(key);
		if(it != m_modules.end())
		{
             info = it->second;
			 return true;
		}
		return false;
	}

	bool insert(std::pair<INT64,PCC_ModuleProperty> const &  val)
	{
		CNPAutoLock lok(m_lock_map);
		std::map<INT64,PCC_ModuleProperty>::_Pairib rt = m_modules.insert(val);
		return rt.second;

	}
    INT64 generateKey()
	{
		InterlockedIncrement64(&m_lastkey);
		return m_lastkey;//maybe
	}
	INT64 generateJobkey()
	{
		InterlockedIncrement64(&m_jobkey);
		return m_jobkey;
	}
	TCPSError ListModules(
		IN INT32 moduleType,
		OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
		) 
	{
        CNPAutoLock lok(m_lock_map);
		if(moduleType == 0)//列举所有的模块
		{
			modulesInfo.Resize(m_modules.size());
			int k=0;
			for(MNG_IT it = m_modules.begin();it!= m_modules.end();++it)
			{
				modulesInfo[k].description = it->second.description;
				modulesInfo[k].moduleFileType = it->second.moduleFileType;
				modulesInfo[k].moduleKey = it->first;
				modulesInfo[k].moduleLatency = it->second.moduleLatency;
				modulesInfo[k].modulePattern = it->second.modulePattern;
				modulesInfo[k].moduleTag = it->second.moduleTag;
				modulesInfo[k].moduleType = (int)it->second.moduleType;
				++k;
			}
		}
		else
		{
			int k=0;
			for(MNG_IT it = m_modules.begin();it!= m_modules.end();++it)
			{
				if(it->second.moduleType & moduleType)
				{
					++k;
					modulesInfo.Resize(k);//可能会超出内存池限制
					modulesInfo[k-1].description = it->second.description;
					modulesInfo[k-1].moduleFileType = it->second.moduleFileType;
					modulesInfo[k-1].moduleKey = it->first;
					modulesInfo[k-1].moduleLatency = it->second.moduleLatency;
					modulesInfo[k-1].modulePattern = it->second.modulePattern;
					modulesInfo[k-1].moduleTag = it->second.moduleTag;
					modulesInfo[k-1].moduleType = (int)it->second.moduleType;
					
				}
			}
		}
		return TCPS_OK;
	}

	TCPSError ListModules(
		OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
		) 
	{
		CNPAutoLock lok(m_lock_map);
		modulesInfo.Resize(m_modules.size());
		int k=0;
		for(MNG_IT it = m_modules.begin();it!= m_modules.end();++it)
		{
			modulesInfo[k].key = it->first;
			modulesInfo[k].prop = it->second;
		}
		return TCPS_OK;
	}

	TCPSError RemoveModule(
		IN INT64 moduleKey
		) 
	{
		CNPAutoLock lok(m_lock_map);
		
		if(!m_modules.erase(moduleKey))
		{
			NPLogError(("找不到#%d#对应的模块",moduleKey));
		}
		return TCPS_OK;
	}

	void pushNode(INT32 key,PCC_Scatter_S* nd)
	{
		CNPAutoLock lock(m_lock_deque);
		//nd->m_jobkey = -1;//重置
		std::map<INT32,PCC_Scatter_S*> ::_Pairib rt= m_nodes.insert(std::make_pair(key,nd));
		ASSERT(rt.second);
		{
			printf("pushNode#当前节点数:%d\n",m_nodes.size());
		}
	}

	bool popNode(SCNode &nd)
	{
		CNPAutoLock lock(m_lock_deque);
		std::map<INT32,PCC_Scatter_S*>::const_iterator
			it = m_nodes.begin();
		
		if(it != m_nodes.end())//可能为空
		{
			nd.key = it->first;
			nd.ps = it->second;
			m_nodes.erase(nd.key);///
			return true;
		}
		return false;	  
	}
	////////////////////////////////////////////////////////
	void clearInvalidJobs(std::queue<INT64> & q)
	{
		INT64 k = -1;
		CNPAutoLock lock(m_locker_job_service);
		while(!q.empty())
		{
            k =q.front();
			q.pop();
			m_job_service.erase(k);
		}

	}

	void pushSS(INT64 jk,PCC_Service_S * ss)
	{
        CNPAutoLock lock(m_locker_job_service);
		printf("%s,%lld,%p\n",__FUNCTION__,jk,ss);
		m_job_service.insert(std::make_pair(jk,ss));

	}
	bool isValidJobRequest(INT64 jk)
	{
		CNPAutoLock lock(m_locker_job_service);
		std::map<INT64,PCC_Service_S*>::const_iterator it = m_job_service.find(jk);
		if(it != m_job_service.end())//可能为空
			return true;
		return false;
	}
	PCC_Service_S * getSS(INT64 jk,bool& isFound)
	{
		isFound = false;
		CNPAutoLock lock(m_locker_job_service);
		std::map<INT64,PCC_Service_S*>::const_iterator it = m_job_service.find(jk);
		if(it != m_job_service.end())//可能为空
		{
			isFound = true;
			return it->second;
		}
		return NULL;
	}
	TCPSError callbackSS(INT64 jk,IN INT64 taskKey,
			IN TCPSError errorCode,
			IN const tcps_Binary& context);
		/*{
	        CNPAutoLock lock(m_locker_job_service);
			std::map<INT64,PCC_Service_S*>::const_iterator it = m_job_service.find(jk);
			if(it != m_job_service.end())//可能为空
			{
				
				it->second->OnExecuted(taskKey,errorCode,context);
				m_job_service.erase(jk);///
				//return true;
			}
		}*/
	
	/////////////////////////////////////////////////////////

	void diableNode(INT32 key)
	{
        CNPAutoLock lock(m_lock_deque);
		m_nodes.erase(key);
	}
    int getNodeCounts()
	{
		CNPAutoLock lock(m_lock_deque);
		return m_nodes.size();
	}
	int getJobsCounts()
	{
		CNPAutoLock lock(m_lock_job);
		return m_jobs.size();
	}
	void pushJob(SCJob& sj)
	{
		CNPAutoLock lock(m_lock_job);
        m_jobs.push(sj);
	}

	bool popJob(SCJob& sj)
	{
		CNPAutoLock lock(m_lock_job);
		if(m_jobs.size()<1)		
			return false;
		
        sj = m_jobs.front();
		m_jobs.pop();
		return true;
	}
	static int processJobs(void* param)
	{
        return ((CScatteredManage*)param)->processJobs();
	}
	int processJobs();
	BOOL getModules(PCC_ModuleIndex &index,tcps_Array<PCC_ModuleFile> &moudleFiles);
public:
	void mytest()
	{
		//PVG://IPP/Passticket/avPath，用于实时分析PVG视频，该作业不用分割
		//VFR://IPP/admin/admin:/FilePath，用于视频转码，即下载原始视频，改作业不用分割
		//VFR://IPP/admin/admin:/FilePath:BeginFrame:EndFrame ，用于分析视图库内的视频，分割单位为帧。
		//NFS:\\Path\StartPos\EndPos
		//PVG://IPP/Passticket/AvPath/VodType/StartTime/EndTime
		std::queue<int> qq;
		int c = qq.front();
		printf("%d\n",c);
		
		exit(0);
	}
public:
	void init();
	BOOL RegisterModules (const char *moduleDir);
	BOOL CopyModules (const char *moduleDir,tcps_Array<PCC_ModuleFile> &moudleFiles);
	BOOL CScatteredManage::GridModules();
private:
	OSThread m_threadID;

private:
	CLocker m_lock_map;
	CLocker m_lock2;//scatter队列的锁
	std::map<INT64,/*PCC_ModuleInfo*/PCC_ModuleProperty> m_modules;
public:    
	INT64 m_lastkey;
    INT64 m_jobkey;

private:
	std::map<INT32,PCC_Scatter_S*> m_nodes;//iscm会保证这个指针的有效性
	CLocker m_lock_deque;
	std::map<INT64,PCC_Service_S*> m_job_service;//service提交作业，待处理的会话
	CLocker m_locker_job_service;
private:
	std::queue<SCJob> m_jobs;
	CLocker m_lock_job;
	//MY_NP_GridUserClient m_gridConn;//建立到网格的连接
};


class CPCCHandler//:public boost::enable_shared_from_this<CPCCHandler>
{
public:
	CPCCHandler(void* session);
	CPCCHandler();
	~CPCCHandler();

	TCPSError OnConnected(
		IN INT32 sessionKey,
		IN const IPP& peerID_IPP,
		IN INT32 sessionCount
		);
	void OnClose(
		IN INT32 sessionKey,
		IN const IPP& peerID_IPP,
		IN TCPSError cause
		);
	
	TCPSError Login(
		IN const tcps_String& ticket
		) ;

	TCPSError Logout(
		);
	TCPSError Execute(
		IN INT64 moduleKey,
		IN const tcps_String& inputUrl,
		IN const tcps_String& outputUrl,
		IN const tcps_Binary& moduleParams,
		OUT INT64& jobKey
		) ;
	TCPSError QueryJobs(
		IN const tcps_Array<INT64>& jobsKey,
		OUT tcps_Array<PCC_Service_T::ExecuteState>& jobsState
		);
	TCPSError CancelJob(
		IN INT64 jobKey
		) ;
	TCPSError AddModule(
		IN const PCC_ModuleProperty& moduleProperty,
		IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
		OUT INT64& moduleKey
		);

	TCPSError ListModules(
		IN INT32 moduleType,
		OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
		) ;

	TCPSError ListModules(
		OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
		) ;

	TCPSError RemoveModule(
		IN INT64 moduleKey
		) ;
public:
	void SetSenssion(void* session);
private:
	MY_NP_GridUserClient m_gridConn;
	IPP m_client_ipp;

	PCC_Service_S *m_psenssion;
	//CModuleManage& m_modules;
};