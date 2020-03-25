# chessboardgen
This was originally intended to be the start of a raspberry pi drone project, but just became a means to generate test data for a simple camera calibration pipeline. 

# Build instructions

In linux, from the root dir:

`mkdir build; cmake -H. -Bbuild && cmake --build build --config Release -- -j16`

If you're on OSX or Windows, the instructions should be similar though you may need to specify a different generator. I mostly tried to keep this all portable but this has only been tested in Linux, so your mileage may vary.

# Execution instructions

The `raytracer` binary takes one argument: a path to a camera configuration json file. You can find a default file in the `config` directory, which has parameters intended to characterize a raspberry pi camera.

The config file also offers *extrinsic* parameters for the camera, so you can translate/rotate around to play with the view angle. 

Radial and tagential distortion parameters for intrinsics are pulled straight from the models described at https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
