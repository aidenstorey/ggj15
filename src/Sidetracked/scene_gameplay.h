#ifndef GUARD_SCENE_GAMEPLAY_H
#define GUARD_SCENE_GAMEPLAY_H

// Library Includes
#include <string>

// Engine Includes
#include <scene.h>
#include <text.h>

// Local Includes
#include "entity_manager.h"
#include "tile.h"
#include "quest.h"

// Constants 
enum EGameState
{
	GAMESTATE_TRAVELLING,
	GAMESTATE_TRANSITIONING,
	GAMESTATE_INTERACTING,
};

// Prototypes
class CSceneGameplay : public CScene
{
	// Member Functions
public:
	// Constructors
	CSceneGameplay( const int _kiSceneID );
	~CSceneGameplay();

	// Other
	void RunEvent( std::string _krEvent );

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:
	// Accessors
	bool IsOnGrid( const unsigned int& _krXIndex, const unsigned int& _krYIndex );

	CItem* GetItem( const std::wstring _wstrName, const int& _krRequiredProgress );
	CNPC* GetNPC( const std::wstring _wstrName );

	// Other 
	void CreateWorldGrid();
	void PopulateWorld();
	void PopulateNPC();
	void PopulateItem();
	void PopulateQuest();
	void PopulateDialog();
	void PopulateTraversable();
	void PopulateEvents();

	void HandleMovement( const unsigned int& _krXIndex, const unsigned int& _krYIndex );
	void MovePlayer( const unsigned int& _krXIndex, const unsigned int& _krYIndex );
	
	void ShowInteractionText( std::wstring _krOutput );
	void SetCameraTarget( const D3DXVECTOR2& _krTarget );

	void SpawnItem( std::string _strItem );
	void SpawnNPC( std::string _strItem );
	void StartQuest( std::string _strItem );
	void AttachDialog( std::string _strItem );
	void ChangeTexture( std::string _strItem );

	// Member Variables
public:
protected:
private:
	CCamera*			m_pMainCamera{ nullptr };

	CEntityManager		m_EntityManager;

	CTile**				m_WorldGrid;
	const unsigned int	m_uiWidth{ 91 };
	const unsigned int	m_uiHeight{ 91 };
	const float			m_fTileSize{ 64.0f };

	CPlayer*			m_pPlayer{ nullptr };

	EGameState			m_eState{ GAMESTATE_TRAVELLING };
	CNPC*				m_pInteractingNPC{ nullptr };

	std::vector< CNPC* >	m_NPCList;
	std::vector< CItem* >	m_ItemList;
	std::vector< CQuest* >	m_QuestList;
	std::vector< CDialog* >	m_DialogList;
	std::vector< CEvent* >	m_EventList;

	std::wstring				m_PlayerTextures[ 4 ];

	bool					m_bShowText{ false };
	CText*					m_pInteractionText{ nullptr };
	CText*					m_pDialogText{ nullptr };
	CTextured2D*			m_pDialogTexture{ nullptr };
	float					m_fTextTimeElapsed{ 0.0f };
	const float				m_fTextDissapearTime{ 2.0f };

	float					m_fTransitioningTimeElapsed{ 0.0f };
	const float				m_fTransitioningTimeRequired{ 0.3f };
	D3DXVECTOR2				m_vTransitioningStart;

	int						m_iAnimationFrame{ 0 };
	const int				m_iAnimationMaxFrame{ 4 };
};

#endif // GUARD_SCENE_GAMEPLAY_H