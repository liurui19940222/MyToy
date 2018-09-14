#pragma once

#define except noexcept(false)

#include"..\SpCommon\Debug.h"
#define throw_and_log(msg) { spgameengine::Debug::Log(msg); throw std::exception(msg); }