#include "request.h"

Request::Request(char *s, int len)
{
    if(!len) throw -1;
    string httpRequest = ::string(s, len);
    std::stringstream ss(httpRequest);

    //HTTP 方法 url 版本 判断
    string method,version;
    ss >> method >> (this->url) >> version;

    //method
    this->method = -1;
    for(int i=0; i<METHOD_NUM; i++){
        if(method.compare(RequestMethod[i]) == 0){
            this->method = i;
            break;
        }
    }
    if(this->method<0) throw -1;

    //version
    if(version.compare("HTTP/1.0") == 0){
        this->version = 10;
    } else if(version.compare("HTTP/1.1") == 0){
        this->version = 11;
    } else if(version.compare("HTTP/2.0") == 0){
        this->version = 20;
    } else {
        throw -1;
    }

    string key,value,line;
    getline(ss,line);
    while (getline(ss, line))
    {
        if(line.length()==0) break;
        int pos = line.find(": ");
        key = line.substr(0,pos);
        value = line.substr(pos+1, line.length());
        this->fieldList[key] = value;
    }
    ss >> this->data;
}



#ifdef _DEBUG_
void Request::print(){
    std::cout << "Method : " << RequestMethod[this->method] << std::endl;
    std::cout << "Url : " << this->url << std::endl;
    std::cout << "Version : " << this->version << std::endl;
    for(auto p : this->fieldList){
        std::cout << "key" << p.first << " value: " <<p.second << std::endl;
    }
    std::cout << "Content:" << this->data << std::endl;
}
#endif

Request::~Request()
{
    
}

//TODO 执行请求
void Request::do_request(int fd)
{
    if(url.length()==0){
         Response(400).sendHeader(fd);
         return;
    }

    std::string fileaddr = Config::get()["HostPath"]; 
    fileaddr += url;
    //定位文件
    struct stat fileInfo;
    if(lstat(fileaddr.c_str(), &fileInfo)<0){
         Response(404).sendHeader(fd);
         return;
    }
    if(S_ISDIR(fileInfo.st_mode)){
        //目录
        std::cout << "Dir:" ;
        DIR *pDir = opendir(fileaddr.c_str());
        struct dirent *pEnt = nullptr;
        if(pDir == nullptr){
             Response(403).sendHeader(fd);
             return;
        }
        while ((pEnt = readdir(pDir)) != nullptr)
        {
        std::cout << pEnt->d_name << " ";
            if(strcasecmp(pEnt->d_name, "index.") > 0){
                break;
            }
        }
        cout << endl;
        if(pEnt == nullptr ||  !S_ISREG( pEnt->d_type) ){ 
            Response(404).sendHeader(fd);
             return;
        }
        fileaddr += pEnt->d_name;
        if( lstat(fileaddr.c_str(), &fileInfo)<0){
            Response(404).sendHeader(fd);
             return;
        }
    } else if(S_ISREG(fileInfo.st_mode)){
        ;
    } else {
        Response(403).sendHeader(fd);
        return;
    }

    //处理文件
    //判断文件类型
    if(strcasecmp(fileaddr.c_str(), ".php")==0 ){
        ;
    }  else {
        Response re(200);
        re.setField("Content-Length", std::to_string(fileInfo.st_size));
        re.setField("Connection", "keep-alive");
        re.sendHeader(fd);
        int file_fd = open(fileaddr.c_str(), O_RDONLY);
        sendfile(fd, file_fd, 0 ,fileInfo.st_size);
    }
}
