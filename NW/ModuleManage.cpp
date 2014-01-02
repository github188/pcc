#include "StdAfx.h"
#include "ModuleManage.h"
#include "singleton.h"
#include "nplog.h"
#include "filesearch.h"
#include <sstream>
#include "filestorage.h"
#include "NP_SCATTEREDSession.h"
#include "strtmpl.h "
//#include "singleton.h"
#include "ipcvt.h"
typedef char BIN_DIR[MAX_PATH] ;
char bin_dir[MAX_PATH];
BIN_DIR &bbb = pgrid_util::Singleton<BIN_DIR>::instance();
//BIN_DIR bbc= (BIN_DIR)bin_dir;
CScatteredManage::CScatteredManage(void)
{
	m_lastkey = BASE_ID;
	m_jobkey = BASE_ID;
	if (!OSCreateThread(&m_threadID, NULL, processJobs, this, 0))
	{
		NPLogError(("创建线程失败"));
	}
}

CScatteredManage::~CScatteredManage(void)
{
}

void CScatteredManage::init()
{
	char startup_dir[MAX_PATH];
	pgrid_util::Singleton<BIN_DIR>::instance();
	GetModuleFileName(NULL,(char*)(void*)pgrid_util::Singleton<BIN_DIR>::instance(),MAX_PATH);
	GetModuleFileName(NULL,startup_dir,MAX_PATH);
	_tcsrchr(startup_dir,'\\')[1] = 0;
	_tcsrchr((char*)(void*)pgrid_util::Singleton<BIN_DIR>::instance(),'\\')[1] = 0;
	strcat(startup_dir,"ScatterProgams");
	if(!IsExistDir(startup_dir))
	{
		if(!CreatePath(startup_dir))
		{
			NPLogError(("创建SCATTERED模块目录失败\n"));
			return;
		}
	}
	//遍历搜索目录 获取模块信息
	//模块名+版本号
	RegisterModules(startup_dir);

	//debug 输出启动注册的模块
	for(MNG_IT it= m_modules.begin();it!=m_modules.end();++it)
	{
		printf("%lld,%s\n",it->first,it->second.moduleTag.name.Get());
	}

	//debug
}
BOOL CScatteredManage::GridModules()
{
	//初始化记录网格的模块
	MY_NP_GridUserClient inner_user;
	inner_user.m_user = "netposa";
	inner_user.m_pass = "netposa";
	IPP serverIPP;
	serverIPP.ip_ = inet_addr("127.0.0.1");//连接本机
	serverIPP.port_ = 9012;
	inner_user.SetServeIPP(serverIPP);

	tcps_Array<GRID_User_T::JobProgram> progInfos;
	TCPSError ret = inner_user.ListJobPrograms(progInfos);
	//PCC_ModuleProperty info;
	if (TCPS_OK == ret)
	{
		PCC_ModuleProperty modulesInfo;
		for (int i=0; i<progInfos.Length(); ++i)
		{
			modulesInfo.moduleTag.name= 
				/*info.moduleTag.name = */progInfos[i].programInfo.programID.name;
			modulesInfo.moduleTag.version.major =
				/*info.moduleTag.version.major =*/ progInfos[i].programInfo.programID.ver.majorVer;
			modulesInfo.moduleTag.version.minor =
				/*info.moduleTag.version.minor =*/ progInfos[i].programInfo.programID.ver.minorVer;
			//modulesInfo.moduleKey =
			//	info.moduleKey = progInfos[i].programKey;
			modulesInfo.modulePattern = PCC_LATENCY_LARGE;
			modulesInfo.moduleFileType =PCC_MODULE_MACHINE_RAW;
			modulesInfo.moduleType = PCC_MODULE_IMGPROC;//这个放在模块文件夹的名称里，待后面调整
			modulesInfo.modulePattern =PCC_MODULE_BACKGROUND;
			insert(std::make_pair(progInfos[i].programKey,modulesInfo));
		}
		return TRUE;
	}
	else
		return FALSE;
	//
}
BOOL CScatteredManage::RegisterModules (const char *moduleDir)//这个函数的调用必然是单线程的，不会有竞争，无需同步
{
	struct TFSCB
	{
		static BOOL fn(LPVOID cbParam, BOOL isDir, LPCTSTR filePathname)
		{
			
			if (isDir)
			{
				//CScatteredManage* pmng = ((CScatteredManage*)cbParam);
				//assert(pmng);
				std::string mod = GetFileName(filePathname);
				//std::string mod_name = mod.substr(0,mod.find_last_of('-',-1));//1xxx7#imgcom
				
				std::string mod_name = mod.substr(mod.find_first_of('#')+1,mod.find_last_of('-',-1)-mod.find_first_of('#')-1);
				std::string ver = mod .substr(mod.find_last_of('-',-1)+1,mod.size() -mod.find_last_of('-',-1));								
				int vmax,vmin,t;				
				sscanf(ver.c_str(),"%d.%d",&vmax,&vmin);

				PCC_ModuleProperty minfo;
				//minfo.moduleKey = ++ pmng->m_lastkey;//scattered的模块id是从BASE_ID开始的，以协同grid的模块id管理
				minfo.moduleTag .name = mod_name.c_str() ;
				minfo.moduleTag.version .major = vmax;
				minfo.moduleTag.version .minor = vmin;

				minfo.moduleFileType = PCC_MODULE_MACHINE_RAW;
				minfo.moduleLatency = PCC_LATENCY_LARGE;
				minfo.modulePattern =PCC_MODULE_BACKGROUND;
				
				minfo.moduleType = PCC_MODULE_IMGPROC;//这个放在模块文件夹的名称里，待后面调整
				INT64 key = Atoi64_(mod.substr(0,mod.find_first_of('#')).c_str());
				((CScatteredManage*)cbParam)->m_lastkey = ((CScatteredManage*)cbParam)->m_lastkey>key?((CScatteredManage*)cbParam)->m_lastkey:key;
				((CScatteredManage*)cbParam)->insert(std::make_pair(key,minfo));
				
			}
			
			return true;
		}

		static BOOL RegMod (const char *srcDir,void* param)
		{
			
			return FileSearchEx(srcDir, "*", 0, fn, param , 0, "", "");
		}
	};

	
	return (TFSCB::RegMod(moduleDir,this)&& GridModules());
}
BOOL CScatteredManage::CopyModules (const char *moduleDir,tcps_Array<PCC_ModuleFile> &moudleFiles)//这个函数的调用必然是单线程的，不会有竞争，无需同步
{
	struct TFSCB
	{
		
		static BOOL fn(LPVOID cbParam, BOOL isDir, LPCTSTR filePathname)
		{

			if (!isDir)
			{
				
				CFileStorage file;
				if (!file.Open(filePathname, CFileStorage::
					fs_read_write_ex))
				{
					NPLogError(("打开文件%s失败\n",filePathname));
					//NPRemoveFileOrDir(jobProgramDir.c_str());
					//return TCPS_ERR_FILE_OPEN;
					return true;
				}
                tcps_Array<PCC_ModuleFile> &moudleFiles = *((tcps_Array<PCC_ModuleFile>*)cbParam);

				std::string str1 =GetFileName(filePathname);
				//std::string mod =str1.substr(mod.find_first_of("\\/")+1,-1);
				//CSmartArray<tstring> namesss;
				//StrSeparater_Sep(filePathname,"\\/",namesss);
				//std::string mod = namesss[namesss.size()-2];
				//std::string mod_name = mod.substr(mod.find_first_of('#')+1,mod.find_last_of('-',-1)-mod.find_first_of('#')-1);
				
				int nfile = moudleFiles.Length();
				moudleFiles.Resize(++nfile);
				moudleFiles[nfile-1].name =str1.c_str();// mod_name.c_str();
				moudleFiles[nfile-1].data.Resize(file.GetSize());
				if (-1 == file.Read(0, moudleFiles[nfile-1].data.Get(), file.GetSize()))
				{
					NPLogError(("读取文件%s失败\n",filePathname));
					//NPRemoveFileOrDir(jobProgramDir.c_str());
					//return TCPS_ERR_FILE_OPEN;
					return true;
				}
				file.Close();

			}

			return true;
		}

		static BOOL CpyMod (const char *srcDir,void* param)
		{

			return FileSearchEx(srcDir, "*", 0, fn, param , 0, "", "");
		}
	};

	
	return (TFSCB::CpyMod(moduleDir,&moudleFiles));
}
BOOL CScatteredManage::getModules(PCC_ModuleIndex &index,tcps_Array<PCC_ModuleFile> &moudleFiles)
{
	PCC_ModuleProperty info;
	if(this->find(index.moduleKey,info))
	{
		index.moduleTag = info.moduleTag;

		//读取文件
		std::ostringstream module_name_t ;
		module_name_t<<pgrid_util::Singleton<BIN_DIR>::instance()<<"ScatterProgams\\"<<index.moduleKey<<"#"<<info.moduleTag.name.Get()<<"-"<< info.moduleTag.version.major<<"."<<info.moduleTag.version.minor;
		if(IsExistPath(module_name_t.str().c_str()))//
		{
			return CopyModules(module_name_t.str().c_str(),moudleFiles);
		}
	}
	return false;
}
TCPSError  CScatteredManage::callbackSS(INT64 jk,IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context)///////
{
	TCPSError rt = TCPS_OK;
	PCC_Service_S* second =NULL;
	{
		CNPAutoLock lock(m_locker_job_service);
		std::map<INT64,PCC_Service_S*>::const_iterator it = m_job_service.find(jk);
		if(it != m_job_service.end())//可能为空
		{

			second = it->second;
			m_job_service.erase(jk);///
			//return true;
		}
		else
		{
			printf("warn:#####%s,%lld,%p,%d\n",__FUNCTION__,jk,second,m_job_service.size());
		}
	}
	
	if(second)
		rt = second->OnExecuted(taskKey,errorCode,context);
	return rt;
}

TCPSError  CScatteredManage::callbackSS1(
				IN INT64 taskKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context,
				IN const tcps_Array<tcps_Binary>& outArgs
				)///////
{
	TCPSError rt = TCPS_OK;
	PCC_Service_S* second =NULL;
	{
		CNPAutoLock lock(m_locker_job_service);
		std::map<INT64,PCC_Service_S*>::const_iterator it = m_job_service.find(taskKey);
		if(it != m_job_service.end())//可能为空
		{

			second = it->second;
			m_job_service.erase(taskKey);///
			//return true;
		}
		else
		{
			printf("warn:#####%s,%lld,%p,%d\n",__FUNCTION__,taskKey,second,m_job_service.size());
		}
	}
	
	if(second)
		rt = second->OnExecuted1(taskKey,errorCode,context,outArgs);
	return rt;
}
int CScatteredManage::processJobs()
{
	SCNode nd;
	SCJob sj;
	tcps_Array<PCC_ModuleFile> moudleFiles;
	PCC_ModuleIndex index;
	//bool b_rest = true,b_r2 = false;
	int pre_node_num =-1;
	while (1)
	{
		if(pre_node_num != getNodeCounts())
		{
			printf("##当前可调度节点数：%d,待处理作业数：%d\n",pre_node_num = getNodeCounts(),getJobsCounts());
			
		}
		
		//检测job队列
		if(popJob(sj))
		{
			
			printf("当前可调度节点数：%d,待处理作业数：%d\n",getNodeCounts(),getJobsCounts()+1);

			//先判断jobkey对应的会话是否有效 //这一步看需求，是否支持不要回调通知的作业提交，这里标志以回调有效的方式处理
            if(!isValidJobRequest(sj.jobKey))
				continue;//忽略

			while(1)//轮询等待有空余节点可做scattered计算
			{
				
				if(popNode(nd))// 注意节点要在作业处理完后收回
				{
				    
					//nd.ps->m_ss = sj.ss;//回调senssion句柄 要是m_ss的servece_s的客户端断开了连接，这个指针会失效//jobkey
					
                    
					nd.ps->m_jobkey = sj.jobKey;
					printf("节点%p处理作业：%lld\n",nd.ps,sj.jobKey);
					index.moduleKey = sj.moduleKey;
					//确保节点端存在 moduleKey指定的模块
					moudleFiles.Release();
				    BOOL isFound = false;
					if(TCPS_OK ==nd.ps->FindModule(sj.moduleKey,isFound) )// 节点端没有,同步此模块
					{
						if(!isFound)
						{
							//读取模块 填充 index,moudleFiles
							if(getModules(index,moudleFiles))
							{
								//再次调用接口
								if(nd.ps->AddModule(sj.moduleKey,moudleFiles) != TCPS_OK)
								{
									nd.ps->m_ss = NULL;//for mark
									pushNode(nd.key,nd.ps);
									NPLogError(("模块更新失败，作业无法调度\n"));
									break;
								}
							}
							else
							{
								nd.ps->m_ss = NULL;//for mark
								pushNode(nd.key,nd.ps);
								NPLogError(("模块读取失败，作业无法调度\n"));
								break;
							}
						}
					}
					else
					{
						pushNode(nd.key,nd.ps);
						NPLogError(("无法获取模块状态！\n"));
						break;
					}
					//节点暂时分离出去了
					//这里应当添加节点处理超时
					nd.ps->Compute(sj.moduleKey,
					sj.jobKey,
					sj.info.dataInputUrl,
					sj.info.dataOutputUrl,
					sj.info.programParam);
					//nd.ps->AddMoudle(index,moudleFiles);
					//nd.ps->RemoveModule(0);
				
					break;
				}
				Sleep(1);
			}
		}
		
		Sleep(1);
	}
	return 0;
}

CPCCHandler::CPCCHandler()
{

}
CPCCHandler::CPCCHandler(void* session)
{
	m_psenssion= (PCC_Service_S *)session;
}
CPCCHandler::~CPCCHandler()
{
	
}
void CPCCHandler::SetSenssion(void* session)
{
	m_psenssion= (PCC_Service_S *)session;
	printf("%s,%p\n",__FUNCTION__,m_psenssion);
}
TCPSError CPCCHandler::OnConnected(
					  IN INT32 sessionKey,
					  IN const IPP& peerID_IPP,
					  IN INT32 sessionCount
					  )
{
	m_client_ipp = peerID_IPP;
	return TCPS_OK;
}
void CPCCHandler::OnClose(
			 IN INT32 sessionKey,
			 IN const IPP& peerID_IPP,
			 IN TCPSError cause
			 )
{
	m_gridConn.SetServeIPP(INVALID_IPP);
}

TCPSError CPCCHandler::Login(
				IN const tcps_String& ticket
				) 
{
	m_gridConn.m_user = "netposa";
	m_gridConn.m_pass = "netposa";

	IPP serverIPP;
	
	serverIPP.ip_ = GetLocalIP();//inet_addr("127.0.0.1");//连接本机
	serverIPP.port_ = 9012;
	m_gridConn.m_service = m_psenssion;
	return m_gridConn.SetServeIPP(serverIPP,0,m_client_ipp);//
}

TCPSError CPCCHandler::Logout(
				 )
{
	return m_gridConn.Logout();
}
TCPSError CPCCHandler::Execute(
				  IN INT64 moduleKey,
				  IN const tcps_String& inputUrl,
				  IN const tcps_String& outputUrl,
				  IN const tcps_Binary& moduleParams,
				  OUT INT64& jobKey
				  ) 
{
	GRID_JobInfo  jobinfo;
	CScatteredManage& modules = pgrid_util::Singleton<CScatteredManage>::instance();
	//std::map<INT64,PCC_ModuleInfo> tt;
	//tt.insert()
	PCC_ModuleProperty info;
	//CModuleManage::MNG_IT it = modules.find(moduleKey);//不能这样实现，it可能失效
	if(modules.find(moduleKey,info)/*it!=modules.end()*/)
	{
		jobinfo.dataInputUrl = inputUrl;
		jobinfo.dataOutputUrl = outputUrl;
		jobinfo.programParam = moduleParams;//应当支持内容拷贝 ，否则有问题

		//jobinfo.splitter.name = "RecordsSplit";	//
		jobinfo.splitter.ver.majorVer = 1;
		jobinfo.splitter.ver.minorVer = 0;

		jobinfo.programID.ver.majorVer = info.moduleTag.version.major;
		jobinfo.programID.ver.minorVer = info.moduleTag.version.minor;
		jobinfo.programID.name = info.moduleTag.name;

		jobinfo.programID.cpuType = cpu_x86_x64;
		jobinfo.programID.osType = ost_Windows_7;
		jobinfo.programID.executeBits = eb_32bits;
		jobinfo.programID.binaryType =bt_machineRaw; 

		jobinfo.priority = GRID_JOBPRIO_NORMAL;
		jobinfo.jobTaskMaxAttempts = 3;
		jobinfo.skipFailedJobTaskPercent = 0;

		if(moduleKey<BASE_ID)//grid job
			return m_gridConn.CommitJob(jobKey, jobinfo);
		else//scatterd job
		{
			SCJob sj;
			sj.ss = m_psenssion;
			sj.moduleKey = moduleKey;
			sj.info = jobinfo;
			sj.jobKey = jobKey = modules.generateJobkey();
			//int a = (int)m_psenssion;
			printf("%s,%lld,%p\n",__FUNCTION__,jobKey,m_psenssion);
			pgrid_util::Singleton<CScatteredManage>::instance().pushSS(jobKey,m_psenssion);//m_psenssion在此刻定有效
			bool isFound;
			PCC_Service_S *tempp= pgrid_util::Singleton<CScatteredManage>::instance().getSS(jobKey,isFound);
			printf("getSS:%d,%p\n",isFound,tempp);
			m_psenssion->m_que_jobkeys.push(jobKey);
			
			modules.pushJob(sj);
            return TCPS_OK;
		}
	}
	else
	{
		return TCPS_ERROR;
	}
}
TCPSError CPCCHandler::QueryJobs(
					IN const tcps_Array<INT64>& jobsKey,
					OUT tcps_Array<PCC_Service_T::ExecuteState>& jobsState
					)
{
	tcps_Array<GRID_User_T::JobReport> jobReports;
	if(TCPS_OK == m_gridConn.QueryJobs(jobsKey,jobReports))
	{
		if(jobReports.Length()>0)jobsState.Resize(jobReports.Length());
		for (int i=0; i<jobReports.Length(); ++i)
		{
			jobsState[i].errorCode =jobReports[i].errorCode ;
			jobsState[i].progress = jobReports[i].progress;
		}
		return TCPS_OK;
	}
	return TCPS_ERROR;
}
TCPSError CPCCHandler::CancelJob(
					IN INT64 jobKey
					) 
{
	tcps_Array<INT64> tc;
	tc.Resize(1);
	tc[0]= jobKey;
	return m_gridConn.CancelJob(tc);
}

TCPSError CPCCHandler::AddModule(
					IN const PCC_ModuleProperty& moduleProperty,
					IN const tcps_Array<PCC_ModuleFile>& moudleFiles,
					OUT INT64& moduleKey
					)
{
	CScatteredManage& modules = pgrid_util::Singleton<CScatteredManage>::instance();

	if(moduleProperty.moduleType == PCC_MODULE_VIDSTRUCTURE)//网格的算法库
	{
		GRID_ProgramInfo progInfo;
		progInfo.programID.name = moduleProperty.moduleTag.name;
		progInfo.programID.ver.majorVer = moduleProperty.moduleTag.version.major;
		progInfo.programID.ver.minorVer = moduleProperty.moduleTag.version.minor;


		progInfo.programID.cpuType = (GRID_CPUType )0;// cpu_x86_x64;
		progInfo.programID.osType = ost_Windows_7;//ost_Unknown;
		progInfo.programID.executeBits = (GRID_ExecuteBits )32;//eb_32bits;
		progInfo.programID.binaryType =(GRID_BinaryType )0;//bt_machineRaw; 



		tcps_Array<GRID_ProgramFile> files;
		files.Resize(moudleFiles.Length());
		for(int i=0;i<moudleFiles.Length();++i)
		{
			files[i].name = moudleFiles[i].name;
			files[i].isExecutable = FALSE;
			files[i].data = moudleFiles[i].data;//这样复制可行
		}
		if( TCPS_OK == m_gridConn.AddJobProgram(progInfo, files))//网格内部分配了一个id号
		{
			moduleKey = -1;//初始化
			tcps_Array<GRID_User_T::JobProgram> progInfos;
			if( TCPS_OK == m_gridConn.ListJobPrograms(progInfos))
				for(int i=0;i< progInfos.Length();++i)
				{
					if(progInfos[i].programInfo.programID.name == progInfo.programID.name &&
						progInfos[i].programInfo.programID.ver.majorVer == progInfo.programID.ver.majorVer &&
						progInfos[i].programInfo.programID.ver.minorVer == progInfo.programID.ver.minorVer )
					{
						moduleKey = progInfos[i].programKey;
						assert(moduleKey < BASE_ID);
						break;
					}
				}
				assert(moduleKey > -1);
				/*
				PCC_ModuleInfo minfo;
								minfo.moduleKey = moduleKey;
								minfo.moduleFileType = PCC_MODULE_MACHINE_RAW;
								minfo.moduleLatency = PCC_LATENCY_LARGE;
								minfo.modulePattern = PCC_TOOL_BACKGROUND;
								minfo.moduleTag = moduleProperty.moduleTag ;
								minfo.moduleType = PCC_MODULE_VIDSTRUCTURE;*/
				

				assert(modules.insert(std::make_pair(moduleKey,moduleProperty)));
				return TCPS_OK ;
		}
		else
		{
			return TCPS_ERROR;
		}
	}
	else//scattered 算法库
	{
		//没有严格的同步
		//验证模块没有重复添加

		moduleKey =  modules.generateKey();
		std::ostringstream module_name_t ;
		module_name_t<<pgrid_util::Singleton<BIN_DIR>::instance()<<"ScatterProgams\\"<<moduleKey<<"#"<<moduleProperty.moduleTag.name.Get()<<"-"<< moduleProperty.moduleTag.version.major<<"."<<moduleProperty.moduleTag.version.minor;
		if(!IsExistPath(module_name_t.str().c_str()))//后面最好加上文件比对，暂时只做文件夹是否存在匹配
		{
            //
			if(!CreatePath(module_name_t.str().c_str()))
			{
				NPLogError(("创建模块目录失败！\n"));
				return TCPS_ERROR;
			}
			//添加模块：拷贝文件
			for (int i = 0; i < moudleFiles.Length(); ++i)
			{
				ASSERT(!moudleFiles[i].name.IsEmpty());
				ASSERT(!moudleFiles[i].data.IsEmpty());
				std::ostringstream filePathStream;
				filePathStream << module_name_t.str()<< "/" << moudleFiles[i].name.Get();
				CFileStorage file;
				if (!file.Open(filePathStream.str().c_str(), CFileStorage::
					fs_read_write_ex))
				{
					NPLogError(("%p,%d\n",moudleFiles[i].data.Get(),moudleFiles[i].data.Length()));
					NPLogError(("Create file %s for job program %s-%d.%d failed",
						module_name_t.str().c_str(), moduleProperty.moduleTag.name.Get(),
						moduleProperty.moduleTag.version.major,
						moduleProperty.moduleTag.version.minor));
					NPRemoveFileOrDir(module_name_t.str().c_str());
					file.Close();
					return TCPS_ERR_FILE_OPEN;
				}

				if (-1 == file.Write(0, moudleFiles[i].data.Get(), moudleFiles[i].data.Length()))
				{
					NPLogError(("%p,%d\n",moudleFiles[i].data.Get(),moudleFiles[i].data.Length()));
					NPLogError(("Write file %s for job program %s-%d.%d failed",
						module_name_t.str().c_str(), moduleProperty.moduleTag.name.Get(),
						moduleProperty.moduleTag.version.major,
						moduleProperty.moduleTag.version.minor));
					//NPRemoveFileOrDir(jobProgramDir.c_str());
					NPRemoveFileOrDir(module_name_t.str().c_str());
					file.Close();
					return TCPS_ERR_FILE_WRITE;
				}
				file.Close();
			}
			
			
			ASSERT(modules.insert(std::make_pair(moduleKey,moduleProperty)));

		}
		return TCPS_OK ;
	}

}


TCPSError CPCCHandler::ListModules(
					  IN INT32 moduleType,
					  OUT tcps_Array<PCC_ModuleInfo>& modulesInfo
					  ) 
{
	
	
	return pgrid_util::Singleton<CScatteredManage>::instance().ListModules(moduleType,modulesInfo);
		
}

TCPSError CPCCHandler::ListModules(
					  OUT tcps_Array<PCC_ModulePropWithKey>& modulesInfo
					  ) 
{
	return pgrid_util::Singleton<CScatteredManage>::instance().ListModules(modulesInfo);
}

TCPSError CPCCHandler::RemoveModule(
					   IN INT64 moduleKey
					   ) 
{
	
	if(moduleKey>BASE_ID)//scattered的模块
	{//如果有别的session使用此模块，则会导致问题，需要同步 待解决 考虑使用引用记数
		PCC_ModuleProperty info;
		if(pgrid_util::Singleton<CScatteredManage>::instance().find(moduleKey,info))
		{
			std::ostringstream module_name_t ;
			module_name_t<<pgrid_util::Singleton<BIN_DIR>::instance()<<"ScatterProgams\\"<<moduleKey<<"#"<<info.moduleTag.name.Get()<<"-"<< info.moduleTag.version.major<<"."<<info.moduleTag.version.minor;
			if(IsExistPath(module_name_t.str().c_str()))//
			{
				NPRemoveFileOrDir(module_name_t.str().c_str());
			}
		}
	}
	else//grid的模块
	{
		tcps_Array<INT64> delProId;
		delProId.Resize(1);
		delProId[0] = moduleKey;
		m_gridConn.DelJobProgram(delProId);
	}
	pgrid_util::Singleton<CScatteredManage>::instance().RemoveModule(moduleKey);//暂时简化处理，
	return TCPS_OK;
}