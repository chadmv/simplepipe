[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/nlohmann/json/master/LICENSE.MIT)

# simplepipe
Simple pipeline tools for small game productions.  Pipeline production tools I created for my own personal projects and learning.

## Features
* Consistent project and asset folder creation

That's all for now :)

## Why C++?
You typically don't see a lot of game production pipeline tools written in C++.  They are usually written in Python or C#.  I wanted to try something new and have executables that are easier to run without having to deal with Python environments/libraries or .NET redistributables.

## Build Instructions

```sh
$ cd /path/to/simplepipe
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release
```

## Usage
```sh
  simplepipe.exe [OPTION...]

  -h, --help          Print help
  -v, --version       Print version
  -p, --project arg   Project name
  -a, --asset arg     Asset name
  -s, --settings arg  Settings file path
```

1. Build and add simplepipe[.exe] somewhere in your PATH.
2. Save a json file describing the desired settings.
3. Set environment variable SIMPLEPIPE_SETTINGS_FILE to point to the json file (or use -s flag in command)
4. Run:

```sh
$ simplepipe.exe -p ProjectName -a Characters/Chad,Characters/Wiley
```

Using the sample settings json described below, results in folders:
```
ProjectName
|--Characters
   |--Chad
      |--Animations
         |--Export
         |--Work
            |--Maya
            |--Mobu
      |--Models
         |--Export
         |--Work
            |--Marvelous
            |--Maya
            |--Substance
            |--Zbrush
   |--Wiley
   ...
```

## Settings file
Sample settings file:
```json
{
    "project_root": "D:/projects",
    "asset_categories": {
        "Characters": {},
        "Environments": {
            "Jungle": {},
            "Snow": {}
        },
        "Props": {
            "Weapons": {},
            "Items": {}
        },
        "Vehicles": {}
    },
    "asset_template": {
        "Models": {
            "Work": {
                "Maya": {},
                "Substance": {},
                "Zbrush": {},
                "Marvelous": {}
            },
            "Export": {
            }
        },
        "Animations": {
            "Work": {
                "Maya": {},
                "Mobu": {}
            },
            "Export": {
            }
        }
    }
}
```
* project_root - The directory to create projects in
* asset_categories - Valid asset categories to allow in the project
* asset_templates - Default directories to create in each new asset folder
