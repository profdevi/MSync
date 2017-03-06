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

//v1.2 copyright Comine.com 20150816U1930
#ifndef MFilePathBuilder_h
#define MFilePathBuilder_h

//////////////////////////////////////////////////
#include "MStdLib.h"
#include "MBuffer.h"
#include "MString.h"
#include "TQueueArray.h"

/*
	Class can be used to build paths
	Output Path will have generally /a/b/c format accept in windows
	where no leading / is placed for drives.

	Cannot Push . or .. as path elements

*/

//******************************************************
//**  MFilePathBuilder class
//******************************************************
class MFilePathBuilder
	{
	////////////////////////////////////////////////
	TQueueArray<MString> mList;
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MFilePathBuilder(bool create=false);
	MFilePathBuilder(const char *dirpath);
	~MFilePathBuilder(void);
	bool Create(void);
	bool Create(const char *absdirpath);
	bool Destroy(void);
	const char *Get(int index);
	int GetElementCount(void);
	bool GetFullPath(MBuffer &buffer);	
	bool Push(const char *element);			// Push Element into list
	bool Push(MFilePathBuilder &paths);		// Push More Elements at end
	bool Pop(void);							// Pop Element off list
	bool Pop(int count);					// Pop Till Count elements exist
	const char *GetTop(void);				// Get Last Element
	bool IsSamePath(MFilePathBuilder &lst);	// =true if paths are same
	bool IsSamePath(const char *path);		// =true if paths are same
	};

#endif // MFilePathBuilder_h

