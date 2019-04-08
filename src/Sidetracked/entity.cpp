// This Include 
#include "entity.h"

// Implementation
CEntity::CEntity( CTextured2D* _pTexture ) :
	m_pTexture( _pTexture )
{
	this->AttachChild( _pTexture );
}

void CEntity::SetPosition( const D3DXVECTOR2& _krPosition )
{
	this->m_vPosition = _krPosition;
	this->ResetMatrix();
}

void CEntity::SetPosition( const float& _krX, const float& _krY )
{
	this->m_vPosition = D3DXVECTOR2( _krX, _krY );
	this->ResetMatrix();
}

void CEntity::SetPositionX( const float& _krX )
{
	this->m_vPosition.x = _krX;
	this->ResetMatrix();
}

void CEntity::SetPositionY( const float& _krY )
{
	this->m_vPosition.y = _krY;
	this->ResetMatrix();
}

D3DXVECTOR2 CEntity::GetPosition() const
{
	return ( this->m_vPosition );
}

float CEntity::GetPositionX() const
{
	return ( this->m_vPosition.x );
}

float CEntity::GetPositionY() const
{
	return ( this->m_vPosition.y );
}

void CEntity::SetRotation( const float& _krRotation )
{
	this->m_fRotation = _krRotation;
	this->ResetMatrix();
}

float CEntity::GetRotation() const
{
	return ( this->m_fRotation );
}

void CEntity::SetContainer( CTile* _pContainer )
{
	this->m_pContainer = _pContainer;
}

CTile* CEntity::GetContainer()
{
	return ( this->m_pContainer );
}

void CEntity::SetTexture( CTexture* _pTexture )
{
	this->m_pTexture->SetTexture( _pTexture );
}

void CEntity::ResetMatrix()
{
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslation;
	D3DXMatrixRotationYawPitchRoll( &matRotate, 0.0f, 0.0f, m_fRotation );
	D3DXMatrixTranslation( &matTranslation, this->m_vPosition.x, this->m_vPosition.y, 0.0f );

	m_matWorld = matRotate * matTranslation;
}
