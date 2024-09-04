# PDFViewer
This is an Unreal 5.3 project that uses PDFium binaries to read PDF and show it on widget in runtime. 
* Currently it is supported for Windows 64 bit operating sytem.
* It reads the PDF and converts to bitmap using PDFium library. Then the bitmap is converted to unreal texture to display it on widget.
* In case if you want to use the plugin in any other unreal version, create your unreal project in C++, copy plugins folder from this repository to your project. Then open your project root folder in Visual studio or VS Code and build the project. Once it is built, you can open the project. In case if any error occres while building, please solve it.  


The plugin is c++ based that uses PDFium precompiled binaries. Please compile the project for the first time and then use it. 
The PDFium binaries can be found: https://github.com/bblanchon/pdfium-binaries
<br>
<br>
<br>

![Alt text](Docs/Capture.PNG?raw=true "Title")
