// This Include
#include "entity_manager.h"

// Implementation
CEntityManager::~CEntityManager()
{
	for ( auto& aEntity : this->m_EntityList )
	{
		delete ( aEntity );
	}
}

CPlayer* CEntityManager::CreatePlayer( CTextured2D* _pTexture )
{
	CPlayer* pReturn = new CPlayer( _pTexture );

	this->m_EntityList.push_back( pReturn );

	return ( pReturn );
}

CItem* CEntityManager::CreateItem( std::wstring _wstrName, CTextured2D* _pTexture, const unsigned int& _krRequiredProgress )
{
	CItem* pReturn = new CItem( _pTexture, _wstrName, _krRequiredProgress );
	
	this->m_EntityList.push_back( pReturn );
	
	return ( pReturn );
}

CNPC* CEntityManager::CreateNPC( std::wstring _wstrName, CTextured2D* _pTexture )
{
	CNPC* pReturn = new CNPC( _wstrName, _pTexture );
	
	this->m_EntityList.push_back( pReturn );
	
	return ( pReturn );
}