#include <iostream>
#include <memory>

#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static bool test_playback = true;

void log(const char* message) {
  std::cout << message;
  OutputDebugString(message);
}

class MiniAudioTest {
public:
  MiniAudioTest() = default;
  virtual ~MiniAudioTest() = default;
  void Create() {
    log("Creating ma_engine object...\n");
    engine_ = std::make_unique<ma_engine>();
    if (!engine_) {
      log("Failed to create ma_engine object.\n");
      return;
    }

    log("Invoking ma_engine_config_init()...\n");
    ma_engine_config engine_config = ma_engine_config_init();

    log("Invoking ma_engine_init()...\n");
    ma_result result = ma_engine_init(&engine_config, engine_.get());
    if (result != MA_SUCCESS) {
      log("ma_engine_init() error.\n");
      return;
    }

    was_initialized_ = true;
  }

  void Destroy() {
    if (!engine_) {
      log("No ma_engine.\n");
      return;
    }

    if (was_initialized_) {
      log("Invoking ma_engine_stop()...\n");
      ma_result result = ma_engine_stop(engine_.get());
      if (result != MA_SUCCESS) {
        log("ma_engine_stop() error.\n");
      }

      log("Invoking ma_engine_uninit()...\n");
      ma_engine_uninit(engine_.get());
    }

    log("Destroying ma_engine object...\n");
    engine_ = nullptr;
    was_initialized_ = false;
  }

  void Play() {
    if (!engine_) {
      std::cout << "No ma_engine.\n";
      return;
    }

    if (!was_initialized_) {
      std::cout << "ma_engine was not initialized.\n";
      return;
    }

    std::cout << "Creating ma_sound object...\n";
    sound_ = std::make_unique<ma_sound>();
    if (!sound_) {
      std::cout << "Could not create ma_sound object.\n";
      return;
    }

    ma_uint32 flags = 0;

    std::cout << "Invoking ma_sound_init_from_file()...\n";
    ma_result result = ma_sound_init_from_file(engine_.get(), "test.wav", flags, nullptr, nullptr, sound_.get());
    if (result != MA_SUCCESS) {
      std::cout << "ma_sound_init_from_file() error.\n";
      return;
    }

    std::cout << "Invoking ma_sound_start()...\n";
    result = ma_sound_start(sound_.get());
    if (result != MA_SUCCESS) {
      std::cout << "ma_sound_start() error.\n";
      return;
    }
  }

  void Stop() {
    if (!engine_) {
      std::cout << "No ma_engine.\n";
      return;
    }

    if (!was_initialized_) {
      std::cout << "ma_engine was not initialized.\n";
      return;
    }

    if (!sound_) {
      std::cout << "No ma_sound.\n";
      return;
    }

    std::cout << "Invoking ma_sound_stop()...\n";
    ma_result result = ma_sound_stop(sound_.get());
    if (result != MA_SUCCESS) {
      std::cout << "ma_sound_stop() error.\n";
    }

    std::cout << "Invoking ma_sound_uninit()...\n";
    ma_sound_uninit(sound_.get());

    std::cout << "Destroying ma_sound object...\n";
    sound_ = nullptr;
  }

private:
  std::unique_ptr<ma_engine> engine_{ nullptr };
  std::unique_ptr<ma_sound> sound_{ nullptr };
  bool was_initialized_{ false };
};

int main() {
  MiniAudioTest miniaudio_test;

  while (true) {
    miniaudio_test.Create();
    if (test_playback) {
      miniaudio_test.Play();
      miniaudio_test.Stop();
    }
    miniaudio_test.Destroy();
  }
}