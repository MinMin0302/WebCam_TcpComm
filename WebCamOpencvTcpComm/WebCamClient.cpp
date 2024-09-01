#include <iostream>

#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "opencv_world4100.lib")  // OpenCV 라이브러리 링크 (버전 맞게 수정)

#define SERVER_IP "127.0.0.1"  // 서버 IP 주소
#define PORT 8080
#define BUFFER_SIZE 65536
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

int main() {
    // Winsock 초기화
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 클라이언트 소켓 생성
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    // 서버 주소 설정
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server.\n";

    // 웹캠으로부터 영상 캡처
    cv::VideoCapture cap(0);  // 웹캠 ID (기본적으로 0)
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

        // 영상 압축 (JPEG 포맷)
        cv::imencode(".jpg", frame, buffer);

        // 서버로 데이터 전송
        int bytesSent = send(clientSocket, (const char*)buffer.data(), buffer.size(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Send failed.\n";
            break;
        }

        cv::imshow("Client - Sending Video", frame);
        if (cv::waitKey(1) == 27) {  // ESC 키를 누르면 종료
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
