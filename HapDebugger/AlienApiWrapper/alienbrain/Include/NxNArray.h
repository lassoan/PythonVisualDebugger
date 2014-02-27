// \addtodoc

#ifndef INC_NXN_ARRAY_H
#define INC_NXN_ARRAY_H

/* \class		CNxNArray
 *
 *	\file		NxNArray.h
 *
 *  \brief		This is a template class for all kind of parameter lists. 
 *				Similar to MFC CStringArray class.
 *
 *  \author		Axel Pfeuffer, Helmut Klinger
 *
 *  \version	1.0
 *
 *  \date		2000
 *
 *	\mod
 *		[ap]-10-02-2000 file created.
 *		[gs]-10-11-2000 file renamed and integrated into SDK.
 *	\endmod
 */

// necessary include for using STL's vector template
#include <vector>

template <class ArrayItem> class NXNINTEGRATORSDK_API CNxNArray {
	private:
		std::vector<ArrayItem> m_oArray;

	public:
		//---------------------------------------------------------------------------
		//	construction/destruction
		//---------------------------------------------------------------------------

		/*!	\fn			CNxNArray::CNxNArray()
		 *	
		 *				Default constructor.
		 *	
		 *	\param		none
		 *	
		 *	\return		none
		 *	
		 *	\note		
		 */
		inline CNxNArray() 
		{ 
		};

		inline CNxNArray(const CNxNArray<ArrayItem>& srcArray)
		{
			RemoveAllAndCopyFrom(srcArray);
		}
		
		inline virtual ~CNxNArray() 
		{
			// not empty? => clean up!
			if (!IsEmpty()) {
				RemoveAll();
			}
		};

		//---------------------------------------------------------------------------
		// operators
		//---------------------------------------------------------------------------
		inline const CNxNArray<ArrayItem>& operator =(const CNxNArray<ArrayItem>& srcArray)
		{
			RemoveAllAndCopyFrom(srcArray);
			return *this;
		}


		inline void RemoveAllAndCopyFrom(const CNxNArray<ArrayItem>& srcArray)
		{
			RemoveAll();

			int nSize = srcArray.GetCount();

			for(int i=0; i<nSize; i++) {
				Add(srcArray.GetAt(i));
			}
		}


		bool Add(const ArrayItem& param)
		{
			// add parameter
			m_oArray.insert(m_oArray.end(), param);

			return true;
		};

		ArrayItem GetAt(long nIndex) const
		{
			return m_oArray[nIndex];
		};

		ArrayItem& ElementAt(long nIndex)
		{
			return m_oArray[nIndex];
		}

		bool SetAt(long nIndex, const ArrayItem& param)
		{
			m_oArray[nIndex] = param;

			return true;
		};

		void RemoveAt(long nIndex)
		{
			m_oArray.erase(m_oArray.begin()+nIndex);
		};

		long GetCount() const
		{
			return m_oArray.size();
		};

		long GetSize() const
		{
			return GetCount();
		}

		bool IsEmpty() const
		{
			return (GetSize() == 0);
		}

		void RemoveAll()
		{
			m_oArray.clear();
		}

		ArrayItem& operator [](int nIndex)
		{
			return ElementAt(nIndex);
		}

		ArrayItem operator [](int nIndex) const
		{
			return GetAt(nIndex);
		}
};


#endif // INC_NXN_ARRAY_H
