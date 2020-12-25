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