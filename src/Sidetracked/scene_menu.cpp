// Engine Includes
#include <engine.h>

// Local Includes
#include "audio_files.h"
#include "scene_id_list.h"

// This Include
#include "scene_menu.h"

// Implementation
CSceneMenu::CSceneMenu( int _kiSceneID ) :
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

		this->m_pPlayButton = CEngine::GetInstance().GetObject2DManager().CreateButtonAsRectangle( L"PlayButton", L"Play.png", 200.0f, 100.0f );
		this->m_pPlayButton->SetClickFunction( []{ CEngine::GetInstance().GetSceneManager().SetCurrentScene( SCENE_GAMEPLAY ); } );
		this->AttachUIElement( this->m_pPlayButton );

		this->m_pTextured = CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"", L"Protagonist_Left0000.png", 64.0f, 64.0f, 450.0f, -170.0f );
		this->AttachChild( this->m_pTextured );

		this->AttachChild( CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"", L"TitleScreen.png", 800.0f, 600.0f ) );
	}
}

CSceneMenu::~CSceneMenu()
{}

bool CSceneMenu::OnActivate()
{
	bool bReturn{ true };

	CEngine::GetInstance().GetAudioManager().Play( AUDIO_MUSIC, true );
	CEngine::GetInstance().GetAudioManager().SetVolume( AUDIO_MUSIC, -2500 );

	return ( bReturn );
}

bool CSceneMenu::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	static int iFrame{ 0 };

	static float fTimeElapesd{ 0.0f };
	static float fTweenElapsed{ 0.0f };
	const float fTimeTaken{ 20.0f };

	fTimeElapesd += _kfTimeDelta;
	if ( fTimeElapesd > 0.3f )
	{
		iFrame = ( iFrame + 1 ) % 4;
		this->m_pTextured->SetTexture( CEngine::GetInstance().GetTextureManager().GetTexture( std::wstring( L"Protagonist_Left000" + std::to_wstring( iFrame ) + L".png" ) ) );
		
		fTimeElapesd = 0.0f;
	}

	fTweenElapsed += _kfTimeDelta;

	D3DXVECTOR2 vDirection{ this->m_vEnd - this->m_vStart };
	vDirection /= fTimeTaken;
	vDirection *= fTweenElapsed;

	this->m_pTextured->SetPosition( this->m_vStart + vDirection );

	if ( fTweenElapsed > fTimeTaken )
	{
		fTweenElapsed = 0.0f;
	}

	return ( bReturn );
}