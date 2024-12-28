#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QTimer>
#include "gauge.h"
#include "obdhandler.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    int id = QFontDatabase::addApplicationFont(":/Orbitron.ttf");
    if (id == -1) {
        qWarning() << "Failed to load Orbitron font.";
    } else {
        QStringList loadedFamilies = QFontDatabase::applicationFontFamilies(id);
        if (!loadedFamilies.isEmpty()) {
            qDebug() << "Font loaded successfully:" << loadedFamilies.first();
        }
    }

    qmlRegisterType<Gauge>("com.voknal.gauge", 1, 0, "Gauge");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *object = engine.rootObjects()[0];

    QObject *speedGauge = object->findChild<QObject*>("speedGauge");
    QObject *rpmGauge = object->findChild<QObject*>("rpmGauge");

    Gauge *ptrSpeedGauge = dynamic_cast<Gauge*>(speedGauge);
    Gauge *ptrRpmGauge = dynamic_cast<Gauge*>(rpmGauge);

    ptrSpeedGauge->setValue(0);
    ptrRpmGauge->setValue(0);

    // Initialize OBD-II handler
    ObdHandler obdHandler;
    obdHandler.connectToELM327();

    QObject::connect(&obdHandler, &ObdHandler::speedUpdated, ptrSpeedGauge, &Gauge::setValue);
    QObject::connect(&obdHandler, &ObdHandler::rpmUpdated, ptrRpmGauge, &Gauge::setValue);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]()
                     {
                        obdHandler.getSpeed();
                        obdHandler.getRPM();
                     }
                     );
    timer.start(100);
    return app.exec();
}
