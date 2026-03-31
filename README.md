# rune-logger

A lightweight C++ logging wrapper built on [spdlog](https://github.com/gabime/spdlog) with themed terminal output and rotating file support.

### Requirements

- C++17 or later
- CMake 3.16+

### Integration

Add rune-logger to your project with CMake `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
    rune_logger
    GIT_REPOSITORY https://github.com/YuichiKamishiro/rune_logger.git
    GIT_TAG main  # recommend pinning to a specific commit or tag
)
FetchContent_MakeAvailable(rune_logger)

target_link_libraries(your_target PRIVATE rune_logger)
```

spdlog is fetched automatically as a transitive dependency.

### Building the examples 

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```cpp
#include <rune_logger.hpp>

int main() {
    rune::config()
        .theme(rune::theme::ocean)
        .prefix("myapp")
        .log_file_path("logs/myapp.log")
        .initialize();

    RUNE_DEBUG("debug message");
    RUNE_INFO("info message");
    RUNE_WARN("warning message");
    RUNE_ERROR("error message");
    RUNE_CRITICAL("critical message");
}
```

## Config options

| Method | Description | Default |
|---|---|---|
| `.theme(t)` | Set color theme | `theme::classic` |
| `.prefix(s)` | Logger name shown in output | `"rune"` |
| `.log_file_path(s)` | Path to rotating log file | `"rune.log"` |
| `.log_level(l)` | Minimum log level | `info` |
| `.max_file_size(n)` | Max size per log file (bytes) | 10 MB |
| `.max_files(n)` | Number of rotating files to keep | 5 |
| `.file_pattern(s)` | Custom spdlog pattern for file output | — |
| `.console_pattern(s)` | Custom spdlog pattern for console output | — |
| `.set_as_default(b)` | Register logger as the spdlog default | `true` |

## Built-in themes

`classic` · `ocean` · `sunset` · `matrix` · `forest` · `desert` · `night` · `solarized` · `minimal` · `vibrant` · `gray_theme`

## Custom theme

```cpp
rune::theme t = {};
t.timestamp_color = rune::color::bright_yellow;
t.logger_color    = rune::color::bold_magenta;
t.message_color   = rune::color::bright_white;
t.bracket_color   = rune::color::bright_cyan;
t.level_colors    = rune::level_color_variant::vivid; // classic | vivid | monochrome

rune::config().theme(t).prefix("custom").initialize();
```

Available colors are in the `rune::color` namespace (e.g. `red`, `bold_green`, `bg_blue`, `gray`, …).
