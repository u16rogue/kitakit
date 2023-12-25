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

struct CreateExtended {
  const char * inifile = nullptr;
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
  Instance * instance;
};
using CallbackRender = void(EventRender&);

/*
 *
 */
struct EventPreRender {
  Instance * instance;
  bool skip;
};
using CallbackPreRender = void(EventPreRender&);

/*
 *
 */
struct EventKey {
  Instance * instance;
};
using CallbackKey = void(EventKey&);

/*
 *
 */
struct EventClose {
  Instance * instance;
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
  auto polled_draw() -> bool;

public:
  auto run(RunFrame nframes = FOREVER) -> void;

public:
  auto valid() const noexcept -> bool;
  operator bool() const noexcept;

  auto get_window() const noexcept -> void*;
  auto get_wsize_cache(int & width, int & height) -> void;

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

  CallbackRender    * cb_render    = nullptr;
  CallbackPreRender * cb_prerender = nullptr;
  CallbackKey       * cb_key       = nullptr;
  CallbackClose     * cb_close     = nullptr;
};

//------------------------------------------------------------------------------

} // namespace kitakit
