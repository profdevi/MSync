/*    
    MSync.exe : Synchronizes files in a current directory with files in another 
                directory heirarchy
    Copyright (C) 2017  Comine.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//v2.1 copyright Comine.com 20150816U1925
#ifndef MStringSplitter_h
#define MStringSplitter_h

/////////////////////////////////////////////////////
#include "MStdLib.h"
#include "MBuffer.h"
#include "TVector.h"

//******************************************************
//**  MStringSplitter class
//******************************************************
class MStringSplitter
	{
	////////////////////////////////////////////////
	MBuffer mString;							// Modified String
	TVector<char *> mArray;						// Array of pointers
	int mCount;

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MStringSplitter(void);
	MStringSplitter(const char *str,const char *splitchars="\r\n \t");
	~MStringSplitter(void);
	bool Create(const char *str,const char *splitchars="\r\n \t");
	bool Destroy(void);
	int GetCount(void);							// Get Count of split strings
	const char *Get(int index);					// Get Split String
	};

#endif // MStringSplitter_h

