# kitakit
Crossplatform GUI library powered by GLFW and ImGui (Improved and revised [kita](https://github.com/u16rogue/kita) project)

### Usage
* With [CPM](https://github.com/cpm-cmake/CPM.cmake). Check the repository's [tags](https://github.com/u16rogue/kitakit/tags) for a specific or the latest version.
```CMake
CPMAddPackage("gh:u16rogue/kitakit@<tag>") # ex: CPMAddPackage("gh:u16rogue/kitakit@0.1.0")
#...
target_link_libraries(yourproject PRIVATE kitakit)
```
*This will automatically resolve all the dependencies needed. If you want to provide your own just make sure the target dependency is available before loading kitakit.*

### Example
```c++
#include <kitakit/kitakit.hh>
#include <kitakit/kk_imgui.hh>
#include <kitakit/kk_kit.hh>

auto main(int, char**) -> int {
  kitakit::run(300, 600, "hello world", nullptr,
  [](kitakit::EventClose & e) {
    e.cancel = true;
    e.instance.hide();
  },
  [](kitakit::EventRender & e) {
    ImGui::kk_BeginFilled(e, "##window");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

    static kitakit::AsyncTask<void> task; 
    if (ImGui::Button("heavy task") && !task) task = [] { 
      std::this_thread::sleep_for(std::chrono::seconds(2));
      kitakit::notify();
    };

    if (task) {
      ImGui::Text("working");
    }

    ImGui::End();
  });

  return 0;
}
```

### Events
* Render
* Pre Render
* Close
* Key

### Libraries
* [metapp](https://github.com/u16rogue/metapp)
* [ImGui](https://github.com/ocornut/imgui)
* [GLFW](https://github.com/glfw/glfw)

### License
