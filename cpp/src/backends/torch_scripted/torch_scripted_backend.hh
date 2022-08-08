#ifndef TS_CPP_BACKENDS_TORCH_SCRIPTED_TORCH_SCRIPTED_BACKEND_HH_
#define TS_CPP_BACKENDS_TORCH_SCRIPTED_TORCH_SCRIPTED_BACKEND_HH_

#include <fmt/format.h>
#include <memory>
#include <torch/script.h>
#include <torch/torch.h>

#include "src/backends/core/backend.hh"
#include "src/backends/torch_scripted/handler/base_handler.hh"
#include "src/backends/torch_scripted/handler/handler_factory.hh"
#include "src/utils/dl_loader.hh"
#include "src/utils/message.hh"
#include "src/utils/model_archive.hh"

namespace torchserve {
  namespace torchscripted {
    class Backend final : public torchserve::Backend {
      public:
      Backend() {};
      ~Backend() {};

      bool Initialize(const std::string& model_path) override;

      std::pair<
      std::unique_ptr<torchserve::LoadModelResponse>, std::shared_ptr<torchserve::ModelInstance>> 
      LoadModelInternal(
        std::shared_ptr<torchserve::LoadModelRequest> load_model_request) override;
      
      private:
      void LoadHandler();
      
      std::unique_ptr<torchserve::DLLoader<BaseHandler>> dl_loader_;
      std::shared_ptr<BaseHandler> handler_;
    };

    class ModelInstance final: public torchserve::ModelInstance {
      public:
      ModelInstance(
        const std::string& instance_id,
        std::shared_ptr<torch::jit::script::Module> model, 
        std::shared_ptr<torchserve::torchscripted::BaseHandler> handler, 
        std::shared_ptr<torch::Device> device) :
        torchserve::ModelInstance(instance_id), model_(model), handler_(handler), device_(device) {};
      ~ModelInstance() {};

      std::shared_ptr<torchserve::InferenceResponse> Predict(
        torchserve::InferenceRequestBatch batch) override;

      private:
      torch::Device device_;
      std::shared_ptr<torch::jit::script::Module> model_;
      std::shared_ptr<torchserve::torchscripted::BaseHandler> handler_;
    };
  } // namespace torchscripted
} // namespace torchserve
#endif // TS_CPP_BACKENDS_TORCH_SCRIPTED_TORCH_SCRIPTED_BACKEND_HH_