/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
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

#pragma once

class CWormhole : public CSession
{
public:
	CWormhole();
	virtual ~CWormhole();

	void InsertString(CString key, CString value);
	void InsertLong(CString key, long value);
	void Insertint64(CString key, __int64 value);
	void InsertFloat(CString key, float value);
	CString GetString(CString key);
	long GetLong(CString key);
	__int64 Getint64(CString key);
	float GetFloat(CString key);
	void SendMessage();
	void addEventListener(CString ListenerName);
	void removeEventListener(CString ListenerName);

	IPCSession* m_pSession;
};

