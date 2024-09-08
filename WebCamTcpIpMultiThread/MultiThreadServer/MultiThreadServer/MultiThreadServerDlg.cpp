#include "pch.h"
#include "framework.h"
#include "MultiThreadServer.h"
#include "MultiThreadServerDlg.h"
#include "afxdialogex.h"
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <atlimage.h>

CMultiThreadServerDlg::CMultiThreadServerDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MULTITHREADSERVER_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThreadServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiThreadServerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CMultiThreadServerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 소켓 초기화 및 리스닝 시작
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    // 자식 스레드에서 리스닝 시작
    listenThread = std::thread(&CMultiThreadServerDlg::StartListening, this);

    return TRUE;  // 포커스를 컨트롤로 설정하지 않으면 TRUE를 반환합니다.
}

void CMultiThreadServerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CMultiThreadServerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMultiThreadServerDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // 소켓 및 스레드 정리
    closesocket(serverSocket);
    if (clientSocket != INVALID_SOCKET) closesocket(clientSocket);
    listenThread.join();
    WSACleanup();
}

void CMultiThreadServerDlg::StartListening()
{
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

    while (true)
    {
        // 프레임 크기 수신
        int frameSize;
        int result = recv(clientSocket, (char*)&frameSize, sizeof(int), 0);
        if (result <= 0) break;

        // 프레임 데이터 수신
        std::vector<uchar> buffer(frameSize);
        result = recv(clientSocket, (char*)buffer.data(), frameSize, 0);
        if (result <= 0) break;

        // 수신한 데이터를 이미지로 디코딩
        cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
        DisplayImageAndTime(frame);
    }

    closesocket(clientSocket);
}

void CMultiThreadServerDlg::DisplayImageAndTime(cv::Mat& frame)
{
    // 현재 시간을 얻기
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm localTime;
    localtime_s(&localTime, &now);  // 안전한 방법으로 현재 시간 얻기

    char timeBuffer[100];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &localTime);
    CString strTime(timeBuffer);

    // OpenCV 이미지를 MFC 윈도우에 표시
    CImage img;
    // CImage::Create(int nWidth, int nHeight, int nBpp, int nPitch, UINT uUsage = 0, CImage* pImage = NULL);
    img.Create(frame.cols, frame.rows, 24, frame.step);  // 24 비트 이미지 (RGB)

    // 이미지 데이터를 CImage에 복사
    for (int y = 0; y < frame.rows; ++y) {
        for (int x = 0; x < frame.cols; ++x) {
            cv::Vec3b color = frame.at<cv::Vec3b>(y, x);
            img.SetPixel(x, y, RGB(color[2], color[1], color[0]));
        }
    }

    CClientDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    img.Draw(dc.m_hDC, 0, 0);

    // 현재 시간도 함께 표시
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(255, 255, 255));
    dc.TextOut(10, frame.rows + 10, strTime);
}
