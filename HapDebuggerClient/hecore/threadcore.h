#ifndef	_THREADCORE_H_
#define	_THREADCORE_H_


#ifdef _MT
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 
	#include <Windows.h>
#elif defined(__APPLE__)
	#include <multiprocessing.h>
#endif
#endif


namespace HumongousEntertainment
{

//The following code is only applicable with multi-threaded builds
#ifdef _MT

//forward reference (see below)
class CCodeSingleUseEntry;


///////////////////////////////////////////////////////////////////////////////
// class CCodeSingleUseGate
//
// This class is used in conjunction with the CCodeSingleUseEntry to protect a
// resource or section of code from being used at the same time by two different
// threads.
//
// Each resource that needs to be protected should have an instance of this class
// associated with it.  Before accessing the resource, an instance of 
// CCodeSingleUseEntry should be created with a reference to the associated 
// CCodeSingleUseGate object.  This constructor will block until all other 
// threads have released their Entry objects.  

class CCodeSingleUseGate
{
	//allow access to Enter and Leave functions
	friend class CCodeSingleUseEntry;

public:
	CCodeSingleUseGate();
	~CCodeSingleUseGate();

private:
	//no copying allowed
	CCodeSingleUseGate(const CCodeSingleUseEntry& Copy);
	CCodeSingleUseGate& operator=(const CCodeSingleUseEntry& Copy);

	//these functions are accessible through the constructor and destructor
	//of the CCodeSingleUseEntry class
	
	//call to enter the protected section - blocks until the gate is clear
	void EnterGate();
	//leave the protected section
	void LeaveGate();

private:
	//member data - 

#ifdef	WIN32
	//critical section under windows
	CRITICAL_SECTION m_CritSection;

//#elif defined(__APPLE__)
	//critical 'region' under mac
//	MPCriticalRegionID m_CritRegion;

#else
#error Unknown operating system - needs critical section implementation.
#endif

}; //CCodeSingleUseGate

///////////////////////////////////////////////////////////////////////////////
// class CCodeSingleUseEntry
//
// This class is used to gain access to a resource protected by a CCodeSingleUseGate
// object.  Simply construct an instance of this object with a reference to the 
// gate object before accessing the resource, when the object is destroyed, the
// resource will be open to access once more.

class CCodeSingleUseEntry
{
public:
	CCodeSingleUseEntry(CCodeSingleUseGate& Gate);
	~CCodeSingleUseEntry();

private:
	//no copying allowed
	CCodeSingleUseEntry(const CCodeSingleUseEntry& Copy);
	CCodeSingleUseEntry& operator=(const CCodeSingleUseEntry& Copy);

private:
	//reference to the gate object that we are entering
	CCodeSingleUseGate& m_rGate;

}; // CCodeSingleUseEntry
#endif //_MT - end of multi thread only code

}	// End of he namespace

namespace he = HumongousEntertainment;



#endif // _THREADCORE_H_
