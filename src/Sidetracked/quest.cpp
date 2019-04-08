// This Include
#include "quest.h"

// Implementation
CQuest::CQuest( CItem* _pItem, unsigned int _uiProgression ) :
	m_pItem( _pItem ), m_uiProgression( _uiProgression )
{}

bool CQuest::IsCompleted( CPlayer* _pPlayer )
{
	bool bReturn{ false };

	if ( _pPlayer->GetProgress() >= this->m_uiProgression )
	{
		if ( _pPlayer->HasItem( this->m_pItem ) )
		{
			_pPlayer->IncrementProgress();
			_pPlayer->RemoveItem( this->m_pItem );
			bReturn = true;
		}
	}

	return ( bReturn );
}