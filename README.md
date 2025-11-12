# noHEIC

## Overview

noHEIC is a simple command line tool to convert .heic images into a .jpeg
representation. This project was created for Olin College's Software Systems
course.

## Installation
These installation instructions are for an Ubuntu system. Assumes the user already has *cmake*, *make*, and *gcc* installed.

1. Install the necessary libraries for the program:

```bash
sudo apt install libheif-dev
sudo apt install libjpeg-dev
```

2. Clone the noHEIC repo onto your local machine. Move into the directory.

```bash
git clone git@github.com:olincollege/noHEIC.git
cd noHEIC
```

3. Build the program

```bash
mkdir build
cd build/
cmake ..
make
```

## Using noHEIC

Run the program with the following, replacing YOUR_FILE_PATH with the absolute file path to the .heic image, not including the extension.

```bash
./src/noHEIC YOUR_FILE_PATH
```

The new .jpeg file will be created in the same directory as the original image.

## Resources

- [libheif docs](https://deepwiki.com/strukturag/libheif/5-c-api-reference)
- [ligjpeg-turbo docs](https://libjpeg-turbo.org/Documentation/Documentation)
