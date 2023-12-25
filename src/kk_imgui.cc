#include <kitakit/kk_imgui.hh>

auto ImGui::kk_BeginFilled(kitakit::Instance * instance, const char* name, bool* p_open, ImGuiWindowFlags flags) -> bool {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  int w, h;
  instance->get_wsize_cache(w, h);
  ImGui::SetNextWindowSize(ImVec2(w, h));
  return ImGui::Begin(name, p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | flags);
}

auto ImGui::kk_BeginFilled(kitakit::EventRender & e, const char* name, bool* p_open, ImGuiWindowFlags flags) -> bool {
  return kk_BeginFilled(e.instance, name, p_open, flags);
}
