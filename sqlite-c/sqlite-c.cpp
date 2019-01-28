#include "Poco/Data/Session.h"
#include "Poco/SingletonHolder.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <algorithm>
#include "sqlite-c.h"

using namespace Poco::Data::Keywords;
using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;

typedef struct {
    string name;
    string nickName;
    Poco::Data::LOB<uint8_t> photo;
    Poco::Data::LOB<uint8_t> feature; 
} driversCenter;

POCO_DECLARE_EXCEPTION(, SQLException, Poco::Exception)

POCO_IMPLEMENT_EXCEPTION(SQLException, Poco::Exception, "DriverCenter: Invalid SQL:")

class DriverCenter {
  public:
    static DriverCenter &instance();
    uint32_t GetCount() {
        uint32_t count = 0;
        *session << "SELECT COUNT(*) FROM DRIVERS", into(count), now;
        return count;
    }

    int32_t GetDriversInfo(std::vector<driversCenter> &driversCenters) {
        Statement select(*session);
        driversCenter driversCentersTmp;
        uint32_t count = 0;
        select << "SELECT * FROM DRIVERS", into(driversCentersTmp.name),
            into(driversCentersTmp.nickName), into(driversCentersTmp.photo),
            into(driversCentersTmp.feature), range(0, 1);

        while (!select.done()) {
            select.execute();
            driversCenters.push_back(driversCentersTmp);
        }

        return 0;
    }

    int32_t GetFeature(Poco::Data::LOB<uint8_t> *feature, string driverName) {
        if (NULL == feature) {
            cout << "DriverCenter: invalid input feature buffer" << endl;
            return -1;
        }

        try {
            *session << "SELECT feature FROM DRIVERS WHERE name=?", into(*feature),
                use(driverName), now;
        } catch (SQLException &ex) {
            cout << ex.displayText() << endl;
            return -1;
        }
        return 0;
    }

    int32_t GetPhoto(Poco::Data::LOB<uint8_t> *photo, string driverName) {
        if (NULL == photo) {
            cout << "DriverCenter: invalid input feature buffer" << endl;
            return -1;
        }

        try {
            *session << "SELECT photo FROM DRIVERS WHERE name=?", into(*photo),
                use(driverName), now;
        } catch (SQLException &ex) {
            cout << ex.displayText() << endl;
            return -1;
        }
        return 0;
    }

    int32_t RegisterDriver(driversCenter driverInfo) {
        try {
            *session << "INSERT INTO DRIVERS(name, nickName, photo, feature) "
                     << "VALUES(?, ?, ?, ?)",
                use(driverInfo.name), use(driverInfo.nickName), 
                bind(Poco::Nullable<Poco::Data::LOB<uint8_t> >(driverInfo.photo)),
                bind(Poco::Nullable<Poco::Data::LOB<uint8_t> >(driverInfo.feature)), now;
        } catch (Poco::Data::SQLite::ConstraintViolationException &ex) {
            cout << ex.displayText() << endl;
            return -1;
        }
        return 0;
    }

    int32_t DeleteDriver(string driverName) {
        try {
            *session << "DELETE FROM DRIVERS WHERE name=?", use(driverName), now;
        } catch (SQLException &ex) {
            cout << ex.displayText() << endl;
            return -1;
        }
        return 0;
    }

    DriverCenter() {
        // regist SQLite connector
        Poco::Data::SQLite::Connector::registerConnector();
        // create a session
        cout << "DriverCenter start up!" << endl;
        session = new Session("SQLite", "DriverCenter.db");
        // may create table
        *session << "CREATE TABLE IF NOT EXISTS DRIVERS"
            << "(name VARCHAR PRIMARY KEY, nickName VARCHAR, photo BLOB, feature BLOB)",
            now;
    }
    ~DriverCenter() {
        Poco::Data::SQLite::Connector::unregisterConnector();
        delete session;
    }

  private:
    Session *session;
};


namespace
{
	static Poco::SingletonHolder<DriverCenter> sh;
}


DriverCenter& DriverCenter::instance()
{
	return *sh.get();
}

uint32_t DriverCenter_getCount() {
    return DriverCenter::instance().GetCount();
}

uint32_t DriverCenter_getFeature(uint8_t *feature, char *driverName) {
    if(NULL == feature) {
        cout << "Invalid feature buffer for GetFeature" << endl;
        return -1;
    }
    if(NULL == driverName) {
        cout << "Invalid driverName buffer for GetFeature" << endl;
        return -1;
    }

    Poco::Data::LOB<uint8_t> featureTmp;
    string driverNameTmp(driverName);
    uint32_t status;

    status =  DriverCenter::instance().GetFeature(&featureTmp, driverNameTmp);
    memcpy(feature, &featureTmp, featureTmp.size());
    return status;
}

uint32_t DriverCenter_getPhoto(uint8_t *photo, char *driverName) {
    if(NULL == photo) {
        cout << "Invalid photo buffer for GetPhoto" << endl;
        return -1;
    }
    if(NULL == driverName) {
        cout << "Invalid driverName buffer for GetPhoto" << endl;
        return -1;
    }

    Poco::Data::LOB<uint8_t> photoTmp;
    string driverNameTmp(driverName);
    uint32_t status;

    status =  DriverCenter::instance().GetPhoto(&photoTmp, driverNameTmp);
    memcpy(photo, &photoTmp, photoTmp.size());
    return status;
}

uint32_t DriverCenter_getDriverInfo(driverInfo *driverInfos, uint32_t driverInfoLength) {
    if(NULL == driverInfos) {
        cout << "Invalid driverInfos buffer" << endl;
        return -1;
    }
    std::vector<driversCenter> driverInfosTmp;
    uint32_t status;
    status = DriverCenter::instance().GetDriversInfo(driverInfosTmp);
    
    uint32_t memcpyLength = min(driverInfosTmp.size(), (size_t)driverInfoLength);

    for(uint32_t i = 0; i < memcpyLength; i++){
        strncpy(driverInfos[i].name, driverInfosTmp[i].name.c_str(),
                DRIVERCENTER_MAX_NAME_LENGTH);
        strncpy(driverInfos[i].nickName, driverInfosTmp[i].nickName.c_str(),
                DRIVERCENTER_MAX_NAME_LENGTH);
        memcpy(driverInfos[i].feature, &(driverInfosTmp[i].feature),
               driverInfosTmp[i].feature.size());
        driverInfos[i].featureLength = driverInfosTmp[i].feature.size();
        memcpy(driverInfos[i].photo, &(driverInfosTmp[i].photo), driverInfosTmp[i].photo.size());
        driverInfos[i].photoLength = driverInfosTmp[i].photo.size();
    }
    return status;
}

uint32_t DriverCenter_registerDriver(driverInfo driverInfo) {
    driversCenter driverInfoTmp;

    driverInfoTmp.name.assign(driverInfo.name);
    driverInfoTmp.nickName.assign(driverInfo.nickName);
    if(NULL != driverInfo.feature) {
        memcpy(&(driverInfoTmp.feature), driverInfo.feature, driverInfo.featureLength);
    }
    if(NULL != driverInfo.photo) {
        memcpy(&(driverInfoTmp.photo), driverInfo.photo, driverInfo.photoLength);
    }
    return DriverCenter::instance().RegisterDriver(driverInfoTmp);
}

uint32_t DriverCenter_deleteDriver(char *driverName) {
    if(NULL == driverName) {
        cout << "Invalid driverName buffer for deleteDriver" << endl;
        return -1;
    }
    string driverNameTmp(driverName);

    return DriverCenter::instance().DeleteDriver(driverNameTmp);
}