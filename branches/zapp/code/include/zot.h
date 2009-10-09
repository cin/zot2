#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "zypes.h"
#include "zimtime.h"

//#include "convar.h"
//#include "zapp.h"

#ifdef _DEBUG
#define D(x) x
#define D1(x) x
#define D2(x) x
#else
#define D(x)
#define D1(x)
#define D2(x)
#endif
