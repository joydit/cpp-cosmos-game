#include "BaseConfig.h"

using namespace std;

///Public
BaseConfig::BaseConfig(): console("config/console.ini", true), settings("config/settings.ini", true)
{

}

BaseConfig::~BaseConfig()
{

}
