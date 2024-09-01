#include <iostream>
#include <winsock2.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "opencv_world4100.lib")  // OpenCV 라이브러리 링크 (버전 맞게 수정)

#define PORT 8080
#define BUFFER_SIZE 65536

int main() {
    // Winsock 초기화
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 서버 소켓 생성
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    // 주소 구조체 설정
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // 연결 대기
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Waiting for client connection...\n";

    // 클라이언트 연결 수락
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Client connected.\n";

    // 영상 수신 및 출력
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while (true) {
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Receive failed or connection closed.\n";
            break;
        }

        std::vector<uchar> data(buffer, buffer + bytesReceived);
        cv::Mat frame = cv::imdecode(data, cv::IMREAD_COLOR);

        if (!frame.empty()) {
            cv::imshow("Server - Received Video", frame);
            if (cv::waitKey(1) == 27) {  // ESC 키를 누르면 종료
                break;
            }
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
