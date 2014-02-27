// \addtodoc

#ifndef INC_NXN_WORKSPACE_H
#define INC_NXN_WORKSPACE_H

/* \class		CNxNWorkspace
 *
 *	\file		NxNWorkspace.h
 *
 *  \brief		CNxNWorkspace encapsulates a workspace node. Each opened or created workspace
 *				has got a local workspace file having an '.nwk' extension. Usually they contain
 *				project object and thus are used to create, insert, load, unload and remove projects.
 *				A workspace object implicitly needs a CNxNIntegraotr object as a parent.
 *
 *  \author		Axel Pfeuffer, Helmut Klinger
 *
 *  \version	1.00
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-05-02-2000 file created.
 *      [jr]-12-18-2000 Internationalization
 *	\endmod
 */

//
// This file was changed during internationalization on 12/18/2000 by Jens.

//---------------------------------------------------------------------------
//	hidden structure for data storage
//---------------------------------------------------------------------------
typedef struct _tNxNWorkspaceData*	tNxNWorkspaceData;

class NXNINTEGRATORSDK_API CNxNWorkspace : public CNxNVirtualNode {
	NXN_DECLARE_DYNCREATE(CNxNWorkspace);

	public:
		//--------------------------------------------------------------------
		// construction / destruction
		//--------------------------------------------------------------------
		CNxNWorkspace(CNxNIntegrator* pNxNIntegrator = NULL);
		virtual ~CNxNWorkspace();

		//--------------------------------------------------------------------
		// project specific methods
		//--------------------------------------------------------------------
		CNxNProject* NewProject();

		CNxNProject* NewProjectEx(	const CNxNString& strProjectName, const CNxNString& strPath, 
									const CNxNString& strUserName, const CNxNString& strPassword, 
									const CNxNString& strHostName, const CNxNString& strMountPath,
									const CNxNString& strBuildPath, const CNxNString& strWorkingPath);

		CNxNProject* InsertProject(	const CNxNString& strFileName = _STR(""), const CNxNString& strUserName = _STR(""), 
									const CNxNString& strPassword = _STR(""), const	CNxNString& strHostName = _STR(""));

		CNxNProject* InsertProjectEx(	const CNxNString& strFileName, const CNxNString& strUserName, const CNxNString& strPassword, 
										const CNxNString& strHostName);

		CNxNProject* LoadProject(const CNxNString& strProjectName);

		CNxNProject* LoadProjectAs(const CNxNString& strProjectName);

		CNxNProject* LoadProjectEx(	const CNxNString& strProjectName, const CNxNString& strUserName, const CNxNString& strPassword, 
									const CNxNString& strHostName);

		bool UnloadProject(CNxNProject*& pProject);	


		bool RemoveProject(CNxNProject*& pProject, bool bShowDialog = true);

		CNxNProject* GetProject(const CNxNString& strProjectName);

		//--------------------------------------------------------------------
		// workspace information access methods
		//--------------------------------------------------------------------
		CNxNString GetFileName() const;

		//---------------------------------------------------------------------------
		//	project browsing
		//---------------------------------------------------------------------------
        // Internationalization change (12/20/2000 by Jens): Function bodies moved
        //      to NxNWorkspace.cpp because they use resources. File is included by
        //      others, resource accesses have to stay in DLL!
		CNxNProject* BrowseForProject();

		//--------------------------------------------------------------------
		// data retrieval
		//--------------------------------------------------------------------
		bool IsOpened() const;

		//---------------------------------------------------------------------------
		//	static object type information
		//---------------------------------------------------------------------------
		static CNxNType NXNINTEGRATORSDK_API_CALL GetType() { return CNxNType(NXN_TYPE_WORKSPACE); };

		//--------------------------------------------------------------------
		// direct access to workspace data
		//--------------------------------------------------------------------
		inline tNxNWorkspaceData GetWorkspaceData() { return m_pWorkspaceData; };

	protected:
		//--------------------------------------------------------------------
		// overloaded methods
		//--------------------------------------------------------------------
		void SetParent(CNxNNode* pNewParent);


	private:
		tNxNWorkspaceData	m_pWorkspaceData;
};

#endif // INC_NXN_WORKSPACE_H
