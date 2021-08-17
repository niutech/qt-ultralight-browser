Qt Ultralight Browser
=====================

This is an ultra-lightweight web browser powered by the [Ultralight](https://ultralig.ht/) web engine embedded in [Qt 5](https://www.qt.io/) app as a custom webview widget - **QUltralight (Qt Ultralight)** - a lightweight replacement for Qt WebKit. The project is at very early stage, so it may be unstable.

Here is the screenshot:

![Qt Ultralight Browser](https://i.imgur.com/xBSjyXL.png)

Qt Ultralight Browser is very lightweight, it uses only 68 MB of RAM after loading the homepage (DuckDuckGo) and it provides GPU rendering on supported hardware:

![Qt Ultralight Browser RAM usage](https://i.imgur.com/bN8gktT.png)

## Download

You can download the latest binary for Windows in the [releases](https://github.com/niutech/qt-ultralight-browser/releases). Keep in mind, this is an alfa version, so use at your own risk!

## Build

This project is being developed on Windows 10 using Qt 5.14, MSVC 2017 and qmake, but it should also run on Linux and Mac OS, provided that you have downloaded the lastest [SDK](https://github.com/ultralight-ux/Ultralight#getting-the-latest-sdk) for your platform. Extract it to `../ultralight` folder relative to `qt-ultralight-browser`, open this project in Qt Creator and run it. Check that the required Ultralight DLL libraries were copied to the build destination.

## Contribute

Contributions are welcome. Let's meet in #app-development channel on [Ultralight Discord](https://discord.gg/3vzUGbaAdp).

## License

Qt Ultralight Browser &copy; 2021 Jerzy Głowacki under [LGPL 3.0](https://www.gnu.org/licenses/lgpl-3.0.html).

Ultralight &copy; 2021 Ultralight Inc. under [Ultralight Free License Agreement](https://github.com/ultralight-ux/Ultralight/blob/master/license/LICENSE.txt).
