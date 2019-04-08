// Library Includes
#include <ctime>

// Engine Includes
#include <engine.h>

// Local Includes
#include "audio_files.h"
#include "scene_id_list.h"
#include "scene_gameplay.h"
#include "scene_menu.h"
#include "scene_endgame.h"

// This Include 
#include "program_entry.h"

// Implementation
int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iShowCmd )
{
	srand( static_cast< unsigned int >( time( NULL ) ) );

	CProgramEntry Program;

	if ( Program.Initialise( _hInstance ) )
	{
		Program.Run();
	}

	return ( 0 );
}

//**************************************************
//
// CProgramEntry: Initialises program entry
//
// @author: Aiden Storey
//
//**************************************************
CProgramEntry::CProgramEntry()
{}

//**************************************************
//
// ~CProgramEntry: Cleans up program entry
//
// @author: Aiden Storey
//
//**************************************************
CProgramEntry::~CProgramEntry()
{}

//**************************************************
//
// Initialise: Initialses engine 
//
// @author: Aiden Storey
//
// @param[in]	_hInstance			instance for window
//
// @returns		if initialisation was successful
//
//**************************************************
bool CProgramEntry::Initialise( HINSTANCE _hInstance )
{
	bool bReturn{ true };

	bReturn &= CEngine::GetInstance().Initialise( _hInstance, L"Sidetracked", L"Game", L"Assets/Textures/", L"Assets/Shaders/" );
	this->InitialiseAudio();

	return ( bReturn );
}


void CProgramEntry::InitialiseAudio()
{
	CEngine::GetInstance().GetAudioManager().Initialise( CEngine::GetInstance().GetHWnd() );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Music/MainMusic.wav", AUDIO_MUSIC, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Sounds/talk1.wav", AUDIO_TALK1, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Sounds/talk2.wav", AUDIO_TALK2, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Sounds/talk3.wav", AUDIO_TALK3, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Music/Walking.wav", AUDIO_WALK, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Music/PickupSFX.wav", AUDIO_PICKUP, false, DSBCAPS_CTRLVOLUME );
	CEngine::GetInstance().GetAudioManager().PreloadAudio( L"Assets/Sounds/blocked.wav", AUDIO_BLOCKED, false, DSBCAPS_CTRLVOLUME );
}

//**************************************************
//
// Run: description
//
// @author: Aiden Storey

// @returns		completion code
//
//**************************************************
int CProgramEntry::Run()
{
	CEngine::GetInstance().GetSceneManager().CreateScene< CSceneMenu >( SCENE_MENU );
	CEngine::GetInstance().GetSceneManager().CreateScene< CSceneGameplay >( SCENE_GAMEPLAY );
	CEngine::GetInstance().GetSceneManager().CreateScene< CSceneEndgame >( SCENE_ENDGAME );

	CEngine::GetInstance().GetSceneManager().SetCurrentScene( SCENE_MENU );

	MSG msg{ 0 };
	for ( ;; )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if ( !CEngine::GetInstance().Update() )
			{
				PostQuitMessage( 0 );
			}
		}
	}

	return ( static_cast< int >( msg.wParam ) );
}