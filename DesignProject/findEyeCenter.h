#pragma once

//
// Author:  https://github.com/trishume/eyeLike
//

#include "opencv2/imgproc/imgproc.hpp"

cv::Point findEyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow);