#ifndef GUARD_NPC_H
#define GUARD_NPC_H

// Library Includes
#include <unordered_map>

// Local Includes
#include "entity.h"
#include "dialog.h"
#include "quest.h"

// Prototypes
class CNPC : public CEntity
{
	friend class CEntityManager;

	// Member Functions
public:
	// Accessors
	std::wstring GetName() const;

	EEntityType GetType() const
	{
		return ( ENTITY_NPC );
	}

	bool ShowDialogText( CPlayer* _pPlayer );
	void AddDialog(int _kiDialogID, CDialog* _pDialog);
	void AddQuest( CQuest* _pQuest, int _iProgression );
	bool CanInteract( int _iProgression );
	void CheckQuest( CPlayer* _pPlayer );

protected:
	// Constructors
	CNPC( std::wstring _wstrName, CTextured2D* _pTexture );

private:

	// Member Variables
public:
protected:
	std::wstring							m_wstrName;
	std::unordered_map < int, CDialog* >	m_pDialogMap;
	std::unordered_map < int, CQuest* >		m_QuestMap;

private:
};

#endif // GUARD_NPC_H