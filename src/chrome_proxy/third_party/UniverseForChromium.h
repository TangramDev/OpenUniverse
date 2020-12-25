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

// Check no prior poisonous defines were made.
#include "base/win/windows_defines.inc"
// Undefine before windows header will make the poisonous defines
#include "base/win/windows_undefines.inc"

#include "../../CommonFile/CommonUniverse.h"
extern CommonUniverse::CCosmosImpl* g_pCosmosImpl;

// Undefine the poisonous defines
#include "base/win/windows_undefines.inc"
// Check no poisonous defines follow this include
#include "base/win/windows_defines.inc"
