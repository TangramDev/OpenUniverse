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
namespace Utilities
{
	class CDownLoadObj
	{
	public:
		CDownLoadObj();
		~CDownLoadObj();

		CString m_strFileURL;
		CString m_strFilePath;
		CString m_strActionXml;
		BOOL InerDownloadAFile();
		BOOL OnDownLoadSuccess(int fileid);
		BOOL DownLoadFile(CString strURL, CString strTarget);
	private:
		ULONG crc32Table[256];			//CRC±í
		CString m_strMac;

		void InitCRC32Table();
		int GetCRC32(CString& csData, DWORD dwSize);
		ULONG Reflect(ULONG ref, char ch);
		CString OpenFile(CString &filePath);
		DWORD ExecCmd(const CString cmd, const BOOL setCurrentDirectory);
	};
}
