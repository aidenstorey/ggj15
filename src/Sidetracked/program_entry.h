#ifndef GUARD_PROGRAM_ENTRY_H
#define GUARD_PROGRAM_ENTRY_H

// Library Includes
#include <Windows.h>

// Prototypes
class CProgramEntry
{
	// Member Functions
public:
	// Constructors
	CProgramEntry();
	~CProgramEntry();

	// Other 
	bool Initialise( HINSTANCE _hInstance );
	void InitialiseAudio();
	int Run();

protected:
private:

	// Member Variables
public:
protected:
private:
};

#endif // GUARD_PROGRAM_ENTRY_H