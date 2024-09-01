#pragma once
#include "afxwin.h"

class CTimeThread : public CWinThread
{
    DECLARE_DYNCREATE(CTimeThread)

protected:
    CTimeThread();           // ������
    virtual ~CTimeThread();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();

    CWnd* m_pMainWnd; // ���� ������ ������ ����

protected:
    DECLARE_MESSAGE_MAP()
};

