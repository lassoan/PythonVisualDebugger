// \addtodoc

#ifndef INC_NXN_TYPE_H
#define INC_NXN_TYPE_H

/* \class		CNxNType
 *
 *	\file		NxNType.h
 *
 *  \brief		CNxNType is a class for using object types. In alienbrain
 *				object types are represented by path strings.
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
//	public type constants
//---------------------------------------------------------------------------

// node objects (derived from CNxNObject)
#define NXN_TYPE_NODE				""
#define NXN_TYPE_NODELIST			"NodeList"
#define NXN_TYPE_MENU				"Menu"

// extended nodes (separated into categories, derived from CNxNNode)
#define NXN_TYPE_INTEGRATOR			"Integrator"
#define NXN_TYPE_EVENTMANAGER		"EventManager"
#define NXN_TYPE_DBNODE				"\\Workspace\\DbItem\\"
#define NXN_TYPE_VIRTUALNODE		""

// db nodes (derived from CNxNDbNode)
#define NXN_TYPE_DBNODELIST			"DbNodeList"
#define NXN_TYPE_PROJECT			"\\Workspace\\DbItem\\FileFolder\\Folder\\Project\\"
#define NXN_TYPE_FOLDER				"\\Workspace\\DbItem\\FileFolder\\Folder\\Asset\\"
#define NXN_TYPE_FILE				"\\Workspace\\DbItem\\FileFolder\\File\\Asset\\"

// virutal nodes (derived from CNxNVirtualNode)
#define NXN_TYPE_WORKSPACE			"\\Workspace\\Workspace\\"
#define NXN_TYPE_DISKITEM			"\\Workspace\\DiskItem\\"
#define NXN_TYPE_VERSIONCONTROLITEM	"\\VersionControl\\"

// version control stuff - history items, admin (derived from CNxNVersionControlItem)
#define NXN_TYPE_HISTORY			"\\VersionControl\\History\\"
#define NXN_TYPE_HISTORYLABEL		"\\VersionControl\\Label\\"
#define NXN_TYPE_HISTORYVERSION		"\\VersionControl\\Version\\"
// version control - admin (derived from CNxNVersionControlItem)
#define NXN_TYPE_ADMIN_ROOT			"\\VersionControl\\Admin\\"
// version control - admin - bucket folder 
#define NXN_TYPE_BUCKET_FOLDER		"\\VersionControl\\Buckets\\"
// version control - bucket
#define NXN_TYPE_BUCKET				"\\VersionControl\\Buckets\\Bucket\\"

// disk item nodes (derived from CNxNDiskItem)
#define NXN_TYPE_DISKITEM_FOLDER    "\\Workspace\\DiskItem\\Folder\\"
#define NXN_TYPE_DISKITEM_FILE      "\\Workspace\\DiskItem\\File\\"

#define NXN_TYPE_GLOBALSELECTION	"GlobalSelection"

//---------------------------------------------------------------------------
//	public constants
//---------------------------------------------------------------------------
const CNxNPath NXN_TYPE_NAMESPACE_ROOT(L"\\");	// Namespace types for the root object 

//---------------------------------------------------------------------------
//	hidden structure for data storage
//---------------------------------------------------------------------------
typedef struct _tNxNTypeData*	tNxNTypeData;

class NXNINTEGRATORSDK_API CNxNType {
	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------
		CNxNType();
		CNxNType(const CNxNType& tType);
		CNxNType(const CNxNString& strType);
		CNxNType(const CNxNString& strParentType, const CNxNString& strType);
		virtual ~CNxNType();

		//---------------------------------------------------------------------------
		//	overloaded operators
		//---------------------------------------------------------------------------
		operator=(const CNxNType& tType);
		operator=(const CNxNString& strString);
		operator const CNxNString& () const;
		operator const CNxNPath& () const;
	

		//---------------------------------------------------------------------------
		//	type information retrieval
		//---------------------------------------------------------------------------
		long GetLength() const;
		bool IsEmpty() const;
		bool IsValid(const CNxNString& strString) const;
		virtual bool InheritsFrom(const CNxNType& tType) const;
		virtual bool InheritsFrom(const CNxNString& strType) const;

		virtual bool DerivesFrom(const CNxNType& tType) const;
		virtual bool DerivesFrom(const CNxNString& strType) const;

		//---------------------------------------------------------------------------
		//	direct access to type object data
		//---------------------------------------------------------------------------
		inline tNxNTypeData GetTypeData() const { return m_pTypeData; };

	private:
		tNxNTypeData		m_pTypeData;
};

//bool NXNINTEGRATORSDK_API operator == (const CNxNBSTR&, const CNxNBSTR&);

#endif // INC_NXN_TYPE_H

