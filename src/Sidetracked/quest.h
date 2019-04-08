#ifndef GUARD_QUEST_H
#define GUARD_QUEST_H

// Local Includes
#include "item.h"
#include "player.h"

// Prototypes
class CQuest
{

	// Member Functions
public:
	// Constructors
	CQuest( CItem* _pItem, unsigned int _uiProgression );

	// Other
	bool IsCompleted( CPlayer* _pPlayer );

protected:
private:

	// Member Variables
public:
protected:
	CItem*			m_pItem;
	unsigned int	m_uiProgression;
private:

};

#endif //GUARD_QUEST_H