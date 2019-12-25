#include <qtwin/MainWindow.h>
#include <qtwin/RenderWidget.h>
#include <qtwin/CameraIntrinsics.h>
#include <memory>

#include <glm/glm.hpp>

#include <iostream>

const float PI{3.141592636};

static float DegreesToRadians(float degrees)
{
    return degrees * PI / 180.0f;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //
    // Hardcoding camera parameters for now
    //

    const glm::vec2 PrincipalPoint(0.0, 0.0);

    const float FovHorizontal = DegreesToRadians(62.2f);
    const float FovVertical   = DegreesToRadians(48.8f);

    const float FocalLength{0.00304f}; // In meters

    const float ax{2.0f * FocalLength * tanf(FovHorizontal / 2.0f)};
    const float ay{2.0f * FocalLength * tanf(FovVertical   / 2.0f)};

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

