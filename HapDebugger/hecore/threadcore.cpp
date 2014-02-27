///////////////////////////////////////////////////////////////////////////////
// threadcore.cpp
//
// Implementation file for objects related to multi-platform multi threading.
//
///////////////////////////////////////////////////////////////////////////////
#include "priv_coreprecompiled.h"
#include "threadcore.h"

namespace HumongousEntertainment
{

///////////////////////////////////////////////////////////////////////////////
// The Windows version of the CCodeSingleUseGate class implementation
#ifdef WIN32

	#ifdef	_MT
		#define	MULTI_THREADING_ENABLED
		CCodeSingleUseGate::CCodeSingleUseGate()
		{
			InitializeCriticalSection(&m_CritSection);
		}

		CCodeSingleUseGate::~CCodeSingleUseGate()
		{
			DeleteCriticalSection(&m_CritSection);
		}

		void CCodeSingleUseGate::EnterGate()
		{
			EnterCriticalSection(&m_CritSection);
		}

		void CCodeSingleUseGate::LeaveGate()
		{
			LeaveCriticalSection(&m_CritSection);
		}	
	#endif

///////////////////////////////////////////////////////////////////////////////
// The Mac version of the CCodeSingleUseGate class implementation
/* commented out for now..
#elif defined(__APPLE__)	


CCodeSingleUseGate::CCodeSingleUseGate()
{
	MPCreateCriticalRegion(&m_CritRegion);
}

CCodeSingleUseGate::~CCodeSingleUseGate()
{
	MPDeleteCriticalRegion(m_CritRegion);
}

void CCodeSingleUseGate::EnterGate()
{
	MPEnterCriticalRegion (m_CritRegion, kDurationForever);
}

void CCodeSingleUseGate::LeaveGate()
{
	MPExitCriticalRegion (m_CritRegion);
}
*/
#elif defined(VK_CUBE)
	// Not used.
#else
#error Unknown operating system needs multi thread implementation
#endif



///////////////////////////////////////////////////////////////////////////////
// CCodeSingleUseEntry::CCodeSingleUseEntry
//
// Construct a CCodeSingleUseEntry object by gaining exclusive access to a
// CCodeSingleUseGate - block until the object is available.
//
// Inputs
//		Gate - reference to a CCodeSingleUseGate object
//
///////////////////////////////////////////////////////////////////////////////
#ifdef MULTI_THREADING_ENABLED
CCodeSingleUseEntry::CCodeSingleUseEntry(CCodeSingleUseGate& Gate)
	:m_rGate(Gate)
{
	m_rGate.EnterGate();
}

///////////////////////////////////////////////////////////////////////////////
// CCodeSingleUseEntry::~CCodeSingleUseEntry
//
// Destroy a CCodeSingleUseEntry object by releasing exclusive access to the
// CCodeSingleUseGate
//
///////////////////////////////////////////////////////////////////////////////
CCodeSingleUseEntry::~CCodeSingleUseEntry()
{
	m_rGate.LeaveGate();
}
#endif


}	// End of he namespace
