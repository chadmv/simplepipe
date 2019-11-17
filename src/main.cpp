#include <fstream>
#include <iostream>
#include "SimplePipe.h"
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {
  cxxopts::Options options(argv[0], "Create project and asset directories from a template.");
  options.allow_unrecognised_options();

  // clang-format off
  options.add_options()
    ("h,help", "Print help")
    ("v,version", "Print version")
    ("p,project", "Project name", cxxopts::value<std::string>())
    ("a,asset", "Asset name", cxxopts::value<std::vector<std::string>>())
    ("s,settings", "Settings file path", cxxopts::value<std::string>());
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result.count("version")) {
    std::cout << "SimplePipe v" << SIMPLEPIPE_VERSION_MAJOR << "." << SIMPLEPIPE_VERSION_MINOR
              << std::endl;
    exit(0);
  }

  if (result.count("help") || result.count("project") == 0) {
    std::cout << options.help({""}) << std::endl;
    exit(0);
  }

  // Check for settings template file
  std::unique_ptr<Settings> settings;
  if (result.count("settings")) {
    fs::path path = result["settings"].as<std::string>();
    settings = Settings::create_from_file(path);
  } else {
    // Check for template file specified by env var
    settings = Settings::create_from_env();
  }
  if (!settings) {
    exit(0);
  }

  Project project(result["project"].as<std::string>(), *settings);
  project.create_project();

  std::vector<std::string> assets = result["asset"].as<std::vector<std::string>>();
  for (auto& a : assets) {
    project.create_asset(a);
  }

  return 0;
}
