// serve.cpp

#include "StdAfx.h"
#include "ipcvt.h"
#include "SingletonsRegister.h"
#include "CmdArgParser.h"
#include "MainLoopHandler.h"
#include "nplog.h"
#include "rpc_serve.h"
#include "NP_SCATTEREDSession.h"

int g_majorVer = 1;
int g_minorVer = 0;
int g_thirdVer = 0;
char g_compileNumInfo[32] =
{
	"ISCMComp"	// 8个开始标识字符
	"-1\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 共16个字符用于存放编译号（svn号，由外部工具写入）
	"ISCMNum"	// 8个结束标识字符（包括最后的'\0'）
};
BOOL g_exitFlag = false;

IPP g_serveIPP = IPP(INADDR_ANY, 2011);

static void LogPrint__(void* /*param*/, const char* txt)
	{	NPLogInfo(("%s", txt));	}

int main(int argc, const char* argv[])
{
	// 分析命令行选项
	CmdArgParser cmdParser("", TCtoC_str(CStrFormat(_T("服务版本: %d.%d.%d"), g_majorVer, g_minorVer, g_thirdVer)));

	CStringA sip;
	VERIFY(cmdParser.AddStr(
			sip, "-ip", "", "IP",
			"服务的TCP监听IP，默认监听本机所有IP"
			));

	int tcpPort = g_serveIPP.port_;
	VERIFY(cmdParser.AddInt(
			tcpPort, "-p",
			1, GET_INT_MAX_VALUE(WORD),
			tcpPort, "port",
			"服务的TCP监听端口"
			));

	if(!cmdParser.RunParse(argc, argv))
		return -1;

	if(!sip.IsEmpty())
	{
		if(!IsValidIP((const char*)sip, &g_serveIPP.ip_))
		{
			NPLogError((_T("非法ip参数: %s"), (LPCTSTR)CtoTC_str((const char*)sip)));
			return -1;
		}
	}
	g_serveIPP.port_ = tcpPort;

	// 设置基本系统参数
#if defined(WIN32)
	SetCurrentDirectory(GetExeDir());
#endif
	tcps_SetLogPrint(LogPrint__, NULL);

	// 构造rpc服务对象
	// 可在构造NP_SCATTEREDSessionMaker时传递一 void* 参数给会话对象
	// 如：NP_SCATTEREDSessionMaker sessionMaker(param);
	// 在会话对象中可使用this->m_sessionMaker.m_userParameter获得
	NP_SCATTEREDSessionMaker sessionMaker;
	iscm_IRPCServeMan* rpcMan = NULL;
	TCPSError err = iscm_MakeRPCServeMan(rpcMan, g_serveIPP.port_, g_serveIPP.ip_, &sessionMaker, TCPS_MAX_TOTAL_SESSIONS/2);
	if(TCPS_OK == err)
	{
		// 运行主循环
		NPLogInfo(("Running serve..."));
		struct TCheck
		{
			static BOOL RunCheck(void* /*param*/)
				{	return !g_exitFlag;	}
		};
#if defined(WIN32)
		typedef HWND (WINAPI* FNGetConsoleWindow)();
		FNGetConsoleWindow fnGetConsoleWindow
			= (FNGetConsoleWindow)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetConsoleWindow");
		HWND wnd;
		if(fnGetConsoleWindow && NULL!=(wnd=fnGetConsoleWindow()))
		{
			char ch[1024];
			int l = GetWindowTextA(wnd, ch, sizeof(ch)-16);
			char ippStr[32];
			GetIPPortTxt(g_serveIPP, ippStr);
			sprintf(ch+l, " - %s", ippStr);
			SetWindowTextA(wnd, ch);
		}
#endif
		RunMainLoop(TCheck::RunCheck, NULL);
		g_exitFlag = true;
		NPLogInfo(("Exiting serve..."));
	}
	else
	{
		NPLogError(("iscm_MakeRPCServeMan(%d) failed, %s(%d)", tcpPort, tcps_GetErrTxt(err), err));
	}

	// 析构rpc服务对象
	if(rpcMan)
		rpcMan->DeleteThis();

//#if defined(_NP_IS_X86) || defined(_NP_IS_X64)
//	NPCRT_API_SetPerformancePolicy(false, true);
//#endif

	// 销毁tcps/iscm库中所有的全局对象（拥有线程的）
	tcps_DestroyAllGlobalObjects();

	return 0;
}
