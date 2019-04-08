#ifndef GUARD_DIALOG_H
#define GUARD_DIALOG_H

// Library Includes
#include <string>
#include <vector>

// Local Includes
#include <text.h>
#include <textured_2d.h>

// Prototypes
class CDialog
{
	// Member Functions
public:
	// Constructors
	CDialog( CText* _pText, CTextured2D* _pTexture, int _iDialogID, int _iProgressRequired )
		:m_pText( _pText ),
		m_pTexture( _pTexture ),
		m_iDialogCount( 0 ),
		m_iDialogID( _iDialogID ),
		m_iProgressRequired( _iProgressRequired )
	{};

	// Accessors
	int GetDialogID() const;

	int GetRequiredProgress() const;

	void AddDialog(std::wstring& _sDialog);
	bool NextDialog();

protected:
private:

	// Member Variables
public:
protected:
	std::vector<std::wstring>	m_sDialogList;
	unsigned int				m_iDialogCount;
	CText*						m_pText;
	CTextured2D*				m_pTexture;
	int							m_iProgressRequired;
	int							m_iDialogID;

private:
};

#endif // GUARD_ENTITY_H