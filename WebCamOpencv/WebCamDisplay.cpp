#include <opencv2/opencv.hpp>  // OpenCV�� ��� ��� ����

using namespace cv;

int main() {
    // ��ķ ���� (�⺻ ��ķ�� ID 0)
    VideoCapture cap(0);

    // ��ķ�� ������ �ʴ� ��� ���� ó��
    if (!cap.isOpened()) {
        printf("��ķ�� �� �� �����ϴ�.\n");
        return -1;
    }

    Mat frame;
    while (true) {
        // ��ķ���� ������ �б�
        cap >> frame;

        // �������� ����� ���� ���� ��� ���� ����
        if (frame.empty()) {
            printf("�������� ���� �� �����ϴ�.\n");
            break;
        }

        // �������� �����쿡 ǥ��
        imshow("Webcam Video", frame);

        // ESC Ű�� ������ ���� ���� (waitKey(1)�� �����Ӵ� 1ms ���)
        if (waitKey(1) == 27) {  // 27�� ESC Ű�� ASCII �ڵ�
            break;
        }
    }

    // ��ķ�� ������ �ڿ� ����
    cap.release();
    destroyAllWindows();

    return 0;
}
