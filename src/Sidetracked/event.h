#ifndef GUARD_EVENT_H
#define GUARD_EVENT_H

// Library Includes
#include <string>

// Prototypes
class CEvent
{
	// Member Functions
public:
	// Constructors
	CEvent( const int& _krProgress, std::string _strEvent );

	// Accessors
	int GetProgressRequired() const;

	// Other
	void Trigger();
protected:
private:

	// Member Variables
public:
protected:
private:
	int			m_iProgress;
	std::string	m_strEvent;
};

#endif // GUARD_EVENT_H