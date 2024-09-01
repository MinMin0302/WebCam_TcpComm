#include "pch.h"
#include "resource.h" // ���ҽ� ID ����
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
        // ���� �ð��� �����ͼ� ���ڿ��� ��ȯ
        time_t now = time(0);
        struct tm localtm;
        localtime_s(&localtm, &now); // ������ �Լ� ���

        // asctime_s�� ����Ͽ� ���ڿ� ��ȯ
        char timeBuffer[26]; // asctime_s�� 26�� ���۸� �ʿ�� ��
        asctime_s(timeBuffer, sizeof(timeBuffer), &localtm);
        CString strTime(timeBuffer); // CString���� ��ȯ

        // ���� �������� ���̺� ������Ʈ
        if (m_pMainWnd != nullptr)
        {
            m_pMainWnd->SetDlgItemText(IDC_TIME_LABEL, strTime);
        }

        Sleep(1000); // 1�ʸ��� ����
    }

    return 0;
}

BEGIN_MESSAGE_MAP(CTimeThread, CWinThread)
END_MESSAGE_MAP()
