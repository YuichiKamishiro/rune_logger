#ifndef RUNE_LOGGER_H
#define RUNE_LOGGER_H

#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace rune {

namespace color {
// Reset and basic styles (widely supported)
constexpr const char *reset = "\033[0m";
constexpr const char *bold = "\033[1m";
constexpr const char *dim = "\033[2m";
constexpr const char *underline = "\033[4m";

// Foreground colors (normal)
constexpr const char *black = "\033[30m";
constexpr const char *red = "\033[31m";
constexpr const char *green = "\033[32m";
constexpr const char *yellow = "\033[33m";
constexpr const char *blue = "\033[34m";
constexpr const char *magenta = "\033[35m";
constexpr const char *cyan = "\033[36m";
constexpr const char *white = "\033[37m";
constexpr const char *default_fg = "\033[39m";

// Foreground colors (bright)
constexpr const char *bright_black = "\033[90m";
constexpr const char *bright_red = "\033[91m";
constexpr const char *bright_green = "\033[92m";
constexpr const char *bright_yellow = "\033[93m";
constexpr const char *bright_blue = "\033[94m";
constexpr const char *bright_magenta = "\033[95m";
constexpr const char *bright_cyan = "\033[96m";
constexpr const char *bright_white = "\033[97m";

// Background colors (normal)
constexpr const char *bg_black = "\033[40m";
constexpr const char *bg_red = "\033[41m";
constexpr const char *bg_green = "\033[42m";
constexpr const char *bg_yellow = "\033[43m";
constexpr const char *bg_blue = "\033[44m";
constexpr const char *bg_magenta = "\033[45m";
constexpr const char *bg_cyan = "\033[46m";
constexpr const char *bg_white = "\033[47m";
constexpr const char *bg_default = "\033[49m";

// Background colors (bright)
constexpr const char *bg_bright_black = "\033[100m";
constexpr const char *bg_bright_red = "\033[101m";
constexpr const char *bg_bright_green = "\033[102m";
constexpr const char *bg_bright_yellow = "\033[103m";
constexpr const char *bg_bright_blue = "\033[104m";
constexpr const char *bg_bright_magenta = "\033[105m";
constexpr const char *bg_bright_cyan = "\033[106m";
constexpr const char *bg_bright_white = "\033[107m";

// Combined style + color (common combinations)
constexpr const char *bold_red = "\033[1;31m";
constexpr const char *bold_green = "\033[1;32m";
constexpr const char *bold_yellow = "\033[1;33m";
constexpr const char *bold_blue = "\033[1;34m";
constexpr const char *bold_magenta = "\033[1;35m";
constexpr const char *bold_cyan = "\033[1;36m";
constexpr const char *bold_white = "\033[1;37m";

constexpr const char *dim_green = "\033[2;32m";
constexpr const char *dim_blue = "\033[2;34m";

constexpr const char *inverse = "\033[7m";

// Aliases for convenience
constexpr const char *gray = bright_black;
constexpr const char *grey = bright_black;
constexpr const char *orange = yellow; // Closest approximation
constexpr const char *purple = magenta;
} // namespace color

enum class level_color_variant { classic, vivid, monochrome };

struct theme {
  const char *timestamp_color = color::gray;
  const char *logger_color = color::bold_cyan;
  const char *message_color = nullptr;
  const char *bracket_color = color::gray;
  level_color_variant level_colors = level_color_variant::classic;

  // Predefined themes
  static const theme classic;
  static const theme ocean;
  static const theme sunset;
  static const theme matrix;
  static const theme gray_theme;
  static const theme forest;
  static const theme desert;
  static const theme night;
  static const theme solarized;
  static const theme minimal;
  static const theme vibrant;

  void apply_level_colors(
      const std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> &sink) const {
    switch (level_colors) {
    case level_color_variant::classic:
      sink->set_color(spdlog::level::trace, sink->cyan);
      sink->set_color(spdlog::level::debug, sink->blue);
      sink->set_color(spdlog::level::info, sink->green);
      sink->set_color(spdlog::level::warn, sink->yellow_bold);
      sink->set_color(spdlog::level::err, sink->red_bold);
      sink->set_color(spdlog::level::critical, sink->bold_on_red);
      break;
    case level_color_variant::vivid:
      sink->set_color(spdlog::level::trace, color::bright_cyan);
      sink->set_color(spdlog::level::debug, color::bright_blue);
      sink->set_color(spdlog::level::info, color::bright_green);
      sink->set_color(spdlog::level::warn, color::bold_yellow);
      sink->set_color(spdlog::level::err, color::bold_red);
      sink->set_color(spdlog::level::critical,
                      std::string(color::bold_white) + color::bg_red);
      break;
    case level_color_variant::monochrome:
      sink->set_color(spdlog::level::trace, color::bright_black);
      sink->set_color(spdlog::level::debug, color::gray);
      sink->set_color(spdlog::level::info, color::white);
      sink->set_color(spdlog::level::warn, color::bold_white);
      sink->set_color(spdlog::level::err, color::bold_white);
      sink->set_color(spdlog::level::critical,
                      std::string(color::bold_white) + color::inverse);
      break;
    }
  }

  std::string console_pattern() const {
    std::string p;
    // Wrap text with color and reset to avoid style bleed across fields.
    // Some terminals may render underline or bright colors differently; reset is needed
    // to keep each segment consistent.
    auto wrap = [&](const char *color, const std::string &text) -> std::string {
      if (!color)
        return text;
      return std::string(color) + text + color::reset;
    };
    const char *brk = bracket_color ? bracket_color : "";
    const std::string brk_reset = bracket_color ? color::reset : "";

    p += wrap(timestamp_color, std::string(brk) + "[" + brk_reset);
    p += wrap(timestamp_color, "%Y-%m-%d %H:%M:%S.%e");
    p += wrap(timestamp_color, std::string(brk) + "]" + brk_reset);
    p += " ";
    p += wrap(logger_color, std::string(brk) + "[" + brk_reset);
    p += wrap(logger_color, "%n");
    p += wrap(logger_color, std::string(brk) + "]" + brk_reset);
    p += " ";
    p += std::string(brk) + "[" + brk_reset + "%^%l%$" + brk + "]" + brk_reset;
    p += " ";
    p += wrap(message_color, "%v");
    return p;
  }
};

class config {
public:
  config &log_file_path(const std::string &log_file_path) {
    this->log_file_path_ = log_file_path;
    return *this;
  }
  config &log_level(spdlog::level::level_enum log_level) {
    this->log_level_ = log_level;
    return *this;
  }
  config &max_file_size(size_t max_file_size) {
    this->max_file_size_ = max_file_size;
    return *this;
  }
  config &max_files(size_t max_files) {
    this->max_files_ = max_files;
    return *this;
  }
  config &theme(const theme &log_theme) {
    this->log_theme_ = log_theme;
    return *this;
  } 
  config &file_pattern(const std::string &file_pattern) {
    this->file_pattern_ = file_pattern;
    return *this;
  }
  config &console_pattern(const std::string &console_pattern) {
    this->console_pattern_ = console_pattern;
    return *this;
  }
  config &prefix(const std::string &prefix) {
    this->prefix_ = prefix;
    return *this;
  }
  config &set_as_default(bool set_default) {
    this->set_as_default_ = set_default;
    return *this;
  }
  void initialize() {
    try {
      auto console_sink =
          std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      console_sink->set_level(log_level_);
      console_sink->set_pattern(
          console_pattern_.empty() ? log_theme_.console_pattern() : console_pattern_);
      log_theme_.apply_level_colors(console_sink);
      console_sink->set_color_mode(spdlog::color_mode::always);

      auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          log_file_path_, max_file_size_, max_files_);
      file_sink->set_level(log_level_);
      file_sink->set_pattern(file_pattern_);

      std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
      auto logger =
          std::make_shared<spdlog::logger>(prefix_, sinks.begin(), sinks.end());
      logger->set_level(log_level_);
      logger->flush_on(spdlog::level::warn);

      spdlog::register_logger(logger);
      if (set_as_default_) {
        spdlog::set_default_logger(logger);
      }

    } catch (const spdlog::spdlog_ex &ex) {
      std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
  }

  static void flush() { spdlog::default_logger()->flush(); }

private:
  std::string log_file_path_ = "rune.log";
  spdlog::level::level_enum log_level_ = spdlog::level::info;
  size_t max_file_size_ = 1048576 * 10;
  size_t max_files_ = 5;
  rune::theme log_theme_ = theme::classic;
  std::string file_pattern_ = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v";
  std::string console_pattern_; // if empty, use theme default
  std::string prefix_ = "rune";
  bool set_as_default_ = true;
};

// Theme definitions
const theme theme::classic = {};

const theme theme::ocean = {.timestamp_color = color::bright_blue,
                            .logger_color = color::bold_cyan,
                            .message_color = color::bright_white,
                            .bracket_color = color::cyan,
                            .level_colors = level_color_variant::classic};

const theme theme::sunset = {.timestamp_color = color::bright_yellow,
                             .logger_color = color::bold_magenta,
                             .message_color = color::bright_white,
                             .bracket_color = color::bright_red,
                             .level_colors = level_color_variant::vivid};

const theme theme::matrix = {.timestamp_color = color::green,
                             .logger_color = color::bold_green,
                             .message_color = color::bright_green,
                             .bracket_color = color::green,
                             .level_colors = level_color_variant::monochrome};

const theme theme::gray_theme = {.timestamp_color = color::bright_black,
                                 .logger_color = color::white,
                                 .message_color = color::bright_white,
                                 .bracket_color = color::bright_black,
                                 .level_colors =
                                     level_color_variant::monochrome};

const theme theme::forest = {.timestamp_color = color::green,
                             .logger_color = color::bold_green,
                             .message_color = color::bright_white,
                             .bracket_color = color::dim_green,
                             .level_colors = level_color_variant::classic};

const theme theme::desert = {.timestamp_color = color::yellow,
                             .logger_color = color::bold_yellow,
                             .message_color = color::bright_white,
                             .bracket_color = color::orange,
                             .level_colors = level_color_variant::vivid};

const theme theme::night = {.timestamp_color = color::bright_black,
                            .logger_color = color::bright_blue,
                            .message_color = color::bright_white,
                            .bracket_color = color::dim_blue,
                            .level_colors = level_color_variant::monochrome};

const theme theme::solarized = {.timestamp_color = color::bright_yellow,
                                .logger_color = color::bright_blue,
                                .message_color = color::bright_white,
                                .bracket_color = color::bright_cyan,
                                .level_colors = level_color_variant::classic};

const theme theme::minimal = {.timestamp_color = color::gray,
                              .logger_color = color::white,
                              .message_color = nullptr,
                              .bracket_color = color::gray,
                              .level_colors = level_color_variant::monochrome};

const theme theme::vibrant = {.timestamp_color = color::bright_magenta,
                              .logger_color = color::bold_cyan,
                              .message_color = color::bright_white,
                              .bracket_color = color::bright_red,
                              .level_colors = level_color_variant::vivid};

} // namespace rune

#define RUNE_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define RUNE_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define RUNE_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define RUNE_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define RUNE_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define RUNE_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#endif // RUNE_LOGGER_H
