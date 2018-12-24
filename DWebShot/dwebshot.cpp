#include "dwebshot.h"
#include <QImage>
#include <QPainter>

using namespace DSuite;

DWebShot::DWebShot(QString url, QString filename, int width, int height, int delay) :
    m_url(url),
    m_filename(filename),
    m_width(width),
    m_height(height),
    m_delay(delay)
{
    m_delayTimer.setInterval(delay);
    m_delayTimer.stop();
    m_delayTimer.setSingleShot(true);
    connect(&m_delayTimer,&QTimer::timeout,this,&DWebShot::onShoot);

    m_webView.reset(new QWebEngineView());
    connect(m_webView.data(),&QWebEngineView::loadFinished,this,&DWebShot::onPageLoaded);
}

QString DWebShot::url() const
{
    return m_url;
}

QString DWebShot::filename() const
{
    return m_filename;
}

int DWebShot::width() const
{
    return m_width;
}

int DWebShot::height() const
{
    return m_height;
}

int DWebShot::delay() const
{
    return m_delay;
}

void DWebShot::shoot()
{
    m_webView->setFixedWidth(m_width);
    m_webView->setFixedHeight(m_height);
    m_webView->load(m_url);
}

void DWebShot::onPageLoaded(bool ok)
{
    if(ok)
        m_delayTimer.start();
    else
        emit done(ERROR_WEBPAGE);

    m_webView->setWindowFlag(Qt::SubWindow);
    m_webView->setWindowOpacity(0);
    m_webView->show();
}

void DWebShot::onShoot()
{
    QImage image(width(),height(),QImage::Format_ARGB32);
    QPainter painter(&image);

    m_webView->page()->view()->render(&painter);
    painter.end();

    if(image.save(m_filename,"PNG"))
        emit done(ERROR_NONE);
    else
        emit done(ERROR_IMAGE);
}
