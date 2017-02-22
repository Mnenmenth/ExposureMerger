/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#ifndef EXPOSUREMERGER_MERGEEXPOSURES_H
#define EXPOSUREMERGER_MERGEEXPOSURES_H


#include <opencv2/core/mat.hpp>
#include <string>
#include <vector>

class MergeExposures {
public:
    static void merge(std::vector<std::string>&, std::vector<float>&);
};


#endif //EXPOSUREMERGER_MERGEEXPOSURES_H
