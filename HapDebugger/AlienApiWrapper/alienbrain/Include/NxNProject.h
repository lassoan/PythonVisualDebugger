// \addtodoc

#ifndef INC_NXN_PROJECT_H
#define INC_NXN_PROJECT_H

/* \class		CNxNProject
 *
 *	\file		NxNProject.h
 *
 *  \brief		The class CNxNProject encapsulates projects that rely on servers in databases.
 *				Usually projects contain folders and files which can be created, checked in/out,
 *				removed, etc. It is possible to share the same projects over a large scale of persons
 *				gaining them access to the same data.
 *
 *  \author		Axel Pfeuffer, Helmut Klinger
 *
 *  \version	1.00
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-05-02-2000 file created.
 *		[ap]-06-13-2000 GetFile() implemented.
 *		[ap]-06-13-2000 GetFolder() implemented.
 *		[ap]-06-13-2000 CreateFile() implemented.
 *		[ap]-06-13-2000 CreateFolder() implemented.
 *		[ap]-09-08-2000 BUGFIX: CreateFolder() now works.
 *      [jr]-12-18-2000 Internationalization
 *	\endmod
 */

//
// This file was changed during internationalization on 12/18/2000 by Jens.

//---------------------------------------------------------------------------
//	hidden structure for data storage
//---------------------------------------------------------------------------
typedef struct _tNxNProjectData*	tNxNProjectData;

class NXNINTEGRATORSDK_API CNxNProject : public CNxNDbNode {
	NXN_DECLARE_DYNCREATE(CNxNProject);

	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------
		CNxNProject(CNxNWorkspace* pParent = NULL);
		virtual ~CNxNProject();

		//---------------------------------------------------------------------------
		//	project information retrieval
		//---------------------------------------------------------------------------
		bool SetUserName(const CNxNString& strUserName);
		bool SetPassword(const CNxNString& strPassword);
		bool SetHostName(const CNxNString& strHostName);

		CNxNString GetUserName() const;
		CNxNString GetHostName() const;
		bool IsConnected() const;

		bool Connect(	const CNxNString& strUserName = _STR(""), 
						const CNxNString& strPassword = _STR(""), 
						const CNxNString& strHostName = _STR(""));

		bool Disconnect();

		//---------------------------------------------------------------------------
		//	folder & file browsing
		//---------------------------------------------------------------------------
        // Internationalization change (12/20/2000 by Jens): Function bodies moved
        //      to NxNProject.cpp because they use resources. File is included by
        //      others, resource accesses have to stay in DLL!
        CNxNFolder* BrowseForFolder();
		CNxNFile* BrowseForFile();

		//---------------------------------------------------------------------------
		//	folder & file access
		//---------------------------------------------------------------------------
		/*!	\fn			inline CNxNFolder* CNxNProject::GetFolder(const CNxNString& strName)
		 *	
		 *				Returns a CNxNFolder pointer to the specified child object.
		 *				The specified name is used to find the folder node concerned, which is
		 *				located directly below this object. 
		 *	
		 *	\param		strName is a reference to the object's name to retrieve.
		 *	
		 *	\return		A pointer to a CNxNFolder object. If the returned value indicates NULL
		 *				then the CNxNFolder object could either not be found or was not of type CNxNFolder. 
		 *				For detailed information use GetLastError() or GetLastErrorMessage().
		 *	
		 *	\note		
		 */
		inline CNxNFolder* GetFolder(const CNxNString& strName)
		{
			SetLastError(S_OK);

			CNxNNode* pFolder = GetNode(strName); // get folder node

			// folder object found?
			if (!pFolder) {
				// => not found => return NULL;
				SetLastError(NXN_ERR_PROJECT_SUBFOLDER_NOT_FOUND);

				return NULL;
			}
	
			// node is not of type folder?
			if (!pFolder->IsOfType(CNxNFolder::GetType())) {
				// => not of type folder => return NULL;
				SetLastError(NXN_ERR_PROJECT_NO_FOLDER);

				return NULL;
			}
			
			return (CNxNFolder*)pFolder;
		}

		/*!	\fn			inline CNxNFile* CNxNProject::GetFile(const CNxNString& strName)
		 *	
		 *				Returns a CNxNFile pointer to the specified child object.
		 *				The specified name is used to find the folder node concerned, which is
		 *				located directly below this object. 
		 *	
		 *	\param		strName is a reference to the object's name to retrieve.
		 *	
		 *	\return		A pointer to a CNxNFile object. If the returned value indicates NULL
		 *				then the CNxNFile object could either not be found or was not of type CNxNFile. 
		 *				For detailed information use GetLastError() or GetLastErrorMessage().
		 *	
		 *	\note		
		 */
		inline CNxNFile* GetFile(const CNxNString& strName)
		{
			SetLastError(S_OK);

			CNxNNode* pFile = GetNode(strName); // get file node

			// file object found?
			if (!pFile) {
				// => not found => return NULL;
				SetLastError(NXN_ERR_PROJECT_FILE_NOT_FOUND);

				return NULL;
			}
	
			// node is not of type file?
			if (!pFile->IsOfType(CNxNFile::GetType())) {
				// => not of type file => return NULL;
				SetLastError(NXN_ERR_PROJECT_NO_FILE);

				return NULL;
			}
			
			return (CNxNFile*)pFile;
		}


		CNxNFolder* CreateFolder(	const CNxNString& strFolderName, 
									bool bShowDialog = true,
									const CNxNString& strComment = _STR(""),
									const CNxNString& strKeywords = _STR(""),
									bool bFailIfExists = true);

		/*!	\fn			inline CNxNFile* CNxNProject::CreateFile(	const CNxNString& strFileName,
		  															const CNxNString& strComment = _STR(""),
		  															const CNxNString& strKeywords = _STR(""),
		  															const CNxNString& strLocalPath = _STR(""))
		 *	
		 *				Returns a CNxNFile pointer to the created child object.
		 *				The specified name is used to create the file node concerned, which is
		 *				located directly below this object. 
		 *	
		 *	\param		strFileName is a reference to the file's name to create.
		 *	\param		strComment is a reference to a creation comment.
		 *	\param		strKeywords is a reference to creation keywords.
		 *	\param		strLocalPath is a reference to the local path of the file to create.
		 *	
		 *	\return		A pointer to a CNxNFile object. If the return value indicates NULL
		 *				that specified CNxNFile object could not be created. 
		 *				Use GetLastError() to get more details about the
		 *				error.
		 *	
		 *	\note		
		 */
		inline CNxNFile* CreateFile(	const CNxNString& strFileName, 
										const CNxNString& strComment = _STR(""),
										const CNxNString& strKeywords = _STR(""),
										const CNxNString& strLocalPath = _STR(""))
		{
			return (CNxNFile*)CreateNode(strFileName, CNxNFile::GetType(), true, strComment, strKeywords,
											strLocalPath, false, false);
		}

		//---------------------------------------------------------------------------
		//	file & folder retrieval using the concerning project specific handle
		//---------------------------------------------------------------------------
		CNxNFile* GetFileFromHandle(long lHandle);
		CNxNFolder* GetFolderFromHandle(long lHandle);

		//---------------------------------------------------------------------------
		//	static object type information
		//---------------------------------------------------------------------------
		static CNxNType NXNINTEGRATORSDK_API_CALL GetType() { return CNxNType(NXN_TYPE_PROJECT); };

		//---------------------------------------------------------------------------
		//	direct access to node object data
		//---------------------------------------------------------------------------
		inline tNxNProjectData GetProjectData() const { return m_pProjectData; };

	protected:
//		void Reset();

	private:
		tNxNProjectData	m_pProjectData;
};


#endif // INC_NXN_PROJECT_H
