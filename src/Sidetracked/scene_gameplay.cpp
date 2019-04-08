#define NOMINMAX

// Library Includes
#include <algorithm>
#include <fstream>
#include <sstream>

// Engine Includes
#include <engine.h>
#include <math_functions.h>

// Local Includes
#include "audio_files.h"
#include "scene_id_list.h"

// This Include
#include "scene_gameplay.h"

// Implementation
CSceneGameplay::CSceneGameplay( const int _kiSceneID ) :
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
		unsigned int uiCam1ID = 0;
		this->SetCamera( m_pMainCamera = dynamic_cast< CCamera* >( pMainViewport->CreateCamera( 10, CAMERAMODE_ORTHOGONAL, 1, 200, 1, uiCam1ID ) ) );
		this->m_pMainCamera->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, -10.0f ) );
				
		// Create Interaction text
		CEngine::GetInstance().GetTextManager().CreateD3DFont( L"InteractionText" );
		this->m_pInteractionText = CEngine::GetInstance().GetTextManager().Create(L"", 10.0f, static_cast<float>(uiScreenHeight - 20));
		CEngine::GetInstance().GetTextManager().SetFont( this->m_pInteractionText, L"InteractionText" );		
		this->AttachUIElement(this->m_pInteractionText);

		// Create Dialog text
		this->m_pDialogTexture = CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle(L"", L"Dialoge.png", 600.0f, 150.0f, 0.0f, -200.0f);
		this->AttachUIElement(m_pDialogTexture);
		this->m_pDialogTexture->SetVisible(false);

		this->m_pDialogText = CEngine::GetInstance().GetTextManager().Create( L"", 150.0f, static_cast< float >( uiScreenHeight ) / 1.337f );
		CEngine::GetInstance().GetTextManager().SetFont(this->m_pDialogText, L"InteractionText");
		this->AttachUIElement(this->m_pDialogText);

		this->CreateWorldGrid();
		this->PopulateWorld();
		float fHTileSize{ this->m_fTileSize / 2.0f };
		this->AttachChild( CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"background", L"background.png", this->m_uiWidth * this->m_fTileSize, this->m_uiHeight * this->m_fTileSize, -fHTileSize, fHTileSize ) );		
	}
}

CSceneGameplay::~CSceneGameplay()
{
	for ( auto& aQuest : this->m_QuestList )
	{
		delete ( aQuest );
	}

	for (auto& aDialog : this->m_DialogList)
	{
		delete (aDialog);
	}

	for ( auto& aEvent : this->m_EventList )
	{
		delete ( aEvent );
	}
}

bool CSceneGameplay::IsOnGrid( const unsigned int& _krXIndex, const unsigned int& _krYIndex )
{
	bool bReturn{ false };

	if ( _krXIndex >= 0 && _krXIndex < this->m_uiWidth &&
		 _krYIndex >= 0 && _krYIndex < this->m_uiHeight )
	{
		bReturn = true;
	}

	return ( bReturn );
}

CItem* CSceneGameplay::GetItem( const std::wstring _wstrName, const int& _krRequiredProgress )
{
	CItem* pReturn{ nullptr };

	for ( auto& aItem : this->m_ItemList )
	{
		if ( aItem->GetName() == _wstrName && aItem->GetRequiredProgress() == _krRequiredProgress )
		{
			pReturn = aItem;
			break;
		}
	}

	return ( pReturn );
}

CNPC* CSceneGameplay::GetNPC( const std::wstring _wstrName )
{
	CNPC* pReturn{ nullptr };

	for ( auto& aNPC : this->m_NPCList )
	{
		if ( aNPC->GetName() == _wstrName )
		{
			pReturn = aNPC;
			break;
		}
	}

	return ( pReturn );
}


void CSceneGameplay::CreateWorldGrid()
{
	float fXStart{ ( static_cast< float >( this->m_uiWidth ) * -0.5f ) * this->m_fTileSize };
	float fYStart{ ( static_cast< float >( this->m_uiHeight ) * 0.5f ) * this->m_fTileSize };	

	// Create the world grid
	this->m_WorldGrid = new CTile*[ this->m_uiHeight ];
	for ( unsigned int uiHeight = 0; uiHeight < this->m_uiHeight; ++uiHeight )
	{
		this->m_WorldGrid[ uiHeight ] = new CTile[ this->m_uiWidth ];

		// Set positions of new tiles
		for ( unsigned int uiWidth = 0; uiWidth < this->m_uiWidth; ++uiWidth )
		{
			this->m_WorldGrid[ uiHeight ][ uiWidth ].SetPosition( { fXStart + this->m_fTileSize * uiWidth, fYStart - this->m_fTileSize * uiHeight } );
			this->AttachChild( &this->m_WorldGrid[ uiHeight ][ uiWidth ] );
		}
	}
	
	// Create player 
	CTextured2D* pTextured = CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"Protagonist", L"Protagonist.png", this->m_fTileSize, this->m_fTileSize );

	m_PlayerTextures[ DIRECTION_LEFT ] = std::wstring( L"Protagonist_Left000" );
	m_PlayerTextures[ DIRECTION_RIGHT ] = std::wstring( L"Protagonist_Right000" );
	m_PlayerTextures[ DIRECTION_UP ] = std::wstring( L"Protagonist_Back000" );
	m_PlayerTextures[ DIRECTION_DOWN ] = std::wstring( L"Protagonist_Front000" );

	this->m_pPlayer = this->m_EntityManager.CreatePlayer( pTextured );
	this->MovePlayer( 20, 21 );	
}

void CSceneGameplay::PopulateWorld()
{
	this->PopulateNPC();
	this->PopulateItem();
	this->PopulateQuest();
	this->PopulateDialog();
	this->PopulateTraversable();
	this->PopulateEvents();
}

void CSceneGameplay::PopulateNPC()
{
	std::ifstream inStream( "Assets/Data/NPC.txt" );
	if ( inStream.is_open() )
	{
		std::string strLine;

		while ( std::getline( inStream, strLine ) )
		{
			this->SpawnNPC( strLine );
		}

		inStream.close();
	}	
}

void CSceneGameplay::PopulateItem()
{
	std::ifstream inStream( "Assets/Data/Item.txt" );
	if ( inStream.is_open() )
	{
		std::string strLine;

		while ( std::getline( inStream, strLine ) )
		{
			this->SpawnItem( strLine );
		}

		inStream.close();
	}
}

void CSceneGameplay::PopulateQuest()
{
	std::ifstream QuestInput("Assets/Data/Quests.txt");

	if (QuestInput.is_open())
	{
		std::string strLine;
		while (std::getline(QuestInput, strLine))
		{
			this->StartQuest( strLine );
		}
		QuestInput.close();
	}

}

void CSceneGameplay::PopulateDialog()
{
	//Open the Dialog txt file.
	const int iDataSize = 4;
	std::ifstream DialogInput("Assets/Data/Dialog.txt");
	
	
	if (DialogInput.is_open())
	{
		CDialog* pDialog = nullptr;
		std::string strLine;
		while (std::getline(DialogInput, strLine))
		{
			if (strLine[0] == 'n' && strLine[1] == 'd')
			{
				std::stringstream strLineInput(strLine);
				std::string strCell;
				std::wstring wstrData[iDataSize];
				int iCount{ 0 };

				while (std::getline(strLineInput, strCell, ','))
				{
					//Deal with each cell value
					wstrData[iCount] = std::wstring(strCell.begin(), strCell.end());
					++iCount;
				}
				pDialog = new CDialog( m_pDialogText, m_pDialogTexture, std::stoi( wstrData[ 3 ] ), std::stoi( wstrData[ 2 ] ) );
				CNPC* pTargetNPC = this->GetNPC(wstrData[1]);
				if (pTargetNPC != nullptr)
				{
					pTargetNPC->AddDialog(std::stoi(wstrData[2]), pDialog);
				}
				
				m_DialogList.push_back(pDialog);
			}
			else if (pDialog != nullptr)
			{
				std::wstringstream wssDialog;
				for(size_t cursor = 0; cursor < strLine.length(); ++cursor)
				{
					if(strLine[cursor] == '\\' && ( (cursor+1) < strLine.length() ) && strLine[cursor+1] == 'n')
					{
						wssDialog << "\n";
						cursor++;
					}
					else
					{
						wssDialog << strLine[cursor];
					}
				}
				pDialog->AddDialog(wssDialog.str());
			}	
		}
		DialogInput.close();
	}
	
}

void CSceneGameplay::PopulateTraversable()
{
	const int kiDataCount{ 4 };

	std::ifstream inStream( "Assets/Data/background.csv" );
	if ( inStream.is_open() )
	{
		std::string strLine;
		
		unsigned int uiLineCount{ 0 };
		while ( std::getline( inStream, strLine ) && uiLineCount < this->m_uiHeight )
		{
			std::stringstream lineStream( strLine );
			std::string strCell;

			unsigned int uiRowCount{ 0 };
			while ( std::getline( lineStream, strCell, ',' ) && uiRowCount < this->m_uiWidth )
			{
				this->m_WorldGrid[ uiLineCount ][ uiRowCount ].SetTraversable( std::stoi( strCell ) == -1 );

				uiRowCount++;
			}

			uiLineCount++;
		}

		inStream.close();
	}
}

void CSceneGameplay::PopulateEvents()
{
	const int kiDataCount{ 1 };

	std::ifstream inStream( "Assets/Data/Event.txt" );
	if ( inStream.is_open() )
	{
		std::string strLine;

		while ( std::getline( inStream, strLine ) )
		{
			std::stringstream lineStream( strLine );
			std::string strCell;

			std::getline( lineStream, strCell, ',' );

			int iProgress = std::stoi( strCell );
			strLine.erase( 0, strCell.size() + 1 );

			CEvent* pEvent = new CEvent( iProgress, strLine );
			this->m_pPlayer->AddEvent( pEvent );
			this->m_EventList.push_back( pEvent );
		}

		inStream.close();
	}
}

void CSceneGameplay::SpawnItem( std::string _strItem )
{
	const int kiDataCount{ 5 };

	std::stringstream lineStream( _strItem );
	std::string strCell;
	std::wstring wstrData[ kiDataCount ];

	int iCount{ 0 };
	while ( std::getline( lineStream, strCell, ',' ) && iCount < kiDataCount )
	{
		wstrData[ iCount ] = std::wstring( strCell.begin(), strCell.end() );

		iCount++;
	}

	CTextured2D* pTextured{ CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( wstrData[ 0 ], wstrData[ 1 ], this->m_fTileSize, this->m_fTileSize ) };
	CItem* pItem = this->m_EntityManager.CreateItem( wstrData[ 0 ], pTextured, std::stoi( wstrData[ 2 ] ) );

	this->m_ItemList.push_back( pItem );
	this->m_WorldGrid[ std::stoi( wstrData[ 4 ] ) ][ std::stoi( wstrData[ 3 ] ) ].AttatchEntity( pItem );
}

void CSceneGameplay::SpawnNPC( std::string _strItem )
{
	const int kiDataCount{ 4 };

	std::stringstream lineStream( _strItem );
	std::string strCell;
	std::wstring wstrData[ kiDataCount ];

	int iCount{ 0 };
	while ( std::getline( lineStream, strCell, ',' ) && iCount < kiDataCount )
	{
		wstrData[ iCount ] = std::wstring( strCell.begin(), strCell.end() );

		iCount++;
	}

	CTextured2D* pTextured{ CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( wstrData[ 0 ], wstrData[ 1 ], this->m_fTileSize, this->m_fTileSize ) };
	CNPC* pNPC = this->m_EntityManager.CreateNPC( wstrData[ 0 ], pTextured );

	this->m_NPCList.push_back( pNPC );
	this->m_WorldGrid[ std::stoi( wstrData[ 3 ] ) ][ std::stoi( wstrData[ 2 ] ) ].AttatchEntity( pNPC );
}

void CSceneGameplay::StartQuest( std::string _strItem )
{
	const int kiDataCount{ 3 };

	std::stringstream strLineInput( _strItem );
	std::string strCell;
	std::wstring wstrData[ kiDataCount ];
	int iCount{ 0 };

	while ( std::getline( strLineInput, strCell, ',' ) )
	{
		//Deal with each cell value
		wstrData[ iCount ] = std::wstring( strCell.begin(), strCell.end() );
		++iCount;
	}

	CQuest* pQuest = new CQuest( this->GetItem( wstrData[ 1 ], std::stoi( wstrData[ 2 ] ) ), std::stoi( wstrData[ 2 ] ) );
	m_QuestList.push_back( pQuest );

	CNPC* pNPC = this->GetNPC( wstrData[ 0 ] );
	if ( pNPC != nullptr )
	{
		pNPC->AddQuest( pQuest, std::stoi( wstrData[ 2 ] ) );
	}
}

void CSceneGameplay::AttachDialog( std::string _strItem )
{
	std::stringstream strLineInput( _strItem );
	std::string strCell;

	std::getline( strLineInput, strCell, ',' );
	_strItem.erase( 0, strCell.size() + 1 );

	CNPC* pNPC = this->GetNPC( std::wstring( strCell.begin(), strCell.end() ) );
	if ( pNPC != nullptr )
	{
		for ( auto& aDialog : this->m_DialogList )
		{
			if ( aDialog->GetDialogID() == std::stoi( _strItem ) )
			{
				pNPC->AddDialog( aDialog->GetRequiredProgress(), aDialog );
			}
		}
	}

}

void CSceneGameplay::ChangeTexture( std::string _strItem )
{
	std::stringstream strLineInput( _strItem );
	std::string strCell;

	std::getline( strLineInput, strCell, ',' );
	_strItem.erase( 0, strCell.size() + 1 );

	CNPC* pNPC = this->GetNPC( std::wstring( strCell.begin(), strCell.end() ) );
	if ( pNPC != nullptr )
	{
		pNPC->SetTexture( CEngine::GetInstance().GetTextureManager().GetTexture( std::wstring( _strItem.begin(), _strItem.end() ) ) );
	}
}

void CSceneGameplay::HandleMovement( const unsigned int& _krXIndex, const unsigned int& _krYIndex )
{
	if ( this->IsOnGrid( _krXIndex, _krYIndex ) )
	{
		CTile* pDestination{ &this->m_WorldGrid[ _krYIndex ][ _krXIndex ] };
		if ( !pDestination->IsOccupied() && pDestination->IsTraversable() )
		{
			this->MovePlayer( _krXIndex, _krYIndex );
		}
		else if ( pDestination->GetEntity() != nullptr )
		{
			this->m_vTransitioningStart = { 0.0f, 0.0f };
			switch ( pDestination->GetEntity()->GetType() )
			{
				case ENTITY_ITEM:
				{
					if ( pDestination->IsTraversable() )
					{
						CItem* pItem = reinterpret_cast< CItem* >( pDestination->GetEntity() );
						if ( pItem->GetRequiredProgress() <= this->m_pPlayer->GetProgress() )
						{
							pDestination->DetachEntity( pItem );

							this->m_pPlayer->PickupItem( pItem );
							this->MovePlayer( _krXIndex, _krYIndex );

							CEngine::GetInstance().GetAudioManager().Play( AUDIO_PICKUP, false );
							CEngine::GetInstance().GetAudioManager().SetVolume( AUDIO_PICKUP, -3000 );
						}
					}
				}break;

				case ENTITY_NPC:
				{		
					this->m_pInteractingNPC = reinterpret_cast< CNPC* >( pDestination->GetEntity() );
					this->m_pInteractingNPC->CheckQuest( this->m_pPlayer );
					if ( m_pInteractingNPC->CanInteract( this->m_pPlayer->GetProgress() ) )
					{
						static int iOffset = 0;
						iOffset = ( iOffset + 1 ) % 3;

						this->m_eState = GAMESTATE_INTERACTING;
						this->m_pInteractingNPC->ShowDialogText( this->m_pPlayer );
						CEngine::GetInstance().GetAudioManager().Play( AUDIO_TALK1 + iOffset, false );
						CEngine::GetInstance().GetAudioManager().SetVolume( AUDIO_TALK1 + iOffset, -3000 );
					}
					else
					{
						m_pInteractingNPC = nullptr;
					}
				}break;

				default: break;
			}
		}
		else
		{
			CEngine::GetInstance().GetAudioManager().Play( AUDIO_BLOCKED, false );
			CEngine::GetInstance().GetAudioManager().SetVolume( AUDIO_BLOCKED, -3000 );
			this->m_vTransitioningStart = { 0.0f, 0.0f };
		}
	}
}

void CSceneGameplay::MovePlayer( const unsigned int& _krXIndex, const unsigned int& _krYIndex )
{
	// Get pointer to tile
	CTile* pDestination{ &this->m_WorldGrid[ _krYIndex ][ _krXIndex ] };

	// Detatch player from its current container
	if ( this->m_pPlayer->GetContainer() != nullptr )
	{
		D3DXVECTOR2 vPosCurr{ this->m_pPlayer->GetContainer()->GetPosition() };
		D3DXVECTOR2 vPosNext{ pDestination->GetPosition() };

		this->m_vTransitioningStart = vPosNext - vPosCurr;
		this->m_pPlayer->SetPosition( -this->m_vTransitioningStart );

		this->m_pPlayer->GetContainer()->DetachEntity( this->m_pPlayer );
	}

	// Set the new index and container of player
	this->m_pPlayer->SetIndex( _krXIndex, _krYIndex );
	this->m_pPlayer->SetContainer( pDestination );
	pDestination->AttatchEntity( this->m_pPlayer );
}

void CSceneGameplay::ShowInteractionText( std::wstring _krOutput )
{
	this->m_pInteractionText->SetText( _krOutput );
	this->m_pInteractionText->SetVisible( true );
}
void CSceneGameplay::SetCameraTarget( const D3DXVECTOR2& _krTarget )
{
	this->m_pMainCamera->SetTarget( { _krTarget.x, _krTarget.y, 0.0f } );
	this->m_pMainCamera->SetPosition( { _krTarget.x, _krTarget.y, -10.0f } );
}

void CSceneGameplay::RunEvent( std::string _krEvent )
{
	std::stringstream lineStream( _krEvent );
	std::string strCell;

	std::getline( lineStream, strCell, ',' );

	if ( strCell.compare( "spawnitem" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "spawnitem" ) );
		this->SpawnItem( _krEvent );
	}
	else if ( strCell.compare( "spawnnpc" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "spawnnpc" ) );
		this->SpawnNPC( _krEvent );
	}
	else if ( strCell.compare( "startquest" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "startquest" ) );
		this->StartQuest( _krEvent );
	}
	else if ( strCell.compare( "attachdialog" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "attachdialog" ) );
		this->AttachDialog( _krEvent );
	}
	else if ( strCell.compare( "endgame" ) == 0 )
	{
		CEngine::GetInstance().GetSceneManager().SetCurrentScene( SCENE_ENDGAME );
	}
	else if ( strCell.compare( "changetexture" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "changetexture" ) );
		this->ChangeTexture( _krEvent );
	}
	else if ( strCell.compare( "removenpc" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "removenpc" ) );
		CNPC* pNPC = this->GetNPC( std::wstring( _krEvent.begin(), _krEvent.end() ) );
		if ( pNPC != nullptr )
		{
			pNPC->GetContainer()->DetachEntity( pNPC );
		}
	}
	else if ( strCell.compare( "settraversable" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "settraversable" ) );
		std::stringstream strLineInput( _krEvent );
		std::string strX;
		std::string strY;

		std::getline( strLineInput, strX, ',' );
		_krEvent.erase( 0, strX.size() + 1 );

		std::getline( strLineInput, strY, ',' );
		_krEvent.erase( 0, strY.size() + 1 );

		this->m_WorldGrid[ std::stoi( strY ) ][ std::stoi( strX ) ].SetTraversable( _krEvent.compare( "true" ) == 0 );

	}
	else if ( strCell.compare( "spawntexture" ) == 0 )
	{
		_krEvent.erase( 0, sizeof( "spawntexture" ) );
		std::stringstream strLineInput( _krEvent );
		std::string strX;
		std::string strY;

		std::getline( strLineInput, strX, ',' );
		_krEvent.erase( 0, strX.size() + 1 );

		std::getline( strLineInput, strY, ',' );
		_krEvent.erase( 0, strY.size() + 1 );

		CTextured2D* pTextured = CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"", std::wstring( _krEvent.begin(), _krEvent.end() ), this->m_fTileSize, this->m_fTileSize, 0.0f, -20.0f );
		this->m_WorldGrid[ std::stoi( strY ) ][ std::stoi( strX ) ].AttachChild( pTextured );
	}
}

bool CSceneGameplay::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	switch ( this->m_eState )
	{
		case GAMESTATE_TRAVELLING:
		{
			EDirectionType eDirection{ DIRECTION_NULL };
			unsigned int uiInitialX{ this->m_pPlayer->GetXIndex() };
			unsigned int uiInitialY{ this->m_pPlayer->GetYIndex() };

			if ( CEngine::GetInstance().GetInputManager().IsDown( VK_LEFT ) )
			{
				--uiInitialX;
				eDirection = DIRECTION_LEFT;
			}
			else if ( CEngine::GetInstance().GetInputManager().IsDown( VK_RIGHT ) )
			{
				++uiInitialX;
				eDirection = DIRECTION_RIGHT;
			}
			else if ( CEngine::GetInstance().GetInputManager().IsDown( VK_UP ) )
			{
				--uiInitialY;
				eDirection = DIRECTION_UP;
			}
			else if ( CEngine::GetInstance().GetInputManager().IsDown( VK_DOWN ) )
			{
				++uiInitialY;
				eDirection = DIRECTION_DOWN;
			}

			if ( eDirection != DIRECTION_NULL )
			{
				CEngine::GetInstance().GetAudioManager().Play( AUDIO_WALK, true );
				CEngine::GetInstance().GetAudioManager().SetVolume( AUDIO_MUSIC, -1500 );

				this->m_eState = GAMESTATE_TRANSITIONING;

				this->m_iAnimationFrame = ( this->m_iAnimationFrame + 1 ) % this->m_iAnimationMaxFrame;
				std::wstring wstrTexture( this->m_PlayerTextures[ eDirection ] + std::to_wstring( this->m_iAnimationFrame ) + L".png" );
				this->m_pPlayer->SetTexture( CEngine::GetInstance().GetTextureManager().GetTexture( wstrTexture ) );

				this->HandleMovement( uiInitialX, uiInitialY );
				this->m_fTransitioningTimeElapsed = 0.0f;

			}
			else
			{
				CEngine::GetInstance().GetAudioManager().Stop( AUDIO_WALK );
				this->m_pPlayer->SetTexture( CEngine::GetInstance().GetTextureManager().GetTexture( L"Protagonist.png" ) );
			}
		}break;

		case GAMESTATE_TRANSITIONING:
		{
			this->m_fTransitioningTimeElapsed += _kfTimeDelta;
			
			if ( this->m_fTransitioningTimeElapsed > this->m_fTransitioningTimeRequired )
			{
				this->m_eState = GAMESTATE_TRAVELLING;
				this->m_pPlayer->SetPosition( { 0.0f, 0.0f } );
			}	
			else
			{
				D3DXVECTOR2 vDirection{ ( this->m_vTransitioningStart / this->m_fTransitioningTimeRequired ) *
					this->m_fTransitioningTimeElapsed };

				this->m_pPlayer->SetPosition( vDirection - this->m_vTransitioningStart );
			}
		}break;

		case GAMESTATE_INTERACTING:
		{
			if ( CEngine::GetInstance().GetInputManager().IsPressed( VK_RETURN ) )
			{
				if ( !this->m_pInteractingNPC->ShowDialogText( this->m_pPlayer ) )
				{
					this->m_eState = GAMESTATE_TRAVELLING;
					this->m_pInteractingNPC = nullptr;
				}
			}

		}break;
	}

	D3DXVECTOR3 vCameraPosition{ this->m_pMainCamera->GetTarget() };
	D3DXVECTOR2 vCameraPosition2D{ vCameraPosition.x, vCameraPosition.y };
	D3DXVECTOR2 vDestination{ ( this->m_pPlayer->GetContainer()->GetPosition() + this->m_pPlayer->GetPosition() ) - vCameraPosition2D };
	
	if ( D3DXVec2Length( &vDestination ) > 0.0f )
	{
		this->SetCameraTarget( vCameraPosition2D + vDestination * 0.002f );
	}
	else
	{
		this->SetCameraTarget( this->m_pPlayer->GetContainer()->GetPosition() );
	}

	if ( this->m_pInteractionText->IsVisible() )
	{
		this->m_fTextTimeElapsed += _kfTimeDelta;
		if ( this->m_fTextTimeElapsed > this->m_fTextDissapearTime )
		{
			this->m_pInteractionText->SetVisible( false );
			this->m_fTextTimeElapsed = 0.0f;
		}
	}

	if ( CEngine::GetInstance().GetInputManager().IsPressed( VK_ESCAPE ) )
	{
		bReturn = false;
	}

	return ( bReturn );
}

