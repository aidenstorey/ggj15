#ifndef GUARD_SCENE_ENDGAME_H
#define GUARD_SCENE_ENDGAME_H

// Engine Includes
#include <scene.h>

// Prototypes
class CSceneEndgame : public CScene
{
	// Member Functions
public:
	// Constructors
	CSceneEndgame( int _kiSceneID );
	~CSceneEndgame();

	// Other
	void CloseApp();

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:

	// Member Variables
public:
protected:
private:
	CCamera*			m_pMainCamera{ nullptr };
	CButtonRectangle*	m_pEndButton{ nullptr };

	bool				m_bClose{ false };
};

#endif // GUARD_SCENE_ENDGAME_H