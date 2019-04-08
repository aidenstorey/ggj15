//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2013 Media Design School
//
//  File Name   :   AudioManager.h
//  Description :   CAudioManager class header file
//  Author      :   Josh Gibbs
//  Mail        :   joshua.gibbs@mediadesign.school.nz
//

#ifndef GUARD_AUDIOMANAGER_H_1377631392l
#define GUARD_AUDIOMANAGER_H_1377631392l

// Library Includes
#include <map>
#include <string>
#include <windows.h>

// Local Includes
#include "SDKsound.h"

// Types

// Constants

// Prototypes

class CAudioManager
{
	// Member Functions
public:
	CAudioManager();
	~CAudioManager();

	bool Initialise( HWND _hWnd );
	void SetAudioEnabledState( const bool _kbState );
	bool GetAudioEnabledState();

	bool PreloadAudio( const std::wstring _kFileName, DWORD dwCreationFlags = 0 );
	bool PreloadAudio( const std::wstring _kFileName, const int _kiSetID, const bool _bLoadAgain = false, DWORD dwCreationFlags = 0 );
	void Play( const std::wstring _kFileName, const bool _kbLooping );
	void Play( const int _kiSoundID, const bool _kbLooping );
	void Play( const int _kiSoundID, long _lVolume, const bool _kbLooping );
	void Stop( const std::wstring _kFileName );
	void Stop( const int _kiSoundID );
	void SetVolume( const int _kiSoundID, long _lVolume );

protected:

private:

	// Member Variables
public:

protected:

private:
	CSoundManager	m_DX8SoundManager;

	std::map< std::wstring, CSound* >	m_AudioObjectsByFileName;
	std::map< int, CSound* >			m_AudioObjectsByIndex;

	bool m_bEnableSound;
};

#endif // GUARD_AUDIOMANAGER_H_1377631392l
