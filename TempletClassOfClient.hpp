
class XXXGRID_User : public GRID_User
{
private:
	XXXGRID_User(const XXXGRID_User&);
	XXXGRID_User& operator= (const XXXGRID_User&);

public:
	XXXGRID_User()
		{}
	virtual ~XXXGRID_User()
		{	DestroyRequester();	}

/// 基本网络事件
protected:
	virtual TCPSError OnConnected()
	{
		// TODO: 请添加连接后处理
		return TCPS_OK;
	}
	virtual void OnPeerBroken()
	{
		// TODO: 请添加对端断线处理
	}
	virtual void OnClose()
	{
		// TODO: 请添加连接关闭处理
	}
	virtual void OnConnectTimeout(IN TCPSError cause)
	{
		// TODO: 请添加连接超时处理
		(void)cause;
	}

/// 回调
protected:
	virtual TCPSError ReplyCommitJob(
				IN INT64 jobKey,
				IN TCPSError replyCode,
				IN const tcps_Binary& context
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_OK;
	}
};

class XXXPCC_Scatter : public PCC_Scatter
{
private:
	XXXPCC_Scatter(const XXXPCC_Scatter&);
	XXXPCC_Scatter& operator= (const XXXPCC_Scatter&);

public:
	XXXPCC_Scatter()
		{}
	virtual ~XXXPCC_Scatter()
		{	DestroyRequester();	}

/// 基本网络事件
protected:
	virtual TCPSError OnConnected()
	{
		// TODO: 请添加连接后处理
		return TCPS_OK;
	}
	virtual void OnPeerBroken()
	{
		// TODO: 请添加对端断线处理
	}
	virtual void OnClose()
	{
		// TODO: 请添加连接关闭处理
	}
	virtual void OnConnectTimeout(IN TCPSError cause)
	{
		// TODO: 请添加连接超时处理
		(void)cause;
	}

/// 回调
protected:
	virtual TCPSError Compute(
				IN INT64 moduleKey,
				IN INT64 taskKey,
				IN const tcps_String& inputUrl,
				IN const tcps_String& outputUrl,
				IN const tcps_Binary& moduleParams
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

	virtual TCPSError AddModule(
				IN INT64 moduleKey,
				IN const tcps_Array<PCC_ModuleFile>& moudleFiles
				) callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

	virtual TCPSError RemoveModule(
				IN INT64 moduleKey
				) callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

	virtual TCPSError FindModule(
				IN INT64 moduleKey,
				OUT BOOL& found
				) callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_OK;
	}
};

class XXXPCC_Service : public PCC_Service
{
private:
	XXXPCC_Service(const XXXPCC_Service&);
	XXXPCC_Service& operator= (const XXXPCC_Service&);

public:
	XXXPCC_Service()
		{}
	virtual ~XXXPCC_Service()
		{	DestroyRequester();	}

/// 基本网络事件
protected:
	virtual TCPSError OnConnected()
	{
		// TODO: 请添加连接后处理
		return TCPS_OK;
	}
	virtual void OnPeerBroken()
	{
		// TODO: 请添加对端断线处理
	}
	virtual void OnClose()
	{
		// TODO: 请添加连接关闭处理
	}
	virtual void OnConnectTimeout(IN TCPSError cause)
	{
		// TODO: 请添加连接超时处理
		(void)cause;
	}

/// 回调
protected:
	virtual TCPSError OnExecuted(
				IN INT64 jobKey,
				IN TCPSError errorCode,
				IN const tcps_Binary& context
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_ERR_NOT_IMPLEMENTED;
	}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_OK;
	}
};

class XXXPCC_Toolkit : public PCC_Toolkit
{
private:
	XXXPCC_Toolkit(const XXXPCC_Toolkit&);
	XXXPCC_Toolkit& operator= (const XXXPCC_Toolkit&);

public:
	XXXPCC_Toolkit()
		{}
	virtual ~XXXPCC_Toolkit()
		{	DestroyRequester();	}

/// 基本网络事件
protected:
	virtual TCPSError OnConnected()
	{
		// TODO: 请添加连接后处理
		return TCPS_OK;
	}
	virtual void OnPeerBroken()
	{
		// TODO: 请添加对端断线处理
	}
	virtual void OnClose()
	{
		// TODO: 请添加连接关闭处理
	}
	virtual void OnConnectTimeout(IN TCPSError cause)
	{
		// TODO: 请添加连接超时处理
		(void)cause;
	}

protected:
	// 返回TCPS_OK允许重定向；返回非TCPS_OK禁止重定向
	virtual TCPSError OnPrepareRedirect_(
				IN const IPP& redirectIPP,
				IN const tcps_Binary& redirectParam
				) posting_callback
	{
		// TODO: 请实现此函数
		return TCPS_OK;
	}
};
