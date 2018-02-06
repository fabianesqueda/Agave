# Agave

This is a collection of free open-source modules for VCV Rack. This is an ongoing project, so the number of modules is likely to grow rapidly over the following weeks!

For the time being, these modules are only available via this repository. They haven't been packaged for distribution yet. Instructions on how to load these modules into your VCV Rack development environment can be found [here](https://github.com/VCVRack/Rack) under "Building plugins".

### FΛLDER

This module implements a "wavefolding" operation on the incoming signal. Wavefolding is a operation in which signal values that exceed certain treshold are inverted or folded back (hence the name of the effect). When driven by a signal with low harmonic content (e.g. a sinusoid or triangular oscillator) this module generates complex harmonically-rich waveforms, making it ideal for a West Coast-style patch.

### METALLIC

This module generates "metallic" or "pitched" noise inspired by the multi-oscillator arrays used in the Roland TR-808 and TR-606 drum machines to synthesize cymbal and hi-hat sounds. A great reference on cymbal synthesis using metallic noise can be found [here](https://web.archive.org/web/20160403120912/http://www.soundonsound.com/sos/Jul02/articles/synthsecrets0702.asp).

### LPF Bank

This module implements a passive RC filter bank ideal for separating wideband signals, such as noise, into sub-bands. Perfect for percussive patches.

-Fabian Esqueda
01/2018
