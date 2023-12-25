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

auto main(int, char**) -> int {
  kitakit::CreateResult window = kitakit::Instance::create(300, 600, "my app", nullptr,
    [](kitakit::EventRender & e) {
      ImGui::kk_BeginFilled(e, "##window");
      ImGui::Text("Hello World");
      ImGui::End();
    }
  );

  if (!window) {
    return (int)window.why();
  }

  window->run();

  kitakit::Instance::destroy(window);
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
