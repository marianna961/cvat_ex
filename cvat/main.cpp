#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

void track(int, void*);
Mat orjinalGoruntu;
Mat fgMaskMOG2;
Mat griGoruntu, kirpik, kenarlar, aynali;
int thresh = 140, maxVal = 255;
int type = 1, deger = 8;

int main() {
    Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();
    cv::Rect myRoi(288, 12, 288, 288);
    VideoCapture cap;

    if (!cap.open(0)) {
        cerr << "видеопоток" << endl;
        return -1;
    }

    while (1) {
        cap >> orjinalGoruntu;

        if (orjinalGoruntu.empty()) {
            cerr << "кадр" << endl;
            continue;
        }

        cv::flip(orjinalGoruntu, aynali, 1);
        cv::rectangle(aynali, myRoi, Scalar(0, 0, 255));
        kirpik = aynali(myRoi);
        cvtColor(kirpik, griGoruntu, COLOR_RGB2GRAY);
        GaussianBlur(griGoruntu, griGoruntu, Size(23, 23), 0);

        //окно
        namedWindow("ayarla", WINDOW_AUTOSIZE);
        createTrackbar("Esik", "ayarla", &thresh, 250, track);
        createTrackbar("Maksimum", "ayarla", &maxVal, 255, track);
        createTrackbar("Esik Tipi", "ayarla", &type, 4, track);
        createTrackbar("Kenarlar", "ayarla", &deger, 100, track);

        pMOG2->apply(kirpik, fgMaskMOG2);

        if (!fgMaskMOG2.empty()) {
            GaussianBlur(fgMaskMOG2, fgMaskMOG2, Size(5, 5), 1.5, 1.5);
        }
        else {
            cerr << "fgMaskMOG2 пустая матрица" << endl;
            continue;
        }

        track(0, 0);
        imshow("ORJINAL Goruntu", aynali);
        imshow("ArkaPlan Kaldırıldı", fgMaskMOG2);
        imshow("Gri", griGoruntu);

        char key = waitKey(24);
        if (key == 27) break;
    }

    return 0;
}

void track(int, void*) {
}
