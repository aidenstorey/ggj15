// Engine Includes
#include <engine.h>

// Local Includes
#include "scene_id_list.h"

// This Include
#include "scene_endgame.h"

// Implementation
CSceneEndgame::CSceneEndgame( int _kiSceneID ) :
	CScene( _kiSceneID )
{
	this->m_bIsInitialised = true;

	CViewport* pMainViewport = this->AddViewport();
	if( pMainViewport == nullptr )
	{
		this->m_bIsInitialised = false;
	}
	else
	{
		// Initialise viewport
		unsigned int uiScreenWidth = static_cast< unsigned int >( CEngine::GetInstance().GetClientWidth() );
		unsigned int uiScreenHeight = static_cast< unsigned int >( CEngine::GetInstance().GetClientHeight() );
		pMainViewport->Initialise( 0, 0, uiScreenWidth, uiScreenHeight, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

		// Crete camera
		unsigned int iCam1ID = 0;
		this->SetCamera( m_pMainCamera = dynamic_cast< CCamera* >( pMainViewport->CreateCamera( 10, CAMERAMODE_ORTHOGONAL, 1, 200, 1, iCam1ID ) ) );
		this->m_pMainCamera->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, -10.0f ) );
		
		this->AttachChild( CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"", L"EndScreen.png", 800.0f, 600.0f ) );
	}
}

CSceneEndgame::~CSceneEndgame()
{}

void CSceneEndgame::CloseApp()
{
	this->m_bClose = true;
}

bool CSceneEndgame::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	if ( CEngine::GetInstance().GetInputManager().IsPressed( VK_ESCAPE ) )
	{
		bReturn = false;
	}

	return ( bReturn );
}