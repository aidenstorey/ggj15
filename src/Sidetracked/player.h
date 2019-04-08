#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

// Library Includes
#include <vector>
#include <unordered_map>

// Local Includes
#include "item.h"
#include "entity.h"
#include "event.h"
#include "tile.h"

// Constants
enum EDirectionType
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,

	DIRECTION_NULL,
};

// Prototypes
class CPlayer : public CEntity
{
	friend class CEntityManager;

	// Member Functions
public:
	// Accessors
	void SetIndex( const unsigned int& _krX, const unsigned int& _krY );

	void SetXIndex( const unsigned int& _krX );
	unsigned int GetXIndex() const;

	void SetYIndex( const unsigned int& _krY );
	unsigned int GetYIndex() const;

	unsigned int GetProgress() const;

	bool HasItem( CItem* _pItem );

	EEntityType GetType() const
	{
		return ( ENTITY_PLAYER );
	}

	// Other
	void IncrementProgress();
	void PickupItem( CItem* _pItem );
	void RemoveItem( CItem* _pItem );

	void AddEvent( CEvent* _pEvent );
	
protected:
	// Constructors
	CPlayer( CTextured2D* _pTexture );

	// Other
	void ResetInventory();
	void ResetMatrix() override;

private:

	// Member Variables
public:
protected:
private:
	unsigned int			m_uiXIndex;
	unsigned int			m_uiYIndex;

	unsigned int			m_uiProgress{ 0 };

	std::vector< CItem* >	m_InventoryList;
	std::vector< CEvent* >	m_EventList;
	CEvent					m_PantsOff;
};

#endif // GUARD_NPC_H