// This Include
#include "npc.h"

// Implementation
CNPC::CNPC( std::wstring _wstrName, CTextured2D* _pTexture ) :
CEntity( _pTexture ), m_wstrName( _wstrName )
{}

std::wstring CNPC::GetName() const
{
	return ( this->m_wstrName );
}

/*****************
*
* Run when colliding with an npc
*
******************/
bool CNPC::ShowDialogText( CPlayer* _pPlayer )
{
	bool bReturn{ false };

	if ( this->m_pDialogMap.find( _pPlayer->GetProgress() ) != this->m_pDialogMap.end() )
	{
		bReturn = m_pDialogMap.at( _pPlayer->GetProgress() )->NextDialog();

		if ( !bReturn )
		{
			_pPlayer->IncrementProgress();
		}
	}

	return ( bReturn );
}

void CNPC::AddDialog(int _kiDialogID, CDialog* _pDialog)
{
	m_pDialogMap.insert(std::make_pair(_kiDialogID, _pDialog));
}

void CNPC::AddQuest( CQuest* _pQuest, int _iProgression )
{
	this->m_QuestMap.insert( std::make_pair( _iProgression, _pQuest ) );
}

bool CNPC::CanInteract( int _iProgression )
{
	return ( m_pDialogMap.find( _iProgression ) != m_pDialogMap.end() );
}

void CNPC::CheckQuest( CPlayer* _pPlayer )
{
	if ( m_QuestMap.find( _pPlayer->GetProgress() ) != m_QuestMap.end() )
	{
		m_QuestMap.at( _pPlayer->GetProgress() )->IsCompleted( _pPlayer );
	}
}