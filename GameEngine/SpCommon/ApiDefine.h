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
