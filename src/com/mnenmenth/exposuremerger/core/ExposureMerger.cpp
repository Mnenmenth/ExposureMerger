/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include <iostream>
#include "../imgprocessing/MergeExposures.h"

int main() {
    std::vector<std::string> imgs;
    imgs.push_back("test1.jpg");
    imgs.push_back("test2.jpg");
    imgs.push_back("test3.jpg");
    imgs.push_back("test4.jpg");
    std::vector<float> times;
    times.push_back(15.0);
    times.push_back(2.5);
    times.push_back(0.25);
    times.push_back(0.033);
    MergeExposures::merge(imgs, times);
    return 0;
}