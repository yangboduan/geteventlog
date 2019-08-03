#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
using std::ofstream;
//https://msdn.microsoft.com/ja-jp/windows/aa363674(v=vs.80)
using namespace std;

void DisplayEntries()
{

	ofstream outfile;
	//��������ļ���ios::trunc������ļ��Ѿ����ڣ������ݽ��ڴ��ļ�֮ǰ���ض�
	outfile.open("eventlog.txt", ios::out | ios::trunc);
	//��ȡʱ��
	time_t cTime = time(NULL);
	char sTime[64] = { 0 };
	//��ʽ��ʱ��
	struct tm* localTime = localtime(&cTime);
	strftime(sTime, sizeof(sTime), "%Y-%m-%d %X", localTime);

	//���ļ���д��ʱ��
	outfile << sTime << endl;


	char* tempBuf = new char[100];
	memset(tempBuf, 0, 100);
	HANDLE h;
	EVENTLOGRECORD* pevlr;
	TCHAR bBuffer[0x7ffff] = { 0 }; //0x7ffffΪ��������� 
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

			if (eventID == 12 or eventID == 13) {
				time_t TimeWritten = pevlr->TimeWritten;
				char tmp[64];
				memset(tmp, 0, 64);
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&TimeWritten));
				if (eventID == 12) {
					cout << "power on ";
					outfile << "power on ";
				}
				else {
					outfile << " power off ";
					cout << " power off ";
				}
				cout << "Time:\t" << tmp << endl;
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
	outfile.close();  // ִ���������ر��ļ����
	CloseEventLog(h);

}

int main() {
	DisplayEntries();
	system("pause");
}
