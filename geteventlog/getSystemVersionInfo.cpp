
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
#pragma warning(disable: 4996)   
string getSystemVersionInfo()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	bIsWindowsXPorLater =	((osvi.dwMajorVersion > 5) ||	((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1)));
	string szSystemVersion;
	szSystemVersion = osvi.dwMajorVersion + osvi.dwMinorVersion;
	ostringstream  ossSystemVersion;
	ossSystemVersion << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
	//cout << "===========" << ossSystemVersion.str() << endl;
	/*cout << "osvi.dwMajorVersion\t" << osvi.dwMajorVersion << endl;
	cout << "osvi.dwMinorVersion\t" << osvi.dwMinorVersion << endl;*/
	if (bIsWindowsXPorLater)
		printf("The system meets the requirements.\n");
	else printf("The system does not meet the requirements.\n");
	//https://docs.microsoft.com/zh-cn/windows/win32/sysinfo/operating-system-version  版本号与操作系统版本的对应关系
	return ossSystemVersion.str();
}