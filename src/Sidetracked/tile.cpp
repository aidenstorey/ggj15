// Local Includes
#include "entity.h"

// This Include 
#include "tile.h"

// Implementation
CTile::CTile()
{}

void CTile::SetPosition( const D3DXVECTOR2& _krPosition )
{
	this->m_vPosition = _krPosition;
	this->m_bUpdateMatrix = true;
}

void CTile::SetPosition( const float& _krX, const float& _krY )
{
	this->m_vPosition = D3DXVECTOR2( _krX, _krY );
	this->m_bUpdateMatrix = true;
}

void CTile::SetPositionX( const float& _krX )
{
	this->m_vPosition.x = _krX;
	this->m_bUpdateMatrix = true;
}

void CTile::SetPositionY( const float& _krY )
{
	this->m_vPosition.y = _krY;
	this->m_bUpdateMatrix = true;
}

D3DXVECTOR2 CTile::GetPosition() const
{
	return ( this->m_vPosition );
}

float CTile::GetPositionX() const
{
	return ( this->m_vPosition.x );
}

float CTile::GetPositionY() const
{
	return ( this->m_vPosition.y );
}


CEntity* CTile::GetEntity()
{
	return ( this->m_pEntity );
}

void CTile::AttatchEntity( CEntity* _pEntity )
{
	this->m_pEntity = _pEntity;
	this->m_pEntity->SetContainer( this );
	this->AttachChild( this->m_pEntity );
}

void CTile::DetachEntity( CEntity* _pEntity )
{
	this->m_pEntity->SetContainer( nullptr );
	this->m_pEntity = nullptr;
	this->DetachChild( _pEntity );
}

void CTile::SetTraversable( const bool& _krTraversable )
{
	this->m_bTraversable = _krTraversable;
}

bool CTile::IsOccupied()
{
	return ( this->m_pEntity != nullptr );
}

bool CTile::IsTraversable()
{
	return ( this->m_bTraversable );
}

bool CTile::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn( true );

	if ( this->m_bUpdateMatrix )
	{
		D3DXMATRIX matTranslation;
		D3DXMatrixTranslation( &matTranslation, this->m_vPosition.x, this->m_vPosition.y, 0.0f );

		this->m_matWorld = matTranslation;
	}

	return ( bReturn );
}