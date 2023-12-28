#pragma once

#include <metapp/metapp-std.hh>

namespace kitakit {

//------------------------------------------------------------------------------

enum class CreateResponse {
  SUCCESS,
  FAILURE,
  OPENGL_IMPORT,
  GLFW_INIT,
  CREATE_WINDOW,
  IMGUI_CONTEXT,
  IMGUI_GLFW,
  IMGUI_OPENGL,
};

class Instance;
using CreateResult = mpp::Result<Instance, CreateResponse>;

enum class GLFWEvent {
  INVALID,
  WAIT,
  POLL,
};

struct CreateExtended {
  // Default: nullptr - No ini file.
  const char * inifile = nullptr;

  // Default: 1 - Enable VSync
  int swap_interval = 1;

  //
  int glfw_major = 3; // GLFW_CONTEXT_VERSION_MAJOR
  int glfw_minor = 0; // GLFW_CONTEXT_VERSION_MINOR

  //
  bool transparent_frame = true; // GLFW_TRANSPARENT_FRAMEBUFFER

  //
  GLFWEvent event_management = GLFWEvent::WAIT;
};

//------------------------------------------------------------------------------

enum class DestroyResponse {
  SUCCESS,
  FAILURE,
  INVALID_RESULT,
  INVALID_INSTANCE,
};

//------------------------------------------------------------------------------

/*
 *
 */
struct EventRender {
  Instance & instance;
};
using CallbackRender = void(EventRender&);

/*
 *
 */
struct EventPreRender {
  Instance & instance;
  bool skip;
};
using CallbackPreRender = void(EventPreRender&);

/*
 *
 */
struct EventKey {
  Instance & instance;
};
using CallbackKey = void(EventKey&);

/*
 *
 */
struct EventClose {
  Instance & instance;
  bool cancel;
};
using CallbackClose = void(EventClose&);

//------------------------------------------------------------------------------

using RunFrame = unsigned int;
constexpr RunFrame FOREVER = 0xFFFFFFFF;

class Instance {
public:
  Instance(void * window);

public: 
  auto callback(CallbackRender * callback) -> Instance&;
  auto callback(CallbackPreRender * callback) -> Instance&;
  auto callback(CallbackKey * callback) -> Instance&;
  auto callback(CallbackClose * callback) -> Instance&;

  template <typename... VArgs>
  auto callbacks(VArgs... vfns) -> Instance& {
    (callback(vfns), ...);
    return *this;
  }

private:
  auto draw() -> void;
  auto event_draw() -> bool;

public:
  auto run(RunFrame nframes = FOREVER) -> void;

public:
  auto valid() const noexcept -> bool;
  operator bool() const noexcept;

  auto get_window() const noexcept -> void*;
  auto get_wsize_cache(int & width, int & height) -> void;

  auto hide() -> void;
  auto show() -> void;

public: 
  static auto create(int width, int height, const char * title = nullptr, CreateExtended * extended = nullptr) -> CreateResult;

  template <typename... Callbacks>
  static auto create(int width, int height, const char * title = {}, CreateExtended * extended = nullptr, Callbacks... callbacks) -> CreateResult {
    auto result = create(width, height, title, extended);
    if (result) {
      result->callbacks(callbacks...);
    }
    return result;
  }

  static auto destroy(Instance & instance) -> DestroyResponse;
  static auto destroy(CreateResult & instance) -> DestroyResponse;

private:
  void * window = nullptr;
  int cache_width = 0, cache_height = 0;

  GLFWEvent event = GLFWEvent::INVALID;

  CallbackRender    * cb_render    = nullptr;
  CallbackPreRender * cb_prerender = nullptr;
  CallbackKey       * cb_key       = nullptr;
  CallbackClose     * cb_close     = nullptr;
};

//------------------------------------------------------------------------------

/*
 *  Notifies the event system to render a frame when `event_management` is
 *  set to `WAIT`
 */
auto notify() -> void;

//------------------------------------------------------------------------------

enum class RunResponse {
  SUCCESS,
  FAILURE,
};

/*
 *  Immidiately create and run a kitakit instance
 */
template <typename... Callbacks>
auto run(int width, int height, const char * title = nullptr, CreateExtended * extended = nullptr, Callbacks... callbacks) -> RunResponse {
  auto result = Instance::create(width, height, title, extended, callbacks...);
  if (!result) {
    return RunResponse::FAILURE;
  }
  result->run();
  Instance::destroy(result);
  return RunResponse::SUCCESS;
}

//------------------------------------------------------------------------------

} // namespace kitakit
