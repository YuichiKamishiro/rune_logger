#include <rune_logger.hpp>

int main() {
  rune::config()
      .theme(rune::theme::ocean)
      .prefix("ocean")
      .log_file_path("logs/ocean.log")
      .initialize();

  RUNE_INFO("Ocean theme: calm blue waters");
  RUNE_WARN("Ocean theme: stormy warning");

  rune::config()
      .theme(rune::theme::sunset)
      .prefix("sunset")
      .log_file_path("logs/sunset.log")
      .initialize();

  RUNE_INFO("Sunset theme: golden hour glow");
  RUNE_ERROR("Sunset theme: fiery error");

  rune::config()
      .theme(rune::theme::matrix)
      .prefix("matrix")
      .log_file_path("logs/matrix.log")
      .initialize();

  RUNE_DEBUG("Matrix theme: digital rain debug");
  RUNE_CRITICAL("Matrix theme: system breach");

  rune::theme custom_classic = rune::theme::classic;
  custom_classic.timestamp_color = rune::color::bright_black;
  custom_classic.logger_color = rune::color::bold_blue;
  custom_classic.message_color = rune::color::white;
  custom_classic.bracket_color = rune::color::gray;
  custom_classic.level_colors = rune::level_color_variant::classic;

  rune::config()
      .theme(custom_classic)
      .prefix("custom_classic")
      .log_file_path("logs/custom_classic.log")
      .initialize();

  RUNE_INFO("Custom classic: traditional styling");
  RUNE_WARN("Custom classic: standard warning");

  rune::theme custom_vivid = {};
  custom_vivid.timestamp_color = rune::color::bright_yellow;
  custom_vivid.logger_color = rune::color::bold_magenta;
  custom_vivid.message_color = rune::color::bright_white;
  custom_vivid.bracket_color = rune::color::bright_cyan;
  custom_vivid.level_colors = rune::level_color_variant::vivid;

  rune::config()
      .theme(custom_vivid)
      .prefix("custom_vivid")
      .log_file_path("logs/custom_vivid.log")
      .initialize();

  RUNE_INFO("Custom vivid: bright and bold");
  RUNE_ERROR("Custom vivid: striking error");

  rune::theme custom_mono = {};
  custom_mono.timestamp_color = rune::color::gray;
  custom_mono.logger_color = rune::color::white;
  custom_mono.message_color = nullptr;
  custom_mono.bracket_color = rune::color::bright_black;
  custom_mono.level_colors = rune::level_color_variant::monochrome;

  rune::config()
      .theme(custom_mono)
      .prefix("custom_mono")
      .log_file_path("logs/custom_mono.log")
      .initialize();

  RUNE_DEBUG("Custom monochrome: subtle grayscale");
  RUNE_CRITICAL("Custom monochrome: stark critical");

  rune::theme advanced_theme = {};
  advanced_theme.timestamp_color = rune::color::bright_white;
  advanced_theme.logger_color = rune::color::bold_yellow;
  advanced_theme.message_color = rune::color::bright_white;
  advanced_theme.bracket_color = rune::color::bright_red;
  advanced_theme.level_colors = rune::level_color_variant::vivid;

  rune::config()
      .theme(advanced_theme)
      .prefix("advanced")
      .log_file_path("logs/advanced.log")
      .log_level(spdlog::level::trace)
      .file_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] (%t) %v")
      .console_pattern("[%^%l%$] %H:%M:%S %n >> %v")
      .set_as_default(false)
      .initialize();

  auto advanced_logger = spdlog::get("advanced");
  advanced_logger->trace("Advanced: detailed trace with thread info");
  advanced_logger->debug("Advanced: debug with custom pattern");
  advanced_logger->info("Advanced: info with background brackets");
  advanced_logger->warn("Advanced: warning with vivid colors");
  advanced_logger->error("Advanced: error with bold styling");
  advanced_logger->critical("Advanced: critical with background");

  rune::theme final_theme = rune::theme::forest;
  final_theme.message_color = rune::color::bright_green;
  final_theme.bracket_color = rune::color::dim_green;

  rune::config()
      .theme(final_theme)
      .prefix("final_app")
      .log_file_path("logs/final_app.log")
      .console_pattern("[%Y-%m-%d %T] [%n] [%^%l%$] %v")
      .initialize();

  RUNE_INFO("Final theme: forest green elegance");

  rune::config::flush();

  std::cout << "All themes demonstrated. Check logs/ for file outputs." << std::endl;

  return 0;
}