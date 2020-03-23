# dronehome
This was originally intended to be the start of a raspberry pi drone project, but just became a means to generate test data for a simple camera calibration pipeline. 

# Build instructions

In linux, from the root dir:

`mkdir build; cmake -H. -Bbuild && cmake --build build --config Release -- -j16`

If you're on OSX or Windows, the instructions should be similar though you may need to specify a different generator. I mostly tried to keep this all portable but this has only been tested in Linux, so your mileage may vary.

# Execution instructions

The `raytracer` binary takes one argument: a path to a camera configuration json file. You can find a default file in the `config` directory, which has parameters intended to characterize a raspberry pi camera.
