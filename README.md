[![release](https://img.shields.io/github/release/AaronShea/BLUI.svg?style=flat-square)](https://github.com/AaronShea/BLUI/releases)
![BLUI-logo](https://cloud.githubusercontent.com/assets/1334174/5969395/201a1202-a7f1-11e4-98a4-12bc6793f830.png)

## HTML powered UI and HUD for Unreal Engine 4
(Hit up the wiki for a quick start guide!)

Do you use BLUI in your project? I'd really love it if you credit me and it! You can even use the [logo right here!](https://res.cloudinary.com/aaronshea/image/upload/v1423576170/BLUI-Transparent_eu582n.png)

License: MIT

What is it?
---------------------------------------
BLUI is an Unreal Engine 4 plugin that allows easy interaction with the Chromium Embedded Framework. It provides a simple Material Instance and input functions to help streamline the rendering of rich HTML interfaces.

BLUI tries to stay out of the way as much as possible. All rendering of the DOM and processing of the JavaScript happens in a separate process, just like Chromium. BLUI only updates the texture inside the material instance when Chromium requests a redraw, not every tick, saving a bit more processing along the way.

Features
---------------------------------------
+ Chromium Powered (same thing that powers Google Chrome!)
+ Fully compatible with every web technology that Chrome/Chromium works with. (HTML5, WebAudio, WebSockets etc.)
+ No specific ties to ***any*** in game class, simple use Blueprints (or C++) to create a new "BluEye" object and grab its material instance, then you can paint it on anything!
+ Execute JavaScript in the "browser" from your game to pass data to the web page
+ Using `blu_event` JS native function you can pass data from the page's JavaScript back into UE4!
+ C++ or Blueprints, works with both!

Setting up the editor and project
---------------------------------------
**Copy** the files located inside the `shipping` folder to your Unreal Engine binaries folder (such as `C:\Program Files\Unreal Engine\4.6\Engine\Binaries\Win64`)

Then copy the `BLUI` folder into the "Plugins" folder within your project directory, and enable the plugin by adding:

```
PublicDependencyModuleNames.AddRange(new string[] { "Blu" });
```

To YouProject.Build.cs


Re-generate your project's Visual Studio file and load up the editor. Then check the plugin list to ensure it has been loaded!

Updating the CEF event loop
---------------------------------------
~~You must call the `BluManager::doBluMessageLoop();` method every tick in order for the CEF process to update properly. To do this, override the default GameMode's Tick function and include the `Blu/Public/BluManager.h` header file. You can then call the appropriate method.~~

As of now, you no longer need to do this. You can simply call a blueprint node to Tick the CEF loop every tick of the level blueprint.


Loading Local Files
---------------------------------------
Set your default URL or use the "Load URL" node/method to load a URL that starts with `blui://` this will point to the directory root of the project or the game (if packaged). So if you wanted to load an HTML file from `YourProject/UI/file.html`, set the URL to `blui://UI/file.html`


HUD Example Blueprint
---------------------------------------
Within the release, you'll find an ExampleHUD blueprint file, place this into your project's blueprints directory to try it out! (It's a simple UMG widget pre-configures to accept keyboard and mouse input, with a BluEye instance hooked up to a canvas)

[A small demo project to get started!](http://bit.ly/1ODRiTO)

Shipping Your Game (Windows + Linux)
---------------------------------------
Copy all contents of `{Windows/Linux} Shipping` into your packaged game's `GameName/Binaries/{Win64/Linux}`, these are the required files for the Chromium process.

Shipping Your Game (OSX)
---------------------------------------
Copy `ffmpegsumo.so` and `Chromium Embedded Framework`  from `Mac Shipping`  into your game's app `Contents/MacOS` folder. Then copy all content from `Resources` in `Mac Shipping` into your game's app `Contents/Resources`

Then simply copy `blu_ue4_process.app` next to your game app, and ship!

[Video for Shipping on OSX](http://youtu.be/yMkzinUhN2s)

Videos for a little more help!
---------------------------------------
[![YoutubeVideo](http://img.youtube.com/vi/VCPhsxd5rTE/0.jpg)](https://www.youtube.com/watch?v=VCPhsxd5rTE)
