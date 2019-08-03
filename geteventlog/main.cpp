#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
#include "getSystemVersionInfo.h"
using std::ofstream;
//https://msdn.microsoft.com/ja-jp/windows/aa363674(v=vs.80)
using namespace std;
string g_systemname;
void DisplayEntries(string szSystemVersion)
{
	DWORD DWPowerOnEventID = 6005;
	DWORD DWPowerOffEventID = 6006;
	if (szSystemVersion == "6.1") {
		DWPowerOnEventID = 12;
		DWPowerOffEventID = 13;

	}
	/*cout << "on\t" << DWPowerOnEventID << endl;
	cout << "off\t" << DWPowerOffEventID << endl;*/

	ofstream outfile;
	//创建或打开文件，ios::trunc如果该文件已经存在，其内容将在打开文件之前被截断
	outfile.open("eventlog.txt", ios::out | ios::trunc);
	//获取时间
	time_t cTime = time(NULL);
	char sTime[64] = { 0 };
	//格式化时间
	struct tm* localTime = localtime(&cTime);
	strftime(sTime, sizeof(sTime), "%Y-%m-%d %X", localTime);

	//向文件中写入时间
	outfile <<"Run Time:" <<sTime << endl;

	string OSName;

	if (szSystemVersion == "10.0") { OSName = "Windows 10" ; }
	if (szSystemVersion == "6.1") { OSName = "Windows 7" ; }
	if (szSystemVersion == "5.1") { OSName =  "Windows XP"; }
	//写入操作系统版本信息
	//cout<<"Operating system:" << OSName << endl;
	outfile <<"Operating system:"<< OSName << endl;



	char* tempBuf = new char[100];
	memset(tempBuf, 0, 100);
	HANDLE h;
	EVENTLOGRECORD* pevlr;
	TCHAR bBuffer[0x7ffff] = { 0 }; //0x7ffff为缓冲区最大 
	char* pchar;
	DWORD dwRead, dwNeeded, cRecords, dwThisRecord = 0;



	h = OpenEventLog(NULL, ("System"));   // source name : System.
	if (h == NULL)
	{
		printf("Could not open the Application event log.");
	}

	pevlr = (EVENTLOGRECORD*)& bBuffer;
	

	int num = 0, num1 = 0;
	while (ReadEventLogW(h,                // event log handle 
		EVENTLOG_BACKWARDS_READ |  // reads forward 
		EVENTLOG_SEQUENTIAL_READ, // sequential read 
		0,            // ignored for sequential reads 
		pevlr,        // pointer to buffer 
		0x7ffff,  // size of buffer 
		&dwRead,      // number of bytes read 
		&dwNeeded))   // bytes in next record 
	{
		num1++;

		while (dwRead > 0)

		{

			int eventID = (short)pevlr->EventID;

			
			
			if (eventID == DWPowerOnEventID || eventID == DWPowerOffEventID) {
				time_t TimeWritten = pevlr->TimeWritten;
				char tmp[64];
				memset(tmp, 0, 64);
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&TimeWritten));
				if (eventID == DWPowerOnEventID) {
					//cout << "power on \t";
					outfile << "power on \t";
				}
				else {
					outfile << "power off \t";
					//cout << "power off \t";
				}
				//cout << "Time:\t" << tmp << endl;
				outfile << "Time:\t" << tmp << endl;


				pchar = (TCHAR*)pevlr + pevlr->StringOffset;




				for (short j = 0; j < pevlr->NumStrings; j++)
				{
					pchar += strlen(pchar) + 1;

				}
			}




			dwRead -= pevlr->Length;
			pevlr = (EVENTLOGRECORD*)((LPBYTE)pevlr + pevlr->Length);



		}

		pevlr = (EVENTLOGRECORD*)& bBuffer;

	}
	outfile.close();  // 执行完操作后关闭文件句柄
	CloseEventLog(h);

}

int main() {
	string szSystemVersion = getSystemVersionInfo();
	/*cout << "SystemVersion:\t";
	if (szSystemVersion == "10.0") {		cout << "win10" << endl;	}
	if (szSystemVersion == "6.1")  { cout << "win7" << endl; }
	if (szSystemVersion == "5.1") {		cout << "WinXP";	}*/

	DisplayEntries(szSystemVersion);
	//system("pause");
}
