// Library Includes
#include <engine.h>

// Local Includes
#include "scene_id_list.h"

// This Include 
#include "player.h"

// Implementation
CPlayer::CPlayer( CTextured2D* _pTexture ) :
	CEntity( _pTexture ), m_PantsOff( 100, "changetexture,Builder,Builder_NoPants.png" )
{}

void CPlayer::SetIndex( const unsigned int& _krX, const unsigned int& _krY )
{
	this->m_uiXIndex = _krX;
	this->m_uiYIndex = _krY;
}

void CPlayer::SetXIndex( const unsigned int& _krX )
{
	this->m_uiXIndex = _krX;
}

unsigned int CPlayer::GetXIndex() const
{
	return ( this->m_uiXIndex );
}

void CPlayer::SetYIndex( const unsigned int& _krY )
{
	this->m_uiYIndex = _krY;
}

unsigned int CPlayer::GetYIndex() const
{
	return ( this->m_uiYIndex );
}

unsigned int CPlayer::GetProgress() const
{
	return ( this->m_uiProgress );
}

bool CPlayer::HasItem( CItem* _pItem )
{
	return ( std::find( this->m_InventoryList.begin(), this->m_InventoryList.end(), _pItem ) != this->m_InventoryList.end() );
}

void CPlayer::IncrementProgress()
{
	this->m_uiProgress++;

	if ( this->m_uiProgress == 13 )
	{
		int iBreak = 5;
	}

	for ( auto& aEvent : this->m_EventList )
	{
		if ( aEvent->GetProgressRequired() == this->m_uiProgress )
		{
			aEvent->Trigger();
		}
	}
}

void CPlayer::PickupItem( CItem* _pItem )
{
	this->m_InventoryList.push_back( _pItem );
	CEngine::GetInstance().GetSceneManager().GetScene( SCENE_GAMEPLAY )->AttachUIElement( _pItem );
	this->ResetInventory();

	if ( this->m_uiProgress == 8 )
	{
		this->m_PantsOff.Trigger();
	}
}

void CPlayer::RemoveItem( CItem* _pItem )
{
	std::vector< CItem* >::iterator iterDelete = std::find( this->m_InventoryList.begin(), this->m_InventoryList.end(), _pItem );
	if ( iterDelete != this->m_InventoryList.end() )
	{
		this->m_InventoryList.erase( iterDelete );
		CEngine::GetInstance().GetSceneManager().GetScene( SCENE_GAMEPLAY )->DetachUIElement( _pItem );
	}
	this->ResetInventory();
}

void CPlayer::AddEvent( CEvent* _pEvent )
{
	this->m_EventList.push_back( _pEvent );
}

void CPlayer::ResetInventory()
{
	const float fTileSize{ 64.0f };

	float fWidth = static_cast< float >( CEngine::GetInstance().GetClientWidth() );
	float fHeight = static_cast< float >( CEngine::GetInstance().GetClientHeight() );

	D3DXVECTOR2 vInitialPosition{ ( fTileSize / 2.0f ) - ( fWidth / 2.0f ), ( fHeight / 2.0f ) - ( fTileSize / 2.0f ) };
	for ( unsigned int uiCount = 0; uiCount < this->m_InventoryList.size(); ++uiCount )
	{
		this->m_InventoryList[ uiCount ]->SetPosition( vInitialPosition + D3DXVECTOR2( fTileSize * uiCount, 0.0f ) );
	}
}

void CPlayer::ResetMatrix()
{
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation( &matTranslation, this->m_vPosition.x, this->m_vPosition.y, -1.0f );

	m_matWorld = matTranslation;
}