Documentation
=============================

The `Neocities` class provides methods to interact with the Neocities API. 

### Setup

Clone the repo into your working directory

```cpp
#include "neocitiescpp/neocities.h"
// Add the neocities object with your username and password
Neocities site("username", "password");
```

### Upload

Uploading files with the same name as files that already exist on neocities will be rewritten so be careful.

```cpp
// Upload files
std::vector<std::pair<std::string, std::string>> files = {
    {"index.html", "/path/to/index.html"},
    {"style.css", "/path/to/style.css"},
    {"script.js", "/path/to/script.js"}
};

bool uploadSuccess = site.upload(files);
```

### Delete

```cpp
// Delete files
std::vector<std::string> filenames = {
    "oldfile.html",
    "obsolete.css"
};

bool deleteSuccess = site.deleteFiles(filenames);
```

### Get site info

```cpp
// Get site information
std::string siteInfo = site.getInfo("exampleSite");
```

### List files

```cpp
// List all files on the site
std::string allFiles = site.listFiles();
std::cout << "All Files: " << allFiles << std::endl;

// List files in a specific directory (e.g., "images")
std::string pathFiles = site.listFiles("images");
std::cout << "Files in 'images' directory: " << pathFiles << std::endl;
```
```
