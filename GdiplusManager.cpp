#include "GdiplusManager.h"

unsigned int cl::GdiplusManager::userCount = 0;
Gdiplus::GdiplusStartupInput cl::GdiplusManager::stInp = {};
ULONG_PTR cl::GdiplusManager::token = 0;
