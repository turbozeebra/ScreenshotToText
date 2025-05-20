# ScreenshotToText

A simple utility to take a screenshot, select an area, and copy the detected text to your clipboard using OCR.

### Features ✨
1. Take a full-screen screenshot and display it
1. Select a region of interest from the schreenshot with your mouse
1. Detect and extract text from the selected area (OCR)
1. Copy the extracted text to your clipboard

### Demo ✨
![](demo.gif)

### Compilation 
If you want to try compiling the program you should install the dependencies first:

1. install GUI library [wxwidgets](https://wxwidgets.org/)
1. install OCR [tesseract](https://github.com/tesseract-ocr/tesseract)
1. install xclip (used to copy the output of the program to the "clipboard") 
``` 
sudo apt update
sudo apt install xclip 
```

Now you are ready to compile:
``` ./scripts/build.sh ```
and run the program:
``` ./scripts/run.sh ```