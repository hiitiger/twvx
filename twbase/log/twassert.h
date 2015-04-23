#pragma once

#ifdef _DEBUG

bool TW_BASE_API twAssert(bool cond, const char* file, const char* func, int line, const char* msg);

#endif // _DEBUG
