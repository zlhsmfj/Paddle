/* Copyright (c) 2016 Baidu, Inc. All Rights Reserve.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "paddle/utils/Logging.h"
#include "Layer.h"

namespace paddle {
/**
 * A layer for checking EOS for each sample:
 * - output_id = (input_id == conf.eos_id)
 *
 * The result is stored in output_.ids.
 * It is used by recurrent layer group.
 */
class EosIdCheckLayer : public Layer {
public:
  explicit EosIdCheckLayer(const LayerConfig& config) : Layer(config) {}

  virtual bool init(const LayerMap& layerMap,
                    const ParameterMap& parameterMap) {
    bool ret = Layer::init(layerMap, parameterMap);
    CHECK_EQ(1UL, inputLayers_.size());
    return ret;
  }

  virtual void forward(PassType passType) {
    Layer::forward(passType);

    const Argument& input = getInput(0);
    IVector::resizeOrCreate(output_.ids, input.ids->getSize(), useGpu_);
    output_.ids->isEqualTo(*input.ids, config_.eos_id());
  }

  virtual void backward(const UpdateCallback& callback) {}
};

REGISTER_LAYER(eos_id, EosIdCheckLayer);

}  // namespace paddle
