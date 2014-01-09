#include "StdAfx.h"
#include "TrunkManage.h"
#include "npfdk.h"
#include "nplog.h"
#include <sstream>
#include "filestorage.h"
#include "smlib.h"
#include "strtmpl.h"
#include "singleton.h"
#include "ipcvt.h"
#include "modulemanage.h"
const std::string CTrunkManage::m_TrunkSql("CREATE TABLE IF NOT EXISTS PCC_TRUNKS \
										   (id_trunk INTEGER PRIMARY KEY AUTOINCREMENT, trunk_name TEXT NOT NULL unique, trunk_path TEXT NOT NULL)"); 
//这里的module_name 包括了版本号,例如： name_1_0,type :0 授权模块  其他参照 PCC_ModuleType
const std::string CTrunkManage::m_ModuelsSql("CREATE TABLE IF NOT EXISTS PCC_TRUNK_MODULES \
											 (id_module INTEGER PRIMARY KEY AUTOINCREMENT,id_tk INTEGER , module_name TEXT NOT NULL unique,type INTEGER NOT NULL,\
											 module_path TEXT NOT NULL,foreign key(id_tk) references PCC_TRUNKS (id_trunk) on delete cascade)");


CAuthManage::CAuthManage(CTrunkManage& trunk_mng):
	m_trunk_mng(trunk_mng)
{
}
CAuthManage::~CAuthManage()
{
	closeAuths();
}

int CAuthManage::init()
{
	//启动时，加载所有的授权程序

	return 0;
}
int CAuthManage::getPort(std::string key)//查找trunk对应的授权服务
{
	CNPAutoLock lok(m_lock_auth);
	std::map<std::string,nwProcessInfo>::const_iterator it;
	it = m_trunkPort.find(key);
	if(it!=m_trunkPort.end())
		return it->second.port;
	else//尝试启动授权程序
	{
		
		//select * from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk 
		tcps_String path;
		if(m_trunk_mng.getAuthPath(path,key.c_str())==0)
		{
			std::stringstream auth_exe_port;
			OSProcessID pid = INVALID_OSPROCESSID;
			int cnt = 10;
			nwProcessInfo info;
			info.pid= info.port = -1;
			while(INVALID_OSPROCESSID == pid&&(--cnt)){
				auth_exe_port.str("");
				auth_exe_port.clear();
				info.port =  generatePort();
				auth_exe_port<<path.Get()<<" -port"<<info.port;//todo潜在的端口冲突解决策略 考虑srand
				info.pid = pid = NPCreateProcess(auth_exe_port.str().c_str());
				if (INVALID_OSPROCESSID == pid)
				{
					NPLogError(("启动授权服务失败,尝试重启(%d):%s\n",cnt,path.Get()));

				}
			}
			if (INVALID_OSPROCESSID == pid&&(--cnt))
			{
				NPLogError(("启动授权服务失败:%s\n",path.Get()));
				return -1;
			}
			addAuth(key,info);
			return 0;
		}
	}
	return -1;
}
int CAuthManage::closeAuths()
{
	CNPAutoLock lok(m_lock_auth);
	std::map<std::string,nwProcessInfo>::const_iterator it;
	for (it = m_trunkPort.begin();it!=m_trunkPort.end();++it)
	{
		NPKillProcess(it->second.pid);
	}
	m_trunkPort.clear();
	return 0;
}
void CAuthManage::addAuth(std::string key,nwProcessInfo info)
{
	CNPAutoLock lok(m_lock_auth);
	m_trunkPort.insert(std::make_pair(key,info));
}
void CAuthManage::removeAuth(std::string key)
{
	CNPAutoLock lok(m_lock_auth);
	std::map<std::string,nwProcessInfo>::const_iterator it;
	it = m_trunkPort.find(key);
	if(it!=m_trunkPort.end())
	{
		NPKillProcess(it->second.pid);
		m_trunkPort.erase(it);
	}
	
}

int CAuthManage::startAuth(std::string trunk,std::string filepath)
{
	CNPAutoLock lok(m_lock_auth);
	std::stringstream auth_exe_port;
	OSProcessID pid = INVALID_OSPROCESSID;
	int cnt = 10;
	nwProcessInfo info;
	info.pid= info.port = -1;
	
	
	while(INVALID_OSPROCESSID == pid&&(--cnt)){
		auth_exe_port.str("");
		auth_exe_port.clear();
		info.port=generatePort();
		auth_exe_port<<filepath<<" -port"<< info.port;//todo潜在的端口冲突解决策略 考虑srand
		info.pid = pid = NPCreateProcess(auth_exe_port.str().c_str());
		if (INVALID_OSPROCESSID == pid)
		{
			NPLogError(("启动授权服务失败,尝试重启(%d):%s\n",cnt,filepath));
			
		}
	}
	if (INVALID_OSPROCESSID == pid&&(--cnt))
	{
		NPLogError(("启动授权服务失败:%s\n",filepath));
		return -1;
	}
	
	addAuth(trunk,info);
	return 0;
}

///////////////////CTrunkManage/////////////////////////
CTrunkManage::CTrunkManage(void):m_db(NULL),m_auth_mng(*this)
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
int CTrunkManage::getAuthPath(tcps_Array<tcps_String>&auth_paths)
{
	std::stringstream sql_query;
	sql_query<<"select module_name, trunk_name from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=0" ;
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.str().c_str(),-1,&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	char buf[512];
	GetModuleFileName(NULL,buf,512);
	
	strcat(buf,"Trunks\\");
	//strcat(buf,trunk.Get());
	while(sqlite3_step(st_query)== SQLITE_ROW)
	{
		strcat(buf,(const char *)sqlite3_column_text(st_query,1));
		auth_paths.Resize(auth_paths.Length()+1);
		//auth_paths[auth_paths.Length()-1].Assign((const char *)sqlite3_column_text(st_query,0));
		const unsigned char *result = sqlite3_column_text(st_query,0);
		std::stringstream mod;
		mod << result;
		CSmartArray<tstring> modNamVer;
		StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
		strcat(buf,"\\");
		strcat(buf,modNamVer[0].c_str());
		strcat(buf,".exe");
		auth_paths[auth_paths.Length()-1].Assign(buf);
		_tcsrchr(buf,'\\')[1] = 0;
		_tcsrchr(buf,'\\')[1] = 0;
	}
	return 0;
}
int CTrunkManage::getAuthPath(tcps_String&auth_path,const tcps_String& trunk)
{
	std::stringstream sql_query;
	sql_query<<"select module_name from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=0 and trunk_name='"
		<<trunk.Get()<<"'";
	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.str().c_str(),-1,&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	char buf[512];
	GetModuleFileName(NULL,buf,512);
	
	strcat(buf,"Trunks\\");
	//strcat(buf,trunk.Get());
	if(sqlite3_step(st_query)== SQLITE_ROW)
	{
		strcat(buf,(const char *)sqlite3_column_text(st_query,1));
		//auth_paths.Resize(auth_paths.Length()+1);
		//auth_paths[auth_paths.Length()-1].Assign((const char *)sqlite3_column_text(st_query,0));
		const unsigned char *result = sqlite3_column_text(st_query,0);
		std::stringstream mod;
		mod << result;
		CSmartArray<tstring> modNamVer;
		StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
		strcat(buf,"\\");
		strcat(buf,modNamVer[0].c_str());
		strcat(buf,".exe");
		auth_path.Assign(buf);
		_tcsrchr(buf,'\\')[1] = 0;
		_tcsrchr(buf,'\\')[1] = 0;
		return 0;
	}
	return -1;
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

	std::string sql ="insert into PCC_TRUNKS(trunk_name,trunk_path) values('"+std::string(trunk.Get())+"','"+std::string(trunk.Get())+"')";
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
	std::string sql_del = "delete from PCC_TRUNKS where trunk_name='"+std::string(trunk.Get())+"'";
	std::string sql_query = "select trunk_path from PCC_TRUNKS where trunk_name='"+std::string(trunk.Get())+"'";//quey 是多余的 要优化
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
		trunks[trunks.Length()-1].Assign((const char *)sqlite3_column_text(st_query,0));
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
	strcat(buf,trunk.Get());//
	strcat(buf,"\\");//
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
	//auth_exe<<authTag.name.Get()<<".exe -port"<< m_auth_mng.generatePort();//3000;//todo潜在的端口冲突解决策略 考虑srand
	auth_exe<<authTag.name.Get()<<".exe ";//-port"<< m_auth_mng.generatePort();//3000;//todo潜在的端口冲突解决策略 考虑srand
	strcat(buf,auth_exe.str().c_str());
	if(m_auth_mng.startAuth(trunk.Get(),auth_exe.str())<0)
		return -1;
	//_e
	int id = sqlite3_column_int(st_query,0);
	sqlite3_finalize(st_query);  
	
	std::stringstream sql_insert ;
	//type = 0 授权模块 oth 算法模块

	sql_insert<<"insert into  PCC_TRUNK_MODULES(id_tk,module_name,type,module_path)\
		values("<<id<<","<<mod_name.str()<<0<<mod_name.str()<<")";
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
	sql_del<<"delete from PCC_TRUNK_MODULES where type=0 and id_tk="<<id<<" and module_name='"
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
	trunk_dir<<"\\"<<trunk.Get()<<"\\"<<authTag.name.Get()<<"_"<<authTag.version.major<<"_"<<authTag.version.minor;
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
	sql_query<<"select module_name  from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=0 ";
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

		authTags[authTags.Length()-1].name = modNamVer[0].c_str();
		authTags[authTags.Length()-1].version .major = atoi(modNamVer[1].c_str());
		authTags[authTags.Length()-1].version .minor = atoi(modNamVer[2].c_str());
	}
	sqlite3_finalize(st_query);  
	
	return 0;

}
//？？？？？？
int CTrunkManage::FindAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag)
{
	CNPAutoLock lok(m_lock_db);
	std::stringstream sql_query;
	sql_query<<"select module_name  from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where type=0 and trunk_name = '"
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
//只负责pcc模块的添加
int CTrunkManage::AddModule(const tcps_String& trunk,const PCC_ModuleProperty& moduleProperty
			  ,const tcps_Array<PCC_ModuleFile>& moudleFiles,INT64& moduleKey)
{
	//ASSERT(moduleProperty.moduleType);
	int ty =moduleProperty.moduleType;

	CNPAutoLock lok(m_lock_db);//这段代码可以分段加锁来优化，暂时简化处理
	//先判断trunk是否存在
	std::string sql_query = "select id_trunk from PCC_TRUNKS where trunk_name='"+std::string(trunk.Get())+"'";
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
	mod_name<<moduleProperty.moduleTag.name.Get()<<"_"<<moduleProperty.moduleTag.version.major<<"_"<<moduleProperty.moduleTag.version.minor;
	if(moduleProperty.moduleType != PCC_MODULE_VIDSTRUCTURE)//网格的算法库
	{
		
		//_s 创建目录，保存模块文件
		char buf[512];
		GetModuleFileName(NULL,buf,512);
		_tcsrchr(buf,'\\')[1] = 0;
		strcat(buf,"Trunks\\");
		strcat(buf,trunk.Get());//
		strcat(buf,"\\");//
		strcat(buf,mod_name.str().c_str());
		strcat(buf,"\\");//xxx\xxx\...\xx\  //
		if(!CreatePath(buf))//已存在，则也返回TRUE
		{
			NPLogError(("创建Trunks目录失败:%s\n",mod_name.str().c_str()));
			return -1;
		}
		for (int i=0;i<moudleFiles.Length();++i)
		{
			CFileStorage fs;
			strcat(buf,moudleFiles[i].name.Get());
			fs.Open(buf,CFileStorage::fs_create_always);
			fs.Write(0,moudleFiles[i].data.Get(),moudleFiles[i].data.Length());
			fs.Close();
			_tcsrchr(buf,'\\')[1] = 0;
		}
		//_e
	}
	int id = sqlite3_column_int(st_query,0);
	sqlite3_finalize(st_query);  
	
	std::stringstream sql_insert ;
	//type = 1 授权模块 0 算法模块
    sql_query ="select max(seq) from sqlite_sequence where name = 'PCC_TRUNK_MODULES'";
	//sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.c_str(),sql_query.length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query) != SQLITE_ROW)
	{
		NPLogError(("生成模块key失败！\n"));  
		return -1;
		//trunks.Resize(trunks.Length()+1);
		//trunks[trunks.Length()].Assign((const char *)sqlite3_column_text(st_query,0));
	}

	moduleKey = sqlite3_column_int(st_query,0)+1;
	sqlite3_finalize(st_query);  
	

	sql_insert<<"insert into  PCC_TRUNK_MODULES(id_tk,module_name,type,module_path)\
		values("<<id<<",'"<<mod_name.str()<<"',"<<ty<<",'"<<mod_name.str()<<"')";
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
int CTrunkManage::AddModuleFile(const tcps_String& trunk,INT64 moduleKey
				  ,PCC_ModuleFileType fileType,const tcps_Array<PCC_ModuleFile>& moduleFiles)
{
	CNPAutoLock lok(m_lock_db);

	return 0;
}
int CTrunkManage::RemoveModule(const tcps_String& trunk,INT64 moduleKey,MY_NP_GridUserClient &m_gridConn)
{
	CNPAutoLock lok(m_lock_db);
	//先判断trunk是否存在
	std::string sql_query = "select id_trunk from PCC_TRUNKS where trunk_name='"+std::string(trunk.Get())+"'";
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
	int trunk_id = sqlite3_column_int(st_query,0);
	sqlite3_finalize(st_query);  

	std::stringstream sql_if;
	sql_if<<" select module_name ,type from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where id_module="
		<<moduleKey;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_if.str().c_str(),sql_if.str().length(),&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	
	if(sqlite3_step(st_query) != SQLITE_ROW)
	{
		NPLogError(("获取模块信息失败！\n"));  
		return -1;
		//trunks.Resize(trunks.Length()+1);
		//trunks[trunks.Length()].Assign((const char *)sqlite3_column_text(st_query,0));
	}

    const unsigned char *result = sqlite3_column_text(st_query,0);//module_name
	std::stringstream mod;
	mod << result;
	int ty = sqlite3_column_int(st_query,1);	
	sqlite3_finalize(st_query);  

	std::stringstream sql_del ;
	sql_del<<"delete from PCC_TRUNK_MODULES where id_tk="<<trunk_id<<" and id_module="
		<<moduleKey;
	char *zErrMsg = 0;
	if( sqlite3_exec(m_db, sql_del.str().c_str(), 0, 0, &zErrMsg)!=SQLITE_OK )
	{
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		NPLogError(("失败:%s！\n",zErrMsg));
		sqlite3_free(zErrMsg);
		return -1;
	}
	if((ty&4)==0)//pcc端模块
	{
		char buf[512];
		GetModuleFileName(NULL,buf,512);
		_tcsrchr(buf,'\\')[1] = 0;
		strcat(buf,"Trunks\\");
		std::stringstream trunk_dir;
		trunk_dir<<"\\"<<trunk.Get()<<"\\"<<mod.str();//authTag.name.Get()<<"_"<<authTag.version.major<<"_"<<authTag.version.minor;
		strcat(buf,trunk_dir.str().c_str());//trunk_dir为相对路径，这样可以保证portable
		if(!NPRemoveFileOrDir(buf))
		{
			NPLogError(("移除失败trunk:%s\n",buf));
			return -1;
		}
	}
	else
	{
		
			moduleKey = -1;//初始化
			tcps_Array<GRID_User_T::JobProgram> progInfos;
			if( TCPS_OK == m_gridConn.ListJobPrograms(progInfos))
			{
				CSmartArray<tstring> modNamVer;
				StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
				ASSERT(modNamVer.size()==3);
				for(int i=0;i< progInfos.Length();++i)
				{
					if(progInfos[i].programInfo.programID.name == modNamVer[0].c_str() &&
						progInfos[i].programInfo.programID.ver.majorVer == atoi(modNamVer[1].c_str()) &&
						progInfos[i].programInfo.programID.ver.minorVer == atoi(modNamVer[2].c_str()) )
					{
						moduleKey = progInfos[i].programKey;
						assert(moduleKey < BASE_ID);
						break;
					}
				}
				assert(moduleKey > -1);
				tcps_Array<INT64> delProId;
				delProId.Resize(1);
				delProId[0] = moduleKey;
				return (int)m_gridConn.DelJobProgram(delProId);
			}
			return -1;
		
	}
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
	std::stringstream sql_query;
	sql_query<<"select id_module,module_name,type from PCC_TRUNK_MODULES as a left join PCC_TRUNKS as b on a.id_tk=b.id_trunk where trunk_name='"
		<<trunk.Get()<<"'";

	sqlite3_stmt* st_query;
	if (SQLITE_OK !=sqlite3_prepare_v2(m_db,sql_query.str().c_str(),-1,&st_query,NULL))
	{
		sqlite3_finalize(st_query);  
		NPLogError(("失败！\n"));
		return -1;
	}
	while(sqlite3_step(st_query) == SQLITE_ROW)
	{
		int key = sqlite3_column_int(st_query,0);
		const unsigned char *result = sqlite3_column_text(st_query,1);
		int ty = sqlite3_column_int(st_query,2);
		std::stringstream mod;
		mod << result;

		CSmartArray<tstring> modNamVer;
		StrSeparater_Sep(mod.str().c_str(),"_",modNamVer);
		ASSERT(modNamVer.size()==3);
		modulesInfo.Resize(modulesInfo.Length()+1);
		modulesInfo[modulesInfo.Length()-1].key =key;
		modulesInfo[modulesInfo.Length()-1].prop.moduleTag.name = modNamVer[0].c_str();
		modulesInfo[modulesInfo.Length()-1].prop.moduleTag.version .major = atoi(modNamVer[1].c_str());
		modulesInfo[modulesInfo.Length()-1].prop.moduleTag.version .minor = atoi(modNamVer[2].c_str());
		modulesInfo[modulesInfo.Length()-1].prop.moduleType = ty;
	}
	sqlite3_finalize(st_query);  
	return 0;
}

///////////////////////////////CNPDeploy////////////////////////////////////////////////
CNPDeploy::CNPDeploy()
{
}
CNPDeploy::~CNPDeploy()
{

}
TCPSError CNPDeploy::OnConnected(
					  IN INT32 sessionKey,
					  IN const IPP& peerID_IPP,
					  IN INT32 sessionCount
					  )
{
	m_client_ipp = peerID_IPP;
	return TCPS_OK;
}
void CNPDeploy::OnClose(
						IN INT32 sessionKey,
						IN const IPP& peerID_IPP,
						IN TCPSError cause
						)
{
	m_gridConn.SetServeIPP(INVALID_IPP);
}

TCPSError CNPDeploy::Login(
				IN const tcps_String& ticket
				) 
{
	m_gridConn.m_user = "netposa";
	m_gridConn.m_pass = "netposa";

	IPP serverIPP;
	
	serverIPP.ip_ = GetLocalIP();//inet_addr("127.0.0.1");//连接本机
	serverIPP.port_ = 9012;
	m_gridConn.m_service = NULL;
	return m_gridConn.SetServeIPP(serverIPP,0,m_client_ipp);//

}

TCPSError CNPDeploy::Logout(
				 )
{
	return m_gridConn.Logout();
}

TCPSError CNPDeploy::CreateTrunk(const tcps_String& trunk) 
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().CreateTrunk(trunk);
}
TCPSError CNPDeploy::RemoveTrunk(const tcps_String& trunk) 
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().RemoveTrunk(trunk);
}
TCPSError CNPDeploy::ListTrunk(tcps_Array<tcps_String>& trunks )
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().ListTrunk(trunks);
}
TCPSError CNPDeploy::AddAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag,const tcps_Array<PCC_ModuleFile>& files)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().AddAuthCenter(trunk,authTag,files);
}
TCPSError CNPDeploy::RemoveAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().RemoveAuthCenter(trunk,authTag);
}
TCPSError CNPDeploy::ListAuthCenter(const tcps_String& trunk,tcps_Array<PCC_ModuleTag>& authTags) 
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().ListAuthCenter(trunk,authTags);
}
TCPSError CNPDeploy::FindAuthCenter(const tcps_String& trunk,const PCC_ModuleTag& authTag)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().FindAuthCenter(trunk,authTag);
}
TCPSError CNPDeploy::AddModule(const tcps_String& trunk,const PCC_ModuleProperty& moduleProperty,const tcps_Array<PCC_ModuleFile>& moudleFiles,INT64& moduleKey)
{
	if(pgrid_util::Singleton<CTrunkManage>::instance().AddModule(trunk,moduleProperty,moudleFiles,moduleKey)==-1)
		return TCPS_ERROR;
    if (moduleProperty.moduleType == PCC_MODULE_VIDSTRUCTURE)//
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
			{
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
				return TCPS_OK ;
			}
			return TCPS_ERROR;
		}
	}

	return  TCPS_OK ;
}
TCPSError CNPDeploy::AddModuleFile(const tcps_String& trunk,INT64 moduleKey,PCC_ModuleFileType fileType,const tcps_Array<PCC_ModuleFile>& moduleFiles)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().AddModuleFile(trunk,moduleKey,fileType,moduleFiles);
}
TCPSError CNPDeploy::RemoveModule(const tcps_String& trunk,INT64 moduleKey)
{
	return (TCPSError) pgrid_util::Singleton<CTrunkManage>::instance().RemoveModule(trunk,moduleKey,m_gridConn);
}
TCPSError CNPDeploy::RemoveModuleFiles(const tcps_String& trunk,INT64 moduleKey,INT32 fileType)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().RemoveModuleFiles(trunk,moduleKey,fileType);
}
TCPSError CNPDeploy::ListModules(const tcps_String& trunk,tcps_Array<PCC_ModulePropWithKey>& modulesInfo)
{
	return (TCPSError)pgrid_util::Singleton<CTrunkManage>::instance().ListModules(trunk,modulesInfo);
}