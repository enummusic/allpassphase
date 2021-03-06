<h1>AllPassPhase VST</h1>

<h3>What it does</h3>

It creates phase dispersion and leads to a less phase-coherent sound.
It gives a laser zap sound to soften transients, or gives a unique character to bass sounds.
To achieve this, it repeatedly runs the audio through all pass filters, thus creating a giant phase shift.
Note: there is no custom GUI. The interface is provided by the host.

<h3>Knobs</h3>

- Frequency: Controls the frequency of the filters. It affects the frequencies around the value the most. 
Because of the nature of the filters, modulating the frequency knob too quickly is not recommended, 
even though I have attempted to prevent this from happening when the frequency is below 500hz.
- Q: The "pinch" of the filters. The smaller the Q, the more pronounced the dispersion near the frequency. 
Note that when Q<sub>knob</sub>=0, Q<sub>true</sub>=0.005.
- Intensity: Controls how many times the audio is run through the filters. The higher the number, the more CPU usage.
- Mix: Dry/wet mix, can be used to create phaser type effects.

<h3>Installation</h3>

Always check your downloads at virustotal.com to make sure they have not been compromised.

<h4>Windows</h4>

Simply copy the provided `AllPassPhase.dll` or `AllPassPhase_x64.dll` to your preferred VST installation location.

<h4>Mac</h4>

Copy the provided `allpassphase.vst` to `/Library/Audio/Plug-Ins/VST`.

<h3>Compiling from source</h3>

<h4>Windows</h4>

This assumes you are using Visual Studio and have the VST2 SDK (which I cannot include for copyright reasons)
1. You might need to install the Windows 8.1 SDK. Verify this by launching the installer (vsCommunity.exe), 
going to the "Individual components" tab, and making sure "Windows 8.1 SDK" is checked.
2. Make a copy of the provided aDelay folder under `public.sdk/samples/vst2.x` and paste it under `vst2.x`
(Under `vst2.x` you would have `aDelay` and `aDelay - Copy` or whatever you named it).
3. Remove the files `aDelay.cpp/h` and `surroundDelay.cpp/h`. `adelaymain.cpp` will be overwritten later.
4. Copy in all the source files from this repo, navigate to the `win` folder.
5. Launch the vcproj file. It will ask you if you want to make a one way upgrade, accept it.
6. Go to Project > Retarget solution, and set it to Windows 10.
7. In the Solution Explorer (usually on the right in Visual Studio), right click and select Properties. 
Make sure "Configuration Properties > General > Target Name" is set to CrossoverPhase 
(or leave it as $ProjectName if you renamed the project to that), 
and "Configuration Type" is set to "Dynamic Library (.dll)".
8. Build it. Your .dll file will be under `public.sdk/samples/vst2.x/aDelay - Copy/win/Debug`.


<h4>Mac</h4>

This assumes you are using XCode and have the VST2 SDK (which I cannot include for copyright reasons)
1. Make a copy of the provided aDelay folder under `public.sdk/samples/vst2.x` and paste it under `vst2.x` and rename to `allpassphase`
(Under `vst2.x` you would have `aDelay` and `allpassphase` or whatever you named it).
2. Remove the files `aDelay.cpp/h` and `surroundDelay.cpp/h`. `adelaymain.cpp` will be overwritten later.
3. Copy in all the source files from this repo, navigate to the `vstsdk2.4/public.sdk/samples/mac` folder.
4. Launch the xcodeproj file. In the bottom left of the window there should be a +, select Add Files to "vst2.4 examples", add the new `allpassphase` folder to it.
5. Double-click `vst2.4 examples` in the left browser, then in the Project/Target pane select Project > vst2.4 examples, and under Build Settings > Base SDK, select macOS. By default it uses a deprecated OSX SDK.
6. Right click `adelay` under Targets in the Project/Target pane, select Duplicate. Double click and rename the new copy to `allpassphase`. This affects the naming of the generated .vst file.
7. Go to Build Settings > Apple Clang - Language - C++ > C++ Language Dialect, change it to C++ 17.
8. Go back to the project browser, navigate to `allpassphase` and select all the .cpp files, and on the right pane under "Target Membership" select the "allpassphase" checkbox. Navigate to `adelay` and select the .cpp files and deselect "allpassphase".
9. Build it (make sure you are building the correct portion of the project, it might show up as adelay copy before you restart), and it will complain about a casting error from the (ancient) VST SDK code. Apply the recommended fix. (I couldn't find another way to compile without changing my own source code or the SDK's)
10. Build it again. Your .vst file can be found by going to the top bar, Product > Reveal Build Products Folder, under `Debug`.
