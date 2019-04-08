#ifndef GUARD_SCENE_MENU_H
#define GUARD_SCENE_MENU_H

// Engine Includes
#include <scene.h>

// Prototypes
class CSceneMenu : public CScene
{
	// Member Functions
public:
	// Constructors
	CSceneMenu( int _kiSceneID );
	~CSceneMenu();

	// Other
	bool OnActivate() override;

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:

	// Member Variables
public:
protected:
private:
	CCamera*			m_pMainCamera{ nullptr };
	CButtonRectangle*	m_pPlayButton{ nullptr };

	D3DXVECTOR2			m_vStart{ 450.0f, -170.0f };
	D3DXVECTOR2			m_vEnd{ -450.0f, -170.0f };

	CTextured2D*		m_pTextured{ nullptr };
};

#endif // GUARD_SCENE_MENU_H