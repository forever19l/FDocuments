#ifndef _MGR_H_
#define _MGR_H_

/***************************************************************
* Purpose:   单件定义基类
* Author:    wuzx ()
* Created:   2016-04-27
**************************************************************/

#include <assert.h>
#include <string>

template <class MgrT> class Mgr
{
public:
	static inline bool Valid() { return instance; }
	static inline MgrT* Get()
	{
		if (instance == NULL)
		{
			instance = new MgrT();
			instance->Init();
		}

		return instance;
	}

protected:
	static void Free()
	{
		if (instance != NULL)
		{
			instance->UnInit();
			delete instance;
			instance = NULL;
		}
	}

	virtual bool Init() { return true; }
	virtual bool UnInit() { return true; }
	Mgr() { assert(Mgr<MgrT>::instance == NULL); }
	virtual ~Mgr() { Mgr<MgrT>::instance = NULL; }

private:
	static MgrT *instance;
	explicit Mgr(const Mgr<MgrT>&)         {}
	Mgr<MgrT>& operator=(Mgr<MgrT> const&) {}

};

template <class MgrT> MgrT*  Mgr<MgrT>::instance = NULL;


#endif // __MGR_H
