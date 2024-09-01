#include <iostream>

#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "opencv_world4100.lib")  // OpenCV ���̺귯�� ��ũ (���� �°� ����)

#define SERVER_IP "127.0.0.1"  // ���� IP �ּ�
#define PORT 8080
#define BUFFER_SIZE 65536
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

int main() {
    // Winsock �ʱ�ȭ
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Ŭ���̾�Ʈ ���� ����
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    // ���� �ּ� ����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // ������ ����
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server.\n";

    // ��ķ���κ��� ���� ĸó
    cv::VideoCapture cap(0);  // ��ķ ID (�⺻������ 0)
    if (!cap.isOpened()) {
        std::cerr << "Failed to open webcam.\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    cv::Mat frame;
    std::vector<uchar> buffer;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Captured empty frame.\n";
            break;
        }

        // ���� ���� (JPEG ����)
        cv::imencode(".jpg", frame, buffer);

        // ������ ������ ����
        int bytesSent = send(clientSocket, (const char*)buffer.data(), buffer.size(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Send failed.\n";
            break;
        }

        cv::imshow("Client - Sending Video", frame);
        if (cv::waitKey(1) == 27) {  // ESC Ű�� ������ ����
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
