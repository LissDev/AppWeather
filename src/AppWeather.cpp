#include <QtQuick>

#include <sailfishapp.h>
#include "weatherrefresher.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<WeatherRefresher>("io.backend", 1, 0, "WeatherRefresher");

    return SailfishApp::main(argc, argv);
}
