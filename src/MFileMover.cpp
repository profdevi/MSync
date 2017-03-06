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
#include "MStdLib.h"
#include "MString.h"
#include "MFilePathBuilder.h"
#include "MFileNameValues.h"
#include "MFileOps.h"
#include "MDirOps.h"
#include "MFile.h"
#include "MFileMover.h"


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxTargetSize=4096;


////////////////////////////////////////////////
static bool GBuildDir(MFilePathBuilder &list);
static bool GGetFilePairs(const char *configfile,const char *targetdir,vector< pair<MString,MString> > &filepairs);

////////////////////////////////////////////////
// Build up all directory paths if they do not exist
static bool GBuildDir(MFilePathBuilder &list)	
	{
	MDirOps dirops(true);
	MFileOps fileops(true);
	
	const int elementcount=list.GetElementCount();
	for(int i=0;i<elementcount;++i)
		{
		MFilePathBuilder targetdirpath;
		if(targetdirpath.Create()==false)
			{
			return false;
			}

		for(int k=0;k<=i;++k)
			{
			if(targetdirpath.Push(list.Get(k) )==false)
				{
				return false;
				}

			MBuffer fulldirpath;
			if(targetdirpath.GetFullPath(fulldirpath)==false)
				{
				return false;
				}

			// Check if directory exists
			if(dirops.Exists(fulldirpath.GetBuffer())==true) {  continue;  }
			
			if(dirops.MakeDir(fulldirpath.GetBuffer())==false)
				{
				return false;
				}
			}
		}

	return true;
	}


////////////////////////////////////////////////////////
static bool GGetFilePairs(const char *configfile,const char *targetdir,vector< pair<MString,MString> > &filepairs)
	{
	// Get Info of all config files
	MFileNameValues configdata;
	if(configdata.Create(configfile)==false)
		{
		return false;
		}

	// Get Absolute Path
	MString targetabsdir;
	MFileOps fileops(true);
	if(fileops.GetAbsolutePath(targetdir,targetabsdir)==false)
		{
		return false;
		}

	// Clear up all file pairs
	filepairs.clear();

	// Process All the files
	MFilePathBuilder pathdir;
	if(pathdir.Create(targetabsdir.Get())==false)
		{
		return false;
		}

	const int count=configdata.GetCount();
	for(int i=0;i<count;++i)
		{
		MFilePathBuilder targetpathlist;
		targetpathlist.Create();
		targetpathlist.Push(pathdir);

		const char *name=configdata.GetName(i);
		const char *value=configdata.GetValue(i);
		MStdAssert(name!=0 && value!=0);
		
		MFilePathBuilder valuepath;
		if(valuepath.Create(value)==false)
			{
			return false;
			}

		if(targetpathlist.Push(valuepath)==false)	
			{
			return false;
			}

		//=We now have full target path dir
		if(GBuildDir(targetpathlist)==false)
			{
			return false;
			}

		if(targetpathlist.Push(name)==false)
			{
			return false;
			}

		MBuffer fulltargetpath;
		if(targetpathlist.GetFullPath(fulltargetpath)==false)
			{
			return false;
			}

		// Add
		pair<MString,MString> pairinst;
		if( pairinst.first.Create(name)==false
				|| pairinst.second.Create(fulltargetpath.GetBuffer())==false)
			{
			return false;
			}

		filepairs.push_back(pairinst);
		}
	
	return true;
	}


//******************************************************
//**  MFileMover class
//******************************************************
void MFileMover::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MFileMover::MFileMover(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MFileMover::~MFileMover(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MFileMover::Create(const char *configfile,const char *targetdir)
	{
	Destroy();

	if(mSyncFile.Create(configfile)==false)
		{
		Destroy();
		return false;
		}

	MFileOps fileops(true);
	if(fileops.GetAbsolutePath(targetdir,mTargetAbsolutePath)==false)
		{
		Destroy();
		return false;
		}

	if(GGetFilePairs(configfile,targetdir,mFilePairs)==false)
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MFileMover::Destroy(void)
	{
	mFilePairs.clear();
	mSyncFile.Destroy();
	mTargetAbsolutePath.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MFileMover::Print(void)
	{
	MStdPrintf("Config File   : %s\n",GetConfigFile() );
	MStdPrintf("Target Folder : %s\n",GetTargetFolder() );
	const int count=mFilePairs.size();
	for(int i=0;i<count;++i)
		{
		MStdPrintf("\t%-20s=> %s\n",mFilePairs[i].first.Get(),mFilePairs[i].second.Get());
		}

	return true;
	}


//////////////////////////////////////////////
bool MFileMover::MoveToTarget(bool visible)
	{
	MFileOps fileops(true);

	const int count=(int)mFilePairs.size();
	for(int i=0;i<count;++i)
		{
		MString &srcfile=mFilePairs[i].first;
		MString &dstpath=mFilePairs[i].second;

		if(fileops.Copy(srcfile.Get(),dstpath.Get() )==true)
			{
			if(visible==true)
				{
				MStdPrintf("copied %s to %s\n",srcfile.Get(),dstpath.Get() );
				}
			}
		else
			{
			if(visible==true)
				{
				MStdPrintf("**Unable to copy %s to %s\n",srcfile.Get(),dstpath.Get() );
				}
			}
		}
	
	return true;
	}


///////////////////////////////////////////////////////////////
bool MFileMover::MoveFromTarget(bool visible)
	{
	MFileOps fileops(true);

	const int count=(int)mFilePairs.size();
	for(int i=0;i<count;++i)
		{
		MString &srcfile=mFilePairs[i].first;
		MString &dstpath=mFilePairs[i].second;

		if(fileops.Copy(dstpath.Get(),srcfile.Get() )==true)
			{
			if(visible==true)
				{
				MStdPrintf("copied %s to %s\n",dstpath.Get(),srcfile.Get());
				}
			}
		else
			{
			if(visible==true)
				{
				MStdPrintf("**Unable to copy %s to %s\n",dstpath.Get(),srcfile.Get());
				}
			}
		}
	
	return true;
	}


/////////////////////////////////////////////////////
const char *MFileMover::GetTargetFolder(void)
	{
	return mTargetAbsolutePath.Get();
	}


/////////////////////////////////////////////////////
const char *MFileMover::GetConfigFile(void)
	{
	return mSyncFile.Get();
	}


/////////////////////////////////////////////////////
int MFileMover::GetCount(void)
	{
	return (int)mFilePairs.size();
	}


/////////////////////////////////////////////////////
const char *MFileMover::GetFile(int index)
	{
	return mFilePairs[index].first.Get();
	}


/////////////////////////////////////////////////////
const char *MFileMover::GetTarget(int index)
	{
	return mFilePairs[index].second.Get();
	}


