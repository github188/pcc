#include "StdAfx.h"
#include "TrunkManage.h"
#include "npfdk.h"
#include "nplog.h"
#include <sstream>
#include "filestorage.h"
#include "smlib.h"
#include "strtmpl.h"
const std::string CTrunkManage::m_TrunkSql("CREATE TABLE IF NOT EXISTS PCC_TRUNKS \
										   (id_trunk INTEGER PRIMARY KEY AUTOINCREMENT, trunk_name TEXT NOT NULL unique, trunk_path TEXT NOT NULL)"); 
//这里的module_name 包括了版本号,例如： name_1_0
const std::string CTrunkManage::m_ModuelsSql("CREATE TABLE IF NOT EXISTS PCC_TRUNK_MODULES \
											 (id_moudule INTEGER PRIMARY KEY AUTOINCREMENT,id_tk INTEGER , module_name TEXT NOT NULL unique,type INTEGER NOT NULL,\
											 moudule_path TEXT NOT NULL,foreign key(id_tk) references PCC_TRUNKS (id_trunk) on delete cascade)");
CTrunkManage::CTrunkManage(void):m_db(NULL)
{

	char buf[512];
	GetModuleFileName(NULL,buf,512);
	_tcsrchr(buf,'\\')[1] = 0;
	strcat(buf,"Trunks");

	if(!CreatePath(buf))//已存在，则也返回TRUE
	{
		NPLogError(("创建Trunks目录失败！\n"));
		exit(-1);
	};
	
	int rc;
	strcat(buf,"/trunks.db");
	rc = sqlite3_open(buf, &m_db);
	if( rc )
	{
		//fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		NPLogError(("打开trunks数据库失败:%s！\n",sqlite3_errmsg(m_db)));
		sqlite3_close(m_db);
		exit(-1);
	}
	
	//初始化数据库
	char *zErrMsg = 0;
	rc = sqlite3_exec(m_db, CTrunkManage::m_TrunkSql.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("创建表失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		exit(-1);
	}
	rc = sqlite3_exec(m_db, CTrunkManage::m_ModuelsSql.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("创建表失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		exit(-1);
	}
 
}

CTrunkManage::~CTrunkManage(void)
{
	if(m_db)
		sqlite3_close(m_db);
}
int CTrunkManage::CreateTrunk(const tcps_String& trunk) 
{
	CNPAutoLock lok(m_lock_db);

	char buf[512];
	GetModuleFileName(NULL,buf,512);
	_tcsrchr(buf,'\\')[1] = 0;
	strcat(buf,"Trunks\\");
	strcat(buf,trunk.Get());//trunk_dir为相对路径，这样可以保证portable
	if(!CreatePath(buf))//已存在，则也返回TRUE
	{
		NPLogError(("创建Trunks目录失败！\n"));
		return -1;
	};

	std::string sql ="insert into PCC_TRUNKS(trunk_name) values("+std::string(trunk.Get())+")";
	char *zErrMsg = 0;
	if( sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg)!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("创建表失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 0;
}
int CTrunkManage::RemoveTrunk(const tcps_String& trunk)
{
	CNPAutoLock lok(m_lock_db);
	std::string sql_del = "delete from PCC_TRUNKS where trunk_name="+std::string(trunk.Get());
	std::string sql_query = "select trunk_path from PCC_TRUNKS where trunk_name="+std::string(trunk.Get());//quey 是多余的 要优化
	char *zErrMsg = 0;
	//要删除 PCC_TRUNK_MODULES 表中对应数据 并移除相应目录（包括文件）
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.c_str(),sql_query.length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query)== SQLITE_ROW)
	{
		const unsigned char *result = sqlite3_column_text(st_query,0);
		/*if(result == 0)
			return "";*/
		std::stringstream trunk_dir;
		trunk_dir << result;

		char buf[512];
		GetModuleFileName(NULL,buf,512);
		_tcsrchr(buf,'\\')[1] = 0;
		strcat(buf,"Trunks\\");
		strcat(buf,trunk_dir.str().c_str());//trunk_dir为相对路径，这样可以保证portable
		if(!NPRemoveFileOrDir(buf))
		{
			NPLogError(("移除失败trunk:%s\n",buf));
			return -1;
		}
	}		
	
	//删除相应数据库记录 
	if( sqlite3_exec(m_db, sql_del.c_str(), 0, 0, &zErrMsg)!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		return -1;
	}
	
	return 0;
}
int CTrunkManage::ListTrunk(tcps_Array<tcps_String>& trunks )
{
	CNPAutoLock lok(m_lock_db);
	std::string sql_query = "select trunk_name from PCC_TRUNKS ";
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.c_str(),sql_query.length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	while(sqlite3_step(st_query)== SQLITE_ROW)
	{
		trunks.Resize(trunks.Length()+1);
		trunks[trunks.Length()].Assign((const char *)sqlite3_column_text(st_query,0));
	}
	sqlite3_finalize(st_query);  
	return 0;
}
int CTrunkManage::AddAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag,const tcps_Array<PCC_ModuleFile>& files)
{ 
	CNPAutoLock lok(m_lock_db);
	//先判断trunk是否存在
	std::string sql_query = "select id_trunk from PCC_TRUNKS where trunk_name="+std::string(trunk.Get());
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.c_str(),sql_query.length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query) != SQLITE_ROW)
	{
		NPLogError(("不存在trunk:%s！\n",trunk));  
		return -1;
		//trunks.Resize(trunks.Length()+1);
		//trunks[trunks.Length()].Assign((const char *)sqlite3_column_text(st_query,0));
	}
	//verify PCC_TRUNK_MODULES the record  has exised

	//
	std::stringstream mod_name ;
	mod_name<<authTag.name.Get()<<"_"<<authTag.version.major<<"_"<<authTag.version.minor;
	//_s 创建目录，保存模块文件
	char buf[512];
	GetModuleFileName(NULL,buf,512);
	_tcsrchr(buf,'\\')[1] = 0;
	strcat(buf,"Trunks\\");
	strcat(buf,mod_name.str().c_str());
	strcat(buf,"\\");//xxx\xxx\...\xx\  //
	if(!CreatePath(buf))//已存在，则也返回TRUE
	{
		NPLogError(("创建Trunks目录失败:%s\n",mod_name.str().c_str()));
		return -1;
	}
	for (int i=0;i<files.Length();++i)
	{
		CFileStorage fs;
		strcat(buf,files[i].name.Get());
		fs.Open(buf,CFileStorage::fs_create_always);
		fs.Write(0,files[i].data.Get(),files[i].data.Length());
		fs.Close();
		_tcsrchr(buf,'\\')[1] = 0;
	}
	//启动授权程序 需要另外的结构来映射模块及其授权程序 
    //暂时简单处理程序查找
	std::stringstream auth_exe;
	auth_exe<<authTag.name.Get()<<".exe -port"<< 3000;//todo潜在的端口冲突解决策略 考虑srand
	strcat(buf,auth_exe.str().c_str());
	OSProcessID pid = NPCreateProcess(buf);
	if (INVALID_OSPROCESSID == pid)
	{
		NPLogError(("启动授权服务失败:%s\n",buf));
		return -1;
	}
	//_e
	int id = sqlite3_column_int(st_query,0);
	sqlite3_finalize(st_query);  
	
	std::stringstream sql_insert ;
	//type = 1 授权模块 0 算法模块

	sql_insert<<"insert into  PCC_TRUNK_MODULES(id_tk,module_name,type,module_path)\
		values("<<id<<","<<mod_name.str()<<1<<mod_name.str()<<")";
	char *zErrMsg = 0;
	if( sqlite3_exec(m_db, sql_insert.str().c_str(), 0, 0, &zErrMsg)!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 0;

}
int CTrunkManage::RemoveAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag)
{
	CNPAutoLock lok(m_lock_db);
	//先判断trunk是否存在
	std::string sql_query = "select id_trunk from PCC_TRUNKS where trunk_name="+std::string(trunk.Get());
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.c_str(),sql_query.length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query) != SQLITE_ROW)
	{
		NPLogError(("不存在trunk:%s！\n",trunk));  
		return -1;
		//trunks.Resize(trunks.Length()+1);
		//trunks[trunks.Length()].Assign((const char *)sqlite3_column_text(st_query,0));
	}
	int id = sqlite3_column_int(st_query,0);
	sqlite3_finalize(st_query);  

	std::stringstream sql_del ;
	sql_del<<"delete from PCC_TRUNK_MODULES where type=1 and id_tk="<<id<<" and module_name='"
		<<authTag.name.Get()<<"_"<<authTag.version.major<<"_"<<authTag.version.minor<<"'";
	char *zErrMsg = 0;
	if( sqlite3_exec(m_db, sql_del.str().c_str(), 0, 0, &zErrMsg)!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		return -1;
	}

	char buf[512];
	GetModuleFileName(NULL,buf,512);
	_tcsrchr(buf,'\\')[1] = 0;
	strcat(buf,"Trunks\\");
	std::stringstream trunk_dir;
	trunk_dir<<authTag.name.Get()<<"_"<<authTag.version.major<<"_"<<authTag.version.minor;
	strcat(buf,trunk_dir.str().c_str());//trunk_dir为相对路径，这样可以保证portable
	if(!NPRemoveFileOrDir(buf))
	{
		NPLogError(("移除失败trunk:%s\n",buf));
		return -1;
	}
	//NPRemoveFileOrDir();
	return 0;

}
//此函数和 FindAuthCenter的差别 ?应当忽略 const tcps_String& trunk 
int CTrunkManage::ListAuthCenter(const tcps_String& trunk,tcps_Array<PCC_ModuleTag>& authTags)
{
	CNPAutoLock lok(m_lock_db);
	std::stringstream sql_query;
	sql_query<<"select module_name  from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=1 ";
		//<<trunk.Get()<<"'";

	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.str().c_str(),-1,&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	while(sqlite3_step(st_query) == SQLITE_ROW)
	{
		const unsigned char *result = sqlite3_column_text(st_query,0);
		std::stringstream mod;
		mod << result;
		CSmartArray<tstring> modNamVer;
		StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
		ASSERT(modNamVer.size()==3);
		authTags.Resize(authTags.Length()+1);

		authTags[authTags.Length()].name = modNamVer[0].c_str();
		authTags[authTags.Length()].version .major = atoi(modNamVer[1].c_str());
		authTags[authTags.Length()].version .minor = atoi(modNamVer[2].c_str());
	}
	sqlite3_finalize(st_query);  
	
	return 0;

}
//？？？？？？
int CTrunkManage::FindAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag)
{
	CNPAutoLock lok(m_lock_db);
	std::stringstream sql_query;
	sql_query<<"select module_name  from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=1 and trunk_name = '"
		<<trunk.Get()<<"'";

	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.str().c_str(),-1,&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query) != SQLITE_ROW)
	{
		sqlite3_finalize(st_query);  
		NPLogError(("不存在！\n"));  
		return -1;
	}
	const unsigned char *result = sqlite3_column_text(st_query,0);
	std::stringstream mod;
	mod << result;
	CSmartArray<tstring> modNamVer;
	StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
	ASSERT(modNamVer.size()==3);

	//authTag.name = modNamVer[0].c_str();
	//authTag.version .major = atoi(modNamVer[1].c_str());
	//authTag.version .minor = atoi(modNamVer[2].c_str());

	sqlite3_finalize(st_query);  
	return 0;
}
int CTrunkManage::AddModule(const tcps_String& trunk,const PCC_ModuleProperty& moduleProperty
			  ,const tcps_Array<PCC_ModuleFile>& moudleFiles,INT64& moduleKey)
{
	//CNPAutoLock lok(m_lock_db);

	return 0;
}
int CTrunkManage::AddModuleFile(const tcps_String& trunk,INT64 moduleKey
				  ,PCC_ModuleFileType fileType,const tcps_Array<PCC_ModuleFile>& moduleFiles)
{
	CNPAutoLock lok(m_lock_db);

	return 0;
}
int CTrunkManage::RemoveModule(const tcps_String& trunk,INT64 moduleKey)
{
	CNPAutoLock lok(m_lock_db);

	return 0;
}
int CTrunkManage::RemoveModuleFiles(const tcps_String& trunk,INT64 moduleKey,INT32 fileType)
{
	CNPAutoLock lok(m_lock_db);

	return 0;
}
int CTrunkManage::ListModules(const tcps_String& trunk,tcps_Array<PCC_ModulePropWithKey>& modulesInfo)
{
	CNPAutoLock lok(m_lock_db);

	return 0;
}