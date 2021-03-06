/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include "MergeExposures.h"

void
MergeExposures::merge(std::vector<std::string> &img_paths, std::vector<float> &times, float gamma, cv::Mat *debevecOut,
                      cv::Mat *tonemapOut, cv::Mat *mertensOut) {
    using namespace cv;
    std::vector<Mat> imgs;
    for (std::string &img_path : img_paths)
        imgs.push_back(imread(img_path));

    Mat calibrated;
    Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
    calibrate->process(imgs, calibrated, times);

    Mat hdr;
    Ptr<MergeDebevec> debevec = createMergeDebevec();
    debevec->process(imgs, hdr, times);

    Mat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(gamma);
    tonemap->process(hdr, ldr);

    Mat fusion;
    Ptr<MergeMertens> mertens = createMergeMertens();
    mertens->process(imgs, fusion);


    *debevecOut = hdr.clone();
    *tonemapOut = ldr.clone();
    *mertensOut = fusion.clone();
}