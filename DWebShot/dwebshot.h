#ifndef WEBSHOTAPPLICATION_H
#define WEBSHOTAPPLICATION_H

#include <QObject>
#include <QWebEngineView>
#include <QTimer>
#include "dapplication.h"

namespace DSuite {

class DWebShot : public QObject
{
    Q_OBJECT

public:
    enum DWebShotError {
        ERROR_NONE = 0,
        ERROR_WEBPAGE,
        ERROR_IMAGE
    };

public:
    DWebShot(QString url, QString filename, int width, int height, int delay);

public:
    QString url() const;
    QString filename() const;
    int width() const;
    int height() const;
    int delay() const;

public:
    void shoot();

signals:
    void done(DWebShotError error);

private slots:
    void onPageLoaded(bool ok);
    void onShoot();

private:
    QString m_url;
    QString m_filename;
    int m_width;
    int m_height;
    int m_delay;

    QSharedPointer<QWebEngineView> m_webView;
    QTimer m_delayTimer;
};

}

#endif // WEBSHOTAPPLICATION_H
