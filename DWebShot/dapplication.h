#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <QApplication>
#include <QCommandLineParser>
#include <QSharedPointer>

#define DAPPLICATION_VERSION 1.0

namespace DSuite {

class DApplication;

typedef DApplication DWebShotApplication;

class DApplication
{
public:
    DApplication(QString name, QString description = QString(), QString version = "1.0",QString organization = "AntonyDas", QString domain = "antonydas.me");

public:
    DApplication* init(int &argc, char **argv);
    DApplication* addArgument(QString name, QString description);
    int run();

public:
    bool initialized() const;

    QString name() const;
    QString description() const;
    QString version() const;
    QString organization() const;
    QString domain() const;

protected:
    QCommandLineParser argumentParser();

private:
    void exit(QString error, int errorCode = 1);

private:
    bool m_initialized;

    QString m_name;
    QString m_description;
    QString m_version;
    QString m_organization;
    QString m_domain;

    QCommandLineParser m_argumentParser;

    QSharedPointer<QApplication> m_application;
};

}
#endif // DAPPLICATION_H
