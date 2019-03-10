#include<fstream>
#include<vector>
#include<iostream>
#include<stdint.h>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include "Poco/Net/NetworkInterface.h"

using namespace std;
using Poco::Net::NetworkInterface;

int main() {
    NetworkInterface en0 = NetworkInterface::forName("en0");
    NetworkInterface::MACAddress mac(en0.macAddress());
    if (!mac.empty() && (en0.type() != NetworkInterface::NI_TYPE_SOFTWARE_LOOPBACK))
			std::cout << "MAC Address: (" << en0.type() << ") " << mac << std::endl;
    return 0;
}