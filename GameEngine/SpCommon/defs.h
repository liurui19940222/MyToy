#pragma once

#ifndef _API_DEFINE_H_
#define _API_DEFINE_H_

#   if ENGINE_BUILD_DLL_EXPORT
#       define ENGINE_API __declspec(dllexport)
#       define ENGINE_API_TEMPLATE __declspec(dllexport)
#   elif WIN32RUN
#       define ENGINE_API
#       define ENGINE_API_TEMPLATE
#	else
#		define ENGINE_DLL
#       define ENGINE_API __declspec(dllimport)
#       define ENGINE_API_TEMPLATE
#   endif

#endif

#define BEGIN_NAMESPACE_ENGINE namespace spgameengine {
#define END_NAMESPACE_ENGINE }
#define USING_NAMESPACE_ENGINE using namespace spgameengine;