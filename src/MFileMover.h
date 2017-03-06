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

//v1.1 copyright Comine.com 20170305U0807
#ifndef MFileMover_h
#define MFileMover_h

/////////////////////////////////////////////////
#include <vector>
#include <utility>
#include "MStdLib.h"
#include "MString.h"

//////////////////////////////////////////////////
using namespace std;

/*
MFileMover will read a config file in the current directory and based
on the contents of the file will move retrieve entry files files based on 
a target location relative to an file folder.
*/

//******************************************************
//**  MFileMover class
//******************************************************
class MFileMover
	{
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	vector< pair<MString,MString> > mFilePairs;			// holds a list of local/targetdir path files
	MString mSyncFile;									// Name of the Sync File name in current directory
	MString mTargetAbsolutePath;						// Target Absolute Path
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MFileMover(void);
	~MFileMover(void);
	bool Create(const char *configfile,const char *targetdir);
	bool Destroy(void);
	bool Print(void);							// Print out the config settings
	bool MoveToTarget(bool visible=false);		// Move files to target folder
	bool MoveFromTarget(bool visible=false);	// Move files from target folder
	const char *GetTargetFolder(void);			// Get Absolute path of Target folder
	const char *GetConfigFile(void);			// Get Config File Name
	int GetCount(void);							// Get Count of file/target path pairs
	const char *GetFile(int index);				// Get File Name of file pair
	const char *GetTarget(int index);			// Get File Target path
	};

#endif // MFileMover_h

