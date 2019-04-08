#ifndef GUARD_ITEM_H
#define GUARD_ITEM_H

// Library Includes
#include <string>

// Local Includes
#include "entity.h"

// Prototypes
class CItem : public CEntity
{
	friend class CEntityManager;

	// Member Functions
public:
	// Accessors
	std::wstring GetName() const;

	unsigned int GetRequiredProgress() const;

	EEntityType GetType() const
	{
		return ( ENTITY_ITEM );
	}

protected:
	// Constructors
	CItem( CTextured2D* _pTexture, std::wstring _wstrName, const unsigned int& _krRequiredProgress );

private:

	// Member Variables
public:
protected:
private:
	std::wstring	m_wstrName;
	unsigned int	m_uiRequiredProgress;
};

#endif // GUARD_NPC_H