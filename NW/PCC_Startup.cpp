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

	//����jc����
	char jobcenterdir[MAX_PATH];
	GetModuleFileName(NULL,jobcenterdir,MAX_PATH);
	_tcsrchr(jobcenterdir,'\\')[1] = 0;
	strcat(jobcenterdir,"/JobCenter.exe -port 9012");
	OSProcessID id = NPCreateProcess(jobcenterdir);
	if (INVALID_OSPROCESSID == id)
	{
		NPLogError(("�����������ʧ��"));
		return -1;
	}
	Sleep(500);//�ȴ�grid�������
	//��ȡjcģ��
	//ʹ�õ���ģʽ����ȫ�ֱ���
	/*
	MY_NP_GridUserClient& m_gridUser = pgrid_util::Singleton<MY_NP_GridUserClient>::instance();
		m_gridUser.m_user = "netposa";
		m_gridUser.m_pass = "netposa";
		
		IPP serverIPP;
		serverIPP.ip_ = inet_addr("127.0.0.1");//���ӱ���
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
			NPLogError(("�����������ʧ��"));
			return -1;
		}
		else
		{
			m_gridUser.SetKeeping(TRUE);//\
	       
			//��ȡ�������ĳ���ģ��
			//���ﲻ��Ҫ�������
			/ *
			tcps_Array<GRID_User_T::JobProgram> progInfos;
					TCPSError ret = m_gridUser.ListJobPrograms(progInfos);
			
					std::map<>;
					if (TCPS_OK == ret)
					{
			
					}
					else
					{
						NPLogError(("��ȡ����ģ��ʧ��"));
						return -1;
					}* /
			
		}*/
	
	

	//ȫ�ֱ������� ʹ�õ���ģʽ���� ��������Ǳ�־����
	//pgrid_util::Singleton<std::deque< > >::instance();//�û��ύ����ҵ�������FIFO��just for scattered
    //pgrid_util::Singleton<std::map<Node_Id,PCC_Scatter_S*> >::instance();//scattered �ڵ�����
	pgrid_util::Singleton<CScatteredManage>::instance().init();//
	//scatterd ģ��Ŀ¼

	return 0;
}
