﻿//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once

// WIL
#include <wil/cppwinrt.h>
#include <wil/result.h>

// WinRT
#include <winrt/base.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Storage.Streams.h>

// Windows
#include <Windows.h>
#include <evntrace.h>
#include <TraceLoggingProvider.h>
#include <TraceLoggingActivity.h>

// MF
#include <mfapi.h>
#include <mferror.h>

// FFmpeg
extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

// STL
#include <algorithm>
#include <memory>
#include <deque>

// FFmpegInterop
#include "Tracing.h"
#include "Utility.h"

// Disable debug string output on non-debug build
#if !_DEBUG
#define DebugMessage(x)
#else
#define DebugMessage(x) OutputDebugString(x)
#endif
