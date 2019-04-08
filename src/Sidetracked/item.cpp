// This Include
#include "item.h"

// Implementation
CItem::CItem( CTextured2D* _pTexture, std::wstring _wstrName, const unsigned int& _krRequiredProgress ) :
	CEntity( _pTexture ), m_wstrName( _wstrName ), m_uiRequiredProgress( _krRequiredProgress )
{}

std::wstring CItem::GetName() const
{
	return ( this->m_wstrName );
}

unsigned int CItem::GetRequiredProgress() const
{
	return ( this->m_uiRequiredProgress );
}