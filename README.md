# dronehome
This was originally intended to be the start of a raspberry pi drone projct, but just became the start of a camera calibration pipeline.

# Build instructions

In linux, from the root dir:

`mkdir build; cmake -H. -Bbuild && cmake --build build --config Release -- -j16`

If you're on OSX or Windows, the instructions should be similar though you may need to specify a different generator. I mostly tried to keep this all portable but this has only been tested in Linux, so your mileage may vary.

# Execution instructions

The `raytracer` binary takes one argument: a path to a camera configuration json file. You can find a default file in the config path, which has parameters intended to characterize a raspberry pi camera.
