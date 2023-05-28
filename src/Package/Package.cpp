#include "Package.h"

String Buffer;

void Rec_Package(String rec_data)
{
    vector<String> raw_data_arry;
    int index = 0;
    int last_index = 0;
    while (1)
    {
        index = rec_data.indexOf("|", last_index);
        if (index == -1)
        {
        break;
        }
        String temp = rec_data.substring(last_index, index);
        raw_data_arry.push_back(temp);
        last_index = last_index + index + 1;
    }

    if (raw_data_arry.size() < 1)
    {
        log_e("raw_data_arry.size() < 1");
        return;
    }

    // 获取 MAC 地址
    if (raw_data_arry.at(0) == "0")
    {
        char buffer[40];
        sprintf(buffer,"1%016lx",ESP.getEfuseMac());
        bluetooth_sendmessage(buffer);
        log_i("MAC: %s",buffer);
    }

    // 认证（TODO: 直接返回MAC即可通过认证）
    if (raw_data_arry.at(0) == "1")
    {
        char buffer[40];
        sprintf(buffer,"%016lx",ESP.getEfuseMac());
        if(String(buffer).equals(raw_data_arry.at(1))){
            certification = true;
        }
    }


}