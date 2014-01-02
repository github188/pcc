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
		NPLogError(("�����߳�ʧ��"));
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
			NPLogError(("����SCATTEREDģ��Ŀ¼ʧ��\n"));
			return;
		}
	}
	//��������Ŀ¼ ��ȡģ����Ϣ
	//ģ����+�汾��
	RegisterModules(startup_dir);

	//debug �������ע���ģ��
	for(MNG_IT it= m_modules.begin();it!=m_modules.end();++it)
	{
		printf("%lld,%s\n",it->first,it->second.moduleTag.name.Get());
	}

	//debug
}
BOOL CScatteredManage::GridModules()
{
	//��ʼ����¼�����ģ��
	MY_NP_GridUserClient inner_user;
	inner_user.m_user = "netposa";
	inner_user.m_pass = "netposa";
	IPP serverIPP;
	serverIPP.ip_ = inet_addr("127.0.0.1");//���ӱ���
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
			modulesInfo.moduleType = PCC_MODULE_IMGPROC;//�������ģ���ļ��е���������������
			modulesInfo.modulePattern =PCC_MODULE_BACKGROUND;
			insert(std::make_pair(progInfos[i].programKey,modulesInfo));
		}
		return TRUE;
	}
	else
		return FALSE;
	//
}
BOOL CScatteredManage::RegisterModules (const char *moduleDir)//��������ĵ��ñ�Ȼ�ǵ��̵߳ģ������о���������ͬ��
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
				//minfo.moduleKey = ++ pmng->m_lastkey;//scattered��ģ��id�Ǵ�BASE_ID��ʼ�ģ���Эͬgrid��ģ��id����
				minfo.moduleTag .name = mod_name.c_str() ;
				minfo.moduleTag.version .major = vmax;
				minfo.moduleTag.version .minor = vmin;

				minfo.moduleFileType = PCC_MODULE_MACHINE_RAW;
				minfo.moduleLatency = PCC_LATENCY_LARGE;
				minfo.modulePattern =PCC_MODULE_BACKGROUND;
				
				minfo.moduleType = PCC_MODULE_IMGPROC;//�������ģ���ļ��е���������������
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
BOOL CScatteredManage::CopyModules (const char *moduleDir,tcps_Array<PCC_ModuleFile> &moudleFiles)//��������ĵ��ñ�Ȼ�ǵ��̵߳ģ������о���������ͬ��
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
					NPLogError(("���ļ�%sʧ��\n",filePathname));
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
					NPLogError(("��ȡ�ļ�%sʧ��\n",filePathname));
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

		//��ȡ�ļ�
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
		if(it != m_job_service.end())//����Ϊ��
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
		if(it != m_job_service.end())//����Ϊ��
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
			printf("##��ǰ�ɵ��Ƚڵ�����%d,��������ҵ����%d\n",pre_node_num = getNodeCounts(),getJobsCounts());
			
		}
		
		//���job����
		if(popJob(sj))
		{
			
			printf("��ǰ�ɵ��Ƚڵ�����%d,��������ҵ����%d\n",getNodeCounts(),getJobsCounts()+1);

			//���ж�jobkey��Ӧ�ĻỰ�Ƿ���Ч //��һ���������Ƿ�֧�ֲ�Ҫ�ص�֪ͨ����ҵ�ύ�������־�Իص���Ч�ķ�ʽ����
            if(!isValidJobRequest(sj.jobKey))
				continue;//����

			while(1)//��ѯ�ȴ��п���ڵ����scattered����
			{
				
				if(popNode(nd))// ע��ڵ�Ҫ����ҵ��������ջ�
				{
				    
					//nd.ps->m_ss = sj.ss;//�ص�senssion��� Ҫ��m_ss��servece_s�Ŀͻ��˶Ͽ������ӣ����ָ���ʧЧ//jobkey
					
                    
					nd.ps->m_jobkey = sj.jobKey;
					printf("�ڵ�%p������ҵ��%lld\n",nd.ps,sj.jobKey);
					index.moduleKey = sj.moduleKey;
					//ȷ���ڵ�˴��� moduleKeyָ����ģ��
					moudleFiles.Release();
				    BOOL isFound = false;
					if(TCPS_OK ==nd.ps->FindModule(sj.moduleKey,isFound) )// �ڵ��û��,ͬ����ģ��
					{
						if(!isFound)
						{
							//��ȡģ�� ��� index,moudleFiles
							if(getModules(index,moudleFiles))
							{
								//�ٴε��ýӿ�
								if(nd.ps->AddModule(sj.moduleKey,moudleFiles) != TCPS_OK)
								{
									nd.ps->m_ss = NULL;//for mark
									pushNode(nd.key,nd.ps);
									NPLogError(("ģ�����ʧ�ܣ���ҵ�޷�����\n"));
									break;
								}
							}
							else
							{
								nd.ps->m_ss = NULL;//for mark
								pushNode(nd.key,nd.ps);
								NPLogError(("ģ���ȡʧ�ܣ���ҵ�޷�����\n"));
								break;
							}
						}
					}
					else
					{
						pushNode(nd.key,nd.ps);
						NPLogError(("�޷���ȡģ��״̬��\n"));
						break;
					}
					//�ڵ���ʱ�����ȥ��
					//����Ӧ����ӽڵ㴦��ʱ
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
	
	serverIPP.ip_ = GetLocalIP();//inet_addr("127.0.0.1");//���ӱ���
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
	//CModuleManage::MNG_IT it = modules.find(moduleKey);//��������ʵ�֣�it����ʧЧ
	if(modules.find(moduleKey,info)/*it!=modules.end()*/)
	{
		jobinfo.dataInputUrl = inputUrl;
		jobinfo.dataOutputUrl = outputUrl;
		jobinfo.programParam = moduleParams;//Ӧ��֧�����ݿ��� ������������

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
			pgrid_util::Singleton<CScatteredManage>::instance().pushSS(jobKey,m_psenssion);//m_psenssion�ڴ˿̶���Ч
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

	if(moduleProperty.moduleType == PCC_MODULE_VIDSTRUCTURE)//������㷨��
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
			files[i].data = moudleFiles[i].data;//�������ƿ���
		}
		if( TCPS_OK == m_gridConn.AddJobProgram(progInfo, files))//�����ڲ�������һ��id��
		{
			moduleKey = -1;//��ʼ��
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
	else//scattered �㷨��
	{
		//û���ϸ��ͬ��
		//��֤ģ��û���ظ����

		moduleKey =  modules.generateKey();
		std::ostringstream module_name_t ;
		module_name_t<<pgrid_util::Singleton<BIN_DIR>::instance()<<"ScatterProgams\\"<<moduleKey<<"#"<<moduleProperty.moduleTag.name.Get()<<"-"<< moduleProperty.moduleTag.version.major<<"."<<moduleProperty.moduleTag.version.minor;
		if(!IsExistPath(module_name_t.str().c_str()))//������ü����ļ��ȶԣ���ʱֻ���ļ����Ƿ����ƥ��
		{
            //
			if(!CreatePath(module_name_t.str().c_str()))
			{
				NPLogError(("����ģ��Ŀ¼ʧ�ܣ�\n"));
				return TCPS_ERROR;
			}
			//���ģ�飺�����ļ�
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
	
	if(moduleKey>BASE_ID)//scattered��ģ��
	{//����б��sessionʹ�ô�ģ�飬��ᵼ�����⣬��Ҫͬ�� ����� ����ʹ�����ü���
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
	else//grid��ģ��
	{
		tcps_Array<INT64> delProId;
		delProId.Resize(1);
		delProId[0] = moduleKey;
		m_gridConn.DelJobProgram(delProId);
	}
	pgrid_util::Singleton<CScatteredManage>::instance().RemoveModule(moduleKey);//��ʱ�򻯴���
	return TCPS_OK;
}