<h1>CrossoverPhase VST</h1>

<h3>What it does</h3>

It creates phase dispersion and leads to a less phase-coherent sound. It gives a laser zap sound to soften transients.
To achieve this, it does band splitting repeatedly through low pass and high pass filters, with both bands summed back together after every iteration, thus creating a giant phase shift.

This plugin uses 4th order Linkwitz-Riley filters, commonly used in multiband processors due to its nature of retaining the spectral content of the audio (they sum back to all pass). Usually the phase shift associated with multiband processing is undesirable, but in this case, this characteristic is abused for the effect.

<h3>Knobs</h3>

- Frequency: controls the cutoff of the filters. It affects the frequencies around the cutoff the most.
- Intensity: controls how many times the audio is run through the filters. The higher the number, the more CPU usage.
- Volume: self explanatory
- Mix: dry/wet mix, can be used to create phaser type effects
