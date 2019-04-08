#ifndef GUARD_TILE_H
#define GUARD_TILE_H

// Engine Includes
#include <drawable.h>

// Local Includes

// Forward Decleration
class CEntity;

// Prototypes
class CTile : public CDrawable
{
	// Member Functions
public:
	// Constructors
	CTile();

	// Accessors
	void SetPosition( const D3DXVECTOR2& _krPosition );
	void SetPosition( const float& _krX, const float& _krY );
	void SetPositionX( const float& _krX );
	void SetPositionY( const float& _krY );

	D3DXVECTOR2 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;
	
	CEntity* GetEntity();
	void AttatchEntity( CEntity* _pEntity );
	void DetachEntity( CEntity* _pEntity );

	void SetTraversable( const bool& _krTraversable );

	bool IsOccupied();
	bool IsTraversable();

protected:
	// Other 
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:

	// Member Variables
public:
protected:
private:
	bool		m_bUpdateMatrix{ false };
	D3DXVECTOR2 m_vPosition;

	CEntity*	m_pEntity{ nullptr };

	bool		m_bTraversable{ true };
};

#endif // GUARD_TILE_H