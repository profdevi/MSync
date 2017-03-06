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
#ifndef MFileNameValues_h
#define MFileNameValues_h

////////////////////////////////////////////////
#include <vector>
#include <string>

/////////////////////////////////////////////////
using namespace std;

/////////////////////////////////////////////////
#include "MStdLib.h"

/*
	MFileNameValues will read and save name value pairs
	from a text input file of the following form.

	# Pound marks the beginning of a comment
	# Blank lines are skipped
	# Equal seperates name and value

	one=Hello World		# This is the first name/value
	two=Hello Again		# This is the second name/value
	one=Repeat again	# This is same name, but different value from above

*/
//******************************************************
//**  MFileNameValues class
//******************************************************
class MFileNameValues
	{
	////////////////////////////////////////////////
	struct GElement
		{
		string Name;
		string Value;
		};

	////////////////////////////////////////////////
	vector<GElement> mData;

	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MFileNameValues(void);
	~MFileNameValues(void);
	bool Create(string filename);
	bool Destroy(void);
	bool Print(void);					// Print out the name values
	int GetCount(void);					// Get Count of elements
	const char *GetName(int index);		// Get Name of string pair
	const char *GetValue(int index);	// Get Name of string pair
	bool Save(const char *filename);	// Save to file
	};

#endif // MFileNameValues_h

