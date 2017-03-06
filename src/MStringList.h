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
#ifndef MStringList_h
#define MStringList_h

///////////////////////////////////////////////////
class MStringList
	{
	/////////////////////////////////////////////
	struct GNode *mFirstNode;
	struct GNode *mLastNode;
	struct GNode *mCurrent;
	int mCount;

	//////////////////////////////////////////////
	void ClearObject(void);

	//////////////////////////////////////////////
	public:
	MStringList(void);
	MStringList(bool create);
	MStringList(MStringList &reflist);	// Initialize from another list
	~MStringList(void);
	bool Create(void);
	bool Create(MStringList &reflist);	// Initialize from another list
	bool Destroy(void);
	bool AddString(const char *string); // Add to the end of the list
	bool AddList(MStringList &list);	// Add existing list
	bool ReadReset(void);				// Will Reset to the first item again
	const char *ReadString(void);		// Read a string untill NULL
	bool IsMember(const char *string,bool casematch=true);	// Check if string is a member
	int GetCount(void);					// Get Count of strings
	bool Remove(const char *string);	// Remove the string from list
	bool Swap(MStringList &list);		// Swap Two Lists
	};

#endif // MStringList_h

