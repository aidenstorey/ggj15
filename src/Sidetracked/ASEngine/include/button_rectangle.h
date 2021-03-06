//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_BUTTON_RECTANGLE_H_201409251207
#define GUARD_AIDEN_STOREY_BUTTON_RECTANGLE_H_201409251207

// Library Includes
#include <string> 

// Local Includes
#include "button.h"

// Prototypes
class CButtonRectangle : public CButton
{
	friend class CObject2DManager;

public:
	// Accessors
	bool IsOver( const D3DXVECTOR2 _kvMousePos ) override;

protected:
	// Constructors
	CButtonRectangle( const std::wstring& _kstrName = L"", std::wstring _krTextureName = L"DefaultTexture.png", const float _kfWidth = 1.0f, const float _kfHeight = 1.0f, const float _kfPositionX = 0.0f,
				   const float _kfPositionY = 0.0f, const float _kfRotation = 0.0f, const float _kfScaleX = 1.0f, const float _kfScaleY = 1.0f );
	~CButtonRectangle();

private:

public:
protected:
private:

};

#endif // GUARD_AIDEN_STOREY_BUTTON_RECTANGLE_H_201409251207