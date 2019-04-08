#ifndef GUARD_ENTITY_MANAGER_H
#define GUARD_ENTITY_MANAGER_H

// Library Includes
#include <string>
#include <vector>

// Local Includes
#include "entity.h"
#include "item.h"
#include "npc.h"
#include "player.h"

// Prototypes
class CEntityManager
{
	// Member Functions
public:
	// Constructors
	~CEntityManager();

	// Other
	CPlayer* CreatePlayer( CTextured2D* _pTexture );
	CItem* CreateItem( std::wstring _wstrName, CTextured2D* _pTexture, const unsigned int& _krRequiredProgress );
	CNPC* CreateNPC( std::wstring _wstrName, CTextured2D* _pTexture );

protected:
private:

	// Member Variables
public:
protected:
private:
	std::vector< CEntity* > m_EntityList;

};

#endif // GUARD_ENTITY_MANAGER_H