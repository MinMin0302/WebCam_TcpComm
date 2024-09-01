#include "pch.h"
#include "resource.h" // 리소스 ID 정의
#include "CTimeThread.h"
#include "MultithreadEditBoxTimerDlg.h"
#include <ctime>

IMPLEMENT_DYNCREATE(CTimeThread, CWinThread)

CTimeThread::CTimeThread()
{
}

CTimeThread::~CTimeThread()
{
}

BOOL CTimeThread::InitInstance()
{
    return TRUE;
}

int CTimeThread::ExitInstance()
{
    return CWinThread::ExitInstance();
}

int CTimeThread::Run()
{
    while (true)
    {
        // 현재 시간을 가져와서 문자열로 변환
        time_t now = time(0);
        struct tm localtm;
        localtime_s(&localtm, &now); // 안전한 함수 사용

        // asctime_s를 사용하여 문자열 변환
        char timeBuffer[26]; // asctime_s는 26자 버퍼를 필요로 함
        asctime_s(timeBuffer, sizeof(timeBuffer), &localtm);
        CString strTime(timeBuffer); // CString으로 변환

        // 메인 윈도우의 레이블 업데이트
        if (m_pMainWnd != nullptr)
        {
            m_pMainWnd->SetDlgItemText(IDC_TIME_LABEL, strTime);
        }

        Sleep(1000); // 1초마다 갱신
    }

    return 0;
}

BEGIN_MESSAGE_MAP(CTimeThread, CWinThread)
END_MESSAGE_MAP()
