// NP_GridUserClient.cpp
//
// 注意: 此文件为工具生成，请不要修改

#include "StdAfx.h"
#include "NP_GridUserClient.h"
#include "ipcvt.h"
#include "Binary.h"
#include "PtrStr.h"
#include "ScopeGuard.h"
#include "AutoInterface.h"
#include "ILocker.h"
#include "TaskMan.h"
#include "smlib.h"
#include "iscm_requester.h"
#include "iscm_callback_serve.h"
#include "iscm_posting_caller.h"
#include "iscm_udp_serve.h"
#include "tcps_connect_man.h"
#include "iscm_helper.h"
#include "iscm_smart_container.h"

#if defined(_MSC_VER)
	#pragma warning(disable: 4702) // unreachable code
#endif

static const char* s_GRID_User_AddJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_AddJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_DelJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_FindJobProgram_TypeInfo_ = NULL;
static int s_GRID_User_FindJobProgram_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJobPrograms_TypeInfo_ = NULL;
static int s_GRID_User_ListJobPrograms_TypeInfo_len_ = 0;

static const char* s_GRID_User_CommitJob_TypeInfo_ = NULL;
static int s_GRID_User_CommitJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_ReplyCommitJob_TypeInfo_ = NULL;
static int s_GRID_User_ReplyCommitJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_CancelJob_TypeInfo_ = NULL;
static int s_GRID_User_CancelJob_TypeInfo_len_ = 0;

static const char* s_GRID_User_SetJobPriority_TypeInfo_ = NULL;
static int s_GRID_User_SetJobPriority_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJobs_TypeInfo_ = NULL;
static int s_GRID_User_ListJobs_TypeInfo_len_ = 0;

static const char* s_GRID_User_QueryJobs_TypeInfo_ = NULL;
static int s_GRID_User_QueryJobs_TypeInfo_len_ = 0;

static const char* s_GRID_User_UpdateGrid_TypeInfo_ = NULL;
static int s_GRID_User_UpdateGrid_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListModules_TypeInfo_ = NULL;
static int s_GRID_User_ListModules_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetLogCount_TypeInfo_ = NULL;
static int s_GRID_User_GetLogCount_TypeInfo_len_ = 0;

static const char* s_GRID_User_LoadLogInfos_TypeInfo_ = NULL;
static int s_GRID_User_LoadLogInfos_TypeInfo_len_ = 0;

static const char* s_GRID_User_Login_TypeInfo_ = NULL;
static int s_GRID_User_Login_TypeInfo_len_ = 0;

static const char* s_GRID_User_Logout_TypeInfo_ = NULL;
static int s_GRID_User_Logout_TypeInfo_len_ = 0;

static const char* s_GRID_User_AddUser_TypeInfo_ = NULL;
static int s_GRID_User_AddUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelUser_TypeInfo_ = NULL;
static int s_GRID_User_DelUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_UpdatePassword_TypeInfo_ = NULL;
static int s_GRID_User_UpdatePassword_TypeInfo_len_ = 0;

static const char* s_GRID_User_ManageUser_TypeInfo_ = NULL;
static int s_GRID_User_ManageUser_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListAllUsers_TypeInfo_ = NULL;
static int s_GRID_User_ListAllUsers_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetUserInfo_TypeInfo_ = NULL;
static int s_GRID_User_GetUserInfo_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListJTMs_TypeInfo_ = NULL;
static int s_GRID_User_ListJTMs_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetJTMInfo_TypeInfo_ = NULL;
static int s_GRID_User_GetJTMInfo_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetGridProperty_TypeInfo_ = NULL;
static int s_GRID_User_GetGridProperty_TypeInfo_len_ = 0;

static const char* s_GRID_User_AddSplitter_TypeInfo_ = NULL;
static int s_GRID_User_AddSplitter_TypeInfo_len_ = 0;

static const char* s_GRID_User_DelSplitter_TypeInfo_ = NULL;
static int s_GRID_User_DelSplitter_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListSplitters_TypeInfo_ = NULL;
static int s_GRID_User_ListSplitters_TypeInfo_len_ = 0;

static const char* s_GRID_User_ListSplitterParam_TypeInfo_ = NULL;
static int s_GRID_User_ListSplitterParam_TypeInfo_len_ = 0;

static const char* s_GRID_User_GetGridStatistics_TypeInfo_ = NULL;
static int s_GRID_User_GetGridStatistics_TypeInfo_len_ = 0;

static const char* s_GRID_User_UDPLink__TypeInfo_ = NULL;
static int s_GRID_User_UDPLink__TypeInfo_len_ = 0;

static const char* s_GRID_User_UDPLinkConfirm__TypeInfo_ = NULL;
static int s_GRID_User_UDPLinkConfirm__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetRedirect__TypeInfo_ = NULL;
static int s_GRID_User_SetRedirect__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetTimeout__TypeInfo_ = NULL;
static int s_GRID_User_SetTimeout__TypeInfo_len_ = 0;

static const char* s_GRID_User_SetSessionBufferSize__TypeInfo_ = NULL;
static int s_GRID_User_SetSessionBufferSize__TypeInfo_len_ = 0;

static const char* s_GRID_User_MethodCheck__TypeInfo_ = NULL;
static int s_GRID_User_MethodCheck__TypeInfo_len_ = 0;

static const char* s_GRID_User_CallbackCheck__TypeInfo_ = NULL;
static int s_GRID_User_CallbackCheck__TypeInfo_len_ = 0;

static INT_PTR InitializeAllCallsTypeInfo_()
{
	// BOOL和枚举类型长度必须为4字节（网络协议已规定为固定4字节）
	STATIC_ASSERT(4 == sizeof(BOOL));
	STATIC_ASSERT(4 == sizeof(DummyEnumType));

	BEGIN_LOCAL_SAFE_STATIC_OBJ(INT_PTR, dummyVar);

	{
		BYTE const chZipped[] =
		{
			0x75,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0xC2,0x20,0x14,0x86,0xF7,0x5F,
			0xEC,0x25,0x17,0xB5,0xED,0x3A,0x8D,0xEB,0x12,0xAB,0x9B,0xE9,0x62,0xD4,0xF8,0xB5,
			0xDD,0x11,0x5A,0x99,0x25,0x5B,0xA1,0x01,0x8C,0x6D,0xE2,0x8F,0x1F,0x05,0xCC,0xCA,
			0x6E,0xC8,0x79,0x1F,0x0E,0xEF,0x39,0x9C,0xB3,0xD8,0x66,0x73,0x98,0x12,0x8A,0x78,
			0xBB,0x6F,0x6B,0x3C,0xC8,0x25,0xAC,0x50,0x51,0x12,0x8A,0xB7,0xE8,0x9A,0xF8,0x40,
			0xE9,0xF7,0xE9,0x71,0x0A,0xD3,0x56,0xE2,0x82,0x9D,0x70,0xD2,0x91,0xD9,0xAE,0x44,
			0xBC,0xEE,0xB1,0xC9,0xA2,0xB3,0x99,0x6D,0x0E,0xDA,0xA3,0xA8,0x2F,0xB0,0x19,0xC5,
			0xB0,0x89,0x23,0xE5,0xD0,0x29,0xC4,0xAB,0x44,0x07,0x15,0xA9,0x45,0x02,0x4C,0xFA,
			0x6B,0x83,0x8B,0x8B,0xC4,0x29,0x91,0x62,0x80,0x73,0x18,0x06,0xB9,0x8A,0x92,0x30,
			0x00,0x4A,0xC4,0x91,0x16,0x71,0x64,0x73,0xD7,0x3B,0xED,0xCC,0x84,0x84,0x07,0xFA,
			0x4D,0xD9,0x95,0x2A,0xE7,0x4E,0x7D,0x10,0x1A,0x06,0xCA,0xD2,0xC6,0x27,0x76,0x15,
			0x70,0xFC,0x08,0xC7,0x23,0x17,0xAD,0xF6,0xAE,0x0E,0x7C,0xDF,0x77,0xC9,0xE7,0xC6,
			0xD5,0x4B,0x46,0xCF,0x25,0xE3,0xD4,0xA5,0x47,0x22,0x24,0x72,0xD1,0x93,0x2B,0x6D,
			0xE1,0x25,0xA1,0x97,0x06,0x06,0x30,0x4A,0x86,0xBE,0xE9,0xF4,0x4E,0x62,0x45,0x86,
			0xF6,0x5B,0x1B,0xCE,0xCE,0x1C,0x55,0x6F,0xE4,0x07,0x7B,0xE9,0x7A,0xBD,0x04,0x42,
			0x72,0x42,0xCF,0x20,0xD7,0x0B,0x71,0x93,0xB2,0xB9,0x67,0x6F,0xF5,0xB0,0x8F,0x98,
			0x0B,0xC2,0x28,0xE8,0x4F,0xDE,0x08,0x33,0x2B,0x13,0xF7,0x66,0x6C,0xC0,0xDF,0xAA,
			0xFF,0xB9,0xD3,0x2F,0xE6,0x69,0x2F,0xDB,0x53,0x36,0xBF,0x37,0x63,0xB6,0x65,0xEB,
			0x79,0xD9,0x6A,0xAF,0x56,0x64,0xCE,0x49,0x85,0x65,0xC9,0x4E,0xB7,0x6C,0x75,0x73,
			0x9E,0x2A,0x2F,0x95,0x71,0x43,0x9C,0xA3,0xF6,0xB9,0x7F,0xD3,0x7D,0xF4,0x05,0x3C,
			0xFC,0x02,0xC9,0xC0,0xDC,0xC7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x18,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0xF1,0xF4,0x0B,0x31,0x33,0xB1,0xD3,0x61,0x00,0x00,
			0x74,0x73,0x08,0x2B,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x14,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0x41,0x6F,0x83,0x20,0x18,0x86,0xF7,0x5F,
			0xDA,0xA3,0x07,0x6B,0x9D,0x6B,0xD3,0x78,0xA8,0xED,0xD2,0xB8,0x18,0x6D,0x5A,0xEB,
			0x76,0x23,0xA8,0x44,0xC9,0x22,0x18,0xC0,0xA8,0x89,0x3F,0x7E,0x28,0x6C,0x2B,0x17,
			0xF2,0x3D,0x4F,0xC8,0x0B,0xBC,0x5C,0x6E,0xE1,0x19,0x04,0x98,0x40,0x36,0xA6,0x63,
			0x8B,0x56,0xB9,0x00,0x0D,0x2C,0x6A,0x4C,0xD0,0x0D,0xF6,0xBE,0x6D,0x49,0xFE,0x38,
			0x66,0x47,0x10,0x8C,0x02,0x15,0xB4,0x44,0xFE,0x6C,0x4E,0xF7,0x1A,0xB2,0xF6,0xC9,
			0x1D,0x2E,0x73,0xCC,0xE9,0xFA,0x58,0x32,0x8A,0xB6,0x03,0xC3,0xCE,0x03,0x83,0xE7,
			0xCA,0x84,0x99,0x20,0x6B,0xFC,0x65,0x68,0x70,0xCB,0x7D,0x4B,0x6D,0x7F,0x1F,0x50,
			0xD1,0x09,0x14,0x60,0xC1,0x57,0x28,0x07,0x5B,0x27,0x97,0x93,0xBF,0x75,0x2C,0x09,
			0x9E,0xBB,0x80,0xE7,0xEA,0xBD,0xC9,0x7D,0x49,0xA6,0x5C,0x80,0x07,0xF9,0x26,0xB4,
			0x27,0x32,0x79,0xA6,0x4F,0x4C,0xB6,0x8E,0x8C,0xD4,0x73,0x49,0x7B,0x0E,0xF6,0xAF,
			0x60,0xBF,0x33,0x55,0x9C,0x9A,0xEC,0xD8,0xB6,0x6D,0x9A,0xAF,0xAB,0xC9,0x11,0x25,
			0x55,0x4D,0x19,0x31,0x6D,0x86,0xB9,0x80,0xA6,0x7A,0x33,0x51,0x1F,0x1C,0x61,0xD2,
			0x0D,0xC0,0x01,0xAE,0xBF,0xB1,0xD5,0x4D,0x7F,0x8D,0x27,0xCD,0x46,0x3F,0xEB,0xCA,
			0x68,0xC5,0x60,0x13,0x9E,0xD7,0x5C,0x30,0x4C,0x2A,0x6B,0xE9,0x31,0x43,0x8C,0x63,
			0x4A,0x14,0xE8,0x52,0x15,0xA8,0x1A,0xD4,0xFC,0x54,0x9F,0x12,0xFF,0xBF,0xA8,0xD3,
			0x75,0xCE,0x3A,0x8C,0x53,0xD9,0xAA,0x5A,0x0F,0x0D,0x12,0x35,0x2D,0xA7,0x30,0x9E,
			0x96,0xB3,0xFE,0x6E,0x60,0x25,0x8F,0x74,0x0A,0x92,0x24,0xB2,0x5E,0x7E,0x00,0x21,
			0xBF,0xB9,0xB6,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_FindJobProgram_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_FindJobProgram_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x72,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0x82,0x30,0x14,0x86,0xF7,0x5F,
			0xF4,0xB2,0x17,0x88,0x8C,0xF9,0x31,0x96,0xF8,0x15,0xA3,0x61,0x6A,0x14,0xDC,0xEE,
			0x9A,0x82,0x9D,0x34,0x0B,0x2D,0x69,0x6B,0x84,0xC4,0x1F,0xBF,0x96,0x76,0xD3,0xEE,
			0xA6,0x39,0xEF,0xDB,0xD3,0xE7,0xC0,0x7B,0x96,0xFB,0xD5,0x1C,0x4E,0x09,0x45,0xBC,
			0x49,0x9A,0x0A,0x77,0x32,0x09,0x4B,0x94,0x17,0x84,0xE2,0x3D,0xBA,0x46,0x1E,0x50,
			0x7A,0x3D,0x39,0x4E,0xE0,0xB4,0x91,0x38,0x67,0x27,0x1C,0x69,0x67,0x76,0x28,0x10,
			0xAF,0x1E,0xBC,0xF1,0x52,0x63,0x66,0xBB,0xB4,0x65,0xE4,0xD5,0x05,0xD6,0x83,0x10,
			0xD6,0x61,0xA0,0x08,0x5A,0x21,0x5E,0x46,0x6D,0x51,0x92,0x4A,0x44,0xC0,0xB4,0x2F,
			0x6A,0x9C,0x5F,0x24,0x9E,0x12,0x29,0x3A,0x38,0x83,0x7D,0x3F,0x53,0x55,0xD4,0xF7,
			0x81,0x12,0x61,0xD0,0x8A,0x30,0xB0,0xBD,0xDB,0x43,0x4B,0x66,0x42,0xC2,0x94,0x7E,
			0x53,0x76,0xA5,0x8A,0xAC,0xD5,0x07,0xA1,0x7D,0x5F,0x21,0x6D,0x7D,0x62,0x57,0x01,
			0x87,0xCF,0x70,0x38,0x70,0xAD,0x4D,0xE2,0x6A,0xDF,0xF3,0x3C,0xD7,0xF9,0xDC,0xB9,
			0x3A,0x66,0xF4,0x5C,0x30,0x4E,0x5D,0xF7,0x48,0x84,0x44,0xAE,0xF5,0xE2,0x4A,0x3B,
			0x38,0x26,0xF4,0x52,0x43,0x1F,0x06,0x51,0xCF,0x33,0x5F,0xFA,0xEB,0x84,0xCA,0xE9,
			0xD9,0xDF,0xDA,0x71,0x76,0xE6,0xA8,0x5C,0xCD,0xBB,0x42,0x72,0x42,0xCF,0xA0,0xCD,
			0xF1,0x88,0xB9,0x20,0x8C,0x1A,0x61,0x43,0x35,0xC2,0xC4,0x60,0xEA,0x87,0xF8,0x8C,
			0x71,0xDF,0xE2,0x3F,0x3A,0xFD,0x62,0xDD,0x16,0xFC,0x37,0x0E,0xD8,0x71,0x66,0x11,
			0x76,0x5E,0x77,0xB5,0x49,0x54,0xFA,0xE6,0x34,0x37,0xA9,0xC0,0x7C,0x34,0x5A,0xB3,
			0xCC,0xBE,0xD4,0x2D,0x6A,0x27,0x0E,0x4C,0xD1,0x41,0x9C,0xBC,0x1F,0x16,0xB1,0x7E,
			0xAA,0x37,0x56,0x62,0x59,0xB0,0xD3,0x6D,0x9B,0x26,0x37,0xC4,0x39,0x6A,0x5E,0xEF,
			0x80,0x37,0xF0,0xF4,0x03,0x8F,0x81,0xDA,0x0C,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJobPrograms_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJobPrograms_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xFF,0x02,0x00,0x80,0x78,0x01,0x5D,0x92,0xC1,0x8E,0x82,0x30,0x10,0x86,0xF7,0x5D,
			0xF4,0xD8,0x03,0x22,0xCB,0x6A,0x36,0x1C,0x44,0x89,0xD6,0xA0,0x10,0x44,0xDC,0x5B,
			0x03,0xD8,0x95,0x66,0x43,0x6B,0x4A,0x8D,0x9A,0xF8,0xF0,0x5B,0x68,0xDD,0xA5,0x5E,
			0xE8,0xFC,0x1F,0xED,0x4C,0xFB,0xCF,0x2C,0x13,0xB8,0x40,0x3E,0xA1,0x39,0xBF,0xA7,
			0xF7,0x33,0x1E,0x14,0x02,0xD5,0x79,0x59,0x11,0x8A,0x93,0xFC,0xEA,0x59,0x40,0xEA,
			0xF5,0x2C,0x9B,0x21,0xFF,0x2E,0x70,0xC9,0x8E,0xD8,0x6B,0xC9,0x7C,0x57,0xE5,0xFC,
			0xDC,0x63,0x9F,0xCB,0x36,0xCD,0x3C,0xDE,0x77,0x39,0xCA,0xF3,0x05,0xDD,0x26,0x2E,
			0xBA,0xB9,0x8E,0xCC,0xD0,0xAA,0x9C,0xD7,0x5E,0x17,0xD4,0xE4,0xDC,0x78,0x40,0x6D,
			0x0F,0x6E,0xB8,0xBC,0x08,0xEC,0x13,0xD1,0x0C,0x70,0x81,0xC6,0x76,0x21,0x23,0x6F,
			0x6C,0x03,0x29,0x5C,0xA7,0x13,0xAE,0xA3,0xF7,0xAE,0x59,0x01,0xE9,0x37,0x1B,0x76,
			0x75,0x62,0xCE,0x4E,0x3C,0xAF,0xE1,0x02,0xBC,0xC8,0x46,0x70,0x42,0x4F,0x40,0x2F,
			0xDD,0x4F,0x79,0x30,0xE6,0x84,0x71,0x22,0xEE,0x20,0x4C,0x37,0xBB,0x20,0x04,0x70,
			0x9B,0xCA,0x22,0xEA,0xAB,0xB7,0x16,0x9D,0x01,0x40,0x2F,0xEA,0x7A,0xBD,0x93,0x03,
			0x95,0x2A,0xF2,0xE3,0x04,0x46,0x08,0x2E,0xC2,0x40,0xBE,0xCB,0x60,0x61,0x74,0xF0,
			0x4C,0xB2,0x8D,0x92,0xCD,0x2C,0x7C,0x81,0x2B,0xB8,0x5C,0xBD,0xA0,0x38,0x09,0x82,
			0x4D,0x9C,0xC2,0x2C,0x78,0xFA,0x12,0xED,0x3A,0x17,0x59,0x23,0xD0,0x9E,0xFE,0x50,
			0x76,0xA5,0xB2,0x5A,0xAB,0x0E,0x84,0x8E,0x6D,0x69,0x9F,0x8E,0x8F,0xEC,0xDA,0xA0,
			0xE9,0x3B,0x9A,0x4E,0x4C,0xB4,0x4D,0x4D,0x6D,0x5B,0x96,0x65,0x92,0xAF,0xD8,0xD4,
			0x21,0xA3,0xA7,0x8A,0x71,0x6A,0xD2,0x8C,0x34,0x22,0x37,0xD1,0x87,0x29,0x75,0xE1,
			0x90,0xD0,0xCB,0x0D,0xD9,0xC8,0xF1,0x46,0x96,0xBA,0xE9,0x93,0xB8,0x92,0x8C,0x74,
			0x0B,0xFF,0xBA,0x36,0xEC,0xF7,0x27,0xC3,0xBC,0x21,0x8C,0x2A,0xEF,0xF4,0x00,0x29,
			0xA1,0x6C,0x50,0x71,0x6F,0x54,0x14,0xF8,0x9F,0x58,0x9D,0x5D,0xE7,0x19,0xF6,0x9B,
			0xFB,0x59,0x63,0x51,0xB1,0xE3,0x23,0xDA,0xA7,0x0F,0xC9,0xE5,0x30,0xC1,0xED,0xE3,
			0x39,0x15,0xED,0x38,0x81,0xB7,0x5F,0xE9,0xBF,0x05,0x29,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CommitJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CommitJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x2E,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0x03,0xA2,0x10,0x33,0x13,0x1D,0x20,
			0x23,0xB5,0xA8,0x28,0xBF,0x08,0xC4,0x48,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0x61,0x00,
			0x00,0x9D,0x70,0x10,0xCE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ReplyCommitJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ReplyCommitJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x20,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0xF1,0xF4,
			0x0B,0x31,0x33,0xB1,0xD3,0x61,0x00,0x00,0xD5,0x5D,0x0B,0x8E,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CancelJob_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CancelJob_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x9E,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0xF7,0xCA,0x4F,0x0A,0x28,
			0xCA,0xCC,0x2F,0xCA,0x2C,0xA9,0x54,0x76,0x07,0x0B,0xF8,0x3B,0x05,0x04,0x79,0xFA,
			0xC7,0x7B,0xBA,0xF8,0xB8,0xDA,0x1A,0xE8,0xA0,0x88,0xF9,0xF8,0x87,0xDB,0xA2,0x8A,
			0xF8,0xF9,0x07,0xF9,0x3A,0xFA,0xA0,0x09,0x7A,0x78,0xBA,0x7B,0xA0,0x09,0x05,0x04,
			0xB9,0xBA,0xFA,0x06,0x84,0x78,0x86,0xB9,0xDA,0xEA,0x58,0xE7,0xA6,0x96,0x64,0xE4,
			0xA7,0xD4,0x78,0xFA,0x01,0x51,0x88,0x99,0x89,0x0E,0x90,0x01,0xB1,0x06,0xE1,0x16,
			0x1D,0x06,0x00,0x30,0xC6,0x31,0x47,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetJobPriority_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetJobPriority_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xCE,0x03,0x00,0x80,0x78,0x01,0x5D,0x52,0x5D,0x93,0x9A,0x30,0x14,0xED,0x7F,0xD1,
			0xC7,0x74,0x46,0x91,0x5A,0x77,0xB7,0xD9,0x19,0x5D,0xA9,0xCB,0x16,0x85,0x41,0xD4,
			0xBE,0x65,0x02,0xA4,0x9A,0x56,0x12,0x27,0x89,0xA3,0xCC,0xF8,0xE3,0x1B,0x48,0x70,
			0xC1,0x97,0xE4,0x9E,0x93,0xFB,0x95,0x7B,0xCF,0x22,0xF6,0xE7,0x68,0x46,0x19,0x16,
			0x65,0x52,0x9E,0x48,0x2F,0x55,0xA8,0xC0,0xD9,0x81,0x32,0x12,0xE3,0x0B,0x1C,0x00,
			0x8D,0x3F,0xA6,0xDB,0x29,0x9A,0x95,0x8A,0x64,0x3C,0x27,0xB0,0x62,0xDE,0xD6,0x07,
			0x2C,0x4E,0x2D,0xEE,0x65,0x51,0xA5,0x79,0x8B,0x36,0x75,0x8E,0xEC,0x74,0x46,0xD7,
			0xC9,0x18,0x5D,0xC7,0xAE,0xCE,0x50,0x21,0x2C,0x0A,0x58,0x1B,0x05,0x3D,0x49,0x08,
			0x8C,0xBB,0x77,0x25,0xD9,0x59,0x91,0x19,0x55,0xB2,0x47,0x52,0x34,0x72,0x52,0x6D,
			0xC1,0x91,0x03,0x34,0x18,0xBB,0x35,0x18,0xBB,0xD6,0xF7,0x83,0xA7,0x3E,0xFB,0xC3,
			0xFB,0x75,0x9D,0x48,0xF0,0xBD,0xC0,0x85,0x3F,0x07,0x0F,0x50,0x2A,0x41,0xD9,0x1E,
			0xD8,0xAB,0x7E,0xD4,0x81,0x91,0xA0,0x5C,0x50,0x55,0x82,0x20,0x59,0xAE,0xBD,0x00,
			0xF8,0xAB,0x44,0x17,0x31,0xA7,0x75,0x4D,0xEB,0x01,0x00,0x7B,0x99,0xF6,0x5A,0x91,
			0x3D,0x93,0x2A,0x9C,0x45,0xB1,0x1F,0x22,0x7F,0x1E,0x78,0xFA,0x5F,0x1D,0x2E,0x08,
			0x77,0xB0,0xCB,0xAC,0xC2,0x78,0x39,0x0D,0x1E,0xC8,0x77,0x7F,0xF1,0xFE,0x40,0x45,
			0xB1,0xE7,0x2D,0xA3,0xC4,0xDF,0x7A,0xCD,0x5C,0xC2,0x75,0x3D,0x45,0x2E,0x15,0xDA,
			0xB0,0x7F,0x8C,0x5F,0x98,0xAE,0x56,0xA1,0x1D,0x65,0x23,0x47,0x8F,0xCF,0xDA,0x39,
			0xBF,0x48,0xF4,0xF4,0x0D,0x3D,0x4D,0xBA,0xD4,0x2A,0xE9,0x62,0x67,0x30,0x18,0x74,
			0x99,0xDF,0x51,0x17,0x07,0x9C,0xED,0x0F,0x5C,0xB0,0x2E,0xBB,0xA5,0x52,0xE1,0x2E,
			0xF5,0xBD,0x0B,0x6D,0xE1,0x80,0xB2,0xF3,0x15,0x39,0xC8,0x85,0xC3,0x81,0xE9,0xB4,
			0x61,0xC6,0x9A,0x19,0xDA,0x15,0xDE,0xB7,0xD6,0x6F,0xEF,0x67,0x4B,0x84,0xA4,0x9C,
			0x99,0xD9,0x59,0x01,0x19,0x60,0xC6,0x60,0xEC,0x96,0x54,0x0C,0xF1,0xA9,0x58,0x9B,
			0xDD,0xE6,0xE9,0xB7,0x97,0x6B,0xF6,0xB8,0x91,0x44,0x3C,0x3F,0xEB,0x6D,0xC6,0xE4,
			0xC4,0x85,0xAA,0x3C,0xB4,0xAC,0x1A,0x71,0x54,0xAA,0x02,0xFA,0x71,0xAD,0xB0,0x22,
			0x56,0x14,0x56,0x27,0xF6,0x22,0x42,0x70,0xD1,0x68,0xEA,0x21,0x65,0x1D,0xD5,0xFB,
			0x2B,0xD1,0xF4,0x78,0x84,0x5F,0x87,0x40,0x5B,0x3B,0x4C,0x95,0x16,0x21,0xAC,0x81,
			0xFE,0x73,0x46,0xA4,0xAC,0xB0,0x53,0x3D,0x5A,0x4C,0x72,0x14,0xFE,0x82,0x6E,0x97,
			0xF9,0x89,0xE9,0x91,0xE4,0x70,0x02,0x5E,0x0A,0xA2,0x0E,0x3C,0xBF,0x85,0x9B,0xE4,
			0x86,0x85,0xC0,0xE5,0x8F,0x7B,0xF3,0xAF,0xBA,0xC3,0x5B,0xF3,0xC5,0xDB,0x5D,0xCF,
			0x8D,0xF5,0xE5,0x3F,0xC7,0xEE,0x4B,0xF4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJobs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJobs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xC1,0x03,0x00,0x80,0x78,0x01,0x5D,0x52,0xD1,0x6E,0xDA,0x30,0x14,0xDD,0xBF,0xC0,
			0xA3,0x27,0x85,0x90,0x65,0xB4,0x9D,0x2B,0x41,0xC9,0x68,0xBA,0x40,0xA2,0x10,0x60,
			0x6F,0x96,0x93,0x78,0xE0,0x8D,0xD8,0xC8,0x36,0x82,0x48,0xFD,0xF8,0x3A,0xB1,0xE9,
			0x12,0x5E,0xEC,0x7B,0x8E,0xAF,0xCF,0xB5,0xEF,0x3D,0x8B,0x34,0x9C,0xA3,0x19,0x65,
			0x58,0xD4,0x59,0x7D,0x22,0x83,0x5C,0xA1,0x0A,0x17,0x07,0xCA,0x48,0x8A,0x2F,0xD0,
			0x01,0x1A,0xBF,0x4D,0xB7,0x53,0x34,0xAB,0x15,0x29,0x78,0x49,0x60,0xC3,0xBC,0xAC,
			0x0F,0x58,0x9C,0x3A,0xDC,0xD3,0xA2,0x91,0x79,0x49,0x36,0xAD,0x46,0x71,0x3A,0xA3,
			0xEB,0xC4,0x47,0x57,0xDF,0xD3,0x0A,0x0D,0xC2,0xA2,0x82,0x6D,0x50,0xD1,0x93,0x84,
			0xC0,0xA4,0x07,0x57,0x52,0x9C,0x15,0x99,0x51,0x25,0x07,0x24,0x47,0x63,0x37,0xD7,
			0x11,0x1C,0xBB,0x40,0x03,0xDF,0x6B,0x81,0xEF,0xD9,0xDC,0x37,0x9E,0x87,0xEC,0x0F,
			0x1F,0xB6,0x75,0x12,0xC1,0xF7,0x02,0x57,0xE1,0x1C,0xDC,0x41,0xA9,0x04,0x65,0x7B,
			0x60,0xB7,0xF6,0x50,0x5F,0x4C,0x04,0xE5,0x82,0xAA,0x1A,0x44,0xD9,0x72,0x1D,0x44,
			0x20,0x5C,0x65,0xBA,0x88,0x59,0x6D,0x6A,0xDE,0x36,0x00,0xD8,0xCD,0x3C,0xAF,0x73,
			0x73,0x60,0xA4,0xE2,0x59,0x92,0x86,0x31,0x0A,0xE7,0x51,0xA0,0xFF,0xD5,0xE3,0xA2,
			0x78,0x07,0xFB,0xCC,0x2A,0x4E,0x97,0xD3,0xE8,0x8E,0x7C,0x0D,0x17,0xAF,0x77,0x54,
			0x92,0x06,0xC1,0x32,0xC9,0xC2,0x6D,0x70,0xEB,0x4B,0xBC,0x6E,0xBB,0xC8,0xA5,0x42,
			0x1B,0xF6,0x8F,0xF1,0x0B,0xD3,0xD5,0x1A,0xB4,0xA3,0x6C,0xEC,0xEA,0xF6,0xD9,0xB8,
			0xE4,0x17,0x89,0x1E,0xBE,0xA1,0x87,0x49,0x9F,0x5A,0x65,0x7D,0xEC,0x3A,0x8E,0xD3,
			0x67,0x7E,0x27,0x7D,0x1C,0x71,0xB6,0x3F,0x70,0xC1,0xFA,0xEC,0x96,0x4A,0x85,0xFB,
			0xD4,0xF7,0x3E,0xB4,0x85,0x23,0xCA,0xCE,0x57,0xE4,0x22,0x0F,0x8E,0x1C,0xF3,0xD2,
			0x1B,0xE3,0x6B,0x66,0x64,0x47,0xF8,0x39,0xB5,0x61,0x77,0x3E,0x5B,0x22,0x24,0xE5,
			0xCC,0xF4,0xCE,0x1A,0xC8,0x00,0xD3,0x06,0x13,0x77,0xAC,0x62,0x88,0xFF,0x8E,0xB5,
			0xEA,0x56,0x67,0xD8,0x1D,0xAE,0x99,0xE3,0x46,0x12,0xF1,0xF8,0xA8,0xA7,0x99,0x92,
			0x13,0x17,0xAA,0xC9,0xD0,0xB6,0xBA,0x99,0xA3,0x71,0x15,0xD0,0x87,0x6B,0x85,0x15,
			0xB1,0xA6,0xB0,0x3E,0xB1,0x1B,0x11,0x82,0x8B,0x9B,0xA7,0xEE,0x24,0xDB,0x5B,0x83,
			0xBF,0x12,0x4D,0x8F,0x47,0xF8,0x75,0x04,0x74,0xB4,0xC3,0x54,0x69,0x13,0xC2,0x16,
			0xE8,0x3F,0x17,0x44,0xCA,0x06,0xBB,0xCD,0xA1,0xC5,0xA4,0x44,0xF1,0x2F,0xE8,0xF5,
			0x99,0x9F,0x98,0x1E,0x49,0x09,0x27,0xE0,0xA9,0x22,0xEA,0xC0,0xCB,0xF7,0x70,0xF5,
			0x8E,0x85,0xC0,0xF5,0x8F,0xF6,0xC5,0xCF,0x20,0xDE,0x64,0x96,0xF8,0xFC,0xCC,0x33,
			0xF8,0xF2,0x01,0xF6,0x45,0x48,0x72,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_QueryJobs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_QueryJobs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x48,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x28,0xCA,0x4F,0x2F,
			0x4A,0xCC,0x75,0xCB,0xCC,0x49,0x55,0x71,0xF2,0xF7,0xF7,0xD1,0x29,0x2E,0x29,0xCA,
			0xCC,0x4B,0xD7,0x49,0xCA,0xCC,0x4B,0x2C,0xAA,0xD4,0xB1,0xCE,0x4D,0x2D,0xC9,0xC8,
			0x4F,0xA9,0xF1,0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x71,0x47,0xD3,0x64,0xA7,
			0xC3,0x00,0x00,0xB2,0x09,0x19,0xBB,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UpdateGrid_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UpdateGrid_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3D,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x51,0x6F,0xC2,0x20,0x18,0x45,0xF7,0x5F,
			0xF4,0x91,0x87,0xDA,0x76,0x9D,0xC6,0xB1,0xC4,0xEA,0x62,0x5C,0x8C,0x1A,0x6D,0xDD,
			0xDE,0x08,0xAD,0xCC,0x92,0xA5,0xD0,0x00,0xC6,0x36,0xF1,0xC7,0x8F,0x02,0xDB,0x64,
			0x2F,0xCD,0x77,0x6E,0xC8,0xF9,0xE8,0x65,0xB9,0x5F,0x2D,0x50,0x4A,0x19,0x16,0x5D,
			0xD6,0x35,0x64,0x50,0x28,0x54,0xE3,0xB2,0xA2,0x8C,0xEC,0xF1,0x15,0x06,0x40,0xF3,
			0xDB,0xEC,0x38,0x43,0x69,0xA7,0x48,0xC9,0x4F,0x04,0xF6,0xC9,0xFC,0x50,0x61,0xD1,
			0xDC,0x65,0xD3,0x65,0xAF,0x99,0xEF,0x72,0xE3,0x28,0x9B,0x0B,0x6A,0xC7,0x09,0x6A,
			0x93,0x58,0x1B,0x7A,0xC2,0xA2,0x86,0x66,0xA8,0x69,0x23,0x21,0xB0,0xC7,0x5F,0x5B,
			0x52,0x5E,0x14,0x49,0xA9,0x92,0x03,0x52,0xA0,0x28,0x2C,0xF4,0x04,0xA3,0x10,0x68,
			0x48,0x62,0x03,0x49,0xEC,0xCE,0x6E,0x0F,0xC6,0xCC,0xA5,0x42,0x39,0xFB,0x62,0xFC,
			0xCA,0xB4,0xB9,0xA7,0x77,0xCA,0xA2,0x50,0x2B,0xDD,0x7C,0xE2,0x57,0x89,0x26,0x8F,
			0x68,0x32,0xF6,0xA3,0x4D,0xE6,0x73,0x18,0x04,0x81,0x9F,0x7C,0xEC,0x7C,0x5E,0x73,
			0x76,0xAE,0xB8,0x60,0x7E,0x7A,0xA4,0x52,0x61,0x3F,0x7A,0xF2,0xD1,0x2D,0x5E,0x53,
			0x76,0x69,0x51,0x88,0x62,0x38,0x0A,0xEC,0x4D,0x7F,0x92,0x44,0x27,0x23,0xF7,0x5B,
			0x3B,0xC1,0xCF,0x02,0xD7,0xAB,0xC5,0x50,0x2A,0x41,0xD9,0x19,0x98,0x1E,0x8F,0x44,
			0x48,0xCA,0x99,0x05,0x57,0xAA,0x05,0x5B,0x83,0x9D,0xEF,0xEA,0xB3,0xC1,0xDF,0x2B,
			0xFE,0xB3,0xB3,0x4F,0x3E,0x34,0xE2,0xDF,0x75,0xC0,0xAD,0xB3,0x0F,0xE1,0xF6,0x0D,
			0x57,0x9B,0x4C,0xB7,0x6F,0xBF,0xD3,0x9A,0xA8,0x8A,0x9F,0x6E,0xDB,0x3C,0xBB,0x61,
			0x21,0x70,0xF7,0xEC,0x19,0xB4,0xF2,0x05,0x3C,0x7C,0x03,0x66,0xB8,0xC9,0x04,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListModules_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListModules_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x26,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,0x41,0xB0,0xFC,0x43,0x43,0x80,0xE2,0x21,0xC6,
			0x46,0x3A,0x0C,0x00,0x03,0x2A,0x0C,0x0E,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetLogCount_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetLogCount_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x63,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0xF2,0xC9,0x4F,0xF7,0xCC,0x4B,0xCB,0x57,0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0xB1,0xCE,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,
			0x0F,0x0D,0xA9,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x81,0xAA,0xB3,0xD3,0xF1,0xF4,0xAB,
			0x81,0xAA,0x45,0x61,0x79,0xFA,0x85,0x18,0x1B,0x81,0x24,0x21,0x0C,0x06,0x00,0xA8,
			0xFC,0x20,0x88,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_LoadLogInfos_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_LoadLogInfos_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x41,0xB0,0x18,0x00,0xA3,0xA1,0x0A,0xAA,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_Login_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_Login_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x10,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,0x39,0x8D,0x06,0x61,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_Logout_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_Logout_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x7B,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x30,0x5B,0x74,0x18,0x00,0x50,0x39,0x29,
			0x89,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x19,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0x61,0x00,
			0x00,0x83,0xA0,0x09,0x6D,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xD7,0x41,0xB0,0x18,0x00,0xA3,0xA1,0x0A,0xAA,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UpdatePassword_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UpdatePassword_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x7B,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x30,0x5B,0x74,0x18,0x00,0x50,0x39,0x29,
			0x89,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ManageUser_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ManageUser_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x83,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0xFE,0xA1,0x21,0x35,0x89,0x45,0x45,0x89,0x95,0x36,0x30,
			0xCB,0xEC,0x74,0x18,0x00,0xAF,0x09,0x2C,0x83,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListAllUsers_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListAllUsers_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x86,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x02,0x91,0x9E,0x79,0x69,0xF9,0x2A,0xC5,0x25,0x45,0x99,0x79,0xE9,0x3A,0x50,
			0x0A,0x24,0x1C,0x94,0x9F,0x93,0xAA,0x63,0xED,0x8E,0xAA,0x16,0x24,0xA8,0x1C,0x1A,
			0xEC,0x1A,0x14,0xEF,0x1E,0xEA,0x1A,0x1C,0x62,0x6B,0xA0,0x03,0xE6,0x38,0xFB,0x78,
			0xBA,0xFA,0x85,0xD8,0x42,0x38,0x8E,0x2E,0xBE,0x9E,0x7E,0xB6,0x3A,0xD6,0xB9,0xA9,
			0x25,0x19,0xF9,0x29,0x35,0x9E,0x7E,0x35,0x50,0x53,0xFD,0x43,0x43,0x6A,0x60,0x16,
			0xEA,0x30,0x00,0x00,0x39,0x0D,0x2D,0xC0,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetUserInfo_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetUserInfo_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0x61,
			0x00,0x00,0x8D,0xB9,0x09,0xCE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListJTMs_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListJTMs_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0xA9,0x02,0x00,0x80,0x78,0x01,0x85,0x51,0x4D,0x8F,0x82,0x30,0x14,0xDC,0xFF,0xA2,
			0xC7,0x1E,0x10,0x59,0xD6,0x8F,0x70,0x50,0x77,0x63,0x30,0x8A,0x46,0xC1,0xDD,0x5B,
			0x53,0xB0,0x2B,0xCD,0x86,0x96,0xD0,0x12,0x21,0xF1,0xC7,0x6F,0xA1,0xD5,0x6D,0x4F,
			0x7B,0x69,0xDE,0x4C,0x27,0xF3,0x26,0xF3,0xD6,0xC7,0xF0,0x1D,0x2E,0x09,0x45,0x55,
			0x1B,0xB7,0x25,0x1E,0xA4,0x02,0x16,0x28,0xCB,0x09,0xC5,0x47,0x74,0x0B,0x1C,0x20,
			0xF1,0x66,0x71,0x5E,0xC0,0x65,0x2B,0x70,0xC6,0x2E,0x38,0xE8,0x98,0xD5,0x29,0x47,
			0x55,0x69,0x70,0xF3,0x75,0x67,0xB3,0x3A,0x24,0xBD,0x47,0x56,0xD6,0xB0,0x99,0xF8,
			0xB0,0xF1,0x3D,0xE9,0xD0,0x21,0x54,0x15,0x41,0x3F,0x14,0xA4,0xE4,0x01,0x50,0xF2,
			0x8F,0x06,0x67,0xB5,0xC0,0x4B,0x22,0xF8,0x00,0xA7,0x70,0xEC,0xA6,0x72,0x0A,0xC6,
			0x2E,0x90,0xC0,0xF7,0x7A,0xE0,0x7B,0x5A,0xBB,0x53,0x99,0x4E,0x02,0x09,0x92,0xAD,
			0x18,0x15,0xB8,0x11,0x43,0x73,0x29,0x08,0xA3,0x58,0xAA,0xE5,0x2B,0x0D,0x9E,0xAF,
			0x62,0xE4,0xDB,0x4B,0xF7,0xA7,0x2E,0x1E,0xE0,0xA2,0x22,0xF4,0xAA,0x28,0x23,0x83,
			0x22,0xFE,0xAA,0x78,0xE8,0x54,0x58,0x33,0x00,0x97,0x19,0x86,0xCF,0x1D,0xE6,0xBE,
			0x7F,0x67,0x65,0xA6,0x82,0x0C,0x18,0x17,0x30,0xA1,0x3F,0x94,0xDD,0xA8,0xEC,0xA9,
			0x43,0x9F,0x84,0x8E,0x5D,0x59,0x90,0x9E,0x2F,0xEC,0xC6,0xE1,0xF4,0x15,0x4E,0x27,
			0x36,0x15,0xC5,0x36,0x76,0x1D,0xC7,0xB1,0x99,0xAF,0x83,0x8D,0xB7,0x8C,0x5E,0x73,
			0x56,0x51,0x9B,0x3D,0x13,0x2E,0x90,0x4D,0xBD,0xD9,0x50,0x2F,0xDE,0x12,0x5A,0x37,
			0xD0,0x85,0x5E,0x30,0x72,0x54,0xD2,0x07,0xE3,0x4B,0x66,0xA4,0x8F,0x94,0x70,0x5C,
			0xCD,0x66,0x9B,0x78,0x17,0xD2,0x6F,0xA6,0xAE,0x63,0xD6,0xF6,0xB8,0x9B,0x2A,0xDA,
			0xFC,0xE9,0x0A,0x05,0xF3,0x02,0x8B,0x9C,0x5D,0xEE,0x61,0x74,0xD7,0x17,0xDA,0x27,
			0xF1,0x5D,0xBB,0x81,0x97,0x5F,0x05,0x14,0xE9,0x1C,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetJTMInfo_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetJTMInfo_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x72,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x4B,0x2D,0x2A,0xCE,
			0xCC,0xCF,0x53,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x81,0x90,0xD6,0xEE,0x20,0x99,0xD0,
			0xE2,0xD4,0x22,0x2B,0x2B,0xF7,0xA2,0xCC,0x94,0x80,0xA2,0xFC,0x82,0xD4,0xA2,0x92,
			0x4A,0x15,0xB0,0x38,0x54,0x07,0x54,0x2D,0xB2,0xBE,0xE2,0x92,0xA2,0xCC,0xBC,0x74,
			0x1D,0xEB,0xDC,0xD4,0x92,0x8C,0xFC,0x94,0x1A,0xFF,0xD0,0x90,0x1A,0x64,0xED,0x3A,
			0x0C,0x00,0x52,0x57,0x25,0xFB,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetGridProperty_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetGridProperty_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x75,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6F,0xC2,0x20,0x14,0x86,0xF7,0x5F,
			0xEC,0x25,0x17,0xB5,0xED,0x3A,0x8D,0xEB,0x12,0xAB,0x9B,0xE9,0x62,0xD4,0xF8,0xB5,
			0xDD,0x11,0x5A,0x99,0x25,0x5B,0xA1,0x01,0x8C,0x6D,0xE2,0x8F,0x1F,0x05,0xCC,0xCA,
			0x6E,0xC8,0x79,0x1F,0x0E,0xEF,0x39,0x9C,0xB3,0xD8,0x66,0x73,0x98,0x12,0x8A,0x78,
			0xBB,0x6F,0x6B,0x3C,0xC8,0x25,0xAC,0x50,0x51,0x12,0x8A,0xB7,0xE8,0x9A,0xF8,0x40,
			0xE9,0xF7,0xE9,0x71,0x0A,0xD3,0x56,0xE2,0x82,0x9D,0x70,0xD2,0x91,0xD9,0xAE,0x44,
			0xBC,0xEE,0xB1,0xC9,0xA2,0xB3,0x99,0x6D,0x0E,0xDA,0xA3,0xA8,0x2F,0xB0,0x19,0xC5,
			0xB0,0x89,0x23,0xE5,0xD0,0x29,0xC4,0xAB,0x44,0x07,0x15,0xA9,0x45,0x02,0x4C,0xFA,
			0x6B,0x83,0x8B,0x8B,0xC4,0x29,0x91,0x62,0x80,0x73,0x18,0x06,0xB9,0x8A,0x92,0x30,
			0x00,0x4A,0xC4,0x91,0x16,0x71,0x64,0x73,0xD7,0x3B,0xED,0xCC,0x84,0x84,0x07,0xFA,
			0x4D,0xD9,0x95,0x2A,0xE7,0x4E,0x7D,0x10,0x1A,0x06,0xCA,0xD2,0xC6,0x27,0x76,0x15,
			0x70,0xFC,0x08,0xC7,0x23,0x17,0xAD,0xF6,0xAE,0x0E,0x7C,0xDF,0x77,0xC9,0xE7,0xC6,
			0xD5,0x4B,0x46,0xCF,0x25,0xE3,0xD4,0xA5,0x47,0x22,0x24,0x72,0xD1,0x93,0x2B,0x6D,
			0xE1,0x25,0xA1,0x97,0x06,0x06,0x30,0x4A,0x86,0xBE,0xE9,0xF4,0x4E,0x62,0x45,0x86,
			0xF6,0x5B,0x1B,0xCE,0xCE,0x1C,0x55,0x6F,0xE4,0x07,0x7B,0xE9,0x7A,0xBD,0x04,0x42,
			0x72,0x42,0xCF,0x20,0xD7,0x0B,0x71,0x93,0xB2,0xB9,0x67,0x6F,0xF5,0xB0,0x8F,0x98,
			0x0B,0xC2,0x28,0xE8,0x4F,0xDE,0x08,0x33,0x2B,0x13,0xF7,0x66,0x6C,0xC0,0xDF,0xAA,
			0xFF,0xB9,0xD3,0x2F,0xE6,0x69,0x2F,0xDB,0x53,0x36,0xBF,0x37,0x63,0xB6,0x65,0xEB,
			0x79,0xD9,0x6A,0xAF,0x56,0x64,0xCE,0x49,0x85,0x65,0xC9,0x4E,0xB7,0x6C,0x75,0x73,
			0x9E,0x2A,0x2F,0x95,0x71,0x43,0x9C,0xA3,0xF6,0xB9,0x7F,0xD3,0x7D,0xF4,0x05,0x3C,
			0xFC,0x02,0xC9,0xC0,0xDC,0xC7,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_AddSplitter_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_AddSplitter_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x35,0x02,0x00,0x80,0x78,0x01,0x5D,0x91,0x5D,0x6B,0x83,0x30,0x14,0x86,0xF7,0x5F,
			0xEA,0xA5,0x17,0x56,0x9D,0x6B,0x29,0x5E,0xD4,0x76,0x14,0x47,0x71,0xD2,0x0F,0xB7,
			0xBB,0x10,0x35,0xD3,0x30,0x4C,0x24,0x89,0xA8,0xD0,0x1F,0xBF,0x68,0xD2,0xAD,0xD9,
			0x8D,0x9C,0xE7,0x25,0x3E,0x47,0xDF,0x1C,0x4E,0xF1,0x1E,0x44,0x98,0x40,0x36,0x5E,
			0xC6,0x16,0x2D,0x72,0x01,0x1A,0x58,0xD4,0x98,0xA0,0x13,0xEC,0x43,0xC7,0x96,0xFC,
			0xB6,0xCD,0xB6,0x20,0x1A,0x05,0x2A,0x68,0x89,0xC2,0x29,0xD9,0x9D,0x6B,0xC8,0xDA,
			0x87,0x6C,0x73,0x98,0x34,0xBB,0xF4,0x3A,0x3B,0x8A,0xB6,0x03,0xC3,0x2A,0x00,0x43,
			0xE0,0x4B,0xC3,0x44,0x90,0x35,0xE1,0x3C,0x34,0xB8,0xE5,0xA1,0xAD,0x8E,0xBF,0x0E,
			0xA8,0xE8,0x04,0x8A,0xB0,0xE0,0x0B,0x94,0x03,0xCF,0xCD,0xE5,0x14,0x7A,0xAE,0x2D,
			0x21,0xF0,0x67,0x08,0x7C,0x7D,0xF6,0xFD,0x3C,0x9B,0x29,0x17,0xE0,0x4A,0xBE,0x09,
			0xED,0x89,0x34,0x4F,0xF4,0x81,0x89,0xE7,0x4A,0xA5,0x9E,0x4B,0xDA,0x73,0xB0,0x7E,
			0x06,0xEB,0x95,0x19,0x25,0x17,0x93,0x5D,0xC7,0x71,0xCC,0xE4,0x33,0x35,0xF9,0x48,
			0x49,0x55,0x53,0x46,0xCC,0x34,0xC3,0x5C,0x40,0x33,0x7A,0x31,0x51,0x2F,0x3E,0x62,
			0xD2,0x0D,0xC0,0x05,0x7E,0xB8,0x74,0xD4,0x97,0xDE,0x93,0x40,0x26,0x4B,0xFD,0x5B,
			0x29,0xA3,0x15,0x83,0x4D,0xBC,0xB7,0xB8,0x60,0x98,0x54,0xF6,0xDC,0x63,0x86,0x18,
			0xC7,0x94,0x28,0xD0,0xA5,0x2A,0x50,0x35,0xA8,0xF9,0xA1,0x3E,0x15,0xFC,0xDD,0xE2,
			0x3F,0x3B,0xF9,0xA2,0xD6,0x2C,0xFE,0x5D,0x67,0xEB,0x75,0xEA,0x22,0xF4,0x3E,0x2B,
			0x4E,0x2E,0xB2,0x7D,0xF5,0xDC,0x34,0x48,0xD4,0xB4,0xBC,0xC5,0xC9,0xCD,0x78,0x55,
			0xBA,0xEC,0xA7,0x1F,0x20,0xE7,0xC6,0x0A,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_DelSplitter_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_DelSplitter_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x13,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0xD1,0x6E,0x83,0x20,0x18,0x46,0xF7,0x2E,
			0xED,0xA5,0x17,0x56,0x9D,0x6B,0xD3,0xB9,0xA4,0xB6,0x4B,0xE3,0xD2,0xB4,0xA6,0x55,
			0xB7,0x3B,0x82,0x4A,0x94,0x2C,0x82,0x01,0x8C,0x9A,0xF4,0xE1,0x87,0xC2,0xB6,0x72,
			0x43,0xFE,0x73,0x42,0x3E,0xE0,0xE3,0x78,0x8D,0x0E,0x20,0xC4,0x04,0xB2,0x31,0x19,
			0x5B,0xB4,0xC8,0x05,0x68,0x60,0x51,0x63,0x82,0xAE,0xB0,0x0F,0x6C,0x4B,0xF2,0xC7,
			0x2E,0xDB,0x81,0x70,0x14,0xA8,0xA0,0x25,0x0A,0x26,0xB3,0xBF,0xD5,0x90,0xB5,0x0F,
			0x6E,0x7B,0x9C,0x62,0xF6,0x71,0x3A,0x67,0x14,0x6D,0x07,0x86,0xB5,0x0F,0x06,0xDF,
			0x93,0x09,0x13,0x41,0xD6,0x04,0xF3,0xD0,0xE0,0x96,0x07,0x96,0xDA,0xFE,0x3E,0xA0,
			0xA2,0x13,0x28,0xC4,0x82,0x2F,0x50,0x0E,0x5C,0x27,0x97,0x53,0xE0,0x3A,0x96,0x04,
			0xDF,0x9B,0xC1,0xF7,0xF4,0xDE,0xCB,0x6D,0x4E,0xA6,0x5C,0x80,0x94,0x7C,0x13,0xDA,
			0x13,0x99,0x3C,0xD1,0x27,0x26,0xAE,0x23,0x23,0xF5,0x5C,0xD2,0x9E,0x83,0xCD,0x33,
			0xD8,0xAC,0x4D,0x75,0x4E,0x4C,0x76,0x6C,0xDB,0x36,0xCD,0x57,0x6C,0xF2,0x89,0x92,
			0xAA,0xA6,0x8C,0x98,0x36,0xC3,0x5C,0x40,0x53,0xBD,0x98,0xA8,0x0F,0x3E,0x61,0xD2,
			0x0D,0xC0,0x01,0x5E,0xB0,0xB2,0xD5,0x4D,0x7F,0x8D,0x2F,0xCD,0x4A,0x3F,0x2B,0x66,
			0xB4,0x62,0xB0,0x89,0x0E,0x4B,0x2E,0x18,0x26,0x95,0x35,0xF7,0x98,0x21,0xC6,0x31,
			0x25,0x0A,0x74,0xA9,0x0A,0x54,0x0D,0x6A,0x7E,0xA8,0x4F,0x89,0xFF,0x5F,0xD4,0xE9,
			0x3A,0x67,0x19,0x9D,0x13,0xD9,0xAA,0x5A,0xB7,0x0D,0x12,0x35,0x2D,0xEF,0x97,0x34,
			0xB9,0x43,0xC6,0xE0,0xF8,0x3A,0x1F,0xF9,0x77,0x91,0x37,0xEB,0xE9,0x07,0x6F,0xC8,
			0xB9,0xE4,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListSplitters_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListSplitters_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x53,0x02,0x00,0x80,0x78,0x01,0x55,0x91,0x5D,0x6F,0x83,0x20,0x18,0x85,0xF7,0x5F,
			0xDA,0x4B,0x2F,0xAC,0x3A,0xD7,0x8F,0xB9,0xA4,0xB6,0x4B,0xE3,0xD2,0xB4,0xA6,0xD5,
			0x6E,0x77,0x04,0x95,0x28,0xD9,0x04,0x03,0x98,0x6A,0xD2,0x1F,0x3F,0x14,0xBA,0x95,
			0x1B,0x78,0x9F,0x13,0x72,0x38,0x1C,0x76,0xA7,0x68,0x0B,0x42,0x4C,0x20,0xEB,0x93,
			0xBE,0x41,0x93,0x4C,0x80,0x1A,0xE6,0x15,0x26,0xE8,0x04,0xAF,0x81,0x6D,0x49,0xFE,
			0x58,0x5F,0xD6,0x20,0xEC,0x05,0xCA,0x69,0x81,0x82,0x41,0xD9,0x9C,0x2B,0xC8,0x9A,
			0x07,0x6D,0xB5,0x1B,0x6C,0x36,0x71,0x3A,0x7A,0xE4,0x4D,0x0B,0xBA,0xB9,0x0F,0x3A,
			0xDF,0x93,0x0E,0x03,0x41,0x56,0x07,0xE3,0x50,0xE3,0x86,0x07,0x96,0x3A,0xFE,0xDE,
			0xA1,0xBC,0x15,0x28,0xC4,0x82,0x4F,0x50,0x06,0x5C,0x27,0x93,0x53,0xE0,0x3A,0x96,
			0x04,0xDF,0x1B,0xC1,0xF7,0xF4,0xD9,0xE3,0x79,0x74,0xA6,0x5C,0x80,0x94,0x7C,0x13,
			0x7A,0x25,0xD2,0x79,0xA0,0x4F,0x4C,0x5C,0x47,0x5A,0xEA,0xB9,0xA0,0x57,0x0E,0x16,
			0xCF,0x60,0x31,0x37,0xA5,0x43,0x62,0xB2,0x63,0xDB,0xB6,0xA9,0x7C,0xC5,0x26,0xEF,
			0x29,0x29,0x2B,0xCA,0x88,0xA9,0x5E,0x30,0x17,0xD0,0x94,0x5E,0x4C,0xD4,0x17,0xEF,
			0x31,0x69,0x3B,0xE0,0x00,0x2F,0x98,0xD9,0x2A,0xE9,0x5D,0xF1,0xA5,0x32,0xD3,0xCF,
			0x8A,0x19,0x2D,0x19,0xAC,0xA3,0xED,0x94,0x0B,0x86,0x49,0x69,0x8D,0x3D,0x5E,0x10,
			0xE3,0x98,0x12,0x05,0xBA,0x54,0x05,0xAA,0x06,0x35,0x3F,0xD4,0xA7,0x84,0xFF,0x5F,
			0xD4,0xEE,0xDA,0x67,0x1A,0x1D,0x12,0xD9,0xAA,0x5A,0x55,0xF5,0x29,0x47,0x6C,0xB9,
			0x3C,0x37,0x3F,0x58,0x08,0xC4,0x62,0x28,0x33,0xDC,0x13,0xE8,0x20,0x7A,0x5B,0xD5,
			0x48,0x54,0xB4,0xB8,0x45,0x87,0xDB,0x18,0xED,0x2F,0xB0,0x75,0x4C,0x93,0x1B,0x64,
			0x0C,0xF6,0xAF,0x86,0xCD,0x9B,0xF5,0xF4,0x0B,0x0D,0x0C,0xD1,0xF3,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_ListSplitterParam_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_ListSplitterParam_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x77,0x00,0x00,0x80,0x78,0x01,0x73,0x0F,0xF2,0x74,0x89,0x0F,0x2D,0x4E,0x2D,0xB2,
			0xB2,0x72,0x2F,0xCA,0x4C,0x09,0x2E,0x49,0x2C,0xC9,0x2C,0x2E,0xC9,0x4C,0x2E,0x56,
			0xF1,0x09,0xF1,0x0D,0x76,0xF5,0xD1,0x81,0x52,0x9E,0x7E,0x21,0xC6,0x46,0x3A,0xB8,
			0x48,0x54,0xB5,0x50,0x9E,0x75,0x6E,0x6A,0x49,0x46,0x7E,0x4A,0x8D,0x7F,0x68,0x48,
			0x0D,0xAA,0xD9,0x3A,0x0C,0x00,0xAA,0x01,0x25,0x54,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_GetGridStatistics_TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_GetGridStatistics_TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x1C,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0x0F,0x0D,
			0xA9,0xF1,0xF4,0x0B,0x31,0x36,0xD2,0x41,0xB0,0x18,0x00,0x91,0x83,0x08,0xDE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UDPLink__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UDPLink__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x08,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0x61,0x00,0x00,
			0x0E,0xD3,0x02,0xFE,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_UDPLinkConfirm__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_UDPLinkConfirm__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x23,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0x4F,0x4E,
			0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,0xF4,0xAB,0xF1,0x0C,0x08,0xD0,0x01,0x52,
			0x49,0x99,0x79,0x89,0x45,0x95,0x3A,0x0C,0x00,0xF8,0x4D,0x0C,0xF9,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetRedirect__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetRedirect__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetTimeout__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetTimeout__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x22,0x00,0x00,0x80,0x78,0x01,0x2B,0xC8,0x2F,0x2E,0xC9,0xCC,0x4B,0x8F,0xCF,0x4D,
			0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0x03,0xA2,0x10,0x63,0x23,0x1D,0x38,0x83,0x01,
			0x00,0xE4,0xB8,0x0B,0x7F,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_SetSessionBufferSize__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_SetSessionBufferSize__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3A,0x00,0x00,0x80,0x78,0x01,0xCB,0x4D,0x2D,0xC9,0xC8,0x4F,0xA9,0xF1,0xF4,0xAB,
			0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,0xC1,0x10,
			0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,0x9A,0x78,
			0x15,0x41,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_MethodCheck__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_MethodCheck__TypeInfo_len_ = bin.Length()-1;
	}

	{
		BYTE const chZipped[] =
		{
			0x3C,0x00,0x00,0x80,0x78,0x01,0x4B,0x4E,0xCC,0xC9,0x49,0x4A,0x4C,0xCE,0xAE,0xF1,
			0xF4,0xAB,0x49,0x2C,0x2A,0x4A,0xAC,0xB4,0x29,0x2E,0x29,0xCA,0xCC,0x4B,0xB7,0xD3,
			0xC1,0x10,0xF0,0x0F,0x0D,0x81,0x2A,0x71,0xF2,0xF7,0xF7,0xB1,0xD3,0x61,0x00,0x00,
			0xC2,0xD8,0x15,0xED,
		};
		static CBinary idTxt;
		CBinary& bin = idTxt;
		VERIFY(QuickUnzip(chZipped, sizeof(chZipped), bin));
		s_GRID_User_CallbackCheck__TypeInfo_ = (const char*)bin.Get();
		s_GRID_User_CallbackCheck__TypeInfo_len_ = bin.Length()-1;
	}

	END_LOCAL_SAFE_STATIC_OBJ(dummyVar);
}

class GRID_User_RC : public GRID_User_T
{
	friend class GRID_User;
	friend class GRID_User_S;
private:
	GRID_User_RC(const GRID_User_RC&);
	GRID_User_RC& operator= (const GRID_User_RC&);

public:
	explicit GRID_User_RC(GRID_User* owner = NULL);
	virtual ~GRID_User_RC();

protected:
	void DestroyRequester();

	virtual TCPSError OnConnected()
	{
		if(m_owner)
			return m_owner->OnConnected();
		return TCPS_OK;
	}

	virtual void OnPeerBroken()
	{
		if(m_owner)
			m_owner->OnPeerBroken();
	}
	virtual void OnClose()
	{
		if(m_owner)
			m_owner->OnClose();
	}

	virtual void OnConnectTimeout(IN TCPSError cause)
	{
		if(m_owner)
			m_owner->OnConnectTimeout(cause);
	}

public:
	TCPSError SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags = 0,
				IN const IPP& clientID_IPP = INVALID_IPP,
				IN const IPP& secondaryServeIPP = INVALID_IPP,
				IN const IPP& tertiaryServeIPP = INVALID_IPP,
				IN DWORD reconnectIntervalMSELs = 2000
				);
	IPP GetServeIPP() const;
	IPP GetServingIPP() const;
	BOOL IsConnected() const;

	void SetKeeping(
				IN BOOL keeping
				);
	BOOL IsKeeping() const;

	void PrepareDisconnect();

	void SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				);
	void GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const;

	void SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				);
	void GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const;

	void SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				);
	void GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const;

public:
	const GRID_User::MethodMatchingFlag& GetMethodMatchingFlag(
				OUT TCPSError* err = NULL
				);

public:
	TCPSError StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				);

public:
	TCPSError AddJobProgram(
				IN const GRID_ProgramInfo& programInfo,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

public:
	TCPSError DelJobProgram(
				IN const tcps_Array<INT64>& programKeys
				) method;

public:
	TCPSError FindJobProgram(
				IN const GRID_ProgramID& programID,
				OUT BOOL& found
				) method;

public:
	TCPSError ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms
				) method;

public:
	TCPSError CommitJob(
				OUT INT64& jobKey,
				IN const GRID_JobInfo& jobInfo
				) method;

protected:
	virtual TCPSError ReplyCommitJob(
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback
	{
		if(m_owner)
		{
			return m_owner->ReplyCommitJob(
					jobKey,
					replyCode,
					context
					);
		}
		// TODO: 请在派生类中重载此函数
		return TCPS_ERR_CALLBACK_NOT_IMPLEMENTED;
	}

public:
	TCPSError CancelJob(
				IN const tcps_Array<INT64>& jobKeys
				) posting_method;

public:
	TCPSError SetJobPriority(
				IN INT64 jobKey,
				IN GRID_JobPriority priority
				) method;

public:
	TCPSError ListJobs(
				OUT tcps_Array<JobReport>& jobReports,
				IN INT32 jobState,
				IN LTMSEL beginTime,
				IN LTMSEL endTime
				) method;

public:
	TCPSError QueryJobs(
				IN const tcps_Array<INT64>& jobKeys,
				OUT tcps_Array<JobReport>& jobReports
				) method;

public:
	TCPSError UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

public:
	TCPSError ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules
				) method;

public:
	TCPSError GetLogCount(
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				OUT INT32& logCount
				) method;

public:
	TCPSError LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos,
				IN LTMSEL beginTime,
				IN LTMSEL endTime,
				IN INT32 startPos,
				IN INT32 length
				) method;

public:
	TCPSError Login(
				IN const char* userName, IN INT32 userName_len /*= -1*/,
				IN const char* password, IN INT32 password_len /*= -1*/
				) method;

public:
	TCPSError Logout(
				) posting_method;

public:
	TCPSError AddUser(
				IN const UserInfo& userInfo
				) method;

public:
	TCPSError DelUser(
				IN const tcps_Array<tcps_String>& userNames
				) method;

public:
	TCPSError UpdatePassword(
				IN const char* oldPassword, IN INT32 oldPassword_len /*= -1*/,
				IN const char* newPassword, IN INT32 newPassword_len /*= -1*/
				) method;

public:
	TCPSError ManageUser(
				IN const UserInfo& userInfo
				) method;

public:
	TCPSError ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos
				) method;

public:
	TCPSError GetUserInfo(
				IN const char* userName, IN INT32 userName_len /*= -1*/,
				OUT UserInfo& userInfo
				) method;

public:
	TCPSError ListJTMs(
				OUT tcps_Array<tcps_String>& jtms
				) method;

public:
	TCPSError GetJTMInfo(
				IN const char* jtm, IN INT32 jtm_len /*= -1*/,
				OUT JTMInfo& jtmInfo
				) method;

public:
	TCPSError GetGridProperty(
				OUT GridProperty& gridProperty
				) method;

public:
	TCPSError AddSplitter(
				IN const GRID_ProgramInfo& splitter,
				IN const tcps_Array<GRID_ProgramFile>& files
				) method;

public:
	TCPSError DelSplitter(
				IN const GRID_ProgramInfo& splitter
				) method;

public:
	TCPSError ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters
				) method;

public:
	TCPSError ListSplitterParam(
				IN const GRID_ProgramID& splitter,
				OUT tcps_Array<SplitterParam>& splitterParams
				) method;

public:
	TCPSError GetGridStatistics(
				OUT GridStatistics& stat
				) method;

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		if(m_owner)
		{
			return m_owner->OnPrepareRedirect_(
					redirectIPP,
					redirectParam
					);
		}
		// TODO: 请在派生类中重载此函数
		return TCPS_OK;
	}

	//////////////////////////////////////////////////////////////
	////// 在此之后的代码使用者无需关心，为ISCM框架实现代码 //////
private:
	GRID_User* const m_owner;
	iscm_ClientThreadIDs m_callingOutTIDs;
	DWORD m_sessionFlags;
	DWORD m_reconnectIntervalMSELs;
	DataOutfiter m_dataOutfiter;
	iscm_IRequester* m_rpcRequester;
	BOOL m_needBrokenNotify;
	BOOL m_prepareDisconnect;

	struct TAsyncTaskStat_
	{
		INT_PTR postingCallingBytes;// 正在等待执行的posting callback的总调用数据大小
		INT32 postingCallings;		// 正在等待执行的posting callback的调用数量
		long totalAsyncTasks;		// 总的正在执行的异步任务数量，等于postingCallings + 其它异步任务数
		TAsyncTaskStat_() : postingCallingBytes(0), postingCallings(0), totalAsyncTasks(0) {}
	};
	TAsyncTaskStat_ m_asyncTaskStat;

private:
	struct ReconnectTask_ : public NPBaseTask
	{
		GRID_User_RC& face_;
		explicit ReconnectTask_(GRID_User_RC& face) : face_(face) {}
		virtual ~ReconnectTask_() {}
		virtual void OnPoolTask()
		{
			// ReconnectTask_重连动作不同于OnConnectedFailedTask_，不做多服务IPP智能切换
			if(!face_.m_prepareDisconnect && face_.IsConnected())
				face_.m_rpcRequester->Reconnect();
			else
				ASSERT(true);
			VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
			tcps_Delete(this);
		}
	};
	friend struct ReconnectTask_;

private:
	void OnNetworkMalformed_()
	{
		if(m_prepareDisconnect || !m_rpcRequester->IsConnected())
			return;
		ReconnectTask_* task = tcps_NewEx(ReconnectTask_, (*this));
		VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) >= 1);
		FetchTaskPool()->Push(task);
	}

private:
	iscm_MatchingUpdateFlag m_methodMatchingUpdatedFlag;
	GRID_User::MethodMatchingFlag m_methodMatchingFlag;
	TCPSError UpdateMethodMatchingFlag_();

private:
	iscm_PostingPendingParam m_postingPendingParam;

private:
	static TCPSError Wrap_ReplyCommitJob(GRID_User_RC*, GRID_User*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_callback;
	static TCPSError Wrap_SetRedirect_(GRID_User_RC*, GRID_User*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) posting_callback;
	static TCPSError Wrap_CallbackCheck_(GRID_User_RC*, GRID_User*, iscm_PeerCallFlags, const BYTE*&, INT_PTR&, iscm_IRPCOutfiter*) callback;

private:
	TCPSError UDPLink_(
				OUT INT32& servePort,
				OUT INT32& linkKey
				) method;

private:
	TCPSError UDPLinkConfirm_(
				) method;

private:
	TCPSError SetRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		IPP ipp = m_rpcRequester->GetServingIPP();
		if(redirectIPP != ipp)
		{
			TCPSError err = this->OnPrepareRedirect_(redirectIPP, redirectParam);
			if(TCPS_OK != err)
				return err;

			struct SwitchTask : public NPBaseTask
			{
				long* totalAsyncTasks_;
				iscm_IRequester* requester_;
				IPP newIPP_;
				virtual void OnPoolTask()
				{
					requester_->TempSwitchTo(newIPP_);
					VERIFY(InterlockedDecrement(totalAsyncTasks_) >= 0);
					this->~SwitchTask();
					tcps_Free(this);
				}
			};
			SwitchTask* task = ::new(tcps_Alloc(sizeof(SwitchTask))) SwitchTask();
			task->totalAsyncTasks_ = &m_asyncTaskStat.totalAsyncTasks;
			task->requester_ = m_rpcRequester;
			task->newIPP_ = redirectIPP;
			VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) >= 1);
			FetchTaskPool()->Push(task);
		}
		return TCPS_OK;
	}

private:
	TCPSError SetTimeout_(
				IN INT32 recvTimeout,
				IN INT32 sendTimeout
				) posting_method;

private:
	TCPSError SetSessionBufferSize_(
				IN INT32 recvBufBytes,
				IN INT32 sendBufBytes
				) posting_method;

private:
	TCPSError MethodCheck_(
				IN const tcps_Array<tcps_String>& methods,
				IN const tcps_Array<tcps_String>& methodTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) method;

private:
	TCPSError CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback;

private:
	void OnPrepareRPCCallback_(
				IN const void* inParamsData,
				IN INT_PTR inParamsDataLen,
				OUT BOOL& notifyByTaskPool /*= true*/
				);
	TCPSError OnRPCCallback_(
				OUT BOOL& requireReplyData /*= true*/,
				IN const void* inParamsData,
				OUT BOOL& dataTransferred /*= false*/,
				IN INT_PTR inParamsDataLen,
				OUT iscm_IRPCOutfiter* outfiter
				);
	struct CallbackProxy_ : public iscm_ICallbackServeMan::IRPCClient
	{
		GRID_User_RC& face_;
		INT32 sessionKey_;
		long callbackRefCount_;

		explicit CallbackProxy_(GRID_User_RC& face)
			: face_(face), sessionKey_(0), callbackRefCount_(0)
			{}
		virtual INT32& GetSessionKey()
			{	return sessionKey_;	}
		virtual void IncreaseCallbackRef()
			{	ASSERT(callbackRefCount_ >= 0); InterlockedIncrement(&callbackRefCount_);	}
		virtual void DecreaseCallbackRef()
			{	ASSERT(callbackRefCount_ >= 1); InterlockedDecrement(&callbackRefCount_);	}
		virtual void OnCallbackInit()
			{}
		virtual void OnCallbackClose(
					TCPSError cause
					)
			{
				if(face_.m_prepareDisconnect || TCPS_OK==cause || !face_.m_rpcRequester->IsConnected())
					return;
				ReconnectTask_* task = tcps_NewEx(ReconnectTask_, (face_));
				VERIFY(InterlockedIncrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 1);
				FetchTaskPool()->Push(task);
			}
		virtual void OnPrepareRPCCallback(
					IN const void* inParamsData,
					IN INT_PTR inParamsDataLen,
					OUT BOOL& notifyByTaskPool /*= true*/
					)
			{	face_.OnPrepareRPCCallback_(inParamsData, inParamsDataLen, notifyByTaskPool);	}
		virtual TCPSError OnRPCCallback(
					OUT BOOL& requireReplyData /*= true*/,
					IN const void* inParamsData,
					OUT BOOL& dataTransferred /*= false*/,
					IN INT_PTR inParamsDataLen,
					OUT iscm_IRPCOutfiter* outfiter
					)
		{
			TCPSError err;
			while(true)
			{
				err = face_.OnRPCCallback_(requireReplyData, inParamsData, dataTransferred, inParamsDataLen, outfiter);
				if(TCPS_ERR_POSTING_PENDING_FULL == err)
				{
					ASSERT(!dataTransferred);
					Sleep(1);
					continue;
				}
				else
					break;
			}
			return err;
		}
	};
	friend struct CallbackProxy_;
	CallbackProxy_ m_callbackProxy;

private:
	typedef TCPSError (*FCallbackHandler_)(
						IN GRID_User_RC* thisObj /*= NULL*/,
						IN GRID_User* faceObj /*= NULL*/,
						IN iscm_PeerCallFlags peerCallFlags,
						IN OUT const BYTE*& ptrInParams,
						IN OUT INT_PTR& ptrInParamsLen,
						OUT iscm_IRPCOutfiter* outfiter
						);
	struct CallbackSite_
	{
		FCallbackHandler_ handler;
		BOOL isPosting; // 是否为 posting 修饰的方法/回调
		explicit CallbackSite_(FCallbackHandler_ hd = NULL, BOOL pst = false)
			: handler(hd), isPosting(pst)
			{}
	};
	typedef CQuickStringMap<CPtrStrA, CallbackSite_, QSS_Traits<3> > CallbackMap_;
	static const CallbackMap_* sm_callbackMap;
	static CallbackMap_& GetCallbackMap_();
	static void InitCallbackMap_();

private:
	mutable CLocker m_callbackCallLock;
	struct PostingTask_ : public NPBaseTask
	{
		GRID_User_RC& face_;
		iscm_PeerCallFlags peerCallFlags_;
		void* baseInParamsData_;
		CPtrStrA call_id_;
		const BYTE* inParamsData_;
		INT_PTR inParamsDataLen_;
		FCallbackHandler_ handler_;

		explicit PostingTask_(GRID_User_RC& face) : face_(face) {}
		virtual ~PostingTask_() {}
		virtual void OnPoolTask()
		{
			ASSERT(baseInParamsData_ && inParamsData_ && inParamsDataLen_>=0);
			ASSERT((INT_PTR)(inParamsData_ - (const BYTE*)baseInParamsData_) > 0);
			INT_PTR const inParamsDataLen_t = inParamsDataLen_;
			if(!face_.m_prepareDisconnect && face_.IsConnected())
			{
				CNPAutoLock locker(face_.m_callbackCallLock);
				TCPSError err = TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
				if(face_.m_owner)
				{
					void* replyData = NULL;
					INT_PTR replyLen = 0;
					err = face_.m_owner->StreamedCallback_(NULL, 0, NULL, 0, replyData, replyLen);
					if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
					{
						if(face_.m_owner->IsStreamedCallbackMatched_(call_id_.p, call_id_.len))
						{
							err = face_.m_owner->StreamedCallback_(call_id_.p, call_id_.len, inParamsData_, inParamsDataLen_, replyData, replyLen);
							ASSERT(NULL==replyData && 0==replyLen);
							if(TCPS_OK == err)
							{
								inParamsData_ += inParamsDataLen_;
								inParamsDataLen_ = 0;
							}
							else if(TCPS_ERR_MALFORMED_REQ == err)
								face_.OnNetworkMalformed_();
						}
					}
				}
				if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED == err)
				{
					err = handler_(
								&face_, NULL,
								peerCallFlags_,
								inParamsData_,
								inParamsDataLen_,
								NULL/*outfiter*/
								);
				}
				ASSERT(TCPS_OK!=err || 0==inParamsDataLen_);
			}
			else
				ASSERT(true);

			tcps_Free(baseInParamsData_);
			GRID_User_RC& face = face_;
			tcps_Delete(this);

			CNPAutoLock locker(face.m_postingCallbacks.lock);
			face.m_asyncTaskStat.postingCallingBytes -= inParamsDataLen_t;
			ASSERT(face.m_asyncTaskStat.postingCallingBytes >= 0);
			--(face.m_asyncTaskStat.postingCallings);
			ASSERT(face.m_asyncTaskStat.postingCallings >= 0);
			VERIFY(InterlockedDecrement(&face.m_asyncTaskStat.totalAsyncTasks) >= 0);

			ASSERT(face.m_postingCallbacks.calling);
			if(NULL == face.m_postingCallbacks.head)
			{
				face.m_postingCallbacks.calling = false;
				return; // 队列处理完毕
			}

			NPBaseTask* task = face.m_postingCallbacks.head;
			if(face.m_postingCallbacks.head == face.m_postingCallbacks.tail)
				face.m_postingCallbacks.head = face.m_postingCallbacks.tail = NULL;
			else
				face.m_postingCallbacks.head = task->m_nextPoolTask;
			// 此处无须对m_asyncTaskStat.postingCallings等变量进行增加（在OnRPCCallback_()已经加过）
			FetchTaskPool()->Push(task);
		}
	};
	friend struct PostingTask_;
	struct PostingTaskSite_
	{
		mutable CLocker lock;
		BOOL calling;
		NPBaseTask* head;
		NPBaseTask* tail;
		PostingTaskSite_() : calling(false), head(NULL), tail(NULL) {}
	};
	PostingTaskSite_ m_postingCallbacks;

private:
	struct PostingProxy_ : public iscm_IPostingCallerMan::INotifier
	{
		GRID_User_RC& face_;
		INT32 callerKey_; // 为iscm_IPostingCallerMan中的会话标识

		explicit PostingProxy_(GRID_User_RC& face)
			: face_(face), callerKey_(0)
			{}

		virtual void OnSessionClose(
					IN INT32 callerKey,
					IN TCPSError cause
					)
		{
			(void)callerKey; (void)cause;
			ASSERT(callerKey_ == callerKey);
			callerKey_ = 0;
		}

		virtual void OnSessionPost(
					IN INT32 callerKey,
					IN const void* data,
					IN INT_PTR dataLen,
					OUT BOOL& dataTransferred /*= false*/
					)
		{
			(void)callerKey;
			ASSERT(callerKey_ == callerKey);
			while(true)
			{
				BOOL requireReplyData = true;
				TCPSError err = face_.OnRPCCallback_(requireReplyData, data, dataTransferred, dataLen, NULL);
				ASSERT(TCPS_OK!=err || !requireReplyData);
				if(TCPS_ERR_POSTING_PENDING_FULL == err)
				{
					ASSERT(!dataTransferred);
					Sleep(1);
					continue;
				}
				else
					break;
			}
		}
	};
	friend struct PostingProxy_;
	PostingProxy_ m_postingProxy;

private:
	struct NetworkHandler_ : public iscm_IRequester::IEventHandler
	{
		GRID_User_RC& face_;
		explicit NetworkHandler_(GRID_User_RC& face) : face_(face) {}
		virtual BOOL Notify(
					IN iscm_IRequester::EventType ev,
					IN TCPSError cause,
					OUT TCPSError* actionErr /*= NULL*/
					);
	};
	friend struct NetworkHandler_;
	NetworkHandler_ m_networkHandler;

private:
	struct OnConnectedFailedTask_ : public NPBaseTask
	{
		GRID_User_RC& face_;
		explicit OnConnectedFailedTask_(GRID_User_RC& face) : face_(face) {}
		virtual ~OnConnectedFailedTask_() {}
		virtual void OnPoolTask()
		{
			// 停止以前连接
			IPP clientID_IPP, secondaryServeIPP, tertiaryServeIPP;
			IPP serveIPP = face_.m_rpcRequester->GetServeIPP(NULL, &clientID_IPP, &secondaryServeIPP, &tertiaryServeIPP);
			face_.SetServeIPP(INVALID_IPP, face_.m_sessionFlags, INVALID_IPP, INVALID_IPP, INVALID_IPP, face_.m_reconnectIntervalMSELs);

			// 切换IPP后再次异步延迟建立连接
			if(serveIPP.IsValid())
			{
				BOOL const ippSwitched = iscm_ThreeIPPSwitch(serveIPP, secondaryServeIPP, tertiaryServeIPP);
				DWORD delayTicks = face_.m_reconnectIntervalMSELs;
				if(ippSwitched)
					delayTicks = min((DWORD)500, delayTicks);
				face_.m_rpcRequester->SetServeIPP(serveIPP, "GRID_User", iscm_IRequester::cflag_async_connect, clientID_IPP, delayTicks, secondaryServeIPP, tertiaryServeIPP);
				face_.m_prepareDisconnect = false;
			}
			VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
			tcps_Delete(this);
		}
	};
	friend struct OnConnectedFailedTask_;

private:
	struct UDPCallbackProxy_ : public iscm_IUDPSession
	{
		GRID_User_RC& face_;
		BOOL isUsing_;
		explicit UDPCallbackProxy_(GRID_User_RC& face)
			: face_(face), isUsing_(false)
			{}
		virtual void OnUDPCall(
					IN tcps_Binary& frame
					);
	};
	friend struct UDPCallbackProxy_;
	UDPCallbackProxy_ m_UDPCallbackProxy;

private:
	struct StreamedCall_Task_;
	friend struct StreamedCall_Task_;

// posting method方法的异步投递任务类声明，
// 在回调里面调用自身的方法时，使用异步任务（防止死锁）。
private:
	struct CancelJob_Task_;
	friend struct CancelJob_Task_;
	struct Logout_Task_;
	friend struct Logout_Task_;
	struct SetTimeout__Task_;
	friend struct SetTimeout__Task_;
	struct SetSessionBufferSize__Task_;
	friend struct SetSessionBufferSize__Task_;
};

TCPSError GRID_User_RC::CallbackCheck_(
				IN const tcps_Array<tcps_String>& callbacks,
				IN const tcps_Array<tcps_String>& callbackTypeInfos,
				OUT tcps_Array<BOOL>& matchingFlags
				) callback
{
	if(0==callbacks.Length() || callbacks.Length()!=callbackTypeInfos.Length())
		return TCPS_ERR_INVALID_PARAM;

	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<2> > CallbackMap_;
	static CallbackMap_* s_cbMap = NULL;
	if(NULL == s_cbMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_cbMap)
		{
			static CallbackMap_ s_cbMapObj;
			CallbackMap_& cbMap = s_cbMapObj;
			VERIFY(cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ReplyCommitJob"), CPtrStrA(s_GRID_User_ReplyCommitJob_TypeInfo_, s_GRID_User_ReplyCommitJob_TypeInfo_len_))).second);
			VERIFY(cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetRedirect_"), CPtrStrA(s_GRID_User_SetRedirect__TypeInfo_, s_GRID_User_SetRedirect__TypeInfo_len_))).second);
			s_cbMap = &cbMap;
		}
	}

	matchingFlags.Resize(callbacks.Length());
	for(int index=0; index<callbacks.Length(); ++index)
	{
		BOOL& flag = matchingFlags[index];
		const tcps_String& name = callbacks[index];
		const tcps_String& typeInfos = callbackTypeInfos[index];
		CallbackMap_::const_iterator cit = s_cbMap->find(name.Get(), name.Length());
		if(cit!=s_cbMap->end() && 0==typeInfos.Compare(cit->second.p, cit->second.len))
			flag = true;
		else
			flag = false;
	}
	return TCPS_OK;
}

GRID_User::MethodMatchingFlag::MethodMatchingFlag()
{
	Reset();
	mmap_.insert(std::make_pair(CPtrStrA("AddJobProgram", 13), Info(&matching_AddJobProgram, false)));
	mmap_.insert(std::make_pair(CPtrStrA("DelJobProgram", 13), Info(&matching_DelJobProgram, false)));
	mmap_.insert(std::make_pair(CPtrStrA("FindJobProgram", 14), Info(&matching_FindJobProgram, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListJobPrograms", 15), Info(&matching_ListJobPrograms, false)));
	mmap_.insert(std::make_pair(CPtrStrA("CommitJob", 9), Info(&matching_CommitJob, false)));
	mmap_.insert(std::make_pair(CPtrStrA("CancelJob", 9), Info(&matching_CancelJob, true)));
	mmap_.insert(std::make_pair(CPtrStrA("SetJobPriority", 14), Info(&matching_SetJobPriority, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListJobs", 8), Info(&matching_ListJobs, false)));
	mmap_.insert(std::make_pair(CPtrStrA("QueryJobs", 9), Info(&matching_QueryJobs, false)));
	mmap_.insert(std::make_pair(CPtrStrA("UpdateGrid", 10), Info(&matching_UpdateGrid, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListModules", 11), Info(&matching_ListModules, false)));
	mmap_.insert(std::make_pair(CPtrStrA("GetLogCount", 11), Info(&matching_GetLogCount, false)));
	mmap_.insert(std::make_pair(CPtrStrA("LoadLogInfos", 12), Info(&matching_LoadLogInfos, false)));
	mmap_.insert(std::make_pair(CPtrStrA("Login", 5), Info(&matching_Login, false)));
	mmap_.insert(std::make_pair(CPtrStrA("Logout", 6), Info(&matching_Logout, true)));
	mmap_.insert(std::make_pair(CPtrStrA("AddUser", 7), Info(&matching_AddUser, false)));
	mmap_.insert(std::make_pair(CPtrStrA("DelUser", 7), Info(&matching_DelUser, false)));
	mmap_.insert(std::make_pair(CPtrStrA("UpdatePassword", 14), Info(&matching_UpdatePassword, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ManageUser", 10), Info(&matching_ManageUser, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListAllUsers", 12), Info(&matching_ListAllUsers, false)));
	mmap_.insert(std::make_pair(CPtrStrA("GetUserInfo", 11), Info(&matching_GetUserInfo, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListJTMs", 8), Info(&matching_ListJTMs, false)));
	mmap_.insert(std::make_pair(CPtrStrA("GetJTMInfo", 10), Info(&matching_GetJTMInfo, false)));
	mmap_.insert(std::make_pair(CPtrStrA("GetGridProperty", 15), Info(&matching_GetGridProperty, false)));
	mmap_.insert(std::make_pair(CPtrStrA("AddSplitter", 11), Info(&matching_AddSplitter, false)));
	mmap_.insert(std::make_pair(CPtrStrA("DelSplitter", 11), Info(&matching_DelSplitter, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListSplitters", 13), Info(&matching_ListSplitters, false)));
	mmap_.insert(std::make_pair(CPtrStrA("ListSplitterParam", 17), Info(&matching_ListSplitterParam, false)));
	mmap_.insert(std::make_pair(CPtrStrA("GetGridStatistics", 17), Info(&matching_GetGridStatistics, false)));
	mmap_.insert(std::make_pair(CPtrStrA("UDPLink_", 8), Info(&matching_UDPLink_, false)));
	mmap_.insert(std::make_pair(CPtrStrA("UDPLinkConfirm_", 15), Info(&matching_UDPLinkConfirm_, false)));
	mmap_.insert(std::make_pair(CPtrStrA("SetTimeout_", 11), Info(&matching_SetTimeout_, true)));
	mmap_.insert(std::make_pair(CPtrStrA("SetSessionBufferSize_", 21), Info(&matching_SetSessionBufferSize_, true)));
}

void GRID_User::MethodMatchingFlag::Reset()
{
	matching_AddJobProgram = false;
	matching_DelJobProgram = false;
	matching_FindJobProgram = false;
	matching_ListJobPrograms = false;
	matching_CommitJob = false;
	matching_CancelJob = false;
	matching_SetJobPriority = false;
	matching_ListJobs = false;
	matching_QueryJobs = false;
	matching_UpdateGrid = false;
	matching_ListModules = false;
	matching_GetLogCount = false;
	matching_LoadLogInfos = false;
	matching_Login = false;
	matching_Logout = false;
	matching_AddUser = false;
	matching_DelUser = false;
	matching_UpdatePassword = false;
	matching_ManageUser = false;
	matching_ListAllUsers = false;
	matching_GetUserInfo = false;
	matching_ListJTMs = false;
	matching_GetJTMInfo = false;
	matching_GetGridProperty = false;
	matching_AddSplitter = false;
	matching_DelSplitter = false;
	matching_ListSplitters = false;
	matching_ListSplitterParam = false;
	matching_GetGridStatistics = false;
	matching_UDPLink_ = false;
	matching_UDPLinkConfirm_ = false;
	matching_SetTimeout_ = false;
	matching_SetSessionBufferSize_ = false;
}

TCPSError GRID_User_RC::UpdateMethodMatchingFlag_()
{
	if(!m_methodMatchingUpdatedFlag.needUpdate)
		return TCPS_OK;
	InitializeAllCallsTypeInfo_();
	tcps_String methods_ar[33];
	IN tcps_Array<tcps_String> methods;
	methods.Attach(xat_bind, methods_ar, 33);
	tcps_String methodTypeInfos_ar[33];
	IN tcps_Array<tcps_String> methodTypeInfos;
	methodTypeInfos.Attach(xat_bind, methodTypeInfos_ar, 33);
	methods_ar[0].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("AddJobProgram"));
	methodTypeInfos_ar[0].Attach(xat_bind, (char*)s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_);
	methods_ar[1].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("DelJobProgram"));
	methodTypeInfos_ar[1].Attach(xat_bind, (char*)s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_);
	methods_ar[2].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("FindJobProgram"));
	methodTypeInfos_ar[2].Attach(xat_bind, (char*)s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_);
	methods_ar[3].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListJobPrograms"));
	methodTypeInfos_ar[3].Attach(xat_bind, (char*)s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_);
	methods_ar[4].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("CommitJob"));
	methodTypeInfos_ar[4].Attach(xat_bind, (char*)s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_);
	methods_ar[5].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("CancelJob"));
	methodTypeInfos_ar[5].Attach(xat_bind, (char*)s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_);
	methods_ar[6].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetJobPriority"));
	methodTypeInfos_ar[6].Attach(xat_bind, (char*)s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_);
	methods_ar[7].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListJobs"));
	methodTypeInfos_ar[7].Attach(xat_bind, (char*)s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_);
	methods_ar[8].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("QueryJobs"));
	methodTypeInfos_ar[8].Attach(xat_bind, (char*)s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_);
	methods_ar[9].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("UpdateGrid"));
	methodTypeInfos_ar[9].Attach(xat_bind, (char*)s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_);
	methods_ar[10].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListModules"));
	methodTypeInfos_ar[10].Attach(xat_bind, (char*)s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_);
	methods_ar[11].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("GetLogCount"));
	methodTypeInfos_ar[11].Attach(xat_bind, (char*)s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_);
	methods_ar[12].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("LoadLogInfos"));
	methodTypeInfos_ar[12].Attach(xat_bind, (char*)s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_);
	methods_ar[13].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("Login"));
	methodTypeInfos_ar[13].Attach(xat_bind, (char*)s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_);
	methods_ar[14].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("Logout"));
	methodTypeInfos_ar[14].Attach(xat_bind, (char*)s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_);
	methods_ar[15].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("AddUser"));
	methodTypeInfos_ar[15].Attach(xat_bind, (char*)s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_);
	methods_ar[16].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("DelUser"));
	methodTypeInfos_ar[16].Attach(xat_bind, (char*)s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_);
	methods_ar[17].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("UpdatePassword"));
	methodTypeInfos_ar[17].Attach(xat_bind, (char*)s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_);
	methods_ar[18].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ManageUser"));
	methodTypeInfos_ar[18].Attach(xat_bind, (char*)s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_);
	methods_ar[19].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListAllUsers"));
	methodTypeInfos_ar[19].Attach(xat_bind, (char*)s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_);
	methods_ar[20].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("GetUserInfo"));
	methodTypeInfos_ar[20].Attach(xat_bind, (char*)s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_);
	methods_ar[21].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListJTMs"));
	methodTypeInfos_ar[21].Attach(xat_bind, (char*)s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_);
	methods_ar[22].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("GetJTMInfo"));
	methodTypeInfos_ar[22].Attach(xat_bind, (char*)s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_);
	methods_ar[23].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("GetGridProperty"));
	methodTypeInfos_ar[23].Attach(xat_bind, (char*)s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_);
	methods_ar[24].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("AddSplitter"));
	methodTypeInfos_ar[24].Attach(xat_bind, (char*)s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_);
	methods_ar[25].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("DelSplitter"));
	methodTypeInfos_ar[25].Attach(xat_bind, (char*)s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_);
	methods_ar[26].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListSplitters"));
	methodTypeInfos_ar[26].Attach(xat_bind, (char*)s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_);
	methods_ar[27].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("ListSplitterParam"));
	methodTypeInfos_ar[27].Attach(xat_bind, (char*)s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_);
	methods_ar[28].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("GetGridStatistics"));
	methodTypeInfos_ar[28].Attach(xat_bind, (char*)s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_);
	methods_ar[29].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("UDPLink_"));
	methodTypeInfos_ar[29].Attach(xat_bind, (char*)s_GRID_User_UDPLink__TypeInfo_, s_GRID_User_UDPLink__TypeInfo_len_);
	methods_ar[30].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("UDPLinkConfirm_"));
	methodTypeInfos_ar[30].Attach(xat_bind, (char*)s_GRID_User_UDPLinkConfirm__TypeInfo_, s_GRID_User_UDPLinkConfirm__TypeInfo_len_);
	methods_ar[31].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetTimeout_"));
	methodTypeInfos_ar[31].Attach(xat_bind, (char*)s_GRID_User_SetTimeout__TypeInfo_, s_GRID_User_SetTimeout__TypeInfo_len_);
	methods_ar[32].Attach(xat_bind, (char*)CONST_STR_TO_PTR_LEN("SetSessionBufferSize_"));
	methodTypeInfos_ar[32].Attach(xat_bind, (char*)s_GRID_User_SetSessionBufferSize__TypeInfo_, s_GRID_User_SetSessionBufferSize__TypeInfo_len_);
	OUT tcps_Array<BOOL> matchingFlags;
	TCPSError err = this->MethodCheck_(methods, methodTypeInfos, matchingFlags);
	if(TCPS_OK == err)
	{
		if(matchingFlags.Length() == methods.Length())
		{
			m_methodMatchingUpdatedFlag.needUpdate = false;
			const BOOL* const matchingFlags_p = matchingFlags.Get();
			m_methodMatchingFlag.matching_AddJobProgram = matchingFlags_p[0];
			m_methodMatchingFlag.matching_DelJobProgram = matchingFlags_p[1];
			m_methodMatchingFlag.matching_FindJobProgram = matchingFlags_p[2];
			m_methodMatchingFlag.matching_ListJobPrograms = matchingFlags_p[3];
			m_methodMatchingFlag.matching_CommitJob = matchingFlags_p[4];
			m_methodMatchingFlag.matching_CancelJob = matchingFlags_p[5];
			m_methodMatchingFlag.matching_SetJobPriority = matchingFlags_p[6];
			m_methodMatchingFlag.matching_ListJobs = matchingFlags_p[7];
			m_methodMatchingFlag.matching_QueryJobs = matchingFlags_p[8];
			m_methodMatchingFlag.matching_UpdateGrid = matchingFlags_p[9];
			m_methodMatchingFlag.matching_ListModules = matchingFlags_p[10];
			m_methodMatchingFlag.matching_GetLogCount = matchingFlags_p[11];
			m_methodMatchingFlag.matching_LoadLogInfos = matchingFlags_p[12];
			m_methodMatchingFlag.matching_Login = matchingFlags_p[13];
			m_methodMatchingFlag.matching_Logout = matchingFlags_p[14];
			m_methodMatchingFlag.matching_AddUser = matchingFlags_p[15];
			m_methodMatchingFlag.matching_DelUser = matchingFlags_p[16];
			m_methodMatchingFlag.matching_UpdatePassword = matchingFlags_p[17];
			m_methodMatchingFlag.matching_ManageUser = matchingFlags_p[18];
			m_methodMatchingFlag.matching_ListAllUsers = matchingFlags_p[19];
			m_methodMatchingFlag.matching_GetUserInfo = matchingFlags_p[20];
			m_methodMatchingFlag.matching_ListJTMs = matchingFlags_p[21];
			m_methodMatchingFlag.matching_GetJTMInfo = matchingFlags_p[22];
			m_methodMatchingFlag.matching_GetGridProperty = matchingFlags_p[23];
			m_methodMatchingFlag.matching_AddSplitter = matchingFlags_p[24];
			m_methodMatchingFlag.matching_DelSplitter = matchingFlags_p[25];
			m_methodMatchingFlag.matching_ListSplitters = matchingFlags_p[26];
			m_methodMatchingFlag.matching_ListSplitterParam = matchingFlags_p[27];
			m_methodMatchingFlag.matching_GetGridStatistics = matchingFlags_p[28];
			m_methodMatchingFlag.matching_UDPLink_ = matchingFlags_p[29];
			m_methodMatchingFlag.matching_UDPLinkConfirm_ = matchingFlags_p[30];
			m_methodMatchingFlag.matching_SetTimeout_ = matchingFlags_p[31];
			m_methodMatchingFlag.matching_SetSessionBufferSize_ = matchingFlags_p[32];
		}
		else
		{
			ASSERT(false);
			err = TCPS_ERR_MALFORMED_REPLY;
		}
	}
	if(m_methodMatchingUpdatedFlag.needUpdate)
		m_methodMatchingFlag.Reset();
	return err;
}

const GRID_User::MethodMatchingFlag& GRID_User_RC::GetMethodMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	TCPSError ret = UpdateMethodMatchingFlag_();
	if(err)
		*err = ret;
	return m_methodMatchingFlag;
}

BOOL GRID_User_RC::NetworkHandler_::Notify(
				IN iscm_IRequester::EventType ev,
				IN TCPSError cause,
				OUT TCPSError* actionErr /*= NULL*/
				)
{
	if(actionErr)
		*actionErr = TCPS_OK;
	if(iscm_IRequester::et_connected==ev || iscm_IRequester::et_attached==ev)
	{
		if(face_.m_prepareDisconnect)
			return true;
	}

	if(iscm_IRequester::et_connected == ev)
	{
#if 1
		// 建立回调通道
		if(0 == (face_.m_sessionFlags&ISCMC_NO_CALLBACK))
		{
			// 连接回调通道，使用直接连接方式
			IPP const serveIPP = face_.m_rpcRequester->GetServingIPP();
			INT32 peerSessionKey;
			INT64 const bindID = face_.m_rpcRequester->GetPeerSessionKey(peerSessionKey);
			ASSERT(serveIPP.IsValid() && 0!=bindID && 0!=peerSessionKey);

			AutoDeleteInterface<iscm_IRequester> autoRequester;
			autoRequester.face_ = iscm_MakeRequester(false);
			if(NULL == autoRequester.face_)
			{
				// 容错处理
				ASSERT(false);
				if(actionErr)
					*actionErr = TCPS_ERR_OUT_OF_MEMORY;
				return false;
			}

			DWORD connectTimeout = INFINITE;
			DWORD recvTimeout = INFINITE;
			DWORD sendTimeout = INFINITE;
			face_.m_rpcRequester->GetTimeout(&connectTimeout, &recvTimeout, &sendTimeout);
			DWORD const timeoutMSELs = (INFINITE==connectTimeout ? iscm_GetDefaultConnectTimeout() : connectTimeout);
			BOOL const is_AF_LOCAL = tcps_Use_AF_LOCAL(serveIPP.ip_);
			SOCKET cbSock = TCPNewClientByTime(serveIPP.ip_, serveIPP.port_, timeoutMSELs, is_AF_LOCAL);
			if(!SockValid(cbSock))
			{
				if(actionErr)
					*actionErr = TCPS_ERR_CONNECT_FAILED;
				return false;
			}

			iscm_IRequester* callbackRequester = autoRequester.face_;
			VERIFY(TCPS_OK == callbackRequester->AttachSocket(cbSock, true));

			// 若应用程序没有调用过SetTimeout()，则此处使用有限时间超时（ISCM对同步RPC调用默认使用超长时间超时，见'iscm_requester.cpp'）
			recvTimeout = (INFINITE==recvTimeout ? 2000 : recvTimeout);
			sendTimeout = (INFINITE==sendTimeout ? 1000 : sendTimeout);
			callbackRequester->SetTimeout(INFINITE, recvTimeout, sendTimeout);

			SockWriteBuf swb[2];
			swb[0].data = &bindID;
			swb[0].len = sizeof(bindID);
			swb[1].data = &peerSessionKey;
			swb[1].len = sizeof(peerSessionKey);
			tcps_Binary reply;
			TCPSError err = callbackRequester->Call(RPCCT_TO_CALLBACK, swb, ARRAY_LEN(swb), reply);
			if(TCPS_OK != err)
			{
				if(actionErr)
					*actionErr = err;
				return false;
			}
			ASSERT(reply.IsEmpty());

			VERIFY(cbSock == callbackRequester->DropSocket());
			face_.m_callbackProxy.IncreaseCallbackRef();
			iscm_FetchCallbackServeMan().AddCallbackClient(&face_.m_callbackProxy, cbSock);
		}
#endif

#if 1
		// 建立posting通道
		if(0 == (face_.m_sessionFlags&ISCMC_NO_POSTING_CHANNEL)
			&& 0 == (face_.m_sessionFlags&ISCMC_UDP_POSTING)
			)
		{
			// 连接posting通道，使用直接连接方式
			SOCKET postingSock = INVALID_SOCKET;

			AutoDeleteInterface<iscm_IRequester> autoRequester;
			autoRequester.face_ = iscm_MakeRequester(false);
			if(NULL == autoRequester.face_)
			{
				// 容错处理
				ASSERT(false);
				if(actionErr)
					*actionErr = TCPS_ERR_OUT_OF_MEMORY;
				return false;
			}

			DWORD connectTimeout = INFINITE;
			DWORD recvTimeout = INFINITE;
			DWORD sendTimeout = INFINITE;
			face_.m_rpcRequester->GetTimeout(&connectTimeout, &recvTimeout, &sendTimeout);
			DWORD const timeoutMSELs = (INFINITE==connectTimeout ? iscm_GetDefaultConnectTimeout() : connectTimeout);
			INT32 peerSessionKey;
			INT64 const bindID = face_.m_rpcRequester->GetPeerSessionKey(peerSessionKey);
			ASSERT(0!=bindID && 0!=peerSessionKey);

			IPP const serveIPP = face_.m_rpcRequester->GetServingIPP();
			ASSERT(serveIPP.IsValid());
			BOOL is_AF_LOCAL = tcps_Use_AF_LOCAL(serveIPP.ip_);
			postingSock = TCPNewClientByTime(serveIPP.ip_, serveIPP.port_, timeoutMSELs, is_AF_LOCAL);
			if(!SockValid(postingSock))
			{
				if(actionErr)
					*actionErr = TCPS_ERR_CONNECT_FAILED;
				return false;
			}

			iscm_IRequester* postingRequester = autoRequester.face_;
			VERIFY(TCPS_OK == postingRequester->AttachSocket(postingSock, true));

			// 若应用程序没有调用过SetTimeout()，则此处使用有限时间超时（ISCM对同步RPC调用默认使用超长时间超时，见'iscm_requester.cpp'）
			recvTimeout = (INFINITE==recvTimeout ? 2000 : recvTimeout);
			sendTimeout = (INFINITE==sendTimeout ? 1000 : sendTimeout);
			postingRequester->SetTimeout(INFINITE, recvTimeout, sendTimeout);

			SockWriteBuf swb[2];
			swb[0].data = &bindID;
			swb[0].len = sizeof(bindID);
			swb[1].data = &peerSessionKey;
			swb[1].len = sizeof(peerSessionKey);
			tcps_Binary reply;
			TCPSError err = postingRequester->Call(RPCCT_TO_POSTING_CHANNEL, swb, ARRAY_LEN(swb), reply);
			/*if(TCPS_OK != err)
			{
				if(TCPS_ERR_NOT_SUPPORTED != err)
				{
					if(actionErr)
						*actionErr = err;
					return false;
				}
			}*/
			ASSERT(reply.IsEmpty());

			if(TCPS_OK == err)
			{
				VERIFY(postingSock == postingRequester->DropSocket());
				iscm_FetchPostingCallerMan().AddSession(face_.m_postingProxy.callerKey_, postingSock, true, &face_.m_postingProxy);
			}
		}
#endif
	}
	else if(iscm_IRequester::et_broken==ev || iscm_IRequester::et_dropped==ev)
	{
		// 总尝试删除一次
#if 1
		iscm_FetchCallbackServeMan().DelCallbackClient(&face_.m_callbackProxy);
#endif
#if 1
		if(0 != face_.m_postingProxy.callerKey_)
			iscm_FetchPostingCallerMan().CloseSession(face_.m_postingProxy.callerKey_);
#endif
	}

	if(iscm_IRequester::et_connected==ev || iscm_IRequester::et_attached==ev)
	{
		ASSERT(!face_.m_UDPCallbackProxy.isUsing_);
		while(ISCMC_UDP_POSTING&face_.m_sessionFlags)
		{
			AutoSock udpSock;
			udpSock.sock_ = UDPNew();
			if(!SockValid(udpSock.sock_))
				break;
			IPP localIPP = SockGetLocal(face_.m_rpcRequester->GetSocket());
			if(!localIPP.IsValid())
				break;
			if(TCPS_OK != tcps_AutoBindUDPPort(udpSock.sock_, localIPP.ip_, &localIPP.port_))
				break;

			INT32 serveUDPPort = -1;
			INT32 udpLinkKey = 0;
			TCPSError err = face_.UDPLink_(serveUDPPort, udpLinkKey);
			if(TCPS_OK == err)
			{
				ASSERT(0 != udpLinkKey);
				IPP peerIPP = face_.GetServingIPP();
				if(0 != UDPConnect(udpSock.sock_, peerIPP.ip_, serveUDPPort))
					break;
				iscm_UDPFrag linkFrag;
				linkFrag.fragType = ISCM_UDP_FRAG_LINK;
				linkFrag.dummyI32 = 0;
				linkFrag.sendKey = udpLinkKey;
				linkFrag.fragNum = 0;
				int r = SockWrite(udpSock.sock_, &linkFrag, sizeof(linkFrag));
				if((int)sizeof(linkFrag) != r)
					break;
				err = face_.UDPLinkConfirm_();
				if(TCPS_OK != err)
					break;
				INT32 sessionID;
				face_.m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.AddSession(sessionID, udpSock.Drop(), &face_.m_UDPCallbackProxy);
				face_.m_UDPCallbackProxy.isUsing_ = true;
			}
			break;
		}
		// 忽略UDP连接错误，若UDP连接失败，则只使用TCP

		DWORD recvTimeout = INFINITE;
		DWORD sendTimeout = INFINITE;
		face_.GetTimeout(NULL, &recvTimeout, &sendTimeout);
		if(INFINITE!=recvTimeout || INFINITE!=sendTimeout)
			face_.SetTimeout_(recvTimeout, sendTimeout);

		INT32 recvBufBytes = -1;
		INT32 sendBufBytes = -1;
		face_.GetSessionBufferSize(&recvBufBytes, &sendBufBytes);
		if(recvBufBytes>=0 || sendBufBytes>=0)
			face_.SetSessionBufferSize_(recvBufBytes, sendBufBytes);
		if((0 == (face_.m_sessionFlags&ISCMC_NO_CALLBACK)) && (recvBufBytes>=0 || sendBufBytes>=0))
		{
			iscm_ICallbackServeMan& man = iscm_FetchCallbackServeMan();
			man.SetSessionBufferSize(&face_.m_callbackProxy, recvBufBytes, sendBufBytes);
		}

		ASSERT(!face_.m_needBrokenNotify);
		face_.m_needBrokenNotify = true;
		TCPSError err = TCPS_ERROR;

		// 此处对m_asyncTaskStat.totalAsyncTasks提前加一，以防止如下情况发生：
		// 当用户程序在OnConnected()中异步投递了删除本Client对象的任务，且返回非TCPS_OK；
		// 此时可能OnConnectedFailedTask_::OnPoolTask()被执行时，本Client对象已经被析构，
		// 导致非法内存错误。
		VERIFY(InterlockedIncrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 1);

		{
			iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(face_.m_callingOutTIDs, iscm_cct_OnConnected);
			try
			{
				err = face_.OnConnected();
			}
			catch(TCPSError ret)
			{
				ASSERT(TCPS_OK != ret);
				err = ret;
			}
			catch(int ret)
			{
				ASSERT(TCPS_OK != ret);
				err = (TCPSError)ret;
			}
			catch(std::bad_alloc)
			{
				err = TCPS_ERR_OUT_OF_MEMORY;
			}
			ISCM_BEGIN_CATCH_ALL_()
				err = TCPS_ERR_UNKNOWN_EXCEPTION;
			ISCM_END_CATCH_ALL_()
		}

		if(TCPS_OK!=err && 0==(ISCMC_NO_ASYNC_RECONNECT&face_.m_sessionFlags))
		{
			// 在OnConnected()之前引用计数已经加一，此处不能再加
		//	VERIFY(InterlockedIncrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 1);
			OnConnectedFailedTask_* task = tcps_NewEx(OnConnectedFailedTask_, (face_));
			FetchTaskPool()->Push(task);
		}
		else
		{
			// OnConnected()返回成功，对在OnConnected()之前的引用计数加一做对称处理
			VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		}
		if(actionErr)
			*actionErr = err;
	}
	else if(iscm_IRequester::et_connect_timeout == ev)
	{
		iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(face_.m_callingOutTIDs, iscm_cct_OnConnectTimeout);
		ISCM_BEGIN_TRY_()
			face_.OnConnectTimeout(cause);
		ISCM_END_TRY_()
		ISCM_BEGIN_CATCH_ALL_()
		ISCM_END_CATCH_ALL_()
	}
	else
	{
		ASSERT(iscm_IRequester::et_broken==ev || iscm_IRequester::et_dropped==ev);
		if(0 != face_.m_postingProxy.callerKey_)
			iscm_FetchPostingCallerMan().CloseSession(face_.m_postingProxy.callerKey_);
		ASSERT(face_.m_callbackProxy.callbackRefCount_ >= 0);
		while(face_.m_callbackProxy.callbackRefCount_ > 0)
			Sleep(1);
		while(0 != face_.m_postingProxy.callerKey_)
			Sleep(1);
		while(face_.m_asyncTaskStat.postingCallings > 0)
			Sleep(1);
		ASSERT(0==face_.m_asyncTaskStat.postingCallingBytes && 0==face_.m_asyncTaskStat.postingCallings);
		if(face_.m_needBrokenNotify)
		{
			face_.m_needBrokenNotify = false;
			{
				iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(face_.m_callingOutTIDs, iscm_cct_OnClose);
				ISCM_BEGIN_TRY_()
					if(face_.m_owner && (INVALID_OSTHREADID==face_.m_owner->GetClosingTID_() || OSThreadSelf()!=face_.m_owner->GetClosingTID_()))
						face_.OnPeerBroken();
					face_.OnClose();
				ISCM_END_TRY_()
				ISCM_BEGIN_CATCH_ALL_()
				ISCM_END_CATCH_ALL_()
			}

			if(face_.m_UDPCallbackProxy.isUsing_)
			{
				face_.m_UDPCallbackProxy.isUsing_ = false;
				INT32 sessionID;
				face_.m_rpcRequester->GetPeerSessionKey(sessionID);
				iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
				udpServer.CloseSession(sessionID);
			}
		}
	}
	face_.m_methodMatchingUpdatedFlag.needUpdate = true;

	return true;
}

const GRID_User_RC::CallbackMap_* GRID_User_RC::sm_callbackMap = NULL;
GRID_User_RC::CallbackMap_& GRID_User_RC::GetCallbackMap_()
	{	LOCAL_SAFE_STATIC_OBJ(CallbackMap_);	}

void GRID_User_RC::InitCallbackMap_()
{
	if(sm_callbackMap)
		return;
	IscmRPCGlobalLocker locker;
	if(sm_callbackMap)
		return;
	CallbackMap_& cbMap = GetCallbackMap_();
	ASSERT(0 == cbMap.size());
	VERIFY(cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::ReplyCommitJob"), CallbackSite_(&GRID_User_RC::Wrap_ReplyCommitJob, true))).second);
	VERIFY(cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::SetRedirect_"), CallbackSite_(&GRID_User_RC::Wrap_SetRedirect_, true))).second);
	VERIFY(cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GRID_User::CallbackCheck_"), CallbackSite_(&GRID_User_RC::Wrap_CallbackCheck_, false))).second);
	sm_callbackMap = &cbMap;
}

GRID_User_RC::GRID_User_RC(GRID_User* owner /*= NULL*/)
	: m_owner(owner)
	, m_sessionFlags(0)
	, m_reconnectIntervalMSELs(2000)
	, m_rpcRequester(iscm_MakeRequester(false))
	, m_needBrokenNotify(false)
	, m_prepareDisconnect(false)
	, m_callbackProxy(*this)
	, m_postingProxy(*this)
	, m_networkHandler(*this)
	, m_UDPCallbackProxy(*this)
{
	STATIC_ASSERT(sizeof(INT32) == sizeof(((tcps_String*)64)->LenRef()));
	STATIC_ASSERT(sizeof(INT32) == sizeof(((tcps_Binary*)64)->LenRef()));
	STATIC_ASSERT(sizeof(INT32) == sizeof(((tcps_Array<int>*)64)->LenRef()));
	STATIC_ASSERT(sizeof(INT32) == sizeof(((CPtrStrA*)64)->len));

	m_rpcRequester->SetEventHandler(&m_networkHandler);
	iscm_InitClientSingletons(true, true);
	InitCallbackMap_();
}

GRID_User_RC::~GRID_User_RC()
{
	DestroyRequester();
}

void GRID_User_RC::DestroyRequester()
{
	if(NULL == m_rpcRequester)
		return;
	NPR_ASSERT_EX(!m_callingOutTIDs.HasFlag(OSThreadSelf(), NULL), "禁止在对象自己的调用里删除自身");
	PrepareDisconnect();
	while(0 != m_postingProxy.callerKey_)
		Sleep(1);
	ASSERT(m_callbackProxy.callbackRefCount_ >= 0);
	while(m_callbackProxy.callbackRefCount_ > 0)
		Sleep(1);
	ASSERT(m_asyncTaskStat.postingCallingBytes>=0 && m_asyncTaskStat.postingCallings>=0 && m_asyncTaskStat.totalAsyncTasks>=0);
	while(m_asyncTaskStat.totalAsyncTasks > 0)
		Sleep(1);
	ASSERT(0==m_asyncTaskStat.postingCallingBytes && 0==m_asyncTaskStat.postingCallings);
	m_rpcRequester->DeleteThis();
	m_rpcRequester = NULL;
}

TCPSError GRID_User_RC::SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags /*= 0*/,
				IN const IPP& clientID_IPP /*= INVALID_IPP*/,
				IN const IPP& secondaryServeIPP /*= INVALID_IPP*/,
				IN const IPP& tertiaryServeIPP /*= INVALID_IPP*/,
				IN DWORD reconnectIntervalMSELs /*= 2000*/
				)
{
	NPR_ASSERT_EX(!m_callingOutTIDs.HasFlag(OSThreadSelf(), NULL), "禁止在对象自己的调用里调用此函数");
	m_sessionFlags = sessionFlags;
	m_sessionFlags &= ~ISCMC_NO_CALLBACK; // 本接口支持重定向，总启动回调（忽略ISCMC_NO_CALLBACK标志）
	m_reconnectIntervalMSELs = max((DWORD)10, reconnectIntervalMSELs);
	DWORD t1, t2, t3;
	m_rpcRequester->GetTimeout(&t1, &t2, &t3, NULL);
	m_rpcRequester->SetTimeout(t1, t2, t2, m_reconnectIntervalMSELs);
	iscm_IRequester::ConnectFlag cflag;
	if(ISCMC_ASYNC_CONNECT&sessionFlags)
		cflag = iscm_IRequester::cflag_async_connect;
	else if(ISCMC_NO_ASYNC_RECONNECT&sessionFlags)
		cflag = iscm_IRequester::cflag_wait_no_reconnect;
	else
		cflag = iscm_IRequester::cflag_wait_async_reconnect;
	TCPSError err = m_rpcRequester->SetServeIPP(serveIPP, "GRID_User", cflag, clientID_IPP, 0, secondaryServeIPP, tertiaryServeIPP);
	if(serveIPP.IsValid())
		m_prepareDisconnect = false;
	return err;
}

IPP GRID_User_RC::GetServeIPP() const
	{	return m_rpcRequester->GetServeIPP();	}

IPP GRID_User_RC::GetServingIPP() const
	{	return m_rpcRequester->GetServingIPP();	}

BOOL GRID_User_RC::IsConnected() const
	{	return m_rpcRequester->IsConnected();	}

void GRID_User_RC::SetKeeping(
			IN BOOL keeping
			)
	{	m_rpcRequester->SetKeeping(keeping);	}

BOOL GRID_User_RC::IsKeeping() const
	{	return m_rpcRequester->IsKeeping();	}

void GRID_User_RC::PrepareDisconnect()
{
	m_prepareDisconnect = true;
	m_rpcRequester->PrepareDisconnect();
	if(0 != m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().CloseSession(m_postingProxy.callerKey_);
	iscm_FetchCallbackServeMan().DelCallbackClient(&m_callbackProxy);
	if(m_UDPCallbackProxy.isUsing_)
	{
		m_UDPCallbackProxy.isUsing_ = false;
		INT32 sessionID;
		m_rpcRequester->GetPeerSessionKey(sessionID);
		iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
		udpServer.CloseSession(sessionID, true);
	}
}

void GRID_User_RC::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	m_rpcRequester->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	if(IsConnected())
		SetTimeout_(recvTimeout, sendTimeout);
}

void GRID_User_RC::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const
	{	m_rpcRequester->GetTimeout(connectTimeout, recvTimeout, sendTimeout);	}

void GRID_User_RC::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	if(recvBufBytes<0 && sendBufBytes<0)
		return;
	m_rpcRequester->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	if(0 == (m_sessionFlags&ISCMC_NO_CALLBACK))
	{
		iscm_ICallbackServeMan& man = iscm_FetchCallbackServeMan();
		man.SetSessionBufferSize(&m_callbackProxy, recvBufBytes, sendBufBytes);
	}
	if(IsConnected())
		SetSessionBufferSize_(recvBufBytes, sendBufBytes);
}

void GRID_User_RC::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
	{	m_rpcRequester->GetSessionBufferSize(recvBufBytes, sendBufBytes);	}

void GRID_User_RC::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
	{	m_postingPendingParam.Set(maxPendingBytes, maxPendings);	}

void GRID_User_RC::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
	{	m_postingPendingParam.Get(maxPendingBytes, maxPendings);	}

void GRID_User_RC::OnPrepareRPCCallback_(
				IN const void* inParamsData,
				IN INT_PTR inParamsDataLen,
				OUT BOOL& notifyByTaskPool /*= true*/
				)
{
#if 1
	// 所有调用都用taskPool异步方式，以保证不同会话的并行调用
	ASSERT(inParamsData && inParamsDataLen>0 && notifyByTaskPool);
	(void)inParamsData; (void)inParamsDataLen; (void)notifyByTaskPool;
#else
	ASSERT(inParamsData && inParamsDataLen>0 && notifyByTaskPool);
	const BYTE* ptrInParams = (const BYTE*)inParamsData;
	INT_PTR ptrInParamsLen = inParamsDataLen;

	if(ptrInParamsLen < (int)sizeof(iscm_PeerCallFlags))
		return;
	iscm_PeerCallFlags peerCallFlags = *(iscm_PeerCallFlags*)ptrInParams;
	if((int)sizeof(BOOL) != peerCallFlags.sizeofBOOL_req)
		return;
	if((int)sizeof(DummyEnumType) != peerCallFlags.sizeofEnum_req)
		return;
	ptrInParams += sizeof(iscm_PeerCallFlags);
	ptrInParamsLen -= sizeof(iscm_PeerCallFlags);

	if(ptrInParamsLen < (int)sizeof(INT32))
		return;
	INT32 call_id_len = *(INT32*)ptrInParams;
	ptrInParams += sizeof(INT32);
	ptrInParamsLen -= sizeof(INT32);
	if(ptrInParamsLen < call_id_len+1)
		return;
	const char* call_id = (const char*)ptrInParams;
	ptrInParams += call_id_len+1;
	ptrInParamsLen -= call_id_len+1;

	ASSERT(sm_callbackMap);
	CallbackMap_::const_iterator cit = sm_callbackMap->find(call_id, call_id_len);
	if(sm_callbackMap->end() == cit)
		return;

	if(cit->second.isPosting)
		notifyByTaskPool = false;
#endif
}

TCPSError GRID_User_RC::OnRPCCallback_(
				OUT BOOL& requireReplyData /*= true*/,
				IN const void* inParamsData,
				OUT BOOL& dataTransferred /*= false*/,
				IN INT_PTR inParamsDataLen,
				OUT iscm_IRPCOutfiter* outfiter
				)
{
	(void)inParamsDataLen; // avoid warning.
	ASSERT(requireReplyData && inParamsData && inParamsDataLen>0/* && outfiter*/);
	const BYTE* ptrInParams = (const BYTE*)inParamsData;
	INT_PTR ptrInParamsLen = inParamsDataLen;

	if(ptrInParamsLen < (int)sizeof(iscm_PeerCallFlags))
		return TCPS_ERR_MALFORMED_REQ;
	iscm_PeerCallFlags peerCallFlags = *(iscm_PeerCallFlags*)ptrInParams;
	if((int)sizeof(BOOL) != peerCallFlags.sizeofBOOL_req)
		return TCPS_ERR_MALFORMED_REQ;
	if((int)sizeof(DummyEnumType) != peerCallFlags.sizeofEnum_req)
		return TCPS_ERR_MALFORMED_REQ;
	ptrInParams += sizeof(iscm_PeerCallFlags);
	ptrInParamsLen -= sizeof(iscm_PeerCallFlags);

	if(ptrInParamsLen < (int)sizeof(INT32))
		return TCPS_ERR_MALFORMED_REQ;
	INT32 call_id_len = *(INT32*)ptrInParams;
	ptrInParams += sizeof(INT32);
	ptrInParamsLen -= sizeof(INT32);
	if(ptrInParamsLen < call_id_len+1)
		return TCPS_ERR_MALFORMED_REQ;
	const char* call_id = (const char*)ptrInParams;
	ptrInParams += call_id_len+1;
	ptrInParamsLen -= call_id_len+1;

	ASSERT(sm_callbackMap);
	CallbackMap_::const_iterator cit = sm_callbackMap->find(call_id, call_id_len);
	if(sm_callbackMap->end() == cit)
		return TCPS_ERR_TRY_CALL_OTHER_METHODS; // 本接口不支持，尝试其它接口

	CPtrStrA const streamedCallID(call_id+CONST_STR_LEN("GRID_User::"), call_id_len-CONST_STR_LEN("GRID_User::"));

	// posting回调
	if(cit->second.isPosting)
	{
		if(m_prepareDisconnect)
			return TCPS_ERR_EXITING;

		// 当缓存的未决调用小于1个时，直接调用
		if(m_postingPendingParam.maxPendings_ <= 1)
		{
			CNPAutoLock locker(m_callbackCallLock);
			TCPSError err = TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
			if(m_owner)
			{
				void* replyData = NULL;
				INT_PTR replyLen = 0;
				err = m_owner->StreamedCallback_(NULL, 0, NULL, 0, replyData, replyLen);
				if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
				{
					if(!m_owner->IsStreamedCallbackMatched_(streamedCallID.p, streamedCallID.len))
						return TCPS_ERR_CALLBACK_NOT_MATCH;

					{
						iscm_ClientCallType cct = (cit->second.isPosting ? iscm_cct_posting_callback : iscm_cct_callback);
						iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(m_callingOutTIDs, cct);
						err = m_owner->StreamedCallback_(streamedCallID.p, streamedCallID.len, ptrInParams, ptrInParamsLen, replyData, replyLen);
					}
					ASSERT(NULL==replyData && 0==replyLen);
					if(TCPS_OK == err)
					{
						ptrInParams += ptrInParamsLen;
						ptrInParamsLen = 0;
					}
					else if(TCPS_ERR_MALFORMED_REQ == err)
					{
						this->OnNetworkMalformed_();
						return TCPS_ERR_MALFORMED_REQ;
					}
				}
			}
			if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED == err)
				err = cit->second.handler(this, NULL, peerCallFlags, ptrInParams, ptrInParamsLen, NULL/*outfiter*/);
#ifdef _DEBUG
			if(TCPS_OK == err)
			{
				ASSERT(ptrInParams-(const BYTE*)inParamsData == inParamsDataLen);
				ASSERT(0 == ptrInParamsLen);
			}
#else
			(void)err;
#endif
			ASSERT(!peerCallFlags.requireReply);
			requireReplyData = false;
			ASSERT(!dataTransferred);
			return TCPS_OK;
		}

		// 投递一个task，异步执行
		if(m_asyncTaskStat.postingCallingBytes >= m_postingPendingParam.maxPendingBytes_
			|| m_asyncTaskStat.postingCallings >= m_postingPendingParam.maxPendings_
			)
		{
			// 对内置的SetRedirect_()回调，不做限制
			if("GRID_User::SetRedirect_" != cit->first)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}

		// 投递一个异步回调任务，或加入待执行队列
		PostingTask_* task = tcps_NewEx(PostingTask_, (*this));
		task->peerCallFlags_ = peerCallFlags;
		task->baseInParamsData_ = (void*)inParamsData;
		task->call_id_ = streamedCallID;
		task->inParamsData_ = ptrInParams;
		task->inParamsDataLen_ = ptrInParamsLen;
		task->handler_ = cit->second.handler;
		VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) >= 1);
		{
			CNPAutoLock locker(m_postingCallbacks.lock);
			m_asyncTaskStat.postingCallingBytes += ptrInParamsLen;
			++(m_asyncTaskStat.postingCallings);
			if(m_postingCallbacks.calling)
			{
				task->m_nextPoolTask = NULL;
				if(m_postingCallbacks.tail)
				{
					m_postingCallbacks.tail->m_nextPoolTask = task;
					m_postingCallbacks.tail = task;
				}
				else
					m_postingCallbacks.head = m_postingCallbacks.tail = task;
			}
			else
			{
				ASSERT(NULL==m_postingCallbacks.head && NULL==m_postingCallbacks.tail);
				ASSERT(ptrInParamsLen==m_asyncTaskStat.postingCallingBytes && 1==m_asyncTaskStat.postingCallings);
				m_postingCallbacks.calling = true;
				FetchTaskPool()->Push(task);
			}
		}

		if(peerCallFlags.requireReply)
		{
			// 给outfiter填充内置返回参数
			struct PostOutParamWrapSite
			{
				iscm_RPCReplyPrefix replyPrefix;
				PostOutParamWrapSite() { replyPrefix.Init(); }
				static void FreeAction(void* p)
				{
					PostOutParamWrapSite* ptr = (PostOutParamWrapSite*)((BYTE*)p - offset_of(PostOutParamWrapSite, replyPrefix));
					ptr->~PostOutParamWrapSite();
					tcps_Free(ptr);
				}
			};
			PostOutParamWrapSite* opWrapper = tcps_New(PostOutParamWrapSite);
			Set_BaseType_(outfiter, opWrapper->replyPrefix, PostOutParamWrapSite::FreeAction);
		}
		else
		{
			// 指示不需要发送回应数据了
			requireReplyData = false;
		}

		dataTransferred = true; // 指示输入参数数据指针inParamsData被转移
		return TCPS_OK; // 立即返回成功
	}

	// 非posting方法/回调
	TCPSError err = TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
	{
		CNPAutoLock locker(m_callbackCallLock);
		if(m_owner)
		{
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			err = m_owner->StreamedCallback_(NULL, 0, NULL, 0, replyData, replyLen);
			if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
			{
				if("CallbackCheck_" == streamedCallID)
				{
					// CallbackCheck_()在流式调用端不提供，必须调用C++的映射版本
					err = TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED;
				}
				else
				{
					if(!m_owner->IsStreamedCallbackMatched_(streamedCallID.p, streamedCallID.len))
						return TCPS_ERR_CALLBACK_NOT_MATCH;

					{
						iscm_ClientCallType cct = (cit->second.isPosting ? iscm_cct_posting_callback : iscm_cct_callback);
						iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(m_callingOutTIDs, cct);
						err = m_owner->StreamedCallback_(streamedCallID.p, streamedCallID.len, ptrInParams, ptrInParamsLen, replyData, replyLen);
					}
					if(TCPS_OK == err)
					{
						ptrInParams += ptrInParamsLen;
						ptrInParamsLen = 0;
						iscm_RPCReplyPrefix* replyPrefix = (iscm_RPCReplyPrefix*)tcps_Alloc(sizeof(iscm_RPCReplyPrefix));
						replyPrefix->Init();
						outfiter->Push(replyPrefix, sizeof(iscm_RPCReplyPrefix), true, NULL);
						if(replyLen > 0)
						{
							ASSERT(replyData && outfiter);
							outfiter->Push(replyData, replyLen, true, NULL);
						}
						else
							ASSERT(NULL == replyData);
					}
					else
					{
						ASSERT(NULL==replyData && 0==replyLen);
						if(TCPS_ERR_MALFORMED_REQ == err)
						{
							this->OnNetworkMalformed_();
							return TCPS_ERR_MALFORMED_REQ;
						}
					}
				}
			}
		}
		if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED == err)
			err = cit->second.handler(this, NULL, peerCallFlags, ptrInParams, ptrInParamsLen, outfiter);
	}
#ifdef _DEBUG
	if(TCPS_OK == err)
	{
		ASSERT(ptrInParams-(const BYTE*)inParamsData == inParamsDataLen);
		ASSERT(0 == ptrInParamsLen);
	}
#endif
	return err;
}

struct GRID_User_RC::StreamedCall_Task_ : public NPBaseTask
{
	GRID_User_RC& face_;
	tcps_String methodName_;
	tcps_Binary data_;
	explicit StreamedCall_Task_(
				GRID_User_RC& face,
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/
				)
		: face_(face)
	{
		methodName_.Assign(methodName, nameLen);
		data_.Assign(data, dataLen);
	}
	virtual void OnPoolTask()
	{
		if(!face_.m_prepareDisconnect)
		{
			SockWriteBuf buf;
			buf.data = data_.Get();
			buf.len = data_.Length();
			face_.StreamedCall_(
						methodName_.Get(), methodName_.Length(),
						&buf, 1,
						NULL, NULL
						);
		}
		VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		tcps_Delete(this);
	}
};
TCPSError GRID_User_RC::StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;
	if(NULL==methodName || 0==nameLen)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if(nameLen < 0)
		nameLen = strlen(methodName);

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	const GRID_User::MethodMatchingFlag& methodMatchingFlag = GetMethodMatchingFlag();
	GRID_User::MethodMatchingFlag::MethodMap::const_iterator itMethod = methodMatchingFlag.mmap_.find(methodName, nameLen);
	if(methodMatchingFlag.mmap_.end() == itMethod)
		return TCPS_ERR_METHOD_NOT_MATCH;
	ASSERT(itMethod->second.pMatchingVar);
	if(!*(itMethod->second.pMatchingVar))
		return TCPS_ERR_METHOD_NOT_MATCH;

	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			if(itMethod->second.isPosting)
			{
				StreamedCall_Task_* task = tcps_NewEx(StreamedCall_Task_, (
								*this,
								methodName, nameLen,
								data, dataLen
								));
				VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) > 0);
				FetchTaskPool()->Push(task);
			}
			else
				NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = !(itMethod->second.isPosting);
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = (INT32)(11+nameLen);
	CSmartBuf<char, 256> call_id_buf(call_id_len+1);
	strncpy(call_id_buf.Get(), "GRID_User::", 11);
	StrAssign(call_id_buf.Get()+11, methodName, nameLen);
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push(call_id_buf.Get(), call_id_len+1);

	// 填充IN参数到outfiter
	if(dataLen > 0)
		m_dataOutfiter.Push(data, dataLen);

	// 调用RPCCall()
	if(peerCallFlags.requireReply) // method
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_RPCReplyPrefix replyPrefix;
		err = m_rpcRequester->RecvD(&replyPrefix, sizeof(replyPrefix));
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}

		INT_PTR leftReplyLen = replyBytes - sizeof(replyPrefix);
		ASSERT(leftReplyLen >= 0);
		if(leftReplyLen > 0)
		{
			ASSERT(replyData && replyLen);
			tcps_Binary replied_data;
			if(!replied_data.Resize(leftReplyLen))
			{
				this->OnNetworkMalformed_();
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			err = m_rpcRequester->RecvD(replied_data.Get(), (int)leftReplyLen);
			if(TCPS_OK != err)
			{
				// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
				return err;
			}
			if(NULL==replyData || NULL==replyLen)
			{
				ASSERT(false);
				return TCPS_ERR_INVALID_PARAM;
			}
			INT32 bytes = 0;
			*replyData = replied_data.Drop(&bytes);
			*replyLen = bytes;
		}
	}
	else // posting method
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_UDPCallbackProxy.isUsing_)
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID;
			m_rpcRequester->GetPeerSessionKey(sessionID);
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

void GRID_User_RC::UDPCallbackProxy_::OnUDPCall(
				IN tcps_Binary& frame
				)
{
	ASSERT(frame.Length() > 0);
	while(true)
	{
		BOOL requireReplyData = true;
		const void* inParamsData = frame.Get();
		BOOL dataTransferred = false;
		INT_PTR inParamsDataLen = frame.Length();
		TCPSError err = face_.OnRPCCallback_(requireReplyData, inParamsData, dataTransferred, inParamsDataLen, NULL/*outfiter*/);
		ASSERT(TCPS_OK!=err || !requireReplyData);
		if(TCPS_ERR_POSTING_PENDING_FULL == err)
		{
			ASSERT(!dataTransferred);
			Sleep(1);
			continue;
		}
		else
		{
			if(dataTransferred)
				frame.Drop();
			break;
		}
	}
}

TCPSError GRID_User_RC::AddJobProgram(
				IN const GRID_ProgramInfo& programInfo_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_AddJobProgram)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 24;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::AddJobProgram", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_ProgramInfo programInfo
			Put_String_(&m_dataOutfiter, programInfo_wrap.programID.name.Get(), programInfo_wrap.programID.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, programInfo_wrap.programID.ver);
			Put_BaseType_(&m_dataOutfiter, programInfo_wrap.programID.cpuType);
			Put_BaseType_(&m_dataOutfiter, programInfo_wrap.programID.osType);
			Put_BaseType_(&m_dataOutfiter, programInfo_wrap.programID.executeBits);
			Put_BaseType_(&m_dataOutfiter, programInfo_wrap.programID.binaryType);
		Put_String_(&m_dataOutfiter, programInfo_wrap.description.Get(), programInfo_wrap.description.LenRef());

	// IN tcps_Array<GRID_ProgramFile> files
	Put_BaseType_(&m_dataOutfiter, files_wrap.LenRef());
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		const GRID_ProgramFile& ref1 = files_wrap[idx1];
			Put_BaseType_(&m_dataOutfiter, ref1.isExecutable);
			Put_String_(&m_dataOutfiter, ref1.name.Get(), ref1.name.LenRef());
			Put_Binary_(&m_dataOutfiter, ref1.data.Get(), ref1.data.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::AddJobProgram() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::DelJobProgram(
				IN const tcps_Array<INT64>& programKeys_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_DelJobProgram)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 24;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::DelJobProgram", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<INT64> programKeys
	Put_PODArray_<DataOutfiter, INT64>(&m_dataOutfiter, programKeys_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::DelJobProgram() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::FindJobProgram(
				IN const GRID_ProgramID& programID_wrap,
				OUT BOOL& found_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_FindJobProgram)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::FindJobProgram", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_ProgramID programID
		Put_String_(&m_dataOutfiter, programID_wrap.name.Get(), programID_wrap.name.LenRef());
		Put_BaseType_(&m_dataOutfiter, programID_wrap.ver);
		Put_BaseType_(&m_dataOutfiter, programID_wrap.cpuType);
		Put_BaseType_(&m_dataOutfiter, programID_wrap.osType);
		Put_BaseType_(&m_dataOutfiter, programID_wrap.executeBits);
		Put_BaseType_(&m_dataOutfiter, programID_wrap.binaryType);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT BOOL found
		PICK_BASETYPE_Q(iscm_replied_picker, found_wrap);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::FindJobProgram() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListJobPrograms)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 26;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListJobPrograms", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<JobProgram> jobPrograms
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		jobPrograms_wrap.Resize(array_len);
		for(int idx2=0; idx2<jobPrograms_wrap.Length(); ++idx2)
		{
			GRID_User_T::JobProgram& ref2 = jobPrograms_wrap[idx2];
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.programKey);
						PICK_STRING_Q(iscm_replied_picker, ref2.programInfo.programID.name);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.programInfo.programID.ver);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.programInfo.programID.cpuType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.programInfo.programID.osType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.programInfo.programID.executeBits);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.programInfo.programID.binaryType);
					PICK_STRING_Q(iscm_replied_picker, ref2.programInfo.description);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.addTime);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.programSize);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListJobPrograms() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::CommitJob(
				OUT INT64& jobKey_wrap,
				IN const GRID_JobInfo& jobInfo_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_CommitJob)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 20;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::CommitJob", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_JobInfo jobInfo
			Put_String_(&m_dataOutfiter, jobInfo_wrap.programID.name.Get(), jobInfo_wrap.programID.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.programID.ver);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.programID.cpuType);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.programID.osType);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.programID.executeBits);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.programID.binaryType);
			Put_String_(&m_dataOutfiter, jobInfo_wrap.splitter.name.Get(), jobInfo_wrap.splitter.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.splitter.ver);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.splitter.cpuType);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.splitter.osType);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.splitter.executeBits);
			Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.splitter.binaryType);
		Put_String_(&m_dataOutfiter, jobInfo_wrap.dataInputUrl.Get(), jobInfo_wrap.dataInputUrl.LenRef());
		Put_String_(&m_dataOutfiter, jobInfo_wrap.dataOutputUrl.Get(), jobInfo_wrap.dataOutputUrl.LenRef());
		Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.priority);
		Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.jobTaskTimeout);
		Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.jobTaskMaxAttempts);
		Put_BaseType_(&m_dataOutfiter, jobInfo_wrap.skipFailedJobTaskPercent);
		Put_String_(&m_dataOutfiter, jobInfo_wrap.jobName.Get(), jobInfo_wrap.jobName.LenRef());
		Put_Binary_(&m_dataOutfiter, jobInfo_wrap.splitterParam.Get(), jobInfo_wrap.splitterParam.LenRef());
		Put_Binary_(&m_dataOutfiter, jobInfo_wrap.programParam.Get(), jobInfo_wrap.programParam.LenRef());

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT INT64 jobKey
		PICK_BASETYPE_Q(iscm_replied_picker, jobKey_wrap);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::CommitJob() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::Wrap_ReplyCommitJob(
				IN GRID_User_RC* thisObj /*= NULL*/,
				IN GRID_User* faceObj /*= NULL*/,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_callback
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN INT64 jobKey
	IN INT64 jobKey_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, jobKey_wrap);

	// IN TCPSError replyCode
	IN TCPSError replyCode_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, replyCode_wrap);

	// IN tcps_Binary context
	IN tcps_Binary context_wrap;
	GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, context_wrap);

	if(0 != ptrInParamsLen)
	{
		// NPLogError(("GRID_User::ReplyCommitJob() posting_callback, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的回调函数
	try
	{
		if(thisObj)
		{
			iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(thisObj->m_callingOutTIDs, iscm_cct_posting_callback);
			errTcps = thisObj->ReplyCommitJob(
				jobKey_wrap,
				replyCode_wrap,
				context_wrap
				);
		}
		else
		{
			ASSERT(faceObj);
			errTcps = faceObj->ReplyCommitJob(
				jobKey_wrap,
				replyCode_wrap,
				context_wrap
				);
		}
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

struct GRID_User_RC::CancelJob_Task_ : public NPBaseTask
{
	GRID_User_RC& face_;
	IN const tcps_Array<INT64> jobKeys_wrap_;
	explicit CancelJob_Task_(
				GRID_User_RC& face,
				IN const tcps_Array<INT64>& jobKeys_wrap
				)
		: face_(face)
		, jobKeys_wrap_(jobKeys_wrap)
		{}
	virtual void OnPoolTask()
	{
		if(!face_.m_prepareDisconnect)
		{
			face_.CancelJob(
						jobKeys_wrap_
						);
		}
		VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		tcps_Delete(this);
	}
};
TCPSError GRID_User_RC::CancelJob(
				IN const tcps_Array<INT64>& jobKeys_wrap
				) posting_method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			CancelJob_Task_* task = tcps_NewEx(CancelJob_Task_, (
							*this,
							jobKeys_wrap
							));
			VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) > 0);
			FetchTaskPool()->Push(task);
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_CancelJob)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 20;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::CancelJob", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<INT64> jobKeys
	Put_PODArray_<DataOutfiter, INT64>(&m_dataOutfiter, jobKeys_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_UDPCallbackProxy.isUsing_)
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID;
			m_rpcRequester->GetPeerSessionKey(sessionID);
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::SetJobPriority(
				IN INT64 jobKey_wrap,
				IN GRID_JobPriority priority_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_SetJobPriority)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::SetJobPriority", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT64 jobKey
	Put_BaseType_(&m_dataOutfiter, jobKey_wrap);

	// IN GRID_JobPriority priority
	Put_BaseType_(&m_dataOutfiter, priority_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::SetJobPriority() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListJobs(
				OUT tcps_Array<JobReport>& jobReports_wrap,
				IN INT32 jobState_wrap,
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListJobs)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 19;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListJobs", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT32 jobState
	Put_BaseType_(&m_dataOutfiter, jobState_wrap);

	// IN LTMSEL beginTime
	Put_BaseType_(&m_dataOutfiter, beginTime_wrap);

	// IN LTMSEL endTime
	Put_BaseType_(&m_dataOutfiter, endTime_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<JobReport> jobReports
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		jobReports_wrap.Resize(array_len);
		for(int idx2=0; idx2<jobReports_wrap.Length(); ++idx2)
		{
			GRID_User_T::JobReport& ref2 = jobReports_wrap[idx2];
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobKey);
						PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.programID.name);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.ver);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.cpuType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.osType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.executeBits);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.binaryType);
						PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.splitter.name);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.ver);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.cpuType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.osType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.executeBits);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.binaryType);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.dataInputUrl);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.dataOutputUrl);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.priority);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.jobTaskTimeout);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.jobTaskMaxAttempts);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.skipFailedJobTaskPercent);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.jobName);
					PICK_BINARY_Q(iscm_replied_picker, ref2.jobInfo.splitterParam);
					PICK_BINARY_Q(iscm_replied_picker, ref2.jobInfo.programParam);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobState);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.progress);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.costedMSELs);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.commitTime);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.errorCode);
				PICK_STRING_Q(iscm_replied_picker, ref2.causesOfError);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListJobs() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::QueryJobs(
				IN const tcps_Array<INT64>& jobKeys_wrap,
				OUT tcps_Array<JobReport>& jobReports_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_QueryJobs)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 20;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::QueryJobs", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<INT64> jobKeys
	Put_PODArray_<DataOutfiter, INT64>(&m_dataOutfiter, jobKeys_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<JobReport> jobReports
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		jobReports_wrap.Resize(array_len);
		for(int idx2=0; idx2<jobReports_wrap.Length(); ++idx2)
		{
			GRID_User_T::JobReport& ref2 = jobReports_wrap[idx2];
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobKey);
						PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.programID.name);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.ver);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.cpuType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.osType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.executeBits);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.programID.binaryType);
						PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.splitter.name);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.ver);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.cpuType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.osType);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.executeBits);
						PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.splitter.binaryType);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.dataInputUrl);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.dataOutputUrl);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.priority);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.jobTaskTimeout);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.jobTaskMaxAttempts);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobInfo.skipFailedJobTaskPercent);
					PICK_STRING_Q(iscm_replied_picker, ref2.jobInfo.jobName);
					PICK_BINARY_Q(iscm_replied_picker, ref2.jobInfo.splitterParam);
					PICK_BINARY_Q(iscm_replied_picker, ref2.jobInfo.programParam);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.jobState);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.progress);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.costedMSELs);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.commitTime);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.errorCode);
				PICK_STRING_Q(iscm_replied_picker, ref2.causesOfError);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::QueryJobs() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_UpdateGrid)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 21;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::UpdateGrid", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<GRID_ProgramFile> files
	Put_BaseType_(&m_dataOutfiter, files_wrap.LenRef());
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		const GRID_ProgramFile& ref1 = files_wrap[idx1];
			Put_BaseType_(&m_dataOutfiter, ref1.isExecutable);
			Put_String_(&m_dataOutfiter, ref1.name.Get(), ref1.name.LenRef());
			Put_Binary_(&m_dataOutfiter, ref1.data.Get(), ref1.data.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::UpdateGrid() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListModules)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListModules", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<GRID_ProgramInfo> modules
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		modules_wrap.Resize(array_len);
		for(int idx2=0; idx2<modules_wrap.Length(); ++idx2)
		{
			GRID_ProgramInfo& ref2 = modules_wrap[idx2];
					PICK_STRING_Q(iscm_replied_picker, ref2.programID.name);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.programID.ver);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.programID.cpuType);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.programID.osType);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.programID.executeBits);
					PICK_BASETYPE_Q(iscm_replied_picker, ref2.programID.binaryType);
				PICK_STRING_Q(iscm_replied_picker, ref2.description);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListModules() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::GetLogCount(
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap,
				OUT INT32& logCount_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_GetLogCount)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::GetLogCount", call_id_len+1);

	// 填充IN参数到outfiter

	// IN LTMSEL beginTime
	Put_BaseType_(&m_dataOutfiter, beginTime_wrap);

	// IN LTMSEL endTime
	Put_BaseType_(&m_dataOutfiter, endTime_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT INT32 logCount
		PICK_BASETYPE_Q(iscm_replied_picker, logCount_wrap);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::GetLogCount() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos_wrap,
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap,
				IN INT32 startPos_wrap,
				IN INT32 length_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_LoadLogInfos)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 23;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::LoadLogInfos", call_id_len+1);

	// 填充IN参数到outfiter

	// IN LTMSEL beginTime
	Put_BaseType_(&m_dataOutfiter, beginTime_wrap);

	// IN LTMSEL endTime
	Put_BaseType_(&m_dataOutfiter, endTime_wrap);

	// IN INT32 startPos
	Put_BaseType_(&m_dataOutfiter, startPos_wrap);

	// IN INT32 length
	Put_BaseType_(&m_dataOutfiter, length_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<LogInfo> logInfos
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		logInfos_wrap.Resize(array_len);
		for(int idx2=0; idx2<logInfos_wrap.Length(); ++idx2)
		{
			GRID_User_T::LogInfo& ref2 = logInfos_wrap[idx2];
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.time);
				PICK_STRING_Q(iscm_replied_picker, ref2.info);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::LoadLogInfos() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::Login(
				IN const char* userName_wrap, IN INT32 userName_wrap_len /*= -1*/,
				IN const char* password_wrap, IN INT32 password_wrap_len /*= -1*/
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_Login)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 16;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::Login", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_String userName
	if(NULL == userName_wrap)
	{
		if(userName_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		userName_wrap = "";
		userName_wrap_len = 0;
	}
	if(userName_wrap_len < 0)
	{
		userName_wrap_len = (INT32)strlen(userName_wrap);
		Put_BaseType_(&m_dataOutfiter, userName_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)userName_wrap, userName_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, userName_wrap, userName_wrap_len);
	}

	// IN tcps_String password
	if(NULL == password_wrap)
	{
		if(password_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		password_wrap = "";
		password_wrap_len = 0;
	}
	if(password_wrap_len < 0)
	{
		password_wrap_len = (INT32)strlen(password_wrap);
		Put_BaseType_(&m_dataOutfiter, password_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)password_wrap, password_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, password_wrap, password_wrap_len);
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::Login() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

struct GRID_User_RC::Logout_Task_ : public NPBaseTask
{
	GRID_User_RC& face_;
	explicit Logout_Task_(
				GRID_User_RC& face
				)
		: face_(face)
		{}
	virtual void OnPoolTask()
	{
		if(!face_.m_prepareDisconnect)
		{
			face_.Logout(
						);
		}
		VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		tcps_Delete(this);
	}
};
TCPSError GRID_User_RC::Logout(
				) posting_method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			Logout_Task_* task = tcps_NewEx(Logout_Task_, (
							*this
							));
			VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) > 0);
			FetchTaskPool()->Push(task);
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_Logout)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 17;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::Logout", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_UDPCallbackProxy.isUsing_)
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID;
			m_rpcRequester->GetPeerSessionKey(sessionID);
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::AddUser(
				IN const UserInfo& userInfo_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_AddUser)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 18;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::AddUser", call_id_len+1);

	// 填充IN参数到outfiter

	// IN UserInfo userInfo
		Put_String_(&m_dataOutfiter, userInfo_wrap.userName.Get(), userInfo_wrap.userName.LenRef());
		Put_String_(&m_dataOutfiter, userInfo_wrap.passWord.Get(), userInfo_wrap.passWord.LenRef());
		Put_BaseType_(&m_dataOutfiter, userInfo_wrap.role);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::AddUser() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::DelUser(
				IN const tcps_Array<tcps_String>& userNames_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_DelUser)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 18;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::DelUser", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> userNames
	Put_BaseType_(&m_dataOutfiter, userNames_wrap.LenRef());
	for(int idx1=0; idx1<userNames_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = userNames_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::DelUser() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::UpdatePassword(
				IN const char* oldPassword_wrap, IN INT32 oldPassword_wrap_len /*= -1*/,
				IN const char* newPassword_wrap, IN INT32 newPassword_wrap_len /*= -1*/
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_UpdatePassword)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 25;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::UpdatePassword", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_String oldPassword
	if(NULL == oldPassword_wrap)
	{
		if(oldPassword_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		oldPassword_wrap = "";
		oldPassword_wrap_len = 0;
	}
	if(oldPassword_wrap_len < 0)
	{
		oldPassword_wrap_len = (INT32)strlen(oldPassword_wrap);
		Put_BaseType_(&m_dataOutfiter, oldPassword_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)oldPassword_wrap, oldPassword_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, oldPassword_wrap, oldPassword_wrap_len);
	}

	// IN tcps_String newPassword
	if(NULL == newPassword_wrap)
	{
		if(newPassword_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		newPassword_wrap = "";
		newPassword_wrap_len = 0;
	}
	if(newPassword_wrap_len < 0)
	{
		newPassword_wrap_len = (INT32)strlen(newPassword_wrap);
		Put_BaseType_(&m_dataOutfiter, newPassword_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)newPassword_wrap, newPassword_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, newPassword_wrap, newPassword_wrap_len);
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::UpdatePassword() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ManageUser(
				IN const UserInfo& userInfo_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ManageUser)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 21;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ManageUser", call_id_len+1);

	// 填充IN参数到outfiter

	// IN UserInfo userInfo
		Put_String_(&m_dataOutfiter, userInfo_wrap.userName.Get(), userInfo_wrap.userName.LenRef());
		Put_String_(&m_dataOutfiter, userInfo_wrap.passWord.Get(), userInfo_wrap.passWord.LenRef());
		Put_BaseType_(&m_dataOutfiter, userInfo_wrap.role);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ManageUser() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListAllUsers)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 23;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListAllUsers", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<UserInfo> userInfos
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		userInfos_wrap.Resize(array_len);
		for(int idx2=0; idx2<userInfos_wrap.Length(); ++idx2)
		{
			GRID_User_T::UserInfo& ref2 = userInfos_wrap[idx2];
				PICK_STRING_Q(iscm_replied_picker, ref2.userName);
				PICK_STRING_Q(iscm_replied_picker, ref2.passWord);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.role);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListAllUsers() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::GetUserInfo(
				IN const char* userName_wrap, IN INT32 userName_wrap_len /*= -1*/,
				OUT UserInfo& userInfo_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_GetUserInfo)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::GetUserInfo", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_String userName
	if(NULL == userName_wrap)
	{
		if(userName_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		userName_wrap = "";
		userName_wrap_len = 0;
	}
	if(userName_wrap_len < 0)
	{
		userName_wrap_len = (INT32)strlen(userName_wrap);
		Put_BaseType_(&m_dataOutfiter, userName_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)userName_wrap, userName_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, userName_wrap, userName_wrap_len);
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT UserInfo userInfo
			PICK_STRING_Q(iscm_replied_picker, userInfo_wrap.userName);
			PICK_STRING_Q(iscm_replied_picker, userInfo_wrap.passWord);
			PICK_BASETYPE_Q(iscm_replied_picker, userInfo_wrap.role);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::GetUserInfo() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListJTMs(
				OUT tcps_Array<tcps_String>& jtms_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListJTMs)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 19;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListJTMs", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<tcps_String> jtms
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		jtms_wrap.Resize(array_len);
		for(int idx2=0; idx2<jtms_wrap.Length(); ++idx2)
		{
			tcps_String& ref2 = jtms_wrap[idx2];
			PICK_STRING_Q(iscm_replied_picker, ref2);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListJTMs() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::GetJTMInfo(
				IN const char* jtm_wrap, IN INT32 jtm_wrap_len /*= -1*/,
				OUT JTMInfo& jtmInfo_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_GetJTMInfo)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 21;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::GetJTMInfo", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_String jtm
	if(NULL == jtm_wrap)
	{
		if(jtm_wrap_len > 0)
		{
			ASSERT(false);
			return TCPS_ERR_INVALID_PARAM;
		}
		jtm_wrap = "";
		jtm_wrap_len = 0;
	}
	if(jtm_wrap_len < 0)
	{
		jtm_wrap_len = (INT32)strlen(jtm_wrap);
		Put_BaseType_(&m_dataOutfiter, jtm_wrap_len);
		Put_Bytes_(&m_dataOutfiter, (const BYTE*)jtm_wrap, jtm_wrap_len+1);
	}
	else
	{
		Put_String_(&m_dataOutfiter, jtm_wrap, jtm_wrap_len);
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT JTMInfo jtmInfo
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.cpuType);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.cpuFreq);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.cpuProcessers);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.cpuThreads);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.totalMemoryBytes);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.networkBandwidth);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.osType);
				PICK_STRING_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.osDetail);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.executeBits);
				PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.binaryType);
				PICK_STRING_Q(iscm_replied_picker, jtmInfo_wrap.staticContext.hardwareInfo);
			PICK_BASETYPE_Q(iscm_replied_picker, jtmInfo_wrap.statisticInfo);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::GetJTMInfo() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::GetGridProperty(
				OUT GridProperty& gridProperty_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_GetGridProperty)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 26;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::GetGridProperty", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT GridProperty gridProperty
			PICK_BASETYPE_Q(iscm_replied_picker, gridProperty_wrap.version);
			PICK_BASETYPE_Q(iscm_replied_picker, gridProperty_wrap.jtmNum);
			PICK_BASETYPE_Q(iscm_replied_picker, gridProperty_wrap.processorNum);
			PICK_BASETYPE_Q(iscm_replied_picker, gridProperty_wrap.threadNum);
			PICK_STRING_Q(iscm_replied_picker, gridProperty_wrap.authorizedID);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::GetGridProperty() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::AddSplitter(
				IN const GRID_ProgramInfo& splitter_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_AddSplitter)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::AddSplitter", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_ProgramInfo splitter
			Put_String_(&m_dataOutfiter, splitter_wrap.programID.name.Get(), splitter_wrap.programID.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.ver);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.cpuType);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.osType);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.executeBits);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.binaryType);
		Put_String_(&m_dataOutfiter, splitter_wrap.description.Get(), splitter_wrap.description.LenRef());

	// IN tcps_Array<GRID_ProgramFile> files
	Put_BaseType_(&m_dataOutfiter, files_wrap.LenRef());
	for(int idx1=0; idx1<files_wrap.Length(); ++idx1)
	{
		const GRID_ProgramFile& ref1 = files_wrap[idx1];
			Put_BaseType_(&m_dataOutfiter, ref1.isExecutable);
			Put_String_(&m_dataOutfiter, ref1.name.Get(), ref1.name.LenRef());
			Put_Binary_(&m_dataOutfiter, ref1.data.Get(), ref1.data.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::AddSplitter() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::DelSplitter(
				IN const GRID_ProgramInfo& splitter_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_DelSplitter)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::DelSplitter", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_ProgramInfo splitter
			Put_String_(&m_dataOutfiter, splitter_wrap.programID.name.Get(), splitter_wrap.programID.name.LenRef());
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.ver);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.cpuType);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.osType);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.executeBits);
			Put_BaseType_(&m_dataOutfiter, splitter_wrap.programID.binaryType);
		Put_String_(&m_dataOutfiter, splitter_wrap.description.Get(), splitter_wrap.description.LenRef());

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::DelSplitter() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListSplitters)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 24;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListSplitters", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<GRID_ProgramID> splitters
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		splitters_wrap.Resize(array_len);
		for(int idx2=0; idx2<splitters_wrap.Length(); ++idx2)
		{
			GRID_ProgramID& ref2 = splitters_wrap[idx2];
				PICK_STRING_Q(iscm_replied_picker, ref2.name);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.ver);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.cpuType);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.osType);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.executeBits);
				PICK_BASETYPE_Q(iscm_replied_picker, ref2.binaryType);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListSplitters() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::ListSplitterParam(
				IN const GRID_ProgramID& splitter_wrap,
				OUT tcps_Array<SplitterParam>& splitterParams_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_ListSplitterParam)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 28;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::ListSplitterParam", call_id_len+1);

	// 填充IN参数到outfiter

	// IN GRID_ProgramID splitter
		Put_String_(&m_dataOutfiter, splitter_wrap.name.Get(), splitter_wrap.name.LenRef());
		Put_BaseType_(&m_dataOutfiter, splitter_wrap.ver);
		Put_BaseType_(&m_dataOutfiter, splitter_wrap.cpuType);
		Put_BaseType_(&m_dataOutfiter, splitter_wrap.osType);
		Put_BaseType_(&m_dataOutfiter, splitter_wrap.executeBits);
		Put_BaseType_(&m_dataOutfiter, splitter_wrap.binaryType);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<SplitterParam> splitterParams
		PICK_BASETYPE_Q(iscm_replied_picker, array_len);
		splitterParams_wrap.Resize(array_len);
		for(int idx2=0; idx2<splitterParams_wrap.Length(); ++idx2)
		{
			GRID_User_T::SplitterParam& ref2 = splitterParams_wrap[idx2];
				PICK_STRING_Q(iscm_replied_picker, ref2.name);
				PICK_STRING_Q(iscm_replied_picker, ref2.data);
				PICK_STRING_Q(iscm_replied_picker, ref2.description);
		}
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::ListSplitterParam() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::GetGridStatistics(
				OUT GridStatistics& stat_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_GetGridStatistics)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 28;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::GetGridStatistics", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT GridStatistics stat
		PICK_BASETYPE_Q(iscm_replied_picker, stat_wrap);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::GetGridStatistics() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::UDPLink_(
				OUT INT32& servePort_wrap,
				OUT INT32& linkKey_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_UDPLink_)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 19;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::UDPLink_", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT INT32 servePort
		PICK_BASETYPE_Q(iscm_replied_picker, servePort_wrap);

		// OUT INT32 linkKey
		PICK_BASETYPE_Q(iscm_replied_picker, linkKey_wrap);
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::UDPLink_() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::UDPLinkConfirm_(
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_UDPLinkConfirm_)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 26;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::UDPLinkConfirm_", call_id_len+1);

	// 填充IN参数到outfiter

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		tcps_Binary replied_data;
		if(!replied_data.Resize(replyBytes))
		{
			this->OnNetworkMalformed_();
			return TCPS_ERR_OUT_OF_MEMORY;
		}
		err = m_rpcRequester->RecvD(replied_data.Get(), replyBytes);
		if(TCPS_OK != err)
		{
			// 无须调用this->OnNetworkMalformed_(), m_rpcRequester->RecvD()已做处理
			return err;
		}
		const BYTE* iscm_replied_picker = replied_data.Get();
		const BYTE* const iscm_replied_end = iscm_replied_picker + replyBytes;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_Q(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.
		if(iscm_replied_picker != iscm_replied_end)
		{
			// NPLogError(("GRID_User::UDPLinkConfirm_() method, Malformed replied"));
			this->OnNetworkMalformed_();
			return TCPS_ERR_MALFORMED_REPLY;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::Wrap_SetRedirect_(
				IN GRID_User_RC* thisObj /*= NULL*/,
				IN GRID_User* faceObj /*= NULL*/,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) posting_callback
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN IPP redirectIPP
	IN IPP redirectIPP_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, redirectIPP_wrap);

	// IN tcps_Binary redirectParam
	IN tcps_Binary redirectParam_wrap;
	GET_BINARY_EX_(thisObj, ptrInParams, ptrInParamsLen, redirectParam_wrap);

	if(0 != ptrInParamsLen)
	{
		// NPLogError(("GRID_User::SetRedirect_() posting_callback, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的回调函数
	try
	{
		if(thisObj)
		{
			iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(thisObj->m_callingOutTIDs, iscm_cct_posting_callback);
			errTcps = thisObj->SetRedirect_(
				redirectIPP_wrap,
				redirectParam_wrap
				);
		}
		else
		{
			(void)faceObj;
			ASSERT(false);
			errTcps = TCPS_ERR_INTERNAL_BUG;
		}
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	return TCPS_OK;
}

struct GRID_User_RC::SetTimeout__Task_ : public NPBaseTask
{
	GRID_User_RC& face_;
	IN INT32 recvTimeout_wrap_;
	IN INT32 sendTimeout_wrap_;
	explicit SetTimeout__Task_(
				GRID_User_RC& face,
				IN INT32 recvTimeout_wrap,
				IN INT32 sendTimeout_wrap
				)
		: face_(face)
		, recvTimeout_wrap_(recvTimeout_wrap)
		, sendTimeout_wrap_(sendTimeout_wrap)
		{}
	virtual void OnPoolTask()
	{
		if(!face_.m_prepareDisconnect)
		{
			face_.SetTimeout_(
						recvTimeout_wrap_,
						sendTimeout_wrap_
						);
		}
		VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		tcps_Delete(this);
	}
};
TCPSError GRID_User_RC::SetTimeout_(
				IN INT32 recvTimeout_wrap,
				IN INT32 sendTimeout_wrap
				) posting_method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			SetTimeout__Task_* task = tcps_NewEx(SetTimeout__Task_, (
							*this,
							recvTimeout_wrap,
							sendTimeout_wrap
							));
			VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) > 0);
			FetchTaskPool()->Push(task);
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_SetTimeout_)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 22;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::SetTimeout_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT32 recvTimeout
	Put_BaseType_(&m_dataOutfiter, recvTimeout_wrap);

	// IN INT32 sendTimeout
	Put_BaseType_(&m_dataOutfiter, sendTimeout_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_UDPCallbackProxy.isUsing_)
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID;
			m_rpcRequester->GetPeerSessionKey(sessionID);
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

struct GRID_User_RC::SetSessionBufferSize__Task_ : public NPBaseTask
{
	GRID_User_RC& face_;
	IN INT32 recvBufBytes_wrap_;
	IN INT32 sendBufBytes_wrap_;
	explicit SetSessionBufferSize__Task_(
				GRID_User_RC& face,
				IN INT32 recvBufBytes_wrap,
				IN INT32 sendBufBytes_wrap
				)
		: face_(face)
		, recvBufBytes_wrap_(recvBufBytes_wrap)
		, sendBufBytes_wrap_(sendBufBytes_wrap)
		{}
	virtual void OnPoolTask()
	{
		if(!face_.m_prepareDisconnect)
		{
			face_.SetSessionBufferSize_(
						recvBufBytes_wrap_,
						sendBufBytes_wrap_
						);
		}
		VERIFY(InterlockedDecrement(&face_.m_asyncTaskStat.totalAsyncTasks) >= 0);
		tcps_Delete(this);
	}
};
TCPSError GRID_User_RC::SetSessionBufferSize_(
				IN INT32 recvBufBytes_wrap,
				IN INT32 sendBufBytes_wrap
				) posting_method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			SetSessionBufferSize__Task_* task = tcps_NewEx(SetSessionBufferSize__Task_, (
							*this,
							recvBufBytes_wrap,
							sendBufBytes_wrap
							));
			VERIFY(InterlockedIncrement(&m_asyncTaskStat.totalAsyncTasks) > 0);
			FetchTaskPool()->Push(task);
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	TCPSError errUpdate = UpdateMethodMatchingFlag_();
	if(TCPS_OK != errUpdate)
		return errUpdate;
	if(!m_methodMatchingFlag.matching_SetSessionBufferSize_)
		return TCPS_ERR_METHOD_NOT_MATCH;

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 32;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::SetSessionBufferSize_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN INT32 recvBufBytes
	Put_BaseType_(&m_dataOutfiter, recvBufBytes_wrap);

	// IN INT32 sendBufBytes
	Put_BaseType_(&m_dataOutfiter, sendBufBytes_wrap);

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		if(m_UDPCallbackProxy.isUsing_)
		{
			int total = SockTotalizeWriteBufVec(reqBufVec, reqBufVecCount);
			BYTE* p = (BYTE*)tcps_Alloc(total);
			if(NULL == p)
			{
				ASSERT(false);
				return TCPS_ERR_OUT_OF_MEMORY;
			}
			BYTE* q = p;
			for(int i=0; i<reqBufVecCount; ++i)
			{
				const SockWriteBuf& swb = reqBufVec[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT((int)(q-p) == total);
			SockWriteBuf swb_udp;
			swb_udp.data = p;
			swb_udp.len = total;
			INT32 sessionID;
			m_rpcRequester->GetPeerSessionKey(sessionID);
			iscm_IUDPServeMan& udpServer = iscm_FetchUDPServeMan();
			udpServer.SendSessionData(sessionID, &swb_udp, 1);
		}
		else if(0 != m_postingProxy.callerKey_)
		{
			INT_PTR queueFullIndexes = -1;
			INT_PTR queueFullCount = 0;
			TCPSError err = iscm_FetchPostingCallerMan().BatchPosting(&m_postingProxy.callerKey_, 1, reqBufVec, reqBufVecCount, &queueFullIndexes, &queueFullCount);
			if(TCPS_OK != err)
				return err;
			ASSERT(0==queueFullCount || 1==queueFullCount);
			if(1 == queueFullCount)
				return TCPS_ERR_POSTING_PENDING_FULL;
		}
		else
		{
			TCPSError err = m_rpcRequester->Post(RPCCT_RPC, reqBufVec, reqBufVecCount);
			if(TCPS_OK != err)
				return err;
		}
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::MethodCheck_(
				IN const tcps_Array<tcps_String>& methods_wrap,
				IN const tcps_Array<tcps_String>& methodTypeInfos_wrap,
				OUT tcps_Array<BOOL>& matchingFlags_wrap
				) method
{
	iscm_ClientCallType iscm_cct_ = iscm_cct_invalid;
	if(m_callingOutTIDs.HasFlag(OSThreadSelf(), &iscm_cct_))
	{
		if(iscm_cct_OnClose == iscm_cct_)
			return TCPS_ERR_EXITING;
		if(iscm_cct_callback==iscm_cct_ || iscm_cct_posting_callback==iscm_cct_)
		{
			NPR_ASSERT_EX(false, _T("在回调函数里调用会话自身的同步方法，可能导致死锁！"));
			return TCPS_OK;
		}
	}

	iscm_IRequester::AutoBeginCallEx locker(m_rpcRequester);
	ASSERT(0 == m_dataOutfiter.bufs_.size());

	DataOutfiter::AutoClear outfiter_AutoClear(m_dataOutfiter);

	// 填充基本类型数据到outfiter
	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 1;
	peerCallFlags.dummy_15 = 0;
	m_dataOutfiter.Push(&peerCallFlags, sizeof(peerCallFlags));

	INT32 call_id_len = 23;
	m_dataOutfiter.Push(&call_id_len, sizeof(INT32));
	m_dataOutfiter.Push("GRID_User::MethodCheck_", call_id_len+1);

	// 填充IN参数到outfiter

	// IN tcps_Array<tcps_String> methods
	Put_BaseType_(&m_dataOutfiter, methods_wrap.LenRef());
	for(int idx1=0; idx1<methods_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = methods_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// IN tcps_Array<tcps_String> methodTypeInfos
	Put_BaseType_(&m_dataOutfiter, methodTypeInfos_wrap.LenRef());
	for(int idx1=0; idx1<methodTypeInfos_wrap.Length(); ++idx1)
	{
		const tcps_String& ref1 = methodTypeInfos_wrap[idx1];
		Put_String_(&m_dataOutfiter, ref1.Get(), ref1.LenRef());
	}

	// 调用RPCCall()
	{
		const SockWriteBuf* reqBufVec = m_dataOutfiter.bufs_.Get();
		int reqBufVecCount = (int)m_dataOutfiter.bufs_.size();
		int replyBytes = 0;
		TCPSError err = m_rpcRequester->CallEx(RPCCT_RPC, reqBufVec, reqBufVecCount, replyBytes);
		if(TCPS_OK != err)
			return err;

		iscm_IRequester* iscm_replied_picker = m_rpcRequester;

		iscm_RPCReplyPrefix replyPrefix;
		PICK_BASETYPE_(iscm_replied_picker, replyPrefix);
		INT32 array_len;
		(void)array_len; // avoid warning.

		// OUT tcps_Array<BOOL> matchingFlags
		PICK_PODARRAY_(iscm_replied_picker, matchingFlags_wrap);
	}
	return TCPS_OK;
}

TCPSError GRID_User_RC::Wrap_CallbackCheck_(
				IN GRID_User_RC* thisObj /*= NULL*/,
				IN GRID_User* faceObj /*= NULL*/,
				IN iscm_PeerCallFlags peerCallFlags,
				IN OUT const BYTE*& ptrInParams,
				IN OUT INT_PTR& ptrInParamsLen,
				OUT iscm_IRPCOutfiter* outfiter
				) callback
{
	ASSERT((NULL==thisObj) != (NULL==faceObj));
	(void)ptrInParams; (void)ptrInParamsLen; (void)outfiter; // avoid warning.
	TCPSError errTcps = TCPS_ERROR;

	// 从ptrInParams中分析出输入参数
	INT32 array_len;
	(void)array_len; // avoid warning.
	(void)peerCallFlags;

	// IN tcps_Array<tcps_String> callbacks
	IN tcps_Array<tcps_String> callbacks_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	callbacks_wrap.Resize(array_len);
	for(int idx1=0; idx1<callbacks_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = callbacks_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	// IN tcps_Array<tcps_String> callbackTypeInfos
	IN tcps_Array<tcps_String> callbackTypeInfos_wrap;
	GET_BASETYPE_EX_(thisObj, ptrInParams, ptrInParamsLen, array_len);
	callbackTypeInfos_wrap.Resize(array_len);
	for(int idx1=0; idx1<callbackTypeInfos_wrap.Length(); ++idx1)
	{
		tcps_String& ref1 = callbackTypeInfos_wrap[idx1];
		GET_STRING_EX_(thisObj, ptrInParams, ptrInParamsLen, ref1);
	}

	if(0 != ptrInParamsLen)
	{
		// NPLogError(("GRID_User::CallbackCheck_() callback, Malformed request"));
		if(thisObj)
			thisObj->OnNetworkMalformed_();
		return TCPS_ERR_MALFORMED_REQ;
	}

	// 定义输出参数
	struct OutParamWrapSite
	{
		iscm_RPCReplyPrefix replyPrefix_iscm;
		OUT tcps_Array<BOOL> matchingFlags;
		OutParamWrapSite() { replyPrefix_iscm.Init(); }
		~OutParamWrapSite() { }
		static void FreeAction(void* p)
		{
			OutParamWrapSite* ptr = (OutParamWrapSite*)((BYTE*)p - offset_of(OutParamWrapSite, replyPrefix_iscm));
			ptr->~OutParamWrapSite();
			tcps_Free(ptr);
		}
	};
	OutParamWrapSite* opWrapper = NULL;
	if(outfiter) // 非posting call
		opWrapper = tcps_New(OutParamWrapSite);
	else
		ASSERT(true); // posting call

	// 调用用户实现的回调函数
	try
	{
		if(thisObj)
		{
			iscm_ClientThreadIDs::AutoThisTID autoCallingFlag(thisObj->m_callingOutTIDs, iscm_cct_callback);
			errTcps = thisObj->CallbackCheck_(
				callbacks_wrap,
				callbackTypeInfos_wrap,
				opWrapper->matchingFlags
				);
		}
		else
		{
			(void)faceObj;
			ASSERT(false);
			errTcps = TCPS_ERR_INTERNAL_BUG;
		}
	}
	catch(TCPSError ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = ret;
	}
	catch(int ret)
	{
		ASSERT(TCPS_OK != ret);
		errTcps = (TCPSError)ret;
	}
	catch(std::bad_alloc)
	{
		errTcps = TCPS_ERR_OUT_OF_MEMORY;
	}
	ISCM_BEGIN_CATCH_ALL_()
		errTcps = TCPS_ERR_UNKNOWN_EXCEPTION;
	ISCM_END_CATCH_ALL_()

	if(TCPS_OK != errTcps)
	{
		if(opWrapper)
			OutParamWrapSite::FreeAction(opWrapper);
		return errTcps;
	}

	// 填充OUT数据到outfiter
	if(opWrapper)
	{
		// FreeAction()负责对所有out数据在网络发送完成后进行析构清理工作
		Set_BaseType_(outfiter, opWrapper->replyPrefix_iscm, OutParamWrapSite::FreeAction);
	}

	// OUT tcps_Array<BOOL> matchingFlags
	OUT const tcps_Array<BOOL>& matchingFlags_wrap = opWrapper->matchingFlags;
	Set_PODArray_(outfiter, matchingFlags_wrap);

	return TCPS_OK;
}

GRID_User::GRID_User(BOOL initNetworkSingletons /*= true*/)
	: m_faceR(NULL), m_faceL(NULL), m_callSiteL(NULL)
	, m_closingTID(INVALID_OSTHREADID)
	, m_connectTimeout(INFINITE), m_recvTimeout(INFINITE), m_sendTimeout(INFINITE)
	, m_streamedCallbackMap_IsInited(false)
{
	m_base_v_tab_ptr = m_derived_v_tab_ptr = *(LPVOID*)this;
	if(initNetworkSingletons)
		iscm_InitClientSingletons(true, true);
	else
		iscm_InitClientSingletons(false, false);
	GRID_User_RC::GetCallbackMap_();
}

GRID_User::~GRID_User()
{
	BOOL isNotDerived = (m_base_v_tab_ptr == m_derived_v_tab_ptr);
	NPR_ASSERT_EX(isNotDerived || (NULL==m_faceR && NULL==m_faceL), "请在最终派生类的析构函数的最后调用一次DestroyRequester()");
	DestroyRequester();
	if(m_callSiteL)
		tcps_Delete(m_callSiteL);
}

void GRID_User::DestroyRequester_(
				IN BOOL byLocalPeer
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);

	CAutoObj<AutoFlag<OSThreadID> > autoClosingTID;
	if(!byLocalPeer)
		ConstructAutoObj(AutoFlag<OSThreadID>, autoClosingTID, (m_closingTID, OSThreadSelf()));

	if(m_faceR)
	{
		tcps_Delete(m_faceR);
		m_faceR = NULL;
	}
	else if(m_faceL)
	{
		if(INVALID_OSTHREADID==m_closingTID || OSThreadSelf()!=m_closingTID)
			this->OnPeerBroken();
		this->OnClose();
		m_faceL->DeleteThis();
		m_faceL = NULL;
		if(m_callSiteL)
			m_callSiteL->Reset();
		m_localServeIPP = INVALID_IPP;
	}
}

void GRID_User::DestroyRequester()
{
	DestroyRequester_(false);
}

TCPSError GRID_User::SetServeIPP_(
				IN BOOL byLocalPeer,
				IN const IPP& serveIPP,
				IN DWORD sessionFlags /*= 0*/,
				IN const IPP& clientID_IPP /*= INVALID_IPP*/,
				IN const IPP& secondaryServeIPP /*= INVALID_IPP*/,
				IN const IPP& tertiaryServeIPP /*= INVALID_IPP*/,
				IN DWORD reconnectIntervalMSELs /*= 2000*/
				)
{
	CNPAutoLock locker(m_lock);
	m_derived_v_tab_ptr = *(LPVOID*)this;
	ASSERT(NULL==m_faceR || NULL==m_faceL);

	if(m_localMakingFlag.making)
	{
		if(serveIPP.IsValid())
		{
			ASSERT(false);
			return TCPS_ERR_EXTERNAL_BUG;
		}
		m_localMakingFlag.failed = true;
		return TCPS_OK;
	}

	if(m_faceL && m_localServeIPP==serveIPP)
		return TCPS_OK;

	if(serveIPP.IsValid() && iscm_IsRegisteredServe(serveIPP))
	{
		DestroyRequester_(byLocalPeer);

		ASSERT(!m_localMakingFlag.making && !m_localMakingFlag.failed);
		AutoFlag<BOOL> autoFlag(m_localMakingFlag.making, true);
		FinalAssign<BOOL> finalAssign(m_localMakingFlag.failed, false);

		union
		{
		NP_GridUserSessionMaker* sessionMaker;
		void* dummyUnionPtr;
		};
		sessionMaker = NULL;
		FNMakeLocalSession_GRID_User fnMake =
					(FNMakeLocalSession_GRID_User)iscm_GetRegisteredFunction(
						serveIPP,
						"MakeLocalSession_GRID_User",
						&dummyUnionPtr
						);
		if(NULL == fnMake)
			return TCPS_ERR_INVALID_INTERFACE;

		ASSERT(sessionMaker);
		TCPSError err = fnMake(clientID_IPP, *sessionMaker, m_faceL, this);
		if(TCPS_OK==err && m_localMakingFlag.failed)
		{
			ASSERT(m_faceL);
			m_faceL->DeleteThis();
			m_faceL = NULL;
			err = TCPS_ERR_CONNECT_FAILED;
		}
		if(TCPS_OK != err)
		{
			ASSERT(NULL == m_faceL);
			return err;
		}
		if(m_callSiteL)
			m_callSiteL->needUpdateFlags = true;

		ASSERT(m_faceL);
		err = this->OnConnected();
		if(TCPS_OK==err && m_localMakingFlag.failed)
			err = TCPS_ERR_CONNECT_FAILED;
		if(TCPS_OK != err)
		{
			if(INVALID_OSTHREADID==m_closingTID || OSThreadSelf()!=m_closingTID)
				this->OnPeerBroken();
			this->OnClose();
			m_faceL->DeleteThis();
			m_faceL = NULL;
			return err;
		}
		m_localServeIPP = serveIPP;
		return TCPS_OK;
	}

	if(m_faceL)
		DestroyRequester_(byLocalPeer);
	if(NULL == m_faceR)
	{
		if(!serveIPP.IsValid())
			return TCPS_OK;
		m_faceR = tcps_NewEx(GRID_User_RC, (this));
	}

	if(INFINITE!=m_connectTimeout || INFINITE!=m_recvTimeout || INFINITE!=m_sendTimeout)
		m_faceR->SetTimeout(m_connectTimeout, m_recvTimeout, m_sendTimeout);
	if(!m_sockRecvSendBufParam.IsDefault())
		m_faceR->SetSessionBufferSize(m_sockRecvSendBufParam.recvBufBytes_, m_sockRecvSendBufParam.sendBufBytes_);
	if(!m_postingPendingParam.IsDefault())
		m_faceR->SetPostingPendingParameters(m_postingPendingParam.maxPendingBytes_, m_postingPendingParam.maxPendings_);
	return m_faceR->SetServeIPP(
						serveIPP,
						sessionFlags,
						clientID_IPP,
						secondaryServeIPP,
						tertiaryServeIPP,
						reconnectIntervalMSELs
						);
}

TCPSError GRID_User::SetServeIPP(
				IN const IPP& serveIPP,
				IN DWORD sessionFlags /*= 0*/,
				IN const IPP& clientID_IPP /*= INVALID_IPP*/,
				IN const IPP& secondaryServeIPP /*= INVALID_IPP*/,
				IN const IPP& tertiaryServeIPP /*= INVALID_IPP*/,
				IN DWORD reconnectIntervalMSELs /*= 2000*/
				)
{
	CNPAutoLock locker(m_lock);
	AutoFlag<OSThreadID> autoClosingTID(m_closingTID, OSThreadSelf());
	return SetServeIPP_(false, serveIPP, sessionFlags, clientID_IPP, secondaryServeIPP, tertiaryServeIPP, reconnectIntervalMSELs);
}

IPP GRID_User::GetServeIPP() const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		return m_faceR->GetServeIPP();
	else if(m_faceL)
		return m_localServeIPP;
	return INVALID_IPP;
}

IPP GRID_User::GetServingIPP() const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		return m_faceR->GetServingIPP();
	else if(m_faceL)
		return m_localServeIPP;
	return INVALID_IPP;
}

BOOL GRID_User::IsConnected() const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		return m_faceR->IsConnected();
	else if(m_faceL)
		return true;
	return false;
}

void GRID_User::SetKeeping(
				IN BOOL keeping
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		m_faceR->SetKeeping(keeping);
}

BOOL GRID_User::IsKeeping() const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		return m_faceR->IsKeeping();
	return false;
}

BOOL GRID_User::IsLocalPeer() const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	return (NULL != m_faceL);
}

void GRID_User::PrepareDisconnect()
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		m_faceR->PrepareDisconnect();
}

void GRID_User::SetTimeout(
				IN DWORD connectTimeout /*= INFINITE*/,
				IN DWORD recvTimeout /*= INFINITE*/,
				IN DWORD sendTimeout /*= INFINITE*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		m_faceR->SetTimeout(connectTimeout, recvTimeout, sendTimeout);
	m_connectTimeout = connectTimeout;
	m_recvTimeout = recvTimeout;
	m_sendTimeout = sendTimeout;
}

void GRID_User::GetTimeout(
				OUT DWORD* connectTimeout /*= NULL*/,
				OUT DWORD* recvTimeout /*= NULL*/,
				OUT DWORD* sendTimeout /*= NULL*/
				) const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		m_faceR->GetTimeout(connectTimeout, recvTimeout, sendTimeout);
		return;
	}
	if(connectTimeout)
		*connectTimeout = m_connectTimeout;
	if(recvTimeout)
		*recvTimeout = m_recvTimeout;
	if(sendTimeout)
		*sendTimeout = m_sendTimeout;
}

void GRID_User::SetSessionBufferSize(
				IN INT32 recvBufBytes /*= -1*/,
				IN INT32 sendBufBytes /*= -1*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		m_faceR->SetSessionBufferSize(recvBufBytes, sendBufBytes);
	m_sockRecvSendBufParam.Set(recvBufBytes, sendBufBytes);
}

void GRID_User::GetSessionBufferSize(
				OUT INT32* recvBufBytes /*= NULL*/,
				OUT INT32* sendBufBytes /*= NULL*/
				) const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	m_sockRecvSendBufParam.Get(recvBufBytes, sendBufBytes);
}

void GRID_User::SetPostingPendingParameters(
				IN INT32 maxPendingBytes /*= -1*/,
				IN INT32 maxPendings /*= -1*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
		m_faceR->SetPostingPendingParameters(maxPendingBytes, maxPendings);
	m_postingPendingParam.Set(maxPendingBytes, maxPendings);
}

void GRID_User::GetPostingPendingParameters(
				OUT INT32* maxPendingBytes /*= NULL*/,
				OUT INT32* maxPendings /*= NULL*/
				) const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	m_postingPendingParam.Get(maxPendingBytes, maxPendings);
}

void GRID_User::SetPostingSendParameters(
				IN INT32 maxSendingBytes /*= -1*/,
				IN INT32 maxSendings /*= -1*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR && 0!=m_faceR->m_postingProxy.callerKey_)
		iscm_FetchPostingCallerMan().SetBufferingParam(m_faceR->m_postingProxy.callerKey_, maxSendingBytes, maxSendings);
	m_postingSendParam.Set(maxSendingBytes, maxSendings);
}

void GRID_User::GetPostingSendParameters(
				OUT INT32* maxSendingBytes /*= NULL*/,
				OUT INT32* maxSendings /*= NULL*/
				) const
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	m_postingSendParam.Get(maxSendingBytes, maxSendings);
}

TCPSError GRID_User::CleanPostingSendingQueue()
{
	if(NULL==m_faceR || 0==m_faceR->m_postingProxy.callerKey_)
		return TCPS_ERR_CALL_WAS_IGNORED;
	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(&m_faceR->m_postingProxy.callerKey_, 1);
}

TCPSError GRID_User::CleanPostingSendingQueue(
				IN const PGRID_User_* clients,
				IN INT_PTR clientsCount
				)
{
	tcps_SmartArray<PGRID_User_, 256> clients_ar_;
	for(INT_PTR i=0; i<clientsCount; ++i)
	{
		if(NULL == clients[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(NULL==clients[i]->m_faceR || 0==clients[i]->m_faceR->m_postingProxy.callerKey_)
			continue;
		clients_ar_.push_back(clients[i]);
	}
	if(0 == clients_ar_.size())
		return TCPS_OK;

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> callerKey_ar_;
	callerKey_ar_.resize(clients_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)clients_ar_.size(); ++i)
		callerKey_ar_[i] = clients_ar_[i]->m_faceR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	return callerMan.BatchCleanQueue(callerKey_ar_.Get(), callerKey_ar_.size());
}

const GRID_User::MethodMatchingFlag& GRID_User::GetMethodMatchingFlag(
				OUT TCPSError* err /*= NULL*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetMethodMatchingFlag(err);
	}
	else if(m_faceL)
	{
		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		if(m_callSiteL->needUpdateFlags)
		{
			InitializeAllCallsTypeInfo_();
			m_callSiteL->matchingFlags.matching_AddJobProgram = (NULL != m_faceL->FindMethod_("AddJobProgram", 13, s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_DelJobProgram = (NULL != m_faceL->FindMethod_("DelJobProgram", 13, s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_FindJobProgram = (NULL != m_faceL->FindMethod_("FindJobProgram", 14, s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListJobPrograms = (NULL != m_faceL->FindMethod_("ListJobPrograms", 15, s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_CommitJob = (NULL != m_faceL->FindMethod_("CommitJob", 9, s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_CancelJob = (NULL != m_faceL->FindMethod_("CancelJob", 9, s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_SetJobPriority = (NULL != m_faceL->FindMethod_("SetJobPriority", 14, s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListJobs = (NULL != m_faceL->FindMethod_("ListJobs", 8, s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_QueryJobs = (NULL != m_faceL->FindMethod_("QueryJobs", 9, s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_UpdateGrid = (NULL != m_faceL->FindMethod_("UpdateGrid", 10, s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListModules = (NULL != m_faceL->FindMethod_("ListModules", 11, s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_GetLogCount = (NULL != m_faceL->FindMethod_("GetLogCount", 11, s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_LoadLogInfos = (NULL != m_faceL->FindMethod_("LoadLogInfos", 12, s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_Login = (NULL != m_faceL->FindMethod_("Login", 5, s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_Logout = (NULL != m_faceL->FindMethod_("Logout", 6, s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_AddUser = (NULL != m_faceL->FindMethod_("AddUser", 7, s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_DelUser = (NULL != m_faceL->FindMethod_("DelUser", 7, s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_UpdatePassword = (NULL != m_faceL->FindMethod_("UpdatePassword", 14, s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ManageUser = (NULL != m_faceL->FindMethod_("ManageUser", 10, s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListAllUsers = (NULL != m_faceL->FindMethod_("ListAllUsers", 12, s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_GetUserInfo = (NULL != m_faceL->FindMethod_("GetUserInfo", 11, s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListJTMs = (NULL != m_faceL->FindMethod_("ListJTMs", 8, s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_GetJTMInfo = (NULL != m_faceL->FindMethod_("GetJTMInfo", 10, s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_GetGridProperty = (NULL != m_faceL->FindMethod_("GetGridProperty", 15, s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_AddSplitter = (NULL != m_faceL->FindMethod_("AddSplitter", 11, s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_DelSplitter = (NULL != m_faceL->FindMethod_("DelSplitter", 11, s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListSplitters = (NULL != m_faceL->FindMethod_("ListSplitters", 13, s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_ListSplitterParam = (NULL != m_faceL->FindMethod_("ListSplitterParam", 17, s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_));
			m_callSiteL->matchingFlags.matching_GetGridStatistics = (NULL != m_faceL->FindMethod_("GetGridStatistics", 17, s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_));
			m_callSiteL->needUpdateFlags = false;
		}
		if(err)
			*err = TCPS_OK;
		return m_callSiteL->matchingFlags;
	}

	static MethodMatchingFlag const defaultFlags;
	if(err)
		*err = TCPS_ERR_NEED_SERVE_IPP;
	return defaultFlags;
}

TCPSError GRID_User::GetStreamedMethodTypeInfo_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				OUT char*& typeInfo,
				OUT INT_PTR& infoLen
				) const
{
	InitializeAllCallsTypeInfo_();
	typedef CQuickStringMap<CPtrStrA, CPtrStrA, QSS_Traits<29> > MethodMap_;
	static MethodMap_* s_mMap = NULL;
	if(NULL == s_mMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_mMap)
		{
			static MethodMap_ s_mMapObj;
			MethodMap_& mMap = s_mMapObj;
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddJobProgram"), CPtrStrA(s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelJobProgram"), CPtrStrA(s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("FindJobProgram"), CPtrStrA(s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobPrograms"), CPtrStrA(s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CommitJob"), CPtrStrA(s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("CancelJob"), CPtrStrA(s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("SetJobPriority"), CPtrStrA(s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJobs"), CPtrStrA(s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("QueryJobs"), CPtrStrA(s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdateGrid"), CPtrStrA(s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListModules"), CPtrStrA(s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetLogCount"), CPtrStrA(s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("LoadLogInfos"), CPtrStrA(s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Login"), CPtrStrA(s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("Logout"), CPtrStrA(s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddUser"), CPtrStrA(s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelUser"), CPtrStrA(s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("UpdatePassword"), CPtrStrA(s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ManageUser"), CPtrStrA(s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListAllUsers"), CPtrStrA(s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetUserInfo"), CPtrStrA(s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListJTMs"), CPtrStrA(s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetJTMInfo"), CPtrStrA(s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridProperty"), CPtrStrA(s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("AddSplitter"), CPtrStrA(s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("DelSplitter"), CPtrStrA(s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitters"), CPtrStrA(s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ListSplitterParam"), CPtrStrA(s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_))).second);
			VERIFY(mMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("GetGridStatistics"), CPtrStrA(s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_))).second);
			s_mMap = &mMap;
		}
	}
	MethodMap_::iterator it = s_mMap->find(methodName, nameLen);
	if(it == s_mMap->end())
		return TCPS_ERR_METHOD_NOT_MATCH;
	infoLen = it->second.GetLength();
	typeInfo = (char*)tcps_Alloc(infoLen+1);
	StrAssign(typeInfo, it->second);
	return TCPS_OK;
}

TCPSError GRID_User::StreamedCall_(
				IN const char* methodName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	if(m_faceR)
	{
		return m_faceR->StreamedCall_(
					methodName, nameLen,
					data, dataLen,
					replyData, replyLen
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			return m_streamedCallSite.func(
						m_streamedCallSite.serverObj, m_streamedCallSite.sessionObj,
						"GRID_User", methodName,
						data, dataLen,
						replyData, replyLen
						);
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnOnStreamedCall_L_;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("OnStreamedCall_L_", -1, NULL, 0);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_OnStreamedCall_L_)methodFuncL)(
					m_faceL,
					methodName, nameLen,
					data, dataLen,
					replyData, replyLen
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

BOOL GRID_User::IsStreamedCallbackMatched_(
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/
				)
{
	if(m_streamedCallbackMap_IsInited)
	{
		StreamedCallbackMap::iterator it = m_streamedCallbackMap.find(callbackName, nameLen);
		if(it == m_streamedCallbackMap.end())
			return false;
		return it->second;
	}

	CNPAutoLock locker(m_lock);

	if(m_streamedCallbackMap_IsInited)
	{
		StreamedCallbackMap::iterator it = m_streamedCallbackMap.find(callbackName, nameLen);
		if(it == m_streamedCallbackMap.end())
			return false;
		return it->second;
	}

	ASSERT(0 == m_streamedCallbackMap.size());

	InitializeAllCallsTypeInfo_();
	BOOL matched;
	TCPSError err;
	char* typeInfo;
	INT_PTR infoLen;
	(void)matched; (void)err; (void)typeInfo; (void)infoLen;

	typeInfo = NULL;
	infoLen = 0;
	err = this->GetStreamedCallbackTypeInfo_("ReplyCommitJob", 14, typeInfo, infoLen);
	matched = (TCPS_OK==err && s_GRID_User_ReplyCommitJob_TypeInfo_len_==infoLen && 0==strncmp(s_GRID_User_ReplyCommitJob_TypeInfo_, typeInfo, infoLen));
	VERIFY(m_streamedCallbackMap.insert(std::make_pair(CPtrStrA("ReplyCommitJob", 14), matched)).second);
	if(typeInfo)
		tcps_Free(typeInfo);

	m_streamedCallbackMap_IsInited = true;
	StreamedCallbackMap::iterator it = m_streamedCallbackMap.find(callbackName, nameLen);
	if(it == m_streamedCallbackMap.end())
		return false;
	return it->second;
}

TCPSError GRID_User::AddJobProgram(
				IN const GRID_ProgramInfo& programInfo_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->AddJobProgram(
					programInfo_wrap,
					files_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(programInfo_wrap);
			inParamsLen += iscm_GetStreamedSize(files_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, programInfo_wrap);
			iscm_StreamedStore(pInParamsBuf, files_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"AddJobProgram",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnAddJobProgram;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("AddJobProgram", 13, s_GRID_User_AddJobProgram_TypeInfo_, s_GRID_User_AddJobProgram_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_AddJobProgram)methodFuncL)(
					m_faceL,
					programInfo_wrap,
					files_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::DelJobProgram(
				IN const tcps_Array<INT64>& programKeys_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->DelJobProgram(
					programKeys_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(programKeys_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, programKeys_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"DelJobProgram",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnDelJobProgram;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("DelJobProgram", 13, s_GRID_User_DelJobProgram_TypeInfo_, s_GRID_User_DelJobProgram_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_DelJobProgram)methodFuncL)(
					m_faceL,
					programKeys_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::FindJobProgram(
				IN const GRID_ProgramID& programID_wrap,
				OUT BOOL& found_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->FindJobProgram(
					programID_wrap,
					found_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(programID_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, programID_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"FindJobProgram",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(found_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnFindJobProgram;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("FindJobProgram", 14, s_GRID_User_FindJobProgram_TypeInfo_, s_GRID_User_FindJobProgram_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_FindJobProgram)methodFuncL)(
					m_faceL,
					programID_wrap,
					found_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListJobPrograms(
				OUT tcps_Array<JobProgram>& jobPrograms_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListJobPrograms(
					jobPrograms_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListJobPrograms",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jobPrograms_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListJobPrograms;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListJobPrograms", 15, s_GRID_User_ListJobPrograms_TypeInfo_, s_GRID_User_ListJobPrograms_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListJobPrograms)methodFuncL)(
					m_faceL,
					jobPrograms_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::CommitJob(
				OUT INT64& jobKey_wrap,
				IN const GRID_JobInfo& jobInfo_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->CommitJob(
					jobKey_wrap,
					jobInfo_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(jobInfo_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jobInfo_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"CommitJob",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jobKey_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnCommitJob;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("CommitJob", 9, s_GRID_User_CommitJob_TypeInfo_, s_GRID_User_CommitJob_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_CommitJob)methodFuncL)(
					m_faceL,
					jobKey_wrap,
					jobInfo_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::CancelJob(
				IN const tcps_Array<INT64>& jobKeys_wrap
				) posting_method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->CancelJob(
					jobKeys_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(jobKeys_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jobKeys_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"CancelJob",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnCancelJob;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("CancelJob", 9, s_GRID_User_CancelJob_TypeInfo_, s_GRID_User_CancelJob_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_CancelJob)methodFuncL)(
					m_faceL,
					jobKeys_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::CancelJob_Batch(
				IN const PGRID_User_* wrap_clients,
				IN INT_PTR wrap_clientsCount,
				IN const tcps_Array<INT64>& jobKeys_wrap,
				OUT PGRID_User_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_method
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_clients || wrap_clientsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PGRID_User_, 256> iscm_clients_ar_;
	for(INT_PTR i=0; i<wrap_clientsCount; ++i)
	{
		if(NULL == wrap_clients[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_clients[i]->m_faceL)
		{
			wrap_clients[i]->CancelJob(
					jobKeys_wrap
					);
			continue;
		}
		if(TCPS_OK != wrap_clients[i]->m_faceR->UpdateMethodMatchingFlag_())
			continue;
		if(!wrap_clients[i]->m_faceR->m_methodMatchingFlag.matching_CancelJob)
		{
		//	IPP peerIPP = wrap_clients[i]->m_faceR->GetServingIPP();
		//	NPLogWarning(("The 'GRID_User::CancelJob()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_clients[i]->m_faceR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_clients[i];
				++notReadies;
			}
			continue;
		}
		iscm_clients_ar_.push_back(wrap_clients[i]);
	}
	if(0 == iscm_clients_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 20;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "GRID_User::CancelJob";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// IN tcps_Array<INT64> jobKeys
	iscm_swb_.data = &jobKeys_wrap.LenRef();
	iscm_swb_.len = sizeof(jobKeys_wrap.LenRef());
	iscm_swb_ar_.push_back(iscm_swb_);
	if(jobKeys_wrap.Length() > 0)
	{
		iscm_swb_.data = jobKeys_wrap.Get();
		iscm_swb_.len = sizeof(*(jobKeys_wrap.Get())) * jobKeys_wrap.Length();
		iscm_swb_ar_.push_back(iscm_swb_);
	}

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_clients_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_clients_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_clients_ar_[i]->m_faceR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_clients_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError GRID_User::SetJobPriority(
				IN INT64 jobKey_wrap,
				IN GRID_JobPriority priority_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->SetJobPriority(
					jobKey_wrap,
					priority_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += sizeof(jobKey_wrap);
			inParamsLen += sizeof(priority_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jobKey_wrap);
			iscm_StreamedStore(pInParamsBuf, priority_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"SetJobPriority",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnSetJobPriority;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("SetJobPriority", 14, s_GRID_User_SetJobPriority_TypeInfo_, s_GRID_User_SetJobPriority_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_SetJobPriority)methodFuncL)(
					m_faceL,
					jobKey_wrap,
					priority_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListJobs(
				OUT tcps_Array<JobReport>& jobReports_wrap,
				IN INT32 jobState_wrap,
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListJobs(
					jobReports_wrap,
					jobState_wrap,
					beginTime_wrap,
					endTime_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += sizeof(jobState_wrap);
			inParamsLen += sizeof(beginTime_wrap);
			inParamsLen += sizeof(endTime_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jobState_wrap);
			iscm_StreamedStore(pInParamsBuf, beginTime_wrap);
			iscm_StreamedStore(pInParamsBuf, endTime_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListJobs",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jobReports_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListJobs;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListJobs", 8, s_GRID_User_ListJobs_TypeInfo_, s_GRID_User_ListJobs_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListJobs)methodFuncL)(
					m_faceL,
					jobReports_wrap,
					jobState_wrap,
					beginTime_wrap,
					endTime_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::QueryJobs(
				IN const tcps_Array<INT64>& jobKeys_wrap,
				OUT tcps_Array<JobReport>& jobReports_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->QueryJobs(
					jobKeys_wrap,
					jobReports_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(jobKeys_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jobKeys_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"QueryJobs",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jobReports_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnQueryJobs;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("QueryJobs", 9, s_GRID_User_QueryJobs_TypeInfo_, s_GRID_User_QueryJobs_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_QueryJobs)methodFuncL)(
					m_faceL,
					jobKeys_wrap,
					jobReports_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::UpdateGrid(
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->UpdateGrid(
					files_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(files_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, files_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"UpdateGrid",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnUpdateGrid;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("UpdateGrid", 10, s_GRID_User_UpdateGrid_TypeInfo_, s_GRID_User_UpdateGrid_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_UpdateGrid)methodFuncL)(
					m_faceL,
					files_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListModules(
				OUT tcps_Array<GRID_ProgramInfo>& modules_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListModules(
					modules_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListModules",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(modules_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListModules;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListModules", 11, s_GRID_User_ListModules_TypeInfo_, s_GRID_User_ListModules_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListModules)methodFuncL)(
					m_faceL,
					modules_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::GetLogCount(
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap,
				OUT INT32& logCount_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetLogCount(
					beginTime_wrap,
					endTime_wrap,
					logCount_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += sizeof(beginTime_wrap);
			inParamsLen += sizeof(endTime_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, beginTime_wrap);
			iscm_StreamedStore(pInParamsBuf, endTime_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"GetLogCount",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(logCount_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnGetLogCount;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("GetLogCount", 11, s_GRID_User_GetLogCount_TypeInfo_, s_GRID_User_GetLogCount_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_GetLogCount)methodFuncL)(
					m_faceL,
					beginTime_wrap,
					endTime_wrap,
					logCount_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::LoadLogInfos(
				OUT tcps_Array<LogInfo>& logInfos_wrap,
				IN LTMSEL beginTime_wrap,
				IN LTMSEL endTime_wrap,
				IN INT32 startPos_wrap,
				IN INT32 length_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->LoadLogInfos(
					logInfos_wrap,
					beginTime_wrap,
					endTime_wrap,
					startPos_wrap,
					length_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += sizeof(beginTime_wrap);
			inParamsLen += sizeof(endTime_wrap);
			inParamsLen += sizeof(startPos_wrap);
			inParamsLen += sizeof(length_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, beginTime_wrap);
			iscm_StreamedStore(pInParamsBuf, endTime_wrap);
			iscm_StreamedStore(pInParamsBuf, startPos_wrap);
			iscm_StreamedStore(pInParamsBuf, length_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"LoadLogInfos",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(logInfos_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnLoadLogInfos;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("LoadLogInfos", 12, s_GRID_User_LoadLogInfos_TypeInfo_, s_GRID_User_LoadLogInfos_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_LoadLogInfos)methodFuncL)(
					m_faceL,
					logInfos_wrap,
					beginTime_wrap,
					endTime_wrap,
					startPos_wrap,
					length_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::Login(
				IN const char* userName_wrap, IN INT32 userName_wrap_len /*= -1*/,
				IN const char* password_wrap, IN INT32 password_wrap_len /*= -1*/
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->Login(
					userName_wrap, userName_wrap_len,
					password_wrap, password_wrap_len
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			tcps_String userName_tmp_wrap;
			userName_tmp_wrap.Attach(xat_bind, (char*)userName_wrap, userName_wrap_len);
			inParamsLen += iscm_GetStreamedSize(userName_tmp_wrap);
			tcps_String password_tmp_wrap;
			password_tmp_wrap.Attach(xat_bind, (char*)password_wrap, password_wrap_len);
			inParamsLen += iscm_GetStreamedSize(password_tmp_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, userName_tmp_wrap);
			iscm_StreamedStore(pInParamsBuf, password_tmp_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"Login",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnLogin;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("Login", 5, s_GRID_User_Login_TypeInfo_, s_GRID_User_Login_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_Login)methodFuncL)(
					m_faceL,
					tcps_String(xat_bind, (char*)userName_wrap, userName_wrap_len),
					tcps_String(xat_bind, (char*)password_wrap, password_wrap_len)
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::Logout(
				) posting_method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->Logout(
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"Logout",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnLogout;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("Logout", 6, s_GRID_User_Logout_TypeInfo_, s_GRID_User_Logout_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_Logout)methodFuncL)(
					m_faceL
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::Logout_Batch(
				IN const PGRID_User_* wrap_clients,
				IN INT_PTR wrap_clientsCount,
				OUT PGRID_User_* wrap_sendFaileds /*= NULL*/,
				OUT INT_PTR* wrap_failedCount /*= NULL*/
				) posting_method
{
	if(wrap_failedCount)
		*wrap_failedCount= 0;

	if(NULL==wrap_clients || wrap_clientsCount<=0)
	{
		ASSERT(false);
		return TCPS_ERR_INVALID_PARAM;
	}
	if((!!wrap_sendFaileds) != (!!wrap_failedCount))
	{
		ASSERT(false); // wrap_sendFaileds、wrap_failedCount要么都为NULL，要么都不为NULL
		return TCPS_ERR_INVALID_PARAM;
	}

	INT_PTR notReadies = 0;
	tcps_SmartArray<PGRID_User_, 256> iscm_clients_ar_;
	for(INT_PTR i=0; i<wrap_clientsCount; ++i)
	{
		if(NULL == wrap_clients[i])
		{
			ASSERT_EX(false, tcps_GetErrTxt(TCPS_ERR_INVALID_PARAM));
			continue;
		}
		if(wrap_clients[i]->m_faceL)
		{
			wrap_clients[i]->Logout(
					);
			continue;
		}
		if(TCPS_OK != wrap_clients[i]->m_faceR->UpdateMethodMatchingFlag_())
			continue;
		if(!wrap_clients[i]->m_faceR->m_methodMatchingFlag.matching_Logout)
		{
		//	IPP peerIPP = wrap_clients[i]->m_faceR->GetServingIPP();
		//	NPLogWarning(("The 'GRID_User::Logout()' of '%s' is not matched!", IPP_TO_STR_A(peerIPP)));
			continue;
		}
		if(0 == wrap_clients[i]->m_faceR->m_postingProxy.callerKey_)
		{
			if(wrap_sendFaileds)
			{
				wrap_sendFaileds[notReadies] = wrap_clients[i];
				++notReadies;
			}
			continue;
		}
		iscm_clients_ar_.push_back(wrap_clients[i]);
	}
	if(0 == iscm_clients_ar_.size())
		return TCPS_OK;

	// 准备调用数据
	tcps_SmartArray<SockWriteBuf, 256> iscm_swb_ar_;
	SockWriteBuf iscm_swb_;

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = 0;
	peerCallFlags.dummy_15 = 0;
	iscm_swb_.data = &peerCallFlags;
	iscm_swb_.len = sizeof(peerCallFlags);
	iscm_swb_ar_.push_back(iscm_swb_);

	INT32 call_id_len = 17;
	iscm_swb_.data = &call_id_len;
	iscm_swb_.len = sizeof(call_id_len);
	iscm_swb_ar_.push_back(iscm_swb_);
	iscm_swb_.data = "GRID_User::Logout";
	iscm_swb_.len = call_id_len+1;
	iscm_swb_ar_.push_back(iscm_swb_);

	// 准备callerKeys
	tcps_SmartArray<INT32, 256> iscm_callerKey_ar_;
	iscm_callerKey_ar_.resize(iscm_clients_ar_.size());
	for(INT_PTR i=0; i<(INT_PTR)iscm_clients_ar_.size(); ++i)
		iscm_callerKey_ar_[i] = iscm_clients_ar_[i]->m_faceR->m_postingProxy.callerKey_;

	iscm_IPostingCallerMan& callerMan = iscm_FetchPostingCallerMan();
	if(NULL == wrap_sendFaileds)
	{
		return callerMan.BatchPosting(
							iscm_callerKey_ar_.Get(),
							iscm_callerKey_ar_.size(),
							iscm_swb_ar_.Get(),
							iscm_swb_ar_.size(),
							NULL,
							NULL
							);
	}

	ASSERT(wrap_failedCount);
	tcps_SmartArray<INT_PTR, 256> iscm_queueFullIndexesAr;
	iscm_queueFullIndexesAr.resize(iscm_callerKey_ar_.size());
	TCPSError err = callerMan.BatchPosting(
						iscm_callerKey_ar_.Get(),
						iscm_callerKey_ar_.size(),
						iscm_swb_ar_.Get(),
						iscm_swb_ar_.size(),
						iscm_queueFullIndexesAr.Get(),
						wrap_failedCount
						);
	ASSERT(0<=*wrap_failedCount && *wrap_failedCount<=(INT_PTR)iscm_queueFullIndexesAr.size());
	for(INT_PTR i=0; i<*wrap_failedCount; ++i)
		(wrap_sendFaileds+notReadies)[i] = iscm_clients_ar_[iscm_queueFullIndexesAr[i]];
	*wrap_failedCount += notReadies;
	return err;
}

TCPSError GRID_User::AddUser(
				IN const UserInfo& userInfo_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->AddUser(
					userInfo_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(userInfo_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, userInfo_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"AddUser",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnAddUser;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("AddUser", 7, s_GRID_User_AddUser_TypeInfo_, s_GRID_User_AddUser_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_AddUser)methodFuncL)(
					m_faceL,
					userInfo_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::DelUser(
				IN const tcps_Array<tcps_String>& userNames_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->DelUser(
					userNames_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(userNames_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, userNames_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"DelUser",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnDelUser;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("DelUser", 7, s_GRID_User_DelUser_TypeInfo_, s_GRID_User_DelUser_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_DelUser)methodFuncL)(
					m_faceL,
					userNames_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::UpdatePassword(
				IN const char* oldPassword_wrap, IN INT32 oldPassword_wrap_len /*= -1*/,
				IN const char* newPassword_wrap, IN INT32 newPassword_wrap_len /*= -1*/
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->UpdatePassword(
					oldPassword_wrap, oldPassword_wrap_len,
					newPassword_wrap, newPassword_wrap_len
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			tcps_String oldPassword_tmp_wrap;
			oldPassword_tmp_wrap.Attach(xat_bind, (char*)oldPassword_wrap, oldPassword_wrap_len);
			inParamsLen += iscm_GetStreamedSize(oldPassword_tmp_wrap);
			tcps_String newPassword_tmp_wrap;
			newPassword_tmp_wrap.Attach(xat_bind, (char*)newPassword_wrap, newPassword_wrap_len);
			inParamsLen += iscm_GetStreamedSize(newPassword_tmp_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, oldPassword_tmp_wrap);
			iscm_StreamedStore(pInParamsBuf, newPassword_tmp_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"UpdatePassword",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnUpdatePassword;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("UpdatePassword", 14, s_GRID_User_UpdatePassword_TypeInfo_, s_GRID_User_UpdatePassword_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_UpdatePassword)methodFuncL)(
					m_faceL,
					tcps_String(xat_bind, (char*)oldPassword_wrap, oldPassword_wrap_len),
					tcps_String(xat_bind, (char*)newPassword_wrap, newPassword_wrap_len)
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ManageUser(
				IN const UserInfo& userInfo_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ManageUser(
					userInfo_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(userInfo_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, userInfo_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ManageUser",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnManageUser;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ManageUser", 10, s_GRID_User_ManageUser_TypeInfo_, s_GRID_User_ManageUser_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ManageUser)methodFuncL)(
					m_faceL,
					userInfo_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListAllUsers(
				OUT tcps_Array<UserInfo>& userInfos_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListAllUsers(
					userInfos_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListAllUsers",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(userInfos_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListAllUsers;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListAllUsers", 12, s_GRID_User_ListAllUsers_TypeInfo_, s_GRID_User_ListAllUsers_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListAllUsers)methodFuncL)(
					m_faceL,
					userInfos_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::GetUserInfo(
				IN const char* userName_wrap, IN INT32 userName_wrap_len /*= -1*/,
				OUT UserInfo& userInfo_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetUserInfo(
					userName_wrap, userName_wrap_len,
					userInfo_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			tcps_String userName_tmp_wrap;
			userName_tmp_wrap.Attach(xat_bind, (char*)userName_wrap, userName_wrap_len);
			inParamsLen += iscm_GetStreamedSize(userName_tmp_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, userName_tmp_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"GetUserInfo",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(userInfo_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnGetUserInfo;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("GetUserInfo", 11, s_GRID_User_GetUserInfo_TypeInfo_, s_GRID_User_GetUserInfo_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_GetUserInfo)methodFuncL)(
					m_faceL,
					tcps_String(xat_bind, (char*)userName_wrap, userName_wrap_len),
					userInfo_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListJTMs(
				OUT tcps_Array<tcps_String>& jtms_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListJTMs(
					jtms_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListJTMs",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jtms_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListJTMs;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListJTMs", 8, s_GRID_User_ListJTMs_TypeInfo_, s_GRID_User_ListJTMs_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListJTMs)methodFuncL)(
					m_faceL,
					jtms_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::GetJTMInfo(
				IN const char* jtm_wrap, IN INT32 jtm_wrap_len /*= -1*/,
				OUT JTMInfo& jtmInfo_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetJTMInfo(
					jtm_wrap, jtm_wrap_len,
					jtmInfo_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			tcps_String jtm_tmp_wrap;
			jtm_tmp_wrap.Attach(xat_bind, (char*)jtm_wrap, jtm_wrap_len);
			inParamsLen += iscm_GetStreamedSize(jtm_tmp_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, jtm_tmp_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"GetJTMInfo",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(jtmInfo_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnGetJTMInfo;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("GetJTMInfo", 10, s_GRID_User_GetJTMInfo_TypeInfo_, s_GRID_User_GetJTMInfo_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_GetJTMInfo)methodFuncL)(
					m_faceL,
					tcps_String(xat_bind, (char*)jtm_wrap, jtm_wrap_len),
					jtmInfo_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::GetGridProperty(
				OUT GridProperty& gridProperty_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetGridProperty(
					gridProperty_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"GetGridProperty",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(gridProperty_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnGetGridProperty;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("GetGridProperty", 15, s_GRID_User_GetGridProperty_TypeInfo_, s_GRID_User_GetGridProperty_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_GetGridProperty)methodFuncL)(
					m_faceL,
					gridProperty_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::AddSplitter(
				IN const GRID_ProgramInfo& splitter_wrap,
				IN const tcps_Array<GRID_ProgramFile>& files_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->AddSplitter(
					splitter_wrap,
					files_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(splitter_wrap);
			inParamsLen += iscm_GetStreamedSize(files_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, splitter_wrap);
			iscm_StreamedStore(pInParamsBuf, files_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"AddSplitter",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnAddSplitter;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("AddSplitter", 11, s_GRID_User_AddSplitter_TypeInfo_, s_GRID_User_AddSplitter_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_AddSplitter)methodFuncL)(
					m_faceL,
					splitter_wrap,
					files_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::DelSplitter(
				IN const GRID_ProgramInfo& splitter_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->DelSplitter(
					splitter_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(splitter_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, splitter_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"DelSplitter",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnDelSplitter;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("DelSplitter", 11, s_GRID_User_DelSplitter_TypeInfo_, s_GRID_User_DelSplitter_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_DelSplitter)methodFuncL)(
					m_faceL,
					splitter_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListSplitters(
				OUT tcps_Array<GRID_ProgramID>& splitters_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListSplitters(
					splitters_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListSplitters",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(splitters_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListSplitters;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListSplitters", 13, s_GRID_User_ListSplitters_TypeInfo_, s_GRID_User_ListSplitters_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListSplitters)methodFuncL)(
					m_faceL,
					splitters_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::ListSplitterParam(
				IN const GRID_ProgramID& splitter_wrap,
				OUT tcps_Array<SplitterParam>& splitterParams_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->ListSplitterParam(
					splitter_wrap,
					splitterParams_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;
			inParamsLen += iscm_GetStreamedSize(splitter_wrap);

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			iscm_StreamedStore(pInParamsBuf, splitter_wrap);
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"ListSplitterParam",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(splitterParams_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnListSplitterParam;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("ListSplitterParam", 17, s_GRID_User_ListSplitterParam_TypeInfo_, s_GRID_User_ListSplitterParam_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_ListSplitterParam)methodFuncL)(
					m_faceL,
					splitter_wrap,
					splitterParams_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

TCPSError GRID_User::GetGridStatistics(
				OUT GridStatistics& stat_wrap
				) method
{
	ASSERT(NULL==m_faceR || NULL==m_faceL);
	if(m_faceR)
	{
		return m_faceR->GetGridStatistics(
					stat_wrap
					);
	}
	else if(m_faceL)
	{
		if(m_streamedCallSite.func)
		{
			// 计算输入参数长度
			INT_PTR inParamsLen = 0;
			(void)inParamsLen;

			// 拷贝输入参数到inParamsBuf
			tcps_Binary inParamsBuf;
			if(!inParamsBuf.Resize(inParamsLen))
				return TCPS_ERR_OUT_OF_MEMORY;
			BYTE* pInParamsBuf = inParamsBuf.Get();
			(void)pInParamsBuf;
			ASSERT(pInParamsBuf-inParamsBuf.Get() == inParamsLen);

			// 调用外部流式方法处理函数
			void* replyData = NULL;
			INT_PTR replyLen = 0;
			TCPSError err = m_streamedCallSite.func(
						m_streamedCallSite.serverObj,
						m_streamedCallSite.sessionObj,
						"GRID_User",
						"GetGridStatistics",
						inParamsBuf.Get(),
						inParamsLen,
						&replyData,
						&replyLen
						);
			tcps_Binary autoFreeReplyData;
			if(replyData)
				autoFreeReplyData.Attach(xat_hold, replyData, replyLen);
			if(TCPS_OK != err)
				return err;

			// 解析返回参数
			const BYTE* pReplyData = (const BYTE*)replyData;
			(void)pReplyData;
			iscm_StreamedLoad(stat_wrap, pReplyData);
			ASSERT(pReplyData-(const BYTE*)replyData == replyLen);
			return TCPS_OK;
		}

		if(NULL == m_callSiteL)
		{
			CNPAutoLock locker(m_lock);
			if(NULL == m_callSiteL)
				m_callSiteL = tcps_New(CallSiteL_);
		}
		PROC& methodFuncL = m_callSiteL->fnGetGridStatistics;
		if(NULL == methodFuncL)
		{
			InitializeAllCallsTypeInfo_();
			methodFuncL = m_faceL->FindMethod_("GetGridStatistics", 17, s_GRID_User_GetGridStatistics_TypeInfo_, s_GRID_User_GetGridStatistics_TypeInfo_len_);
			if(NULL == methodFuncL)
				return TCPS_ERR_METHOD_NOT_MATCH;
		}
		return ((IGRID_User_LocalMethod::FN_GetGridStatistics)methodFuncL)(
					m_faceL,
					stat_wrap
					);
	}
	else
		return TCPS_ERR_NEED_SERVE_IPP;
}

void GRID_User::OnNetworkMalformed_()
{
	if(m_faceR)
		m_faceR->OnNetworkMalformed_();
}

void GRID_User::CloseSession_()
{
	this->SetServeIPP_(true, INVALID_IPP);
}

PROC GRID_User::FindCallback_(
				IN const char* name,
				IN INT_PTR nameLen /*= -1*/,
				IN const char* type,
				IN INT_PTR typeLen /*= -1*/
				)
{
	if(NULL == name)
	{
		ASSERT(false);
		return NULL;
	}

	// 对OnStreamedCallback_L_()特殊处理
	if(nameLen<0 && 0==strcmp("OnStreamedCallback_L_", name))
		return (PROC)&OnStreamedCallback_L_;

	InitializeAllCallsTypeInfo_();
	typedef TwoItems<CPtrStrA, PROC> FuncPair;
	typedef CQuickStringMap<CPtrStrA, FuncPair, QSS_Traits<1> > CallbackMap_;
	static CallbackMap_* s_cbMap = NULL;
	if(NULL == s_cbMap)
	{
		IscmRPCGlobalLocker locker;
		if(NULL == s_cbMap)
		{
			static CallbackMap_ s_cbMapObj;
			CallbackMap_& cbMap = s_cbMapObj;
			cbMap.insert(std::make_pair(CONST_STR_TO_PTRSTR_A("ReplyCommitJob"), FuncPair(CPtrStrA(s_GRID_User_ReplyCommitJob_TypeInfo_, s_GRID_User_ReplyCommitJob_TypeInfo_len_), (PROC)&Local_ReplyCommitJob)));
			s_cbMap = &cbMap;
		}
	}

	CallbackMap_::iterator it = s_cbMap->find(name, nameLen);
	if(s_cbMap->end() == it)
		return NULL;
	const CPtrStrA& ps = it->second.first;
	if(0 != ps.Compare(type, typeLen))
		return NULL;
	return it->second.second;
}

TCPSError GRID_User::OnStreamedCallback_L_(
				IN void* sessionObj,
				IN const char* callbackName,
				IN INT_PTR nameLen /*= -1*/,
				IN const void* data /*= NULL*/,
				IN INT_PTR dataLen /*>= 0*/,
				OUT LPVOID* replyData /*= NULL*/,
				OUT INT_PTR* replyLen /*= NULL*/
				)
{
	if(replyData)
		*replyData = NULL;
	if(replyLen)
		*replyLen = 0;

	GRID_User* const gRID_UserObj = (GRID_User*)sessionObj;
	ASSERT(gRID_UserObj->m_faceL);
	if(nameLen < 0)
		nameLen = strlen(callbackName);

	void* iscm_replyData = NULL;
	INT_PTR iscm_replyLen = 0;
	TCPSError err = gRID_UserObj->StreamedCallback_(NULL, 0, NULL, 0, iscm_replyData, iscm_replyLen);
	if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
	{
		if(!gRID_UserObj->IsStreamedCallbackMatched_(callbackName, nameLen))
			return TCPS_ERR_CALLBACK_NOT_MATCH;
		iscm_replyData = NULL;
		iscm_replyLen = 0;
		err = gRID_UserObj->StreamedCallback_(callbackName, nameLen, data, dataLen, iscm_replyData, iscm_replyLen);
		if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
		{
			if(replyData)
				*replyData = iscm_replyData;
			if(replyLen)
				*replyLen = iscm_replyLen;
			return err;
		}
	}

	INT_PTR call_id_len = 11+nameLen;
	CSmartBuf<char, 256> call_id_buf(call_id_len+1);
	char* const call_id = call_id_buf.Get();
	strncpy(call_id, "GRID_User::", 11);
	StrAssign(call_id+11, callbackName, nameLen);

	GRID_User_RC::InitCallbackMap_();
	ASSERT(GRID_User_RC::sm_callbackMap);
	GRID_User_RC::CallbackMap_::const_iterator cit = GRID_User_RC::sm_callbackMap->find(call_id, call_id_len);
	if(GRID_User_RC::sm_callbackMap->end() == cit)
		return TCPS_ERR_TRY_CALL_OTHER_METHODS; // 本接口不支持，尝试其它接口

	iscm_PeerCallFlags peerCallFlags;
	peerCallFlags.sizeofBOOL_req = (INT8)sizeof(BOOL);
	peerCallFlags.sizeofEnum_req = (INT8)sizeof(DummyEnumType);
	peerCallFlags.requireReply = !(cit->second.isPosting);
	peerCallFlags.dummy_15 = 0;
	const BYTE* ptrInParams = (const BYTE*)data;
	INT_PTR ptrInParamsLen = dataLen;
	if(peerCallFlags.requireReply)
	{
		iscm_RPCDataOutfiter outfiter;
		err = cit->second.handler(NULL, gRID_UserObj, peerCallFlags, ptrInParams, ptrInParamsLen, &outfiter);
		if(TCPS_OK != err)
			return err;
		// outfiter.swbBufs_[0]总指向iscm_RPCDataOutfiter::reply_
		// outfiter.swbBufs_[1]为iscm_RPCReplyPrefix replyPrefix
		ASSERT(outfiter.swbBufs_.size()==1 || outfiter.swbBufs_.size()>=2);
		if(outfiter.swbBufs_.size() >= 2)
		{
			ASSERT(replyData && replyLen);
			*replyLen = SockTotalizeWriteBufVec(outfiter.swbBufs_.Get()+2, outfiter.swbBufs_.size()-2);
			*replyData = tcps_Alloc(*replyLen);
			BYTE* q = (BYTE*)*replyData;
			for(INT_PTR i=2; i<(INT_PTR)outfiter.swbBufs_.size(); ++i)
			{
				const SockWriteBuf& swb = outfiter.swbBufs_[i];
				memcpy(q, swb.data, swb.len);
				q += swb.len;
			}
			ASSERT(q-(BYTE*)*replyData == *replyLen);
		}
	}
	else
	{
		err = cit->second.handler(NULL, gRID_UserObj, peerCallFlags, ptrInParams, ptrInParamsLen, NULL/*outfiter*/);
	}

	return err;
}

TCPSError GRID_User::Local_ReplyCommitJob(
				IN void* sessionObj,
				IN INT64 jobKey_wrap,
				IN TCPSError replyCode_wrap,
				IN const tcps_Binary& context_wrap
				) posting_callback
{
	GRID_User* const gRID_UserObj_wrap = (GRID_User*)sessionObj;
	void* iscm_replyData = NULL;
	INT_PTR iscm_replyLen = 0;
	TCPSError err = gRID_UserObj_wrap->StreamedCallback_(NULL, 0, NULL, 0, iscm_replyData, iscm_replyLen);
	if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
	{
		if(!gRID_UserObj_wrap->IsStreamedCallbackMatched_("ReplyCommitJob", 14))
			return TCPS_ERR_CALLBACK_NOT_MATCH;

		DataOutfiter dataOutfiter;

		// IN INT64 jobKey
		Put_BaseType_(&dataOutfiter, jobKey_wrap);

		// IN TCPSError replyCode
		Put_BaseType_(&dataOutfiter, replyCode_wrap);

		// IN tcps_Binary context
		Put_Binary_(&dataOutfiter, context_wrap.Get(), context_wrap.LenRef());

		ISCM_GAIN_TEMPORARY_CONTINUOUS_CALL_DATA(dataOutfiter.bufs_.Get(), dataOutfiter.bufs_.size(), iscm_data, iscm_dataLen);
		iscm_replyData = NULL;
		iscm_replyLen = 0;
		err = gRID_UserObj_wrap->StreamedCallback_(
						"ReplyCommitJob", 14,
						iscm_data, iscm_dataLen,
						iscm_replyData, iscm_replyLen
						);
		ASSERT(NULL==iscm_replyData && 0==iscm_replyLen);
		if(iscm_replyData)
			tcps_Free(iscm_replyData);
		if(TCPS_ERR_STREAMED_CALLBACK_NOT_IMPLEMENTED != err)
			return err;
		// 忽略返回TCPS_ERR_MALFORMED_REQ情况？？
	}

	return gRID_UserObj_wrap->ReplyCommitJob(
					jobKey_wrap,
					replyCode_wrap,
					context_wrap
					);
}
