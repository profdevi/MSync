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

//v1.18 copyright Comine.com 20150816U2133
#include "MStdLib.h"
#include "MStringList.h"


//********************************************************************
//** Module Elements
//********************************************************************
struct GNode
	{
	char *Data;
	GNode *Next;
	};


//********************************************************************
//** MStringList
//********************************************************************
void MStringList::ClearObject(void)
	{
	mFirstNode=NULL;
	mLastNode=NULL;
	mCurrent=NULL;
	mCount=0;
	}


//////////////////////////////////////////////////////////////
MStringList::MStringList(void)
	{  ClearObject();  }


//////////////////////////////////////////////////////////////
MStringList::MStringList(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


//////////////////////////////////////////////////////////////
MStringList::MStringList(MStringList &reflist)
	{
	ClearObject();
	if(Create(reflist)==false)
		{
		return;
		}
	}


//////////////////////////////////////////////////////////////
MStringList::~MStringList(void)
	{  Destroy();  }


//////////////////////////////////////////////////////////////
bool MStringList::Create(void)
	{
	Destroy();
	return true;
	}

//////////////////////////////////////////////////////////////
bool MStringList::Create(MStringList &reflist)
	{
	Destroy();
	if(Create()==false)
		{
		Destroy();
		return false;
		}

	// Iterate through the list
	GNode *p;
	for(p=reflist.mFirstNode;p!=NULL;p=p->Next)
		{
		MStdAssert(p->Data!=NULL);
		if(AddString(p->Data)==false)
			{
			Destroy();
			return false;
			}
		}

	return true;
	}


/////////////////////////////////////////////////////////////
bool MStringList::Destroy(void)
	{
	GNode *pnode;
	GNode *tmp;
	for(pnode=mFirstNode;pnode!=NULL; pnode=tmp)
		{	
		tmp=pnode->Next;  

		if(pnode->Data!=NULL)
			{
			delete[] pnode->Data;
			pnode->Data=NULL;
			}

		delete pnode;
		}

	ClearObject();
	return true;
	}


//////////////////////////////////////////////////////////////
bool MStringList::AddString(const char *string) // Add to the end of the list
	{
	if(string==NULL)
		{
		return false;
		}

	GNode *newnode;
	newnode=new(std::nothrow) GNode;
	if(newnode==NULL)
		{
		return false;
		}

	newnode->Data=new(std::nothrow) char[MStdStrLen(string)+1];
	if(newnode->Data==NULL)
		{
		delete newnode;
		return false;
		}

	MStdStrCpy(newnode->Data,string);
	newnode->Next=NULL;

	// Increment count
	mCount = mCount + 1;

	if(mFirstNode==NULL)
		{  mCurrent=mFirstNode=mLastNode=newnode;  return true;  }

	mLastNode->Next=newnode;  mLastNode=newnode; return true;
	}


////////////////////////////////////////////////////////////
bool MStringList::AddList(MStringList &list)
	{
	GNode *p;
	for(p=list.mFirstNode;p!=NULL;p=p->Next)
		{
		if(AddString(p->Data)==false)
			{
			return false;
			}
		}

	return true;
	}


////////////////////////////////////////////////////////////
bool MStringList::ReadReset(void)  // Will Reset to the first item again
	{
	mCurrent=mFirstNode;
	return true;
	}


////////////////////////////////////////////////////////////
const char *MStringList::ReadString(void)  // Read a string untill NULL
	{
	if(mCurrent==NULL) { return NULL; }
	const char *retstr=mCurrent->Data;
	mCurrent=mCurrent->Next;
	return retstr;
	}


/////////////////////////////////////////////////////////////
bool MStringList::IsMember(const char *string,bool casematch)
	{
	if(string==NULL) { return false; }

	// Search through all items
	if(casematch==true)
		{
		GNode *p;
		for(p=mFirstNode;p!=NULL;p=p->Next)
			{
			if(MStdStrCmp(p->Data,string)==0)
				{ return true; }
			}
		}
	else
		{
		GNode *p;
		for(p=mFirstNode;p!=NULL;p=p->Next)
			{
			if(MStdStrICmp(p->Data,string)==0)
				{ return true; }
			}
		}

	return false;
	}


////////////////////////////////////////////////////////////////
int MStringList::GetCount(void)
	{
	return mCount;
	}


///////////////////////////////////////////////////////////////
bool MStringList::Remove(const char *string)
	{
	if(mFirstNode==NULL)
		{
		// List is empty
		return false;
		}

	// Check if first node is the element to remove
	if(MStdStrCmp(mFirstNode->Data,string)==0)
		{
		//=We have found the string in the first node
		GNode *tmpnode=mFirstNode;
		mFirstNode=tmpnode->Next;
		
		if(mFirstNode==NULL)
			{
			mLastNode=mFirstNode;
			}

		// Update current node
		if(mCurrent==tmpnode)
			{
			mCurrent=mFirstNode;
			}

		// Release memory
		delete [] tmpnode->Data;

		delete tmpnode;

		mCount=mCount-1;
		return true;
		}
	
	// Remove a node
	GNode *p;
	for(p=mFirstNode;p->Next!=NULL;p=p->Next)
		{
		if(MStdStrCmp(p->Next->Data,string)==0)
			{
			GNode *tmpnode=p->Next;

			// Link past node
			p->Next=tmpnode->Next;

			// Update current node
			if(mCurrent==tmpnode)
				{
				mCurrent=p->Next;
				}

			// Update the last node
			if(mLastNode==tmpnode)
				{  mLastNode=p;	 }

			// Release memory
			delete []tmpnode->Data;

			delete tmpnode;

			mCount=mCount-1;
			return true;
			}

		//=Keep Searching in next node
		}
	
	// We have not found the string
	return false;
	}


////////////////////////////////////////////////////////
bool MStringList::Swap(MStringList &list)
	{
	GNode *tmp;

	tmp=mFirstNode;  
	mFirstNode=list.mFirstNode;
	list.mFirstNode=tmp;

	tmp=mLastNode;
	mLastNode=list.mLastNode;
	list.mLastNode=tmp;

	tmp=mCurrent;
	mCurrent=list.mCurrent;
	list.mCurrent=tmp;

	int tmpcount;
	tmpcount=mCount;
	mCount=list.mCount;
	list.mCount=tmpcount;

	return true;
	}


