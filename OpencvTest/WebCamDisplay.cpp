#include <opencv2/opencv.hpp>  // OpenCV의 모든 모듈 포함

using namespace cv;

int main() {
    // 웹캠 열기 (기본 웹캠은 ID 0)
    VideoCapture cap(0);

    // 웹캠이 열리지 않는 경우 오류 처리
    if (!cap.isOpened()) {
        printf("웹캠을 열 수 없습니다.\n");
        return -1;
    }

    Mat frame;
    while (true) {
        // 웹캠에서 프레임 읽기
        cap >> frame;

        // 프레임을 제대로 읽지 못한 경우 루프 종료
        if (frame.empty()) {
            printf("프레임을 읽을 수 없습니다.\n");
            break;
        }

        // 프레임을 윈도우에 표시
        imshow("Webcam Video", frame);

        // ESC 키를 누르면 루프 종료 (waitKey(1)로 프레임당 1ms 대기)
        if (waitKey(1) == 27) {  // 27은 ESC 키의 ASCII 코드
            break;
        }
    }

    // 웹캠과 윈도우 자원 해제
    cap.release();
    destroyAllWindows();

    return 0;
}
