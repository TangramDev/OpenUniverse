/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#ifndef XHTMLDRAWLINK_H
#define XHTMLDRAWLINK_H

#include "XHtmlDraw.h"

class CXHtmlDrawLink
{
// ctor
public:
	CXHtmlDrawLink();
	virtual ~CXHtmlDrawLink();

// Operations
public:
	BOOL	GotoURL(LPCTSTR url, int showcmd, LPARAM lParam);
	BOOL	ProcessAppCommand(LPCTSTR lpszCommand, LPARAM lParam);
	void	SetAppCommands(CXHtmlDraw::XHTMLDRAW_APP_COMMAND * paAppCommands, 
				int nAppCommands);
	BOOL	SetLinkCursor();

// Implementation
protected:
    HCURSOR		m_hLinkCursor;		// cursor for links
	int			m_nAppCommands;
	CXHtmlDraw::XHTMLDRAW_APP_COMMAND * m_paAppCommands;

	LONG	GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR lpData);
	void	SetDefaultCursor();
};

#endif //XHTMLDRAWLINK_H
