# EasyEDA scripts

- [EasyEDA scripts](#easyeda-scripts)
  - [Rename Script](#rename-script)
    - [Setup](#setup)
    - [Usage](#usage)


## Rename Script
This script is for automatically renaming the exported files from EasyEDA

The folder structure of the exported file is important as the script uses the path to rename the files

The exported files should be in `/PCB_Name/VERSION`. e.g `keypad_node-main/V1.0/`

### Setup

Copy `./rename_export/rename_export_easyEDA.desktop` to `~/.local/share/applications/rename_export_easyEDA.desktop`

Then make a shortcut of `rename_export_easyEDA.desktop` to the desktop, or just copy the .desktop file straight to the desktop

Make the launcher executable
`chmod +x ~/.local/share/applications/rename_export_easyEDA.desktop`

Modify the Exec line in the .desktop to point to the python script in `./rename_export/`

### Usage

- The exported files should be in `/PCB_Name/VERSION`. e.g `keypad_node-main/V1.0/`
- Run by dragging the version folder onto the .desktop 
- Can optionally commit to git by pressing y after files have been renamed