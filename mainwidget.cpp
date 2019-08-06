#include "mainwidget.h"

#include <QDebug>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QPushButton *debug = new QPushButton("Debug", this);
    QPushButton *info = new QPushButton("Info", this);
    QPushButton *warning = new QPushButton("Warning", this);
    QPushButton *critical = new QPushButton("Critical", this);
    QPushButton *fatal = new QPushButton("Fatal", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(debug);
    layout->addWidget(info);
    layout->addWidget(warning);
    layout->addWidget(critical);
    layout->addWidget(fatal);

    QObject::connect(debug, &QPushButton::clicked,
                     this, [=](){qDebug() << "debug"; });

    QObject::connect(info, &QPushButton::clicked,
                     this, [=](){qInfo() << "info"; });

    QObject::connect(warning, &QPushButton::clicked,
                     this, [=](){qWarning() << "warning"; });

    QObject::connect(critical, &QPushButton::clicked,
                     this, [=](){qCritical() << "critical"; });

    QObject::connect(fatal, &QPushButton::clicked,
                     this, [=](){qFatal("fatal"); });

    this->setMinimumSize(120, 165);

}
