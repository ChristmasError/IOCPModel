#pragma once

#include "io_context_pool_class.h"
#include <iostream>

// IOContextPool中的初始数量
#define INIT_IOCONTEXT_NUM (100)

IOContextPool::IOContextPool()
{
	InitializeCriticalSection(&m_csLock);

	contextList.clear();
	EnterCriticalSection(&m_csLock);
	for (size_t i = 0; i < INIT_IOCONTEXT_NUM; i++)
	{
		LPPER_IO_CONTEXT context = new PER_IO_CONTEXT;
		contextList.push_back(context);
	}
	LeaveCriticalSection(&m_csLock);
	std::cout << "IOContextPool 初始化完成\n";
}

IOContextPool::~IOContextPool()
{
	EnterCriticalSection(&m_csLock);

	for (std::list<LPPER_IO_CONTEXT>::iterator it = contextList.begin(); it != contextList.end(); it++)
	{
		delete (*it);
	}
	contextList.clear();
	LeaveCriticalSection(&m_csLock);

	DeleteCriticalSection(&m_csLock);
}

LPPER_IO_CONTEXT IOContextPool::AllocateIoContext()
{
	LPPER_IO_CONTEXT context = NULL;

	EnterCriticalSection(&m_csLock);
	if (contextList.size() > 0) //list不为空，从list中取一个
	{
		context = contextList.back();
		contextList.pop_back();
	}
	else	//list为空，新建一个
	{
		context = new PER_IO_CONTEXT;
	}
	LeaveCriticalSection(&m_csLock);
	return context;
}

void IOContextPool::ReleaseIOContext(LPPER_IO_CONTEXT pIOContext)
{	
	pIOContext->Reset();
	EnterCriticalSection(&m_csLock);
	this->contextList.push_front(pIOContext);
	LeaveCriticalSection(&m_csLock);
}