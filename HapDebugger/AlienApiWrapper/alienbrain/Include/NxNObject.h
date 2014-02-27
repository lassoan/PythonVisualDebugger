// \addtodoc

#ifndef INC_NXN_OBJECT_H
#define INC_NXN_OBJECT_H

/* \class		CNxNObject
 *
 *	\file		NxNObject.h
 *
 *  \brief		CNxNObject is a very basic NxN class from which other NxN classes are derived.  This class has no  *				functionality by default, except for error handling.  The following data members are relevant  *				for error handling:
 *				<ul>
 *					<li><b>m_ulLastError:</b> Contains the encoded error code.
 *					<li><b>m_strLastErrorMessage:</b> Contains the error code as a string.
 *				</ul>
 *
 *  \author		Axel Pfeuffer, Helmut Klinger
 *
 *  \version	1.00
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-05-02-2000 file created.
 *	\endmod
 */

//---------------------------------------------------------------------------
//	hidden structure for data storage
//---------------------------------------------------------------------------
typedef struct _tNxNObjectData*	tNxNObjectData;

class NXNINTEGRATORSDK_API CNxNObject {
	NXN_DECLARE_DYNCREATE(CNxNObject);

	public:
		CNxNObject();
		virtual ~CNxNObject();

		unsigned long GetLastError() const;
		CNxNString GetLastErrorMessage() const;

//		unsigned long AddRef();
//		unsigned long Release();

	protected:
		void SetLastError(unsigned long ulLastError) const;

	private:
		tNxNObjectData	m_pObjectData;
		unsigned long	m_ulLastError;
		CNxNString		m_strLastErrorMessage;
		unsigned long	m_ulRef;
};

#endif // INC_NXN_OBJECT_H
