#include <iostream>
#include <memory>

#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

class MiniAudioTest {
public:
  MiniAudioTest() = default;
  virtual ~MiniAudioTest() = default;
  void Create() {
    std::cout << "Creating ma_engine object...\n";
    engine_ = std::make_unique<ma_engine>();
    if (!engine_) {
      std::cout << "Failed to create ma_engine object.\n";
      return;
    }

    std::cout << "Invoking ma_engine_config_init()...\n";
    ma_engine_config engine_config = ma_engine_config_init();

    std::cout << "Invoking ma_engine_init()...\n";
    ma_result result = ma_engine_init(&engine_config, engine_.get());
    if (result != MA_SUCCESS) {
      std::cout << "ma_engine_init() error.\n";
      return;
    }

    was_initialized_ = true;
  }

  void Destroy() {
    if (!engine_) {
      std::cout << "No ma_engine.\n";
      return;
    }

    if (was_initialized_) {
      std::cout << "Invoking ma_engine_stop()...\n";
      ma_result result = ma_engine_stop(engine_.get());
      if (result != MA_SUCCESS) {
        std::cout << "ma_engine_stop() error.\n";
      }

      std::cout << "Invoking ma_engine_uninit()...\n";
      ma_engine_uninit(engine_.get());
    }

    std::cout << "Destroying ma_engine object...\n";
    engine_ = nullptr;
    was_initialized_ = false;
  }

private:
  std::unique_ptr<ma_engine> engine_{ nullptr };
  bool was_initialized_{ false };
};

int main() {
  MiniAudioTest miniaudio_test;

  while (true) {
    miniaudio_test.Create();
    miniaudio_test.Destroy();
  }
}