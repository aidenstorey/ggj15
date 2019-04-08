// Library Includes
#include <engine.h>

// Local Includes
#include "scene_gameplay.h"

// This Include
#include "event.h"

// Implementation
CEvent::CEvent( const int& _krProgress, std::string _strEventText ) : 
	m_iProgress( _krProgress ), m_strEvent( _strEventText )
{}

int CEvent::GetProgressRequired() const
{
	return ( this->m_iProgress );
}

void CEvent::Trigger()
{
	reinterpret_cast< CSceneGameplay* >( CEngine::GetInstance().GetSceneManager().GetCurrentScene() )->RunEvent( this->m_strEvent );
}