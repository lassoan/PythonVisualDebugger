// \addtodoc

/*!  \class		CNxNSyncEvent NxNSyncEvent.hpp
 *
 *  \brief		This is a wrapper class for event objects used for synchronizing threads
 *				and processes sharing the same resources. 
 *
 *  \author		Axel Pfeuffer
 *
 *  \version	1.0
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-08-09-2000 file created.
 *      [jr]-12-18-2000 Internationalization
 *	\endmod
 */

//
// This file was changed during internationalization on 12/18/2000 by Jens.


#ifndef INC_NXN_SYNCEVENT_HPP
#define INC_NXN_SYNCEVENT_HPP

#ifndef _INC_WINDOWS
#	include <windows.h>
#endif
#ifndef _INC_PROCESS
#	include <process.h>
#endif


//---------------------------------------------------------------------------
//	event flags
//---------------------------------------------------------------------------
#define NXN_SYNCEVENT_SIGNALED			0x00000001		// event is set to signaled after startup

class CNxNSyncEvent  {
	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------
		CNxNSyncEvent(DWORD dwEventFlags = 0);
		virtual ~CNxNSyncEvent();

		//---------------------------------------------------------------------------
		//	event methods
		//---------------------------------------------------------------------------
		void Signal();
		void Wait();
		void Reset();
		bool TimedWait(long lTime);

		HANDLE GetHandle() { return m_hEvent; };

	private:
		HANDLE m_hEvent;
};


/*!	\fn			inline CNxNSyncEvent::CNxNSyncEvent(DWORD dwEventFlags)
 *	
 *				Creates an event object. This is a very basic object and is important 
 *				that it can be created, otherwise synchronizing threads is not possible.
 *	
 *	\param		dwEventFlags is a dword containing some intial flags for the event object
 *	
 *	\return		none
 *	
 *	\note		
 */
inline CNxNSyncEvent::CNxNSyncEvent(DWORD dwEventFlags)
{
	
	BOOL bInitialState = (dwEventFlags & NXN_SYNCEVENT_SIGNALED)?TRUE:FALSE;

    m_hEvent = CreateEvent(NULL, FALSE, bInitialState, NULL);
/*    if (!m_hEvent) {
        // Internationalization change (12/18/2000 by Jens): Replaced hard coded text.
        //      Must be read from registry.
        CLanguageString sText( g_NxNIntegratorSDKLanguageModule, 
                               IDS_EVENT_CREATION_FAILED );

        sText.MessageBox( LANG_GET_MAIN_AFX_WND, MB_OK|MB_ICONWARNING);
	}
*/
}


/*!	\fn			inline CNxNSyncEvent::~CNxNSyncEvent()
 *	
 *				Destroys the event object.
 *	
 *	\param		none
 *	
 *	\return		none
 *	
 *	\note		
 */
inline CNxNSyncEvent::~CNxNSyncEvent()
{
	CloseHandle(m_hEvent);
}


/*!	\fn			inline void CNxNSyncEvent::Signal()
 *	
 *				This method signals the event. 
 *	
 *	\param		none
 *	
 *	\return		none
 *	
 *	\note		
 */
inline void CNxNSyncEvent::Signal()
{
	SetEvent(m_hEvent);
}

/*!	\fn			inline void CNxNSyncEvent::Reset()
 *	
 *				This method resets the signal state of the event.
 *	
 *	\param		none
 *	
 *	\return		none
 *	
 *	\note		
 */
inline void CNxNSyncEvent::Reset()
{
	ResetEvent(m_hEvent);
}


/*!	\fn			inline void CNxNSyncEvent::Wait()
 *	
 *				This method waits for a signal. If a thread calls this method it will reside
 *				inside this method until another process signals this object. (call to Signal())
 *	
 *	\param		none
 *	
 *	\return		none
 *	
 *	\note		
 */
inline void CNxNSyncEvent::Wait()
{
    WaitForSingleObject(m_hEvent, INFINITE);
}

/*!	\fn			inline bool CNxNSyncEvent::TimedWait(long lTime)
 *	
 *				This method waits a specified time frame (in milliseconds) for a signal.
 *	
 *	\param		lTime is a long variable containing the tiem frame in milliseconds 
 *				to wait for a signal
 *			
 *	\return		A boolean expression, if no signal occurred in the time frame then this method
 *				will return false, but if a signal occurred inside the time frame then true
 *				will be returned.
 *	
 *	\note		
 */
inline bool CNxNSyncEvent::TimedWait(long lTime)
{
	int nResult = WaitForSingleObject(m_hEvent, lTime);
	return (WAIT_TIMEOUT == nResult) ? false : true;
}

#endif // INC_NXN_EVENT_HPP
