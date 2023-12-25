#pragma once

#include <kitakit/kitakit.hh>
#include <imgui.h>

namespace ImGui {

auto kk_BeginFilled(kitakit::Instance * instance, const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0) -> bool;
auto kk_BeginFilled(kitakit::EventRender & e, const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0) -> bool;

} // namespace ImGui
