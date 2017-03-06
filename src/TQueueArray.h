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

//v1.1 copyright Comine.com 20150816U1927
#ifndef TQueueArray_h
#define TQueueArray_h

////////////////////////////////////////////
#include "MStdLib.h"
#include "TVector.h"
#include "TAutoPtr.h"

/*
	Class implements a  queue of references pointers implemented in a fixed
	size array.
*/
////////////////////////////////////////////
template <class ItemClass>
class TQueueArray
	{
	TVector<TAutoPtr<ItemClass> > mArray;					
	int mFrontIndex;							// Current Index of the front
	int mBackIndex;								// End Index of the back

	/////////////////////////////////////////
	void ClearObject(void)
		{
		mFrontIndex=0;
		mBackIndex=0;
		}

	public:
	/////////////////////////////////////////
	TQueueArray(void)
		{  ClearObject();  }

	/////////////////////////////////////////
	explicit TQueueArray(int count)
		{
		ClearObject();
		if(Create(count)==false)
			{
			return;
			}
		}

	/////////////////////////////////////////
	~TQueueArray(void)
		{  Destroy();  }

	///////////////////////////////////////
	bool Create(int count)
		{
		Destroy();

		if(mArray.Create(count+1)==false)
			{
			Destroy();
			return false;
			}

		mFrontIndex=mBackIndex=0;

		return true;
		}


	////////////////////////////////////////
	bool Destroy(void)
		{
		mArray.Destroy();
		ClearObject();
		return true;
		}


	///////////////////////////////////////
	int GetMaxCapacity(void) 
		{
		const int arraylength=mArray.GetCount();

		return arraylength-1;
		}

	///////////////////////////////////////
	int GetLength(void)
		{
		const int arraylength=mArray.GetCount();
		return (mBackIndex+arraylength-mFrontIndex)%arraylength;
		}

	//////////////////////////////////////////////
	ItemClass *Get(int index)
		{
		MStdAssert(index>=0 && index<GetLength() );
		const int arraylength=mArray.GetCount();
		return mArray[(mFrontIndex+index)%arraylength].Get();
		}

	////////////////////////////////////////////////
	// Add item to end at mArray[mcCount]
	ItemClass *EnqueueBack(void)
		{
		const int arraylength=mArray.GetCount();
		if(GetLength()>=arraylength-1)
			{
			return NULL;
			}

		if(mArray[mBackIndex].Create()==false)
			{
			return NULL;
			}

		ItemClass *newitem=mArray[mBackIndex].Get();
		MStdAssert(newitem!=NULL);

		mBackIndex = (mBackIndex + 1) % arraylength;
		return newitem;
		}

	////////////////////////////////////////////////
	// Add item to end at mArray[mcCount]
	ItemClass *EnqueueFront(void)
		{
		const int arraylength=mArray.GetCount();
		if(GetLength()>=arraylength-1)
			{
			return NULL;
			}

		mFrontIndex=(mFrontIndex+arraylength-1) % arraylength;

		if(mArray[mFrontIndex].Create()==false)
			{
			mFrontIndex=(mFrontIndex+1) % arraylength;
			return NULL;
			}

		ItemClass *newitem=mArray[mFrontIndex].Get();
		MStdAssert(newitem!=NULL);

		return newitem;
		}


	////////////////////////////////////////////////
	// Add item to end at mArray[mcCount]
	bool DequeueBack(void)
		{
		const int length=GetLength();
		if(length<=0)
			{
			return false;
			}

		const int arraylength=mArray.GetCount();
		
		mBackIndex = (mBackIndex+arraylength-1)%arraylength;

		mArray[mBackIndex].Destroy();
		MStdAssert(mArray[mBackIndex].Get()==NULL);
		
		return true;
		}


	////////////////////////////////////////////////
	bool DequeueFront(void)
		{
		const int length=GetLength();
		if(length<=0)
			{
			return false;
			}

		const int arraylength=mArray.GetCount();
		mArray[mFrontIndex].Destroy();
		MStdAssert(mArray[mFrontIndex].Get()==NULL);

		mFrontIndex = (mFrontIndex + 1) % arraylength;
		return true;
		}

	};

#endif //TQueueArray_h

