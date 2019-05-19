#pragma once

#include "per_io_context_struct.h"
#include "cs_auto_lock_class.h"

//========================================================
//                 
//		       IOContextPool，单例IOContext池
//
//========================================================

class IOContextPool
{
private:
	std::list<LPPER_IO_CONTEXT> contextList;
	CRITICAL_SECTION m_csLock;
public:
	IOContextPool();
	~IOContextPool();

	// 分配一个IOContxt
	LPPER_IO_CONTEXT AllocateIoContext();

	// 回收一个IOContxt
	void ReleaseIOContext(LPPER_IO_CONTEXT pContext);

};


