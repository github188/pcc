#include "StdAfx.h"
#include "PCC_Startup.h"
#include "Singleton.h"
#include "smlib.h"
#include "my_np_griduserclient.h"
#include "nplog.h"
#include "modulemanage.h"
#include "npfdk.h"
class PCC_Scatter_S;
CPCC_Startup::CPCC_Startup(void)
{
}

CPCC_Startup::~CPCC_Startup(void)
{
}

int CPCC_Startup::Startup()
{

	//test

	//boost::shared_ptr<CModuleManage> pmm ;
	//pmm.reset(new CModuleManage );
	//pmm->RemoveModule(0);
	//test
//	pgrid_util::Singleton<CScatteredManage>::instance().mytest();
//	pgrid_util::Singleton<CScatteredManage>::instance().init();

	//启动jc服务
	char jobcenterdir[MAX_PATH];
	GetModuleFileName(NULL,jobcenterdir,MAX_PATH);
	_tcsrchr(jobcenterdir,'\\')[1] = 0;
	strcat(jobcenterdir,"/JobCenter.exe -port 9012");
	OSProcessID id = NPCreateProcess(jobcenterdir);
	if (INVALID_OSPROCESSID == id)
	{
		NPLogError(("启动网格服务失败"));
		return -1;
	}
	Sleep(500);//等待grid启动完成
	//获取jc模块
	//使用单件模式管理全局变量
	/*
	MY_NP_GridUserClient& m_gridUser = pgrid_util::Singleton<MY_NP_GridUserClient>::instance();
		m_gridUser.m_user = "netposa";
		m_gridUser.m_pass = "netposa";
		
		IPP serverIPP;
		serverIPP.ip_ = inet_addr("127.0.0.1");//连接本机
		serverIPP.port_ = 9011;
		
		TCPSError ret = m_gridUser.SetServeIPP(serverIPP);//
		int counter =0;
		while (TCPS_OK != ret && counter++ < 10)
		{
			ret = m_gridUser.SetServeIPP(serverIPP);//
			Sleep(100);
		}
		if (TCPS_OK != ret)
		{
			NPLogError(("连接网格服务失败"));
			return -1;
		}
		else
		{
			m_gridUser.SetKeeping(TRUE);//\
	       
			//获取网格服务的程序模块
			//这里不需要这个操作
			/ *
			tcps_Array<GRID_User_T::JobProgram> progInfos;
					TCPSError ret = m_gridUser.ListJobPrograms(progInfos);
			
					std::map<>;
					if (TCPS_OK == ret)
					{
			
					}
					else
					{
						NPLogError(("获取网格模块失败"));
						return -1;
					}* /
			
		}*/
	
	

	//全局变量定义 使用单件模式管理 这里更多是标志意义
	//pgrid_util::Singleton<std::deque< > >::instance();//用户提交的作业管理队列FIFO，just for scattered
    //pgrid_util::Singleton<std::map<Node_Id,PCC_Scatter_S*> >::instance();//scattered 节点容器
	pgrid_util::Singleton<CScatteredManage>::instance().init();//
	//scatterd 模块目录

	return 0;
}
