// This Include 
#include "dialog.h"

// Implementation

int CDialog::GetDialogID() const
{
	return ( this->m_iDialogID );
}

int CDialog::GetRequiredProgress() const
{
	return ( this->m_iProgressRequired );
}

void CDialog::AddDialog(std::wstring& _sDialog)
{
	this->m_sDialogList.push_back(_sDialog);
}

bool CDialog::NextDialog()
{
	if (m_iDialogCount < m_sDialogList.size())
	{
		// Make sure texture and text are showing
		m_pText->SetVisible(true);
		m_pTexture->SetVisible(true);

		// Output Current Dialog to Text
		m_pText->SetText(m_sDialogList[m_iDialogCount]);

		// Move onto next dialog
		m_iDialogCount++;
		return 1;
	}
	else
	{
		//Reset Conversation
		m_iDialogCount = 0;

		// Hide text and texture
		m_pText->SetVisible(false);
		m_pTexture->SetVisible(false);
		return 0;
	}
}
