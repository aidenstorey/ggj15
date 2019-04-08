//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2013 Media Design School
//
//  File Name   :   Audio.h
//  Description :   Audio definitions
//  Author      :   Aiden Storey, Calvin Norton, Josh Gibbs, Michael Cameron
//  Mail        :   aiden.storey@mediadesign.school.nz, calvin.norton@mediadesign.school.nz, 
//					joshua.gibbs@mediadesign.school.nz, michael.cameron@mediadesign.school.nz
//

#ifndef GUARD_AUDIO_H_201410251021
#define GUARD_AUDIO_H_201410251021

// Types
enum EAudioClips
{
	AUDIO_BLOCK_ROLL_1,
	AUDIO_BLOCK_ROLL_2,
	AUDIO_BLOCK_ROLL_3,
	AUDIO_BLOCK_ROLL_4,
	AUDIO_LEAF_ROLL_1,
	AUDIO_LEAF_ROLL_2,
	AUDIO_LEAF_ROLL_3,
	AUDIO_LEAF_ROLL_4,
	AUDIO_THUD_ROLL_1,
	AUDIO_THUD_ROLL_2,
	AUDIO_THUD_ROLL_3,
	AUDIO_THUD_ROLL_4,
	AUDIO_SCOREA_1,
	AUDIO_SCOREA_2,
	AUDIO_SCOREB_1,
	AUDIO_SCOREB_2,
	AUDIO_GAME_OVER,
	AUDIO_MUSIC,
	AUDIO_PUFF
};

// Constants
const unsigned int g_uiBlockRollSoundSlots{ 4 };

#endif	// GUARD_AUDIO_H_201410251021

