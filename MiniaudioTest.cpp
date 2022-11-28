#include <iostream>
#include <memory>

#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static bool test_playback = true;

void PrintToLog(const char* message) {
  std::cout << message;

#if (defined(_MSC_VER))
  OutputDebugString(message);
#endif
}

void OnLog(ma_uint32 level, const char* pMessage)
{
  constexpr int kBufferSize = 512;
  char buffer[kBufferSize];
  _snprintf(buffer, kBufferSize, "%s: %s", ma_log_level_to_string(level), pMessage);
  PrintToLog(buffer);
}

class MiniAudioTest {
public:
  MiniAudioTest() = default;
  virtual ~MiniAudioTest() = default;
  void Create() {
    PrintToLog("Creating ma_engine object...\n");
    engine_ = std::make_unique<ma_engine>();
    if (!engine_) {
      PrintToLog("Failed to create ma_engine object.\n");
      return;
    }

    PrintToLog("Invoking ma_engine_config_init()...\n");
    ma_engine_config engine_config = ma_engine_config_init();

    PrintToLog("Invoking ma_engine_init()...\n");
    ma_result result = ma_engine_init(&engine_config, engine_.get());
    if (result != MA_SUCCESS) {
      PrintToLog("ma_engine_init() error.\n");
      return;
    }

    was_initialized_ = true;
  }

  void Destroy() {
    if (!engine_) {
      PrintToLog("No ma_engine.\n");
      return;
    }

    if (was_initialized_) {
      PrintToLog("Invoking ma_engine_stop()...\n");
      ma_result result = ma_engine_stop(engine_.get());
      if (result != MA_SUCCESS) {
        PrintToLog("ma_engine_stop() error.\n");
      }

      PrintToLog("Invoking ma_engine_uninit()...\n");
      ma_engine_uninit(engine_.get());
    }

    PrintToLog("Destroying ma_engine object...\n");
    engine_ = nullptr;
    was_initialized_ = false;
  }

  void Play() {
    if (!engine_) {
      PrintToLog("No ma_engine.\n");
      return;
    }

    if (!was_initialized_) {
      PrintToLog("ma_engine was not initialized.\n");
      return;
    }

    PrintToLog("Creating ma_sound object...\n");
    sound_ = std::make_unique<ma_sound>();
    if (!sound_) {
      PrintToLog("Could not create ma_sound object.\n");
      return;
    }

    ma_uint32 flags = 0;

    PrintToLog("Invoking ma_sound_init_from_file()...\n");
    ma_result result = ma_sound_init_from_file(engine_.get(), "test.wav", flags, nullptr, nullptr, sound_.get());
    if (result != MA_SUCCESS) {
      PrintToLog("ma_sound_init_from_file() error.\n");
      return;
    }

    PrintToLog("Invoking ma_sound_start()...\n");
    result = ma_sound_start(sound_.get());
    if (result != MA_SUCCESS) {
      PrintToLog("ma_sound_start() error.\n");
      return;
    }
  }

  void Stop() {
    if (!engine_) {
      PrintToLog("No ma_engine.\n");
      return;
    }

    if (!was_initialized_) {
      PrintToLog("ma_engine was not initialized.\n");
      return;
    }

    if (!sound_) {
      PrintToLog("No ma_sound.\n");
      return;
    }

    PrintToLog("Invoking ma_sound_stop()...\n");
    ma_result result = ma_sound_stop(sound_.get());
    if (result != MA_SUCCESS) {
      PrintToLog("ma_sound_stop() error.\n");
    }

    PrintToLog("Invoking ma_sound_uninit()...\n");
    ma_sound_uninit(sound_.get());

    PrintToLog("Destroying ma_sound object...\n");
    sound_ = nullptr;
  }

private:
  std::unique_ptr<ma_engine> engine_{ nullptr };
  std::unique_ptr<ma_sound> sound_{ nullptr };
  bool was_initialized_{ false };
};

int main() {
  ma_log_simple_callback = &OnLog;
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