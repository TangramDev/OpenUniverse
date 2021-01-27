/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101270019           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
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
