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

#pragma once
#include "XZip.h"
namespace Utilities
{
	class CComponentInstaller
	{
	public:
		CComponentInstaller(void);
		~CComponentInstaller(void);

		CString m_strHostFile;
		//压缩一个文件
		BOOL Zip(LPCTSTR lpszZipArchive, LPCTSTR lpszSrcFile);
		//解压压缩包内的文件
		void VerifyZip(HZIP hz, LPCTSTR lpszFile, LPCTSTR lpszPath);
		//进行文件比较
		BOOL Compare(LPCTSTR lpszFile1, LPCTSTR lpszFile2, BOOL *pbResult);
		//解压压缩包内的文件，支持一个文件,并以strName命名,strName带路径
		BOOL UnZip(CString strZip, CString strName);
		//解压压缩包内的文件，支持多个文件，并不改名
		BOOL UnMultiZip(CString strZip, CString strPath);
		BOOL UnMultiZip2(CString strZip, CString strPath);
		BOOL InstallComponent(CString strXml);

		BOOL RegComDll(const CString& strDllLib);
		BOOL UnRegComDll(const CString& strDllLib);
		BOOL SetDirectory(CString strFile);
	};
}