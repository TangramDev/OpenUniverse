/********************************************************************************
 *           Web Runtime for Application - Version 1.0.1.202105050002
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
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#pragma once

using namespace System;
using namespace System::Reflection;
using namespace System::Collections::Generic;

public ref class AssemblyLocalor
{
	static Dictionary<String^, Assembly^>^ assemblies;

public:

	static void Init()
	{
		assemblies = gcnew Dictionary<String^, Assembly^>();
		AppDomain::CurrentDomain->AssemblyLoad += gcnew AssemblyLoadEventHandler(CurrentDomain_AssemblyLoad);
		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(CurrentDomain_AssemblyResolve);

	}

	static Assembly^ CurrentDomain_AssemblyResolve(Object^ sender, ResolveEventArgs^ args)
	{
		Assembly^ assembly = nullptr;
		assemblies->TryGetValue(args->Name, assembly);
		return assembly;
	}

	static void CurrentDomain_AssemblyLoad(Object^ sender, AssemblyLoadEventArgs^ args)
	{
		Assembly^ assembly = args->LoadedAssembly;
		assemblies[assembly->FullName] = assembly;
	}

};