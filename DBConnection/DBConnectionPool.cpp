//
// Created by Nile on 2018/5/10.
//

#include "DBConnectionPool.h"

DBConnectionPool *DBConnectionPool::connPool = NULL;

//连接池的构造函数
DBConnectionPool::DBConnectionPool(string url, string userName, string password, int maxSize) : maxSize(maxSize),
                                                                                                curSize(0),
                                                                                                username(userName),
                                                                                                password(password),
                                                                                                url(url) {
    try {
        this->driver = get_driver_instance();
    }
    catch (sql::SQLException &e) {
        perror("connect driver error;\n");
    }
    catch (std::runtime_error &e) {
        perror("runtime error\n");
    }
    this->InitConnection(maxSize / 2);
}

//获取连接池对象，单例模式
DBConnectionPool *DBConnectionPool::GetInstance() {
    if (connPool == NULL) {
        connPool = new DBConnectionPool("tcp://127.0.0.1:3306", "root", "20120611", 10);
    }
    return connPool;
}

//初始化连接池，创建最大连接数的一半连接数量
void DBConnectionPool::InitConnection(int iInitialSize) {
    Connection *conn;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < iInitialSize; i++) {
        conn = this->CreateConnection();
        if (conn) {
            connList.push_back(conn);
            ++(this->curSize);
        }
        else {
            perror("create connection error");
        }
    }
    pthread_mutex_unlock(&lock);
}

//创建连接,返回一个Connection
Connection *DBConnectionPool::CreateConnection() {
    Connection *conn;
    try {
        conn = driver->connect(this->url, this->username, this->password);//建立连接
        return conn;
    }
    catch (sql::SQLException &e) {
        perror("create connection error");
        return NULL;
    }
    catch (std::runtime_error &e) {
        perror("runtime error");
        return NULL;
    }
}

//在连接池中获得一个连接
Connection *DBConnectionPool::GetConnection() {
    Connection *con;
    pthread_mutex_lock(&lock);
    if (connList.size() > 0)//连接池容器中还有连接
    {
        con = connList.front();//得到第一个连接
        connList.pop_front();//移除第一个连接
        if (con->isClosed())//如果连接已经被关闭，删除后重新建立一个
        {
            delete con;
            con = this->CreateConnection();
        }
        //如果连接为空，则创建连接出错
        if (con == NULL) {
            --curSize;
        }
        pthread_mutex_unlock(&lock);
        return con;
    }
    else {
        if (curSize < maxSize) {//还可以创建新的连接
            con = this->CreateConnection();
            if (con) {
                ++curSize;
                pthread_mutex_unlock(&lock);
                return con;
            }
            else {
                pthread_mutex_unlock(&lock);
                return NULL;
            }
        }
        else {//建立的连接数已经达到maxSize
            pthread_mutex_unlock(&lock);
            return NULL;
        }
    }
}

//回收数据库连接
void DBConnectionPool::ReleaseConnection(sql::Connection *conn) {
    if (conn) {
        pthread_mutex_lock(&lock);
        connList.push_back(conn);
        pthread_mutex_unlock(&lock);
    }
}

//连接池的析构函数
DBConnectionPool::~DBConnectionPool() {
    this->DestoryConnPool();
}

//销毁连接池,首先要先销毁连接池的中连接
void DBConnectionPool::DestoryConnPool() {
    list<Connection *>::iterator icon;
    pthread_mutex_lock(&lock);
    for (icon = connList.begin(); icon != connList.end(); ++icon) {
        this->DestoryConnection(*icon);//销毁连接池中的连接
    }
    curSize = 0;
    connList.clear();//清空连接池中的连接
    pthread_mutex_unlock(&lock);
}

//销毁一个连接
void DBConnectionPool::DestoryConnection(Connection *conn) {
    if (conn) {
        try {
            conn->close();
        }
        catch (sql::SQLException &e) {
            perror(e.what());
        }
        catch (std::exception &e) {
            perror(e.what());
        }
        delete conn;
    }
}