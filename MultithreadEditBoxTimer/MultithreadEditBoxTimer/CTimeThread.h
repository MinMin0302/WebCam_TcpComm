#pragma once
#include "afxwin.h"

class CTimeThread : public CWinThread
{
    DECLARE_DYNCREATE(CTimeThread)

protected:
    CTimeThread();           // 생성자
    virtual ~CTimeThread();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();

    CWnd* m_pMainWnd; // 메인 윈도우 포인터 저장

protected:
    DECLARE_MESSAGE_MAP()
};

