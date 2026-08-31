#pragma once

// MSVC reports the dialect in _MSVC_LANG; __cplusplus stays 199711 unless /Zc:__cplusplus.
#if defined(_MSVC_LANG)
#   define OFXABLETONLINK_CXX_STD _MSVC_LANG
#else
#   define OFXABLETONLINK_CXX_STD __cplusplus
#endif

// std::invoke_result is C++17. std::result_of was removed in C++20.
#if OFXABLETONLINK_CXX_STD >= 201703L
#   define OFXABLETONLINK_HAS_INVOKE_RESULT 1
#endif
#if OFXABLETONLINK_CXX_STD >= 202002L
#   define OFXABLETONLINK_NO_STD_RESULT_OF 1
#endif
