#include <qtwin/MainWindow.h>
#include <qtwin/RenderWidget.h>
#include <qtwin/CameraIntrinsics.h>

#include <fstream>
#include <memory>
#include <vector>

#include <json/json.hpp>

#include <glm/glm.hpp>

#include <iostream>

const float PI{3.141592636};

static float DegreesToRadians(float degrees)
{
    return degrees * PI / 180.0f;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_config>" << std::endl;
        return -1;
    }

    const std::string ConfigPath(argv[1]);
    std::ifstream in(ConfigPath);

    using json = nlohmann::json;
    json config;

    try 
    {
        in >> config;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    QApplication app(argc, argv);

    //
    // Hardcoding camera parameters for now
    //

    const glm::vec2 PrincipalPoint(
            config["px"].get<float>(), 
            config["py"].get<float>());

    const float FovHorizontal =
        DegreesToRadians(config["fov_horizontal"].get<float>());
    const float FovVertical = 
        DegreesToRadians(config["fov_vertical"].get<float>());

    const float FocalLength{config["focal_length"].get<float>()};

    const float ax{2.0f * FocalLength * tanf(FovHorizontal / 2.0f)};
    const float ay{2.0f * FocalLength * tanf(FovVertical   / 2.0f)};

    //
    // Just a little barrel distortion
    // 
    glm::vec3 RadialDistortion(
            config["k0"].get<float>(),
            config["k1"].get<float>(),
            config["k2"].get<float>());
    glm::vec2 TangentialDistortion(
            config["t0"].get<float>(),
            config["t1"].get<float>());

    const glm::vec2 SpatialResolution(ax, ay);
    const qtwin::CameraIntrinsics Intrinsics(
            FocalLength,
            PrincipalPoint,
            SpatialResolution,
            RadialDistortion,
            TangentialDistortion);

    using namespace qtwin;
    std::unique_ptr<RenderWidget> spGL(
        new RenderWidget(Intrinsics)
        );

    spGL->SetChessboardRotation(
            glm::radians(config["yaw"  ].get<float>()),
            glm::radians(config["pitch"].get<float>()),
            glm::radians(config["roll" ].get<float>()));
    spGL->SetChessboardTranslation(
            config["tx"].get<float>(),
            config["ty"].get<float>(),
            config["tz"].get<float>());
    
    MainWindow window(spGL.get());
    spGL->setParent(&window);

    QSize size(640, 480);
    window.resize(size);
    window.show();

    return app.exec();
}

