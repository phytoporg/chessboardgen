#include <qtwin/MainWindow.h>
#include <qtwin/RenderWidget.h>
#include <memory>

#include <iostream>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QGLFormat glf = QGLFormat::defaultFormat();

    using namespace qtwin;
    std::unique_ptr<RenderWidget> spGL(new RenderWidget(glf));
    
    MainWindow window(spGL.get());
    spGL->setParent(&window);

    QSize size(640, 480);
    window.resize(size);
    window.show();

    return app.exec();
}

