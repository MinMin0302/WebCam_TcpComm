#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>  // inet_pton ����� ���� ��� ����
#include <iostream>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")  // WinSock ���̺귯�� ��ũ
#pragma comment(lib, "opencv_world4100.lib")  // OpenCV ���̺귯�� ��ũ

#define SERVER_IP "127.0.0.1"  // ���� IP �ּ�
#define SERVER_PORT 8080       // ���� ��Ʈ ��ȣ

int main() {
    // WinSock �ʱ�ȭ
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return -1;
    }

    // ���� ����
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return -1;
    }

    // ���� �ּ� ����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // ������ ����
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // OpenCV�� ��ķ ����
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error opening webcam!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // �������� JPEG�� ���ڵ�
        std::vector<uchar> buffer;
        cv::imencode(".jpg", frame, buffer);

        // ������ ũ��� ������ ����
        int frameSize = buffer.size();
        int sentBytes = send(clientSocket, (char*)&frameSize, sizeof(int), 0);
        if (sentBytes == SOCKET_ERROR) {
            std::cerr << "Failed to send frame size! Error: " << WSAGetLastError() << std::endl;
            break;
        }

        sentBytes = send(clientSocket, (char*)buffer.data(), frameSize, 0);
        if (sentBytes == SOCKET_ERROR) {
            std::cerr << "Failed to send frame data! Error: " << WSAGetLastError() << std::endl;
            break;
        }

        if (cv::waitKey(30) >= 0) break;  // ESC Ű �Է� �� ����
    }

    // Ŭ���̾�Ʈ ���� �ݱ�
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
