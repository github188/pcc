#pragma once
#include "sqlite3.h"
#include "Ilocker.h"
#include"tcps_serve.h"
#include "NP_SCATTEREDTypesDefine.h"
//#include ""
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
	int RemoveModule(const tcps_String& trunk,INT64 moduleKey);
	int RemoveModuleFiles(const tcps_String& trunk,INT64 moduleKey,INT32 fileType);
	int ListModules(const tcps_String& trunk,tcps_Array<PCC_ModulePropWithKey>& modulesInfo);
private:
	static const std::string m_ModuelsSql;
	static const std::string m_TrunkSql;
	
private:
  sqlite3 *m_db;
  CLocker m_lock_db;
};
