#include <kitakit/kitakit.hh>
#include <metapp/metapp.hh>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <cstdlib>

static decltype(glViewport)   * kk_glViewport   = nullptr;
static decltype(glClearColor) * kk_glClearColor = nullptr;
static decltype(glClear)      * kk_glClear      = nullptr;

static auto as_window(kitakit::Instance * instance) -> GLFWwindow * { return reinterpret_cast<GLFWwindow*>(instance->get_window()); };
#define _window as_window(this)

kitakit::Instance::Instance(void * window)
  : window(window)
{};

auto kitakit::Instance::valid() const noexcept -> bool {
  return window;
}

kitakit::Instance::operator bool() const noexcept {
  return valid();
}

auto kitakit::Instance::get_window() const noexcept -> void* {
  return window;
}

auto kitakit::Instance::get_wsize_cache(int & width, int & height) -> void {
  width  = cache_width;
  height = cache_height;
}

auto kitakit::Instance::callback(CallbackRender * callback) -> Instance& {
  if (valid()) { cb_render = callback; }
  return *this;
}

auto kitakit::Instance::callback(CallbackPreRender * callback) -> Instance& {
  if (valid()) { cb_prerender = callback; }
  return *this;
}

auto kitakit::Instance::callback(CallbackKey * callback) -> Instance& {
  if (valid()) { cb_key = callback; }
  return *this;
}

auto kitakit::Instance::callback(CallbackClose * callback) -> Instance& {
  if (valid()) { cb_close = callback; }
  return *this;
}

auto kitakit::Instance::draw() -> void {
  if (cb_prerender) {
    EventPreRender e_prerender = {
      .instance = this,
      .skip = false,
    };
    cb_prerender(e_prerender);
    if (e_prerender.skip) {
      return;
    }
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  EventRender e_render = {
    .instance = this,
  };
  cb_render(e_render);

  ImGui::Render();
  glfwGetFramebufferSize(_window, &cache_width, &cache_height);
  kk_glViewport(0, 0, cache_width, cache_height);
  kk_glClearColor(0, 0, 0, 0);
  kk_glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(_window);
}

auto kitakit::Instance::polled_draw() -> bool {
  glfwPollEvents();
  if (glfwWindowShouldClose(_window)) {
    if (cb_close) {
      EventClose e_close = {
        .instance = this,
        .cancel   = false,
      };
      cb_close(e_close);
      if (e_close.cancel) {
        glfwSetWindowShouldClose(_window, GLFW_FALSE);
      }
    }
    return false;
  }
  draw();
  return true;
}

auto kitakit::Instance::run(RunFrame nframes) -> void {
  if (!valid() || !cb_render) { return; }
  glfwShowWindow(_window);
  while (polled_draw()) {
    if (nframes != FOREVER && --nframes == 0) {
      return; 
    }
  }
}

auto kitakit::Instance::create(int width, int height, const char * title, CreateExtended * extended) -> CreateResult {
  
  if (!title) {
    title = "window";
  }

  CreateExtended __ex {};
  if (!extended) {
    extended = &__ex;
  }

  bool failed = true;

  if (!glfwInit()) {
    return CreateResponse::GLFW_INIT;
  }

  mpp_defer {
    if (failed) {
      glfwTerminate();
    }
  };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  GLFWwindow * window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    return CreateResponse::CREATE_WINDOW;
  }

  mpp_defer {
    if (failed) {
      glfwDestroyWindow(window);
    }
  };

  glfwMakeContextCurrent(window);
  glfwSwapInterval(extended->swap_interval);

  #define _load_import(fn)                                                \
    if (!kk_##fn) {                                                       \
      kk_##fn = reinterpret_cast<decltype(fn)*>(glfwGetProcAddress(#fn)); \
      if (!kk_##fn) {                                                     \
        return CreateResponse::OPENGL_IMPORT;                             \
      }                                                                   \
    }
  
  _load_import(glViewport);
  _load_import(glClearColor);
  _load_import(glClear);

  #undef _load_import

  if (!ImGui::CreateContext()) {
    return CreateResponse::IMGUI_CONTEXT;
  }

  mpp_defer {
    if (failed) {
      ImGui::DestroyContext();
    }
  };

  ImGuiIO & io = ImGui::GetIO();
  io.LogFilename = nullptr;
  io.IniFilename = extended->inifile;

  if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
    return CreateResponse::IMGUI_GLFW;
  }

  mpp_defer {
    if (failed) {
      ImGui_ImplGlfw_Shutdown();
    }
  };

  if (!ImGui_ImplOpenGL3_Init("#version 130")) {
    return CreateResponse::IMGUI_OPENGL;
  }

  mpp_defer {
    if (failed) {
      ImGui_ImplOpenGL3_Shutdown();
    }
  };

  failed = false;
  return window;
}

auto kitakit::Instance::destroy(Instance & instance) -> DestroyResponse {
  if (!instance) {
    return DestroyResponse::INVALID_INSTANCE;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(as_window(&instance));
  glfwTerminate();

  return DestroyResponse::SUCCESS;
}

auto kitakit::Instance::destroy(CreateResult & instance) -> DestroyResponse {
  if (!instance) {
    return DestroyResponse::INVALID_RESULT;
  }

  return destroy(*instance);
}

#undef _window
