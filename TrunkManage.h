#pragma once
#include "sqlite3.h"
#include "Ilocker.h"
#include"tcps_serve.h"
#include "NP_SCATTEREDTypesDefine.h"
#include "MY_NP_GridUserClient.h"
#include "map"
//#include ""
class CTrunkManage;
class CAuthManage//授权程序管理
{
public:
	struct nwProcessInfo
	{
		int port;
		int pid;
	};
public:
	CAuthManage(CTrunkManage& trunk_mng);
	~CAuthManage();

public:
	int init();
	int/*port*/ getPort(std::string key);//查找trunk对应的授权服务
	int closeAuths();
	void addAuth(std::string key,nwProcessInfo info);
	void removeAuth(std::string key);
    
	int startAuth(std::string trunk,std::string filepath);
	int generatePort()
	{
		static int port =9100;
		return ++port;
	}

private:
	std::map<std::string,nwProcessInfo> m_trunkPort;
	CLocker m_lock_auth;
	CTrunkManage& m_trunk_mng;
};
class CTrunkManage
{
public:
	CTrunkManage(void);
	~CTrunkManage(void);

	int CreateTrunk(const tcps_String& trunk); 
	int RemoveTrunk(const tcps_String& trunk); 
    int ListTrunk(tcps_Array<tcps_String>& trunks ); 
	int AddAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag,const tcps_Array<PCC_ModuleFile>& files);
	int RemoveAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag);
	int ListAuthCenter(const tcps_String& trunk,tcps_Array<PCC_ModuleTag>& authTags); 
	int FindAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag);
	int AddModule(const tcps_String& trunk,const PCC_ModuleProperty& moduleProperty,const tcps_Array<PCC_ModuleFile>& moudleFiles,INT64& moduleKey);
	int AddModuleFile(const tcps_String& trunk,INT64 moduleKey,PCC_ModuleFileType fileType,const tcps_Array<PCC_ModuleFile>& moduleFiles);
	int RemoveModule(const tcps_String& trunk,INT64 moduleKey,MY_NP_GridUserClient &m_gridConn);
	int RemoveModuleFiles(const tcps_String& trunk,INT64 moduleKey,INT32 fileType);
	int ListModules(const tcps_String& trunk,tcps_Array<PCC_ModulePropWithKey>& modulesInfo);

	int getAuthPath(tcps_Array<tcps_String>&auth_paths);
	int getAuthPath(tcps_String&auth_path,const tcps_String& trunk);
private:
	static const std::string m_ModuelsSql;
	static const std::string m_TrunkSql;
	
private:
	CAuthManage m_auth_mng;
	sqlite3 *m_db;
	CLocker m_lock_db;
};
class CNPDeploy
{
public:
	CNPDeploy();
	~CNPDeploy();
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

public:
	TCPSError CreateTrunk(const tcps_String& trunk); 
	TCPSError RemoveTrunk(const tcps_String& trunk); 
    TCPSError ListTrunk(tcps_Array<tcps_String>& trunks ); 
	TCPSError AddAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag,const tcps_Array<PCC_ModuleFile>& files);
	TCPSError RemoveAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag);
	TCPSError ListAuthCenter(const tcps_String& trunk,tcps_Array<PCC_ModuleTag>& authTags); 
	TCPSError FindAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag);
	TCPSError AddModule(const tcps_String& trunk,const PCC_ModuleProperty& moduleProperty,const tcps_Array<PCC_ModuleFile>& moudleFiles,INT64& moduleKey);
	TCPSError AddModuleFile(const tcps_String& trunk,INT64 moduleKey,PCC_ModuleFileType fileType,const tcps_Array<PCC_ModuleFile>& moduleFiles);
	TCPSError RemoveModule(const tcps_String& trunk,INT64 moduleKey);
	TCPSError RemoveModuleFiles(const tcps_String& trunk,INT64 moduleKey,INT32 fileType);
	TCPSError ListModules(const tcps_String& trunk,tcps_Array<PCC_ModulePropWithKey>& modulesInfo);

private:
	MY_NP_GridUserClient m_gridConn;
	IPP m_client_ipp;
};