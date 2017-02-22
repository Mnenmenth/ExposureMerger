/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include <opencv2/opencv.hpp>
#include "MergeExposures.h"

void MergeExposures::merge(std::vector<std::string>& img_paths, std::vector<float>& times) {
    using namespace cv;
    std::vector<Mat> imgs;
    for(std::string& img_path : img_paths)
        imgs.push_back(imread(img_path));

    std::cout << "ok" << std::endl;
    Mat calibrated;
    CalibrateDebevec* calibrate = createCalibrateDebevec();
    calibrate->process(imgs, calibrated, times);

    std::cout << "ok?" << std::endl;

    Mat hdr;
    MergeDebevec* debevec = createMergeDebevec();
    debevec->process(imgs, hdr, times);

    Mat ldr;
    TonemapDurand* tonemap = createTonemapDurand();
    tonemap->process(hdr, ldr);

    Mat fusion;
    MergeMertens* mertens = createMergeMertens();
    mertens->process(imgs, fusion);

    imwrite("fusion.png", fusion * 255);
    imwrite("ldr.png", ldr * 255);
    imwrite("hdr.hdr", hdr);

}