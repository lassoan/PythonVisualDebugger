/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 */

/*
 * strarray.h - a 0 based array of StrBufs
 *
 * Class Defined:
 *
 *	StrArray - list of StrBufs *'s
 *
 * Public methods:
 *
 * Private methods:
 */

class VarArray;

class StrArray {
	
    public:

			StrArray();
			~StrArray();

	StrBuf *	Put();
	const StrBuf *	Get(int i);
	int		Count() const ;
	void		Sort( int caseFolding );

    private:

	VarArray	*array;
} ;
