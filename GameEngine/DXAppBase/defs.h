#pragma once

#define except noexcept(false)

#define throw_and_log(msg) { spgameengine::Debug::Log(msg); throw std::exception(msg); }