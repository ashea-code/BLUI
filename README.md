![BLUI-logo](https://cloud.githubusercontent.com/assets/1334174/5969395/201a1202-a7f1-11e4-98a4-12bc6793f830.png)

## HTML powered UI and HUD for Unreal Engine 4

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
