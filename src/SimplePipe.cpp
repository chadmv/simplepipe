#include "SimplePipe.h"
#include <SimplePipeConfig.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::unique_ptr<Settings> Settings::create_from_env() {
  char* file_path = std::getenv("SIMPLEPIPE_SETTINGS_FILE");
  if (!file_path) {
    std::cout << "Error - SIMPLEPIPE_SETTINGS_FILE variable is not set." << std::endl;
    return nullptr;
  }
  fs::path path = file_path;
  return Settings::create_from_file(path);
}

std::unique_ptr<Settings> Settings::create_from_file(const fs::path& path) {
  if (!fs::exists(path)) {
    std::cout << "Error - Settings file " << path << " does not exist." << std::endl;
    return nullptr;
  }
  std::ifstream template_file(path.c_str());
  json j;
  template_file >> j;

  std::unique_ptr<Settings> settings(new Settings);
  if (j.count("project_root")) {
    std::string project_root = j["project_root"];
    settings->project_root = project_root;
  }
  if (j.count("asset_categories")) {
    settings->asset_categories = j["asset_categories"];
  }
  if (j.count("asset_template")) {
    settings->asset_template = j["asset_template"];
  }
  return settings;
}


Project::Project(const std::string& name) : name_(name) {}

Project::Project(const std::string& name, const Settings& settings)
    : name_(name), settings_(settings) {}

void Project::create_project() {
  if (fs::exists(path())) {
    // Already created
    return;
  }
  std::cout << "Creating project: " << path() << std::endl;
  fs::create_directories(path());
}

void Project::delete_project() {
  if (fs::exists(path())) {
    fs::remove_all(path());
  }
}

Asset Project::create_asset(const std::string& name) {
  Asset asset(name, this);

  asset.create_asset();

  return asset;
}

void Project::create_template_directory(const fs::path& root, const json& values) {
  if (!fs::exists(root)) {
    fs::create_directories(root);
  }
  for (auto& [key, value] : values.items()) {
    fs::path subdirectory = root / key;
    create_template_directory(subdirectory, value);
  }
}

bool Project::category_is_valid(const std::vector<std::string>& category) const {
  json valid_categories = settings_.asset_categories;
  for (auto& c : category) {
    if (valid_categories.count(c.c_str()) == 0) {
      return false;
    }
    valid_categories = valid_categories[c.c_str()];
  }
  return true;
}

fs::path Project::path() const {
  fs::path path = settings_.project_root;
  path /= name_;
  return path;
}

void SplitString(const std::string& str, char delim, std::vector<std::string>& tokens) {
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim)) {
    tokens.push_back(token);
  }
}

bool CategoryIsValid(const std::vector<std::string>& category) {
  if (category.size() == 0) {
    return true;
  }
  return true;
}

Asset::Asset(const std::string& name, Project* project) {
  std::vector<std::string> tokens;
  SplitString(name, '/', tokens);
  name_ = tokens.back();
  project_ = project;
  if (tokens.size() > 1) {
    category_ = std::vector<std::string>(tokens.begin(), tokens.end() - 1);
  }
}

void Asset::create_asset() {
  if (fs::exists(path())) {
    // Already created
    return;
  }
  if (!project_->category_is_valid(category_)) {
    std::cout << "Invalid asset category ";
    for (auto& c : category_) {
      std::cout << c << "/";
    }
    std::cout << std::endl;
    return;
  }
  std::cout << "Creating asset: " << path() << std::endl;
  fs::create_directories(path());
  project_->create_template_directory(path(), project_->settings().asset_template);
}

void Asset::delete_asset() {
  if (fs::exists(path())) {
    fs::remove_all(path());
  }
}

fs::path Asset::path() const {
  fs::path path = project_->path();
  for (auto& c : category_) {
    path /= c;
  }
  path /= name_;
  return path;
}
