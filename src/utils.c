#include "utils.h"

// File I/O
HANDLE mx_open_file(const wchar_t* filename) {
  return CreateFileW(
    filename,
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );
}

MX_Bool mx_read_file(HANDLE file, void* buffer, DWORD size, DWORD* bytes_read) {
  return ReadFile(file, buffer, size, bytes_read, NULL);
}

void mx_close_file(HANDLE file) {
  if (file != INVALID_HANDLE_VALUE) {
    CloseHandle(file);
  }
}

// Memory
void* mx_malloc(MX_Size size) {
  return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void* mx_realloc(void* ptr, MX_Size size) {
  if (!ptr) return mx_malloc(size);
  return HeapReAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, ptr, size);
}
/**
 * mx_memset - An optimized memset replacement for Windows without CRT
 * 
 * This implementation provides a full featured memset() replacement that:
 * - Uses word-sized operations for speed when possible
 * - Handles unaligned memory properly
 * - Works on any size buffer efficiently
 * - Is compatible with C90/C99 standards
 * - Doesn't rely on any CRT functions
 */
void* __cdecl memset(void* dest, int c, MX_Size count) {
  unsigned char* dst8 = (unsigned char*)dest;
  unsigned char value = (unsigned char)c;
  void* original_dest = dest;
  
  /* Handle small sizes with direct byte operations */
  if (count < 8) {
    while (count-- > 0)
      *dst8++ = value;
    return original_dest;
  }
  
  /* Fill first bytes until we reach an aligned address (if needed) */
  MX_Size align = (MX_Size)dest & (sizeof(MX_Size) - 1);
  if (align) {
    MX_Size alignBytes = sizeof(MX_Size) - align;
    count -= alignBytes;
    while (alignBytes--)
      *dst8++ = value;
  }
  
  /* Create a word-sized pattern from the byte */
  MX_Size pattern = 0;
  if (value) {
    pattern = value;
    pattern |= pattern << 8;
    pattern |= pattern << 16;
    #ifdef _WIN64
    pattern |= pattern << 32;
    #endif
  }
  
  /* Fill using word-sized operations */
  MX_Size* dst_word = (MX_Size*)dst8;
  MX_Size wordCount = count / sizeof(MX_Size);
  count %= sizeof(MX_Size);
  
  while (wordCount-- > 0)
    *dst_word++ = pattern;
  
  /* Fill any remaining bytes */
  dst8 = (unsigned char*)dst_word;
  while (count-- > 0)
    *dst8++ = value;
  
  return original_dest;
}

void* mx_memset(void* dest, int c, MX_Size count) {
  return memset(dest, c, count);
}

/**
 * Specialized zero memory implementation
 * Often zeroing memory is a common case that can be optimized separately
 */
void* mx_zeromem(void* dest, MX_Size count) {
  unsigned char* dst8 = (unsigned char*)dest;
  void* original_dest = dest;
  
  /* Handle small sizes with direct byte operations */
  if (count < 32) {
    MX_Size i;
    for (i = 0; i < count; i++)
      dst8[i] = 0;
    return original_dest;
  }
  
  /* Align to word boundary first */
  while (((MX_Size)dst8 & (sizeof(MX_Size) - 1)) && count) {
    *dst8++ = 0;
    count--;
  }
  
  /* Zero blocks using word-sized operations */
  MX_Size* dst_word = (MX_Size*)dst8;
  MX_Size wordCount = count / sizeof(MX_Size);
  count %= sizeof(MX_Size);
  
  while (wordCount--)
    *dst_word++ = 0;
  
  /* Zero any remaining bytes */
  dst8 = (unsigned char*)dst_word;
  while (count--)
    *dst8++ = 0;
  
  return original_dest;
}

void mx_free(void* ptr) {
  if (ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
  }
}

void mx_memcpy(void* dest, const void* src, MX_Size size) {
  unsigned char* d = (unsigned char*)dest;
  const unsigned char* s = (const unsigned char*)src;
  for (MX_Size i = 0; i < size; i++) {
    d[i] = s[i];
  }
}

MX_Result mx_strsafe_len(const wchar_t* psz, MX_Size cchMax, MX_Size* pcchLength) {
  // Parameter validation
  if (psz == NULL || cchMax == 0 || pcchLength == NULL) {
    if (pcchLength != NULL) {
      *pcchLength = 0;
    }
    return MX_E_INVALIDARG;
  }
  
  MX_Size length = 0;
  
  // Count characters up to null terminator or max length
  while (length < cchMax && psz[length] != L'\0') {
    length++;
  }
  
  // Check if we reached the end of the string
  if (length < cchMax) {
    // String is null-terminated and within the specified length
    *pcchLength = length;
    return MX_S_OK;
  } else {
    // String is not null-terminated within the specified length
    *pcchLength = cchMax;
    return MX_E_OVERFLOW;
  }
}

void mx_debug_print(const wchar_t* message) {
  OutputDebugStringW(message);
}
