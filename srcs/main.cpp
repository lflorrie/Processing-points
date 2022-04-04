#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "greenpointsproject.h"
#include "greenpoints.h"
#include "imageitem.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	GreenPointsProject gp;
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QStringList args = app.arguments();
	app.setOrganizationName("GreenPoints");
	app.setOrganizationDomain("Nodomen");

	QQmlApplicationEngine engine;
	QQmlContext *context = engine.rootContext();
	context->setContextProperty("GreenPoints", &gp);

	qmlRegisterType<ImageItem>("myextension", 1, 0, "ImageItem");

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					&app, [url](QObject *obj, const QUrl &objUrl) {
	   if (!obj && url == objUrl)
		   QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);
	return app.exec();
}
