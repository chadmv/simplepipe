#include <cstdlib>
#include "SimplePipe.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class ProjectTest : public ::testing::Test {
 protected:
  void SetUp() override {
    temp_dir_ = fs::temp_directory_path() / "simplepipetests";
    settings_.project_root = temp_dir_.string();
    // clang-format off
    settings_.asset_template = {
        {"Models", {
          {"Work", {
            {"Maya", nullptr},
            {"Substance", nullptr},
            {"Zbrush", nullptr},
            {"Marvelous", nullptr}}},
          {"Export", nullptr}}},
        {"Animation", {
          {"Work", {
            {"Maya", nullptr},
            {"Mobu", nullptr}}},
          {"Export", nullptr}}}};
    settings_.asset_categories = {
        {"Characters", nullptr},
        {"Environments", {
          {"Jungle", nullptr},
          {"Snow", nullptr}}},
        {"Props", {
          {"Weapons", nullptr},
          {"Items", nullptr}}},
        {"Vehicles", nullptr},
    };
    // clang-format on
  }

  void TearDown() override {
    if (fs::exists(temp_dir_)) {
      fs::remove_all(temp_dir_);
    }
  }

  Settings settings_;
  fs::path temp_dir_;
};

TEST_F(ProjectTest, ProjectPathIsCorrect) {
  Project project("testproject", settings_);
  fs::path expected = settings_.project_root;
  expected /= project.name();
  ASSERT_EQ(expected, project.path());
}

TEST_F(ProjectTest, CreateProjectCreatesDirectories) {
  Project project("testproject", settings_);
  project.create_project();
  ASSERT_TRUE(fs::exists(project.path()));
  project.delete_project();
  ASSERT_FALSE(fs::exists(project.path()));
}

TEST_F(ProjectTest, CreateAsset) {
  Project project("testproject", settings_);
  project.create_project();
  Asset asset = project.create_asset("testasset");
  fs::path expected = project.path() / asset.name();
  ASSERT_EQ(expected, asset.path());
  ASSERT_TRUE(fs::exists(asset.path()));
  asset.delete_asset();
  ASSERT_FALSE(fs::exists(asset.path()));
  project.delete_project();
}

TEST_F(ProjectTest, CreateAssetWithCategory) {
  Project project("testproject", settings_);
  project.create_project();
  Asset asset = project.create_asset("Characters/testasset");
  fs::path expected = project.path() / "Characters" / "testasset";
  ASSERT_EQ(expected, asset.path());
  ASSERT_TRUE(fs::exists(asset.path()));
  project.delete_project();
}

TEST_F(ProjectTest, CreateAssetWithSubCategory) {
  Project project("testproject", settings_);
  project.create_project();
  Asset asset = project.create_asset("Environments/Jungle/leafygreens");
  fs::path expected = project.path() / "Environments" / "Jungle" / "leafygreens";
  ASSERT_EQ(expected, asset.path());
  ASSERT_TRUE(fs::exists(asset.path()));
  project.delete_project();
}

TEST_F(ProjectTest, CreateAssetContainsAssetTemplate) {
  Project project("testproject", settings_);
  project.create_project();
  Asset asset = project.create_asset("testasset");
  EXPECT_TRUE(fs::exists(asset.path() / "models"));
  EXPECT_TRUE(fs::exists(asset.path() / "models" / "work"));
  EXPECT_TRUE(fs::exists(asset.path() / "models" / "work" / "maya"));
  EXPECT_TRUE(fs::exists(asset.path() / "models" / "export"));
  EXPECT_TRUE(fs::exists(asset.path() / "animation"));
  EXPECT_TRUE(fs::exists(asset.path() / "animation" / "export"));
  project.delete_project();
}

TEST_F(ProjectTest, CategoryValidationWorks) {
  Project project("testproject", settings_);
  std::vector<std::string> category;
  EXPECT_TRUE(project.category_is_valid(category));
  category.push_back("Characters");
  EXPECT_TRUE(project.category_is_valid(category));
  category.push_back("InvalidSubcategory");
  EXPECT_FALSE(project.category_is_valid(category));
  category.clear();
  category.push_back("Environments");
  EXPECT_TRUE(project.category_is_valid(category));
  category.push_back("Jungle");
  EXPECT_TRUE(project.category_is_valid(category));
  category.push_back("InvalidSubcategory");
  EXPECT_FALSE(project.category_is_valid(category));
  project.delete_project();
}

TEST_F(ProjectTest, CreateAssetWithInvalidCategoryDoesNotCreateDirectories) {
  Project project("testproject", settings_);
  project.create_project();
  Asset asset = project.create_asset("invalidcategory/testasset");
  ASSERT_FALSE(fs::exists(asset.path()));
  project.delete_project();
}
