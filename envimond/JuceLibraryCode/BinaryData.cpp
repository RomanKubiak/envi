/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== json.bat ==================
static const unsigned char temp_binary_data_0[] =
"@ECHO OFF\n"
"ECHO { \"name\": \"POLOLU data source\", \"values\": [ {\"name\": \"Voltage\",\"value\": 13,\"unit\": \"V\"}, {\"name\": \"Current\",\"value\": 22,\"unit\": \"A\"} ] }";

const char* json_bat = (const char*) temp_binary_data_0;

//================== json.sh ==================
static const unsigned char temp_binary_data_1[] =
"#!/bin/bash\n"
"\n"
"echo '{ \"name\": \"data source name\", \"values\": [ {\"name\": \"Voltage\",\"value\": '$RANDOM',\"unit\": \"V\"}, {\"name\": \"Current\",\"value\": '$RANDOM',\"unit\": \"A\"} ] }'\n";

const char* json_sh = (const char*) temp_binary_data_1;

//================== bmp085.xml ==================
static const unsigned char temp_binary_data_2[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"\n"
"<dataSource \ttype=\"bmp085\" \tname=\"BMP085 barometric pressure and temperature\" \n"
"\t\t          timeout=\"2000\" \tinterval=\"5000\" \n"
"\t\t          i2caddr=\"0x77\">\n"
"\n"
"\t<dataValue name=\"pressure\" \texp=\"\" />\n"
"\t<dataValue name=\"temperature\" \texp=\"\" />\n"
"\t<dataValue name=\"altitude\" \texp=\"\" />\n"
"\n"
"</dataSource>";

const char* bmp085_xml = (const char*) temp_binary_data_2;

//================== cmd_unix.xml ==================
static const unsigned char temp_binary_data_3[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"\n"
"<dataSource type=\"cmd\"      name=\"JSON command interface/UNIX\" \n"
"\t\t        timeout=\"2000\"  interval=\"30000\" \n"
"\t\t        cmd=\"/root/devel/envi/envimond/Source/Config/scripts/json.sh\">\n"
"</dataSource>";

const char* cmd_unix_xml = (const char*) temp_binary_data_3;

//================== cmd_win32.xml ==================
static const unsigned char temp_binary_data_4[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"\n"
"<dataSource type=\"cmd\"      name=\"JSON command interface/WINDOWS\"\n"
"\t\t        timeout=\"2000\"  interval=\"5000\"\n"
"\t\t        cmd=\"c:\\\\devel\\\\json.bat\">\n"
"\t<dataValue name=\"value1\" dataExp=\"result * 2\" />\n"
"\t<dataValue name=\"value2\" dataExp=\"result * 4\" />\n"
"\t<dataValue name=\"value3\" dataExp=\"result / rand()\" />\n"
"</dataSource>";

const char* cmd_win32_xml = (const char*) temp_binary_data_4;

//================== dht.xml ==================
static const unsigned char temp_binary_data_5[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
"\r\n"
"<dataSource type=\"dht11\"    name=\"DHT11 temperature and humidity sensor\" \r\n"
"            timeout=\"2000\" \tinterval=\"5000\" \r\n"
"            gpioPin=\"5\"  \t  delay=\"3000\" iterations=\"4\">\r\n"
"\r\n"
"  <dataValue name=\"temp-celsius\" \t\texp=\"\" />\r\n"
"\t<dataValue name=\"temp-fahrenheit\" \texp=\"\" />\r\n"
"\t<dataValue name=\"humidity\" \t\texp=\"\" />\r\n"
"\r\n"
"</dataSource>";

const char* dht_xml = (const char*) temp_binary_data_5;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x8a4de43e:  numBytes = 151; return json_bat;
        case 0xb1e17d0c:  numBytes = 169; return json_sh;
        case 0x65968ee0:  numBytes = 327; return bmp085_xml;
        case 0xbff44645:  numBytes = 234; return cmd_unix_xml;
        case 0xeff2f04e:  numBytes = 353; return cmd_win32_xml;
        case 0x61f6cbc8:  numBytes = 372; return dht_xml;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const int namedResourceListSize = 6;

const char* namedResourceList[] =
{
    "json_bat",
    "json_sh",
    "bmp085_xml",
    "cmd_unix_xml",
    "cmd_win32_xml",
    "dht_xml"
};

}
