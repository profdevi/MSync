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

//v1.2 copyright Comine.com 20150816U1931
#ifndef MWUDirReader_h
#define MWUDirReader_h

/////////////////////////////////////////////////////
#include "MStdLib.h"

////////////////////////////////////////////////
#if (defined(MSTDLIB_API_WINDOWS) )
#include <windows.h>
#include "MFilePathBuilder.h"
#include "MBuffer.h"
#include "TAutoPtr.h"

///////////////////////////////////////////////
#elif (defined(MSTDLIB_API_LINUX) )
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "MFilePathBuilder.h"
#include "MBuffer.h"
#include "TAutoPtr.h"

#endif // MSTDLIB_API_WINDOWS

//******************************************************
//**  MWUDirReader class
//******************************************************
class MWUDirReader
	{
	////////////////////////////////////////////////
	#if (defined(MSTDLIB_API_WINDOWS) )
	HANDLE mDirHandle;
	TAutoPtr<WIN32_FIND_DATAA> mData;
	bool mDataRead;
	MFilePathBuilder mPathList;
	MBuffer mFullFilePath;
	bool mNoFiles;								//=true if no more files

	///////////////////////////////////////
	#elif (defined(MSTDLIB_API_LINUX) )
	DIR *mDirHandle;
	struct dirent *mData;
	TAutoPtr<struct stat> mStat;
	bool mDataRead;
	MFilePathBuilder mPathList;
	MString mFileName;
	MBuffer mFullFilePath;
	bool mNoFiles;								//=true if no more files

	#endif // MSTDLIB_API_WINDOWS
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MWUDirReader(void);
	~MWUDirReader(void);
	bool Create(const char *dirname);
	bool Destroy(void);
	bool Read(void);							// Returns True for each new entry
	const char *GetFileName(void);				// Get a file name
	const char *GetFullFilePath(void);			// Get Full File Path
	bool IsDirectory(void);	
	unsigned long GetFileSizeLow(void);
	unsigned long GetFileSizeHigh(void);
	bool GetCreateTime(time_t &tim);
	bool GetLastAccessTime(time_t &tim);
	bool GetLastWriteTime(time_t &tim);		
	};

#endif // MWUDirReader_h

