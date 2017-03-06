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

//v1.0 copyright Comine.com 20170304S1056
#include <vector>
#include <string>
#include "MStdLib.h"
#include "MFile.h"
#include "MBuffer.h"
#include "MFileNameValues.h"

////////////////////////////////////////////////
using namespace std;


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxLineLength=2048;

//////////////////////////////////////////////
static bool GGetNameValue(char *line,char *&name,char *&value)
	{
	// Process till # character
	for(int i=0;line[i]!=0;++i)
		{
		if(line[i]=='#') { line[i]=0; break; }
		}

	// Line Split till '='
	name=line;
	value=0;
	for(int i=0; ;++i)
		{
		if(line[i]=='=')
			{
			line[i]=0;
			value=line+i+1;
			break;
			}
		
		if(line[i]==0)
			{
			name=0;
			value=0;
			return false;
			}
		}

	MStdStrTrimLeft(name);
	MStdStrTrimRight(name);
	MStdStrTrimLeft(value);
	MStdStrTrimRight(value);

	return true;
	}


//******************************************************
//**  MFileNameValues class
//******************************************************
void MFileNameValues::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MFileNameValues::MFileNameValues(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MFileNameValues::~MFileNameValues(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MFileNameValues::Create(string filename)
	{
	Destroy();

	MFileInput fileinput;
	if(fileinput.Create(filename.c_str())==false)
		{
		Destroy();
		return false;
		}

	MBuffer line;
	if(line.Create(GMaxLineLength)==false)
		{
		Destroy();
		return false;
		}

	while(fileinput.ReadLine(line.GetBuffer(),line.GetSize()-2)==true)
		{
		char *name;
		char *value;
		if(GGetNameValue(line.GetBuffer(),name,value)==false) { continue; }
		MStdAssert(name!=0 && value!=0);

		GElement namevalue;
		namevalue.Name=name;
		namevalue.Value=value;

		mData.push_back(namevalue);
		}
	
	return true;
	}


////////////////////////////////////////////////
bool MFileNameValues::Destroy(void)
	{
	mData.clear();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MFileNameValues::Print(void)
	{
	const size_t count=mData.size();
	for(auto i=0U;i<count;++i)
		{
		MStdPrintf("%-20s = %s\n",mData[i].Name.c_str(),mData[i].Value.c_str());
		}

	return true;
	}


///////////////////////////////////////////////////////////
int MFileNameValues::GetCount(void)
	{
	return (int)mData.size();
	}


///////////////////////////////////////////////////////////
const char *MFileNameValues::GetName(int index)
	{
	MStdAssert(index>=0 && index<(int)mData.size() );
	return mData[index].Name.c_str();
	}


///////////////////////////////////////////////////////////
const char *MFileNameValues::GetValue(int index)
	{
	MStdAssert(index>=0 && index<(int)mData.size() );
	return mData[index].Value.c_str();
	}


///////////////////////////////////////////////////////
bool MFileNameValues::Save(const char *filename)
	{
	MFileOutput fileout;
	if( fileout.Create(filename)==false)
		{
		return false;
		}

	fileout.WriteChars("#   Auto Generated file\n");
	fileout.WriteChars("\n");

	const int count=(int)mData.size();
	for(int i=0;i<count;++i)
		{
		fileout.WriteChars(GetName(i) );
		fileout.WriteChars("=");
		fileout.WriteChars(GetValue(i) );
		fileout.WriteChars("\n");
		}

	fileout.Destroy();
	return true;
	}



