#include <qtwin/MainWindow.h>
#include <qtwin/RenderWidget.h>
#include <qtwin/CameraIntrinsics.h>
#include <memory>

#include <glm/glm.hpp>

#include <iostream>

const float PI{3.141592636};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //
    // Hardcoding camera parameters for now
    //

    // Sensor image area: 3.76 × 2.74 mm
    const float ax{0.00376};
    const float ay{0.00274};

    const float FocalLength{0.0036}; // In meters
    const glm::vec2 PrincipalPoint(0.0, 0.0);
    const glm::vec2 SpatialResolution(ax, ay);
    const qtwin::CameraIntrinsics Intrinsics(
            FocalLength,
            PrincipalPoint,
            SpatialResolution);

    using namespace qtwin;
    std::unique_ptr<RenderWidget> spGL(
        new RenderWidget(Intrinsics)
        );
    
    MainWindow window(spGL.get());
    spGL->setParent(&window);

    QSize size(640, 480);
    window.resize(size);
    window.show();

    return app.exec();
}

