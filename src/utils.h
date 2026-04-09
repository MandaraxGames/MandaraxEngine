#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <objidl.h>
#include <winerror.h>

// Simple types to match your project style
typedef size_t MX_Size;
typedef unsigned char MX_Bool;
#define MX_True 1
#define MX_False 0

typedef HRESULT MX_Result;
#define MX_S_OK ((MX_Result)S_OK)
#define MX_E_INVALIDARG ((MX_Result)E_INVALIDARG)
#define MX_E_OVERFLOW ((MX_Result)STG_E_INSUFFICIENTMEMORY)

// CRT replacement functions
void* mx_malloc(MX_Size size);
void* mx_realloc(void* ptr, MX_Size size);
void* mx_memset(void* dest, int val, MX_Size size);
void* mx_zeromem(void* dest, size_t count);
void mx_free(void* ptr);
void mx_memcpy(void* dest, const void* src, MX_Size size);

// String printing and formatting
MX_Result mx_strsafe_len(const wchar_t* psz, MX_Size cchMax, MX_Size* pcchLength);
void mx_debug_print(const wchar_t* message);

#endif // UTILS_H
