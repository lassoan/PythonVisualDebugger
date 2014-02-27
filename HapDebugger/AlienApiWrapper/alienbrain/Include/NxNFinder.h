// \addtodoc

#ifndef INC_NXN_FINDER_H
#define INC_NXN_FINDER_H
/*	\class		CNxNFinder NxNFinder.h
 *
 *				CNxNFinder allows do searches within the namespace. The user
 *              can specify search criteria in the form of a string. This can
 *              be done in the constructor or by using SetSearchExpr. The nodes
 *              that match these criteria will be returned by the methods 
 *              GetFirstFoundNode and GetNextFoundNode after the user called
 *              StartSearch.
 *              In addition to the search expression the user can also specify
 *              the root node of the search. The root node is the node in which
 *              the search in done. Nodes that match the search criteria must
 *              be sub nodes (direct or indirect) of the root node.
 *
 *  \author		Jens Riemschneider
 *
 *  \version	1.00
 *
 *  \date		2000
 *
 *	\mod
 *		[jr]-09-04-2000 file created.
 *	\endmod
 */


// The internal data structure for the found nodes is a list. We use the STL 
// list template for this.
#include <list>
using namespace std;

// These are the types for the list of nodes (based on the STL list template).
typedef list<CNxNNode*> NODEPTRLIST;
typedef NODEPTRLIST* LPNODEPTRLIST;
typedef const NODEPTRLIST* LPCNODEPTRLIST;

class NXNINTEGRATORSDK_API CNxNFinder : public CNxNObject {
	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------
		CNxNFinder();
		CNxNFinder( CNxNNode*         pNodeRootOfSearch, 
					const CNxNString& strSearchExpr );

		virtual ~CNxNFinder();

		//---------------------------------------------------------------------------
		//	object information access
		//---------------------------------------------------------------------------
		CNxNNode*   GetRootNode();
		void        SetRootNode( CNxNNode* pNodeRootOfSearch );

		CNxNString  GetSearchExpr();
		void        SetSearchExpr( const CNxNString& strSearchExpr );

		//---------------------------------------------------------------------------
		//	Starting the search
		//---------------------------------------------------------------------------
		bool        StartSearch();
		bool        StartSearch( CNxNNode*         pNodeRootOfSearch, 
								 const CNxNString& strSearchExpr );

		//---------------------------------------------------------------------------
		//	Retrieving of found nodes
		//---------------------------------------------------------------------------
		CNxNNode*   GetFirstFoundNode();
		CNxNNode*   GetNextFoundNode();

	private:
		CNxNNode*               m_pNodeRootOfSearch;      // The root node.
		CNxNString              m_strSearchExpr;          // The search expression.
		NODEPTRLIST             m_listSearchResult;       // The list of found nodes.
		NODEPTRLIST::iterator   m_iterCurrentRetrivalPos; // The current position 
														  // for found node retrieval.

	private:
		//---------------------------------------------------------------------------
		//	Clean up helpers.
		//---------------------------------------------------------------------------
		void DeleteOldListElements();
};


#endif // INC_NXN_FINDER_H



//---------------------------------------------------------------------------
//	(c) 2000 by NxN Software
//---------------------------------------------------------------------------
