// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

#include <cstring>
#include <fstream>
#include <numeric>

#include <include/postprocess_op.h>
#include <include/preprocess_op.h>
#include <include/ocr_det_bm.hpp>

namespace BMPaddleOCR {

class DBDetector {
public:
  explicit DBDetector(const std::string &model_path,
                      const int &device_id,
                      const int &max_side_len, const double &det_db_thresh,
                      const double &det_db_box_thresh,
                      const double &det_db_unclip_ratio,
                      const bool &visualize) {
    this->device_id_ = device_id;
    this->max_side_len_ = max_side_len;
    this->det_db_thresh_ = det_db_thresh;
    this->det_db_box_thresh_ = det_db_box_thresh;
    this->det_db_unclip_ratio_ = det_db_unclip_ratio;
    this->visualize_ = visualize;

    std::shared_ptr<BMOCRDet> det_ptr(new BMOCRDet(model_path, device_id));
    sp_det_ptr_ = det_ptr;
    bm_ocr_det_ = sp_det_ptr_.get();
  }

  // Run predictor
  void Run(cv::Mat &img, std::vector<std::vector<std::vector<int>>> &boxes);

private:
  int device_id_ = 0;
  int max_side_len_ = 960;
  double det_db_thresh_ = 0.3;
  double det_db_box_thresh_ = 0.5;
  double det_db_unclip_ratio_ = 2.0;
  bool visualize_ = true;

  std::vector<float> mean_ = {0.485f, 0.456f, 0.406f};
  std::vector<float> scale_ = {1 / 0.229f, 1 / 0.224f, 1 / 0.225f};
  bool is_scale_ = true;

  BMOCRDet* bm_ocr_det_;
  std::shared_ptr<BMOCRDet> sp_det_ptr_;

  // pre-process
  ResizeImgType0 resize_op_;
  Normalize normalize_op_;
  Permute permute_op_;

  // post-process
  PostProcessor post_processor_;
};

} // namespace BMPaddleOCR
