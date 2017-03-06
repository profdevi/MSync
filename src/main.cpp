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

//v1.0 copyright Comine.com 20170304S1649
#include "MStdLib.h"
#include "MStringList.h"
#include "MWUDirReader.h"
#include "MFileMover.h"
#include "MFileOps.h"
#include "MCommandArg.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MSync";	// Used in Help
static const char *GApplicationVersion="1.0";	// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);
static bool GGetSyncFiles(const char *directory,MStringList &syncfiles);
static bool GListSyncFiles(MStringList &syncfiles);
static bool GShowStatus(MStringList &syncfiles,const char *targetfolder);
static bool GReadFiles(MStringList &syncfiles,const char *targetfolder);
static bool GWriteFiles(MStringList &syncfiles,const char *targetfolder);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	if(args.CheckRemoveHelp()==true)
		{
		GDisplayHelp();
		return 0;
		}

	bool readflag=false;
	bool writeflag=false;
	bool listsyncfiles=false;
	if(args.CheckRemoveArg("-r")==true) { readflag=true; }
	if(args.CheckRemoveArg("-w")==true) { writeflag=true; }
	if(args.CheckRemoveArg("-l")==true) { listsyncfiles=true; }

	if((readflag==true && writeflag==true) || (readflag==true && listsyncfiles==true)
			|| (listsyncfiles==true && writeflag==true) 
			|| (readflag==true && writeflag==true && listsyncfiles==true)  )
		{
		GDisplayHelp();
		return 0;
		}

	// Get Sync Files
	MStringList msyncfiles;	
	if(GGetSyncFiles(".",msyncfiles)==false)
		{
		MStdPrintf("**Unable to get .msync files in current directory");
		return 1;
		}

	if(listsyncfiles==true)
		{
		GListSyncFiles(msyncfiles);
		return 0;
		}

	// Check if target directory specified
	if(args.GetArgCount()<2)
		{
		GDisplayHelp();
		return 1;
		}

	// Get Target Directory
	const char *targetdir=args.GetArg(1);
	MStdAssert(targetdir!=0);

	// Check if we should list of status of all files
	if(writeflag==false && readflag==false)
		{
		GShowStatus(msyncfiles,targetdir);
		return 0;
		}

	if(writeflag==true)
		{
		GWriteFiles(msyncfiles,targetdir);
		return 0;
		}

	if(readflag==true)
		{
		GReadFiles(msyncfiles,targetdir);
		return 0;
		}

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-?][-w|-r|-l] <targetdir>\n"
				"           v%s copyright Comine.com\n"
				"\n"
				"   Application will read the contents of all config files in current\n"
				"   directory with extension .msync.  Based on the contents of the .msync\n"
				"   files, files in the current directory are moved to the target folder\n"
				"   specified by the <targetdir> argument.  The -w flag writes to the target\n"
				"   while the -r flad reads the files from the target directory. The -l flag\n"
				"   will list out the .msync files in the current directory.\n"
				"\n"
				"   Example msync config file with usage of cmd is shown below, that moves\n"
				"   the file one.txt to the file Proj/docs/one.txt and the file one.c to \n"
				"   Proj/src/one.c .\n"
				"\n"
				"             # Pound marks comments\n"
				"             one.txt=docs\n"
				"             one.c=src\n"
				"\n"
				"     The Command line is      %s -w Proj\n"
				"\n"
				,GApplicationName,GApplicationVersion,GApplicationName);
	}


////////////////////////////////////////////////////////
static bool GGetSyncFiles(const char *directory,MStringList &syncfiles)
	{
	MWUDirReader dirreader;
	if(dirreader.Create(directory)==false)
		{
		return false;
		}

	if(syncfiles.Create()==false)
		{
		return false;
		}

	while(dirreader.Read()==true)
		{
		const char *filename=dirreader.GetFileName();
		if(MStdStrIEnds(filename,".msync")==false) { continue; }

		// Add the file to the dir reader
		if(syncfiles.AddString(filename)==false)
			{
			return false;
			}
		}

	return true;
	}


/////////////////////////////////////////////////////////////
static bool GListSyncFiles(MStringList &syncfiles)
	{
	syncfiles.ReadReset();
		
	const char *str;
	while( (str=syncfiles.ReadString())!=0)
		{
		MStdPrintf("%s\n",str);
		}

	return true;
	}


/////////////////////////////////////////////////////////////
static bool GShowStatus(MStringList &syncfiles,const char *targetfolder)
	{
	syncfiles.ReadReset();
	
	MFileOps fileops(true);

	const char *msyncfile;
	while((msyncfile=syncfiles.ReadString())!=0)
		{
		MFileMover filemover;
		if(filemover.Create(msyncfile,targetfolder)==false)
			{
			MStdPrintf("**Unable to read config file %s\n",msyncfile);
			continue;
			}

		// Display all the information about files
		const int pairscount=filemover.GetCount();
		for(int i=0;i<pairscount;++i)
			{
			const char *srcfile=filemover.GetFile(i);
			const char *targetfile=filemover.GetTarget(i);

			MStdPrintf("%-10s => %-50s : ",srcfile,targetfile);
			if(fileops.Exists(srcfile)==false)
				{
				MStdPrintf("**Source file missing.");
				}
			else if(fileops.Exists(targetfile)==false)
				{
				MStdPrintf("**Target file missing.");
				}
			else if(fileops.AreSame(srcfile,targetfile)==false)
				{
				MStdPrintf("**Different.");
				}
			else
				{
				MStdPrintf(" same ");
				}
			
			MStdPrintf("\n");
			}
		}
	
	
	return true;
	}


/////////////////////////////////////////////////////////////
static bool GReadFiles(MStringList &syncfiles,const char *targetfolder)
	{
	syncfiles.ReadReset();
	
	MFileOps fileops(true);

	const char *msyncfile;
	while((msyncfile=syncfiles.ReadString())!=0)
		{
		MFileMover filemover;
		if(filemover.Create(msyncfile,targetfolder)==false)
			{
			MStdPrintf("**Unable to read config file %s\n",msyncfile);
			continue;
			}

		// Display all the information about files
		const int pairscount=filemover.GetCount();
		for(int i=0;i<pairscount;++i)
			{
			const char *srcfile=filemover.GetFile(i);
			const char *targetfile=filemover.GetTarget(i);

			MStdPrintf("%-50s => %-10s : ",targetfile,srcfile);
			if(fileops.Exists(targetfile)==false)
				{
				MStdPrintf("**Target file missing.\n");
				continue;
				}
			
			if(fileops.Copy(targetfile,srcfile)==true)
				{
				MStdPrintf("copied\n");
				continue;
				}
			else
				{
				MStdPrintf("**FAILED\n");
				continue;
				}
			}
		}
		
	return true;
	}


/////////////////////////////////////////////////////////////
static bool GWriteFiles(MStringList &syncfiles,const char *targetfolder)
	{
	syncfiles.ReadReset();
	
	MFileOps fileops(true);

	const char *msyncfile;
	while((msyncfile=syncfiles.ReadString())!=0)
		{
		MFileMover filemover;
		if(filemover.Create(msyncfile,targetfolder)==false)
			{
			MStdPrintf("**Unable to read config file %s\n",msyncfile);
			continue;
			}

		// Display all the information about files
		const int pairscount=filemover.GetCount();
		for(int i=0;i<pairscount;++i)
			{
			const char *srcfile=filemover.GetFile(i);
			const char *targetfile=filemover.GetTarget(i);

			MStdPrintf("%-10s => %-50s : ",srcfile,targetfile);
			if(fileops.Exists(srcfile)==false)
				{
				MStdPrintf("**Source file missing.\n");
				continue;
				}
			
			if(fileops.Copy(srcfile,targetfile)==true)
				{
				MStdPrintf("copied\n");
				continue;
				}
			else
				{
				MStdPrintf("**FAILED\n");
				continue;
				}
			}
		}
		
	return true;
	}


