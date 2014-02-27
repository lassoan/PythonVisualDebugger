// \addtodoc

#ifndef INC_NXN_EVENTMANAGER_H
#define INC_NXN_EVENTMANAGER_H

/*	\class		CNxNEventManager NxNEventManager.h
 *
 *  \brief		This class is responsible for the event message routing mechanism introduced
 *				with alienbrain 4.0.
 *
 *				The alienbrain namespace is capable of event message delivering. Each instance
 *				of the CNxNEventManager registeres itself at the alienbrain namespace instance
 *				for receiving a set of event messages. On the other hand the event manager
 *				has got dispatching capabilities as you may known from the Microsoft Foundation
 *				Classes (Windows message routing).<br><br>
 *				To process an event message from the aleinbrain namespace, it is necessary to
 *				register an event target (see CNxNEventTarget) at the event manager. To do so
 *				you just derive a new user class from the CNxNEventTarget class and implement
 *				an event map, which holds the information for the event message (see CNxNEventMsg) 
 *				to be processed. Each event map entry has got it's own event processing 
 *				method like in the MFC's message map mechanism.<br><br>
 *				In general you can have as much event manager and event targets as you like, but
 *				you should keep in mind that each registered event manager instance will slow
 *				down the processing speed of the alienbrain namespace.<br>
 *				The CNxNIntegrator object has got event manager by default, which you
 *				can easily get using the CNxNIntegrator::GetEventManager() method. See
 *				CNxNIntegrator class for more details about this issue.
 *
 *  \author		Axel Pfeuffer, Helmut Klinger
 *
 *  \version	1.00
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-07-17-2000 file created.
 *	\endmod
 */

//---------------------------------------------------------------------------
//	hidden structure for data storage
//---------------------------------------------------------------------------
typedef struct _tNxNEventManagerData*	tNxNEventManagerData;

class NXNINTEGRATORSDK_API CNxNEventManager : public CNxNObject {
	NXN_DECLARE_DYNCREATE(CNxNEventManager);

	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------
		CNxNEventManager();
		virtual ~CNxNEventManager();

		//---------------------------------------------------------------------------
		//	initialization/deinitialization
		//---------------------------------------------------------------------------
		bool InitInstance();
		bool ExitInstance();

		//---------------------------------------------------------------------------
		//	event target registration/unregistration
		//---------------------------------------------------------------------------
		bool RegisterEventTarget(CNxNEventTarget* pEventTarget);
		bool UnregisterEventTarget(CNxNEventTarget* pEventTarget);

		//---------------------------------------------------------------------------
		//	event dispatcher
		//---------------------------------------------------------------------------
		bool Dispatch(CNxNEventMsg& eEvent);

		//---------------------------------------------------------------------------
		//	static object type information
		//---------------------------------------------------------------------------
		static CNxNType NXNINTEGRATORSDK_API_CALL GetType() { return CNxNType(NXN_TYPE_EVENTMANAGER); };

	private:

		tNxNEventManagerData	m_pEventManagerData;
};

#endif // INC_NXN_EVENTMANAGER_H
