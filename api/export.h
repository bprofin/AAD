#pragma once

#define AAD_VISIBILITY_ENUM

// Platform and build configuration detection
#if defined(AAD_STATIC_DEFINE)
// Static library - no symbol decoration needed
#define AAD_API
#define AAD_VISIBILITY
#define AAD_IMPORT
#define AAD_HIDDEN

#elif defined(AAD_SHARED_DEFINE)
// Shared library - platform-specific symbol decoration
#if defined(_WIN32) || defined(__CYGWIN__)
// Windows DLL export/import
#ifdef AAD_BUILDING_DLL
#define AAD_API __declspec(dllexport)
#else
#define AAD_API __declspec(dllimport)
#endif
#define AAD_VISIBILITY
#define AAD_IMPORT __declspec(dllimport)
#define AAD_HIDDEN
#elif defined(__GNUC__) && __GNUC__ >= 4
// GCC 4+ visibility attributes
#define AAD_API __attribute__((visibility("default")))
#define AAD_VISIBILITY __attribute__((visibility("default")))
#define AAD_IMPORT __attribute__((visibility("default")))
#define AAD_HIDDEN __attribute__((visibility("hidden")))
#else
// Fallback for other compilers
#define AAD_API
#define AAD_VISIBILITY
#define AAD_IMPORT
#define AAD_HIDDEN
#endif

#else
// Default fallback - assume static linking
#define AAD_API
#define AAD_VISIBILITY
#define AAD_IMPORT
#define AAD_HIDDEN
#endif

// python export using clang
#if defined(AAD_PYBIND) && defined(__clang__)
#define AAD_PY __attribute__((annotate("pyexport")))
#else
#define AAD_PY
#endif