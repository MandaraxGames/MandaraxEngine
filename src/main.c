#include <windows.h>

// For Visual C++ compatibility
int _fltused = 1;

// Basic no-CRT environment for Windows
// Entry point
int mainCRTStartup(void) {
  HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD written = 0;
  
  const char* message = "Hello from no-CRT test application\n";
  WriteFile(stdout_handle, message, lstrlenA(message), &written, NULL);
  
  Sleep(2000);
  
  return 0;
}
