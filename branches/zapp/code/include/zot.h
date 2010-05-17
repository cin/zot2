#pragma once

#include "targetver.h"

#include <stdio.h>

#ifdef _WIN32
#include <tchar.h>
#endif

#include "zypes.h"
#include "zimtime.h"

//#include "convar.h"
//#include "zapp.h"

#if (!defined(_WIN32) && defined(_DEBUG))
#include <assert.h>
#define _ASSERT assert
#endif

#ifdef _DEBUG
#define D(x) x
#define D1(x) x
#define D2(x) x
#else
#define D(x)
#define D1(x)
#define D2(x)
#endif
