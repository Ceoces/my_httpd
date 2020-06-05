#include "myConfig.h"
#include<iostream>

myConfig *Config::_config = nullptr;
std::string Config::_path = CONF_PATH;
std::mutex Config::_mConfig;

bool Config::setPath(std::string path)
{
	_path = path;
}
myConfig &Config::get()
{
	if(_config==nullptr){
		_mConfig.lock();
		if(_config==nullptr){
			_config = new myConfig(_path.c_str());
		}
	}
	return *_config;
}

myConfig::myConfig()
{
	if (getConfig(CONF_PATH) == -1)
		std::cout << "Open file error: " << (errno) <<std::endl;
}

myConfig::myConfig(const char * path)
{
	this->getConfig(path);
}


myConfig::~myConfig()
{
}

std::string myConfig::operator[](const std::string s)
{
	return this->config[s];
}

int myConfig::getConfig(const char * path)
{
	// Open config file
	this->confFile = fopen(path, "r");
	if (this->confFile == NULL) {
		std::cout << "Open file error." << std::endl;
		return -1;
	}

	char c;
	char buf[201];
	int bufLen = 0, flag = 0;

	while ((c = static_cast<char>(fgetc(this->confFile))) != EOF)
	{
		std::cout<<c;
		if (c == '#') {
			do {
				c = static_cast<char>(fgetc(this->confFile));
			} while (c != '\n');
			continue;
		
		}
		if (c == ' ' || c == '\n' || c == '\t' || c=='\r') {
			continue;
		}
		if (c == '=') {
			if (flag != 0) {
				bufLen = 0;
				flag = 0;
				continue;
			}
			flag = bufLen;
		}
		if (c == ';') {
			if (flag != 0 && bufLen >= 2) {
				addConfig(buf, bufLen, flag);
			}
			bufLen = 0;
			flag = 0;
			continue;
		}
		buf[bufLen++] = c;
		//std::cout << c;
	}

	return 0;
}

void myConfig::addConfig(const char * s, int len, int flag)
{
	std::string key(s, 0, flag);
	std::string value(s, flag + 1, len - flag-1);
	this->config[key] = value;
}

void myConfig::showAll()
{
	std::cout << "Show all." << std::endl;
	for(auto it : this->config)
	{
		std::cout << it.first << ' ' << it.second <<std::endl;
	}
}
