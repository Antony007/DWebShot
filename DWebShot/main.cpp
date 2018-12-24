#include "dapplication.h"
#include <QScopedPointer>

int main(int argc, char *argv[])
{    
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES); //FIXME: Temporary Fix for QTBUG-69030 [https://bugreports.qt.io/browse/QTBUG-69030]

    QScopedPointer<DSuite::DApplication> app(
                new DSuite::DWebShotApplication("DWebShot","Application to generate image of a website given the URL")
                );

    app     ->addArgument("url","Fully qualified URL of the website")
            ->addArgument("file","Screenshot file path")
            ->addArgument("width","Screenshot width [default:1280]")
            ->addArgument("height","Screenshot height [default:800]")
            ->addArgument("delay","Set delay in generating screenshot incase of loading a heavy website [default:0]")

            ->init(argc,argv);

    return app->run();
}
