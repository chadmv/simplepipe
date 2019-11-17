#include <experimental/filesystem>
#include <string>
#include <vector>
#include <json.hpp>
#include "SimplePipeConfig.h"
namespace fs = std::experimental::filesystem;
using json = nlohmann::json;
class Asset;



struct Settings {
  static std::unique_ptr<Settings> create_from_env();
  static std::unique_ptr<Settings> create_from_file(const fs::path& path);

  fs::path project_root;
  json asset_template;
  json asset_categories;
};

class Project {
 public:
  Project(const std::string& name);
  Project(const std::string& name, const Settings& settings);

  void create_project();
  void delete_project();
  Asset create_asset(const std::string& name);
  void create_template_directory(const fs::path& root, const json& values);
  bool category_is_valid(const std::vector<std::string>& category) const;

  const std::string& name() const { return name_; }
  const Settings& settings() const { return settings_; }
  fs::path path() const;

 private:
  Settings settings_;
  std::string name_;
};

class Asset {
 public:
  Asset(const std::string& name, Project* project);

  const std::string& name() const { return name_; }
  const std::vector<std::string>& category() {return category_;}
  void create_asset();
  void delete_asset();
  fs::path path() const;

 private:
  std::string name_;
  Project* project_;
  std::vector<std::string> category_;
};