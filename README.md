<h1>CrossoverPhase VST</h1>

<h3>What it does</h3>

It creates phase dispersion and leads to a less phase-coherent sound. It gives a laser zap sound to soften transients.
To achieve this, it does band splitting repeatedly through low pass and high pass filters, with both bands summed back together after every iteration, thus creating a giant phase shift.

This plugin uses 4th order Linkwitz-Riley filters, commonly used in multiband processors due to its nature of retaining the spectral content of the audio (they sum back to all pass). Usually the phase shift associated with multiband processing is undesirable, but in this case, this characteristic is abused for the effect.

<h3>Knobs</h3>

- Frequency: controls the cutoff of the filters. It affects the frequencies around the cutoff the most.
- Intensity: controls how many times the audio is run through the filters. The higher the number, the more CPU usage.
- Volume: self explanatory
- Mix: dry/wet mix, can be used to create phaser type effects.

<h3>Installation</h3>

Always check your downloads at virustotal.com to make sure they have not been compromised.

<h4>Windows</h4>

Simply copy the provided `CrossoverPhase.dll` to your preferred VST installation location.

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