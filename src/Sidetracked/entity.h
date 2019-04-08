#ifndef GUARD_ENTITY_H
#define GUARD_ENTITY_H

// Engine Inclues
#include <drawable.h>
#include <textured_2d.h>

// Local Includes
#include "entity_type_list.h"
#include "tile.h"

// Prototypes
class CEntity : public CDrawable
{
	// Member Functions
public:

	// Accessors
	void SetPosition( const D3DXVECTOR2& _krPosition );
	void SetPosition( const float& _krX, const float& _krY );
	void SetPositionX( const float& _krX );
	void SetPositionY( const float& _krY );

	D3DXVECTOR2 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;

	void SetRotation( const float& _krRotation );
	float GetRotation() const;

	void SetContainer( CTile* _pContainer );
	CTile* GetContainer();

	void SetTexture( CTexture* _pTexture );

	virtual EEntityType GetType() const = 0;

protected:
	// Constructors
	CEntity( CTextured2D* _pTexture );

	// Other
	virtual void ResetMatrix();

private:

	// Member Variables
public:
protected:
	CTile*					m_pContainer{ nullptr };

	CTextured2D*	m_pTexture;
	D3DXVECTOR2 m_vPosition;

private:
	float			m_fRotation;
};

#endif // GUARD_ENTITY_H