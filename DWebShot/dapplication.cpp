#include "dapplication.h"
#include "dwebshot.h"

using namespace DSuite;

DApplication::DApplication(QString name, QString description, QString version,QString organization, QString domain)
    :m_initialized(false),
    m_name(name),
    m_description(description),
    m_version(version),
    m_organization(organization),
    m_domain(domain)
{

}

DApplication *DApplication::init(int &argc, char **argv)
{
    m_application.reset(new QApplication(argc,argv));

    QCoreApplication::setApplicationName(name());
    QCoreApplication::setApplicationVersion(version());
    QCoreApplication::setOrganizationName(organization());
    QCoreApplication::setOrganizationDomain(domain());

    m_argumentParser.setApplicationDescription(description());
    m_argumentParser.addHelpOption();
    m_argumentParser.addVersionOption();

    m_argumentParser.process(*m_application.data());

    m_initialized = true;
    return this;
}

DApplication *DApplication::addArgument(QString name, QString description)
{
    m_argumentParser.addOption(QCommandLineOption(name, description, name));
    return this;
}

int DApplication::run()
{
    if(!initialized())
        exit("Application Internal Error: 0x000001");

    if(!m_argumentParser.isSet("url"))
        exit("Enter URL");

    if(!m_argumentParser.isSet("file"))
        exit("Enter screenshot file path");

    QString filePath;
    QString url;
    bool valid = true;
    int delay = 0;
    int width = 1280;
    int height = 800;

    url = m_argumentParser.value("url");
    filePath = m_argumentParser.value("file");

    if(m_argumentParser.isSet("width"))
        width = m_argumentParser.value("width").toInt(&valid);

    if(!valid)
        exit("Invalid Width");

    if(m_argumentParser.isSet("height"))
        height = m_argumentParser.value("height").toInt(&valid);

    if(!valid)
        exit("Invalid Height");

    if(m_argumentParser.isSet("delay"))
        delay = m_argumentParser.value("delay").toInt(&valid);

    if(!valid)
        exit("Invalid Delay");

    QScopedPointer<DWebShot> dWebShotObj(new DWebShot(url,filePath,width,height,delay));

    QObject::connect(dWebShotObj.data(),&DWebShot::done,[=](DWebShot::DWebShotError error){
        if(error == DWebShot::ERROR_NONE)
            QCoreApplication::exit(error);
        else
            exit("Application Internal Error: 0x0000" + QString::number(error) + "0");
    });

    dWebShotObj->shoot();

    // Incase application is stuck in the event loop (eg. due to some popup), forcefully quit the application
    QTimer::singleShot(delay + 5000,[=](){
        QCoreApplication::exit(-1);
    });

    return QCoreApplication::exec();
}

void DApplication::exit(QString error, int errorCode)
{
    fprintf(stderr, "%s\n",qPrintable(QCoreApplication::translate("main",error.toStdString().c_str())));
    m_argumentParser.showHelp(errorCode);
}

bool DApplication::initialized() const
{
    return m_initialized;
}

QString DApplication::domain() const
{
    return m_domain;
}

QString DApplication::organization() const
{
    return m_organization;
}

QString DApplication::version() const
{
    return m_version;
}

QString DApplication::description() const
{
    return m_description;
}

QString DApplication::name() const
{
    return m_name;
}
