#BLUI (Blue Eye)
####HTML Powered UI for Unreal Engine 4
---------------------------------------
Currently under construction, not ready for use just yet!

##Features
----------
Under construction!

##Installation
-------------
Clone BLUI Plugin to either UE4/Engine/Plugins or Project/Plugins.
```
git clone git@github.com:aaron524/BLUI.git
```

Download the Windows 64bit CEF3 build from http://cefbuilds.com and place the contents under ThirdParty/cef so that the structure is as follows.
```
BLUI/ThirdParty/cef/include
```

To force the plugin be linked to your project on shipping build you should add it to the PrivateDependencyModuleNames section of YourProject.Build.cs.

##Contributing
--------------
We're using the normal Epic Games coding Standard
You can find it here: https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/index.html

##Technology
------------
<img src="http://www.w3.org/html/logo/downloads/HTML5_Logo_512.png" width="128px" height="128px" />
<img src="http://i.imgur.com/klDp42x.png" width="128px" height="128px" />
<img src="https://raw.githubusercontent.com/svn2github/cef3/master/tests/cefclient/res/logo.png" width="168px" height="168px"/>

##License and Credits
---------------------
[Third Party Licenses and Credits](CREDITS.md)
```
The MIT License (MIT)

Copyright (c) <year> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```