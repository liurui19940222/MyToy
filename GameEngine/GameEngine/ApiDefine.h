#ifndef _API_DEFINE_H_
#define _API_DEFINE_H_

#   ifdef  ENGINE_DLL
#       define ENGINE_API __declspec(dllimport)
#   elif ENGINE_BUILD_DLL_EXPORT
#       define ENGINE_API __declspec(dllexport)
#   else
#       define ENGINE_API
#   endif

#endif