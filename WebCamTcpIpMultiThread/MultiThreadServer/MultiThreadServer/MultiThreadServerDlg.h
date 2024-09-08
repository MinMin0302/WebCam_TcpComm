#pragma once
#include <winsock2.h>
#include <thread>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")

class CMultiThreadServerDlg : public CDialogEx
{
public:
    CMultiThreadServerDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MULTITHREADSERVER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    SOCKET serverSocket, clientSocket;
    std::thread listenThread;

    void StartListening();  // TCP 리스닝 함수
    void DisplayImageAndTime(cv::Mat& frame);  // 이미지와 시간 출력 함수

public:
    afx_msg void OnDestroy();
};
