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
#include "MStdLib.h"
#include "MBuffer.h"
#include "TVector.h"
#include "MStringSplitter.h"


//******************************************************
//**  MStringSplitter class
//******************************************************
void MStringSplitter::ClearObject(void)
	{
	mCount=0;
	}


////////////////////////////////////////////////
MStringSplitter::MStringSplitter(void)
	{  ClearObject();  }


///////////////////////////////////////////////
MStringSplitter::MStringSplitter(const char *str,const char *splitchars)
	{
	ClearObject();
	if(Create(str,splitchars)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MStringSplitter::~MStringSplitter(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MStringSplitter::Create(const char *str,const char *splitchars)
	{
	Destroy();

	// Check arguments
	if(str==NULL || splitchars==NULL || *splitchars==0)
		{
		Destroy();
		return false;
		}

	// Allocate and copy space for string
	const int length=MStdStrLen(str);

	if(mString.Create(length+1)==false)
		{
		Destroy();
		return false;
		}

	// Copy the string into fresh space
	MStdStrCpy(mString.GetBuffer(),str);

	//*Set all split characters to zero
	const int splitlength=MStdStrLen(splitchars);
	int i,k;
	for(i=0;i<length;++i)
		{
		for(k=0;k<splitlength;++k)
			{
			if(mString[i]!=splitchars[k]) { continue; }
			mString[i]=0;  break;
			}
		}

	//*Now Count of split strings
	int count=0;  
	int state=0;
	for(i=0;i<=length;++i)
		{
		if(state==0 && mString[i]!=0)
			{  count=count+1;  state=1; }
		else if(state==1 && mString[i]==0)
			{  state=0;  }
		}

	// Assign 
	mCount=count;

	if(mArray.Create(mCount+1)==false)
		{
		Destroy();
		return false;
		}

	if(count==0)
		{
		// Return success ( No SPlits)
		mArray[0]=mString.GetBuffer();
		return true;
		}

	// Now Set the array
	state=0; count=0;
	for(i=0;i<length;++i)
		{
		if(state==0 && mString[i]!=0)
			{
			state=1;
			mArray[count]=mString+i;
			count=count+1;
			}
		else if(state==1 && mString[i]==0)
			{  state=0;  }
		}

	return true;
	}


////////////////////////////////////////////////
bool MStringSplitter::Destroy(void)
	{
	mString.Destroy();
	mArray.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
int MStringSplitter::GetCount(void)							// Get Count of split strings
	{
	return mCount;
	}


////////////////////////////////////////////////
const char *MStringSplitter::Get(int index)					// Get Split String
	{
	return mArray[index];
	}


