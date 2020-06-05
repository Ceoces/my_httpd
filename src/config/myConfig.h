#pragma once

#include<string>
#include<map>
#include<mutex>

#define CONF_PATH "./httpd.conf"

class myConfig
{
private:
	FILE *confFile;
	std::map<std::string, std::string> config;

	int getConfig(const char * path);
	void addConfig(const char * s, int len, int flag);

public:
	myConfig();
	myConfig(const char * path);
	~myConfig();
	std::string operator[](const std::string s);
	void showAll();
	
	//int getConfig(char * path);
};

class Config
{
private:
	/* data */
	static myConfig *_config;
	static std::string _path;
	static std::mutex _mConfig;
public:
	static bool setPath(std::string path);
	static myConfig &get();
};


