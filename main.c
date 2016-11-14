//#include "mbed.h"
#include <iio.h>
#include "iio-private.h"
#include "iio-errno.h"
#include <errno.h>
#include "common.h"
//#include "picojson.h"
//#include "config_json.h"


static struct iio_buffer *rxbuf = NULL;
struct iio_device *dev = NULL;
struct iio_channel *ch[2] = {0,0};


#define mA(x) (1000000.0 * x)

#define NUMBER_OF_SAMPLES 1
#define CTX_TIMEOUT 0

/*
const char* find_attribute(const struct iio_device *dev,const char* name,const struct iio_channel **chn, bool &is_channel_attrib, bool &error)
{
    string channel_name;
    string str_name = name;
    channel_name = str_name.substr(0,str_name.find('\\'));
    if(channel_name != str_name)
    {
        // channel attribute 

        int nb_channels = iio_device_get_channels_count(dev);
        //*ch = (iio_channel*)malloc(nb_channels * sizeof(void*));
        for (int j = 0; j < nb_channels; j++) {
            *chn = iio_device_get_channel(dev, j);
            if(strcmp(channel_name.c_str(),iio_channel_get_name(*chn))==0)
                break;
        }
        if(chn==NULL)
        {
            error = 1;
            return NULL;
        }
        error = 0;
        is_channel_attrib = 1;
        const char *attr_name = (strchr(name,'\\')+1);
        return iio_channel_find_attr(*chn, attr_name);

    }
    else
    {
        // device attribute 
        error = 0;
        is_channel_attrib = 0;
        return iio_device_find_attr(dev, name);
    }
}

void write_attribute_string(const struct iio_device *dev,const char* name,const char* value)
{
    bool is_channel_attr;
    bool error;
    const char* attr;
    const struct iio_channel *chn = NULL;
    attr = find_attribute(dev,name,&chn,is_channel_attr,error);
    if(error)
        return;
    if(is_channel_attr)
    {
        iio_channel_attr_write(chn,attr,value);
    }
    else
    {
        iio_device_attr_write(dev,attr,value);
    }
}
*/

//==============================================================================
// General delay code. Delays approx. in mSec.
//==============================================================================
void Delay_ms(unsigned int mSec)
{
  int d1, d2;
  d1 = mSec;
  d2 = mSec;
  for ( d1 = 0; d1 < mSec; d1++ )
  {
   for ( d2 = 0; d2 < 3400; d2++ )
   {
   }
  }
}

void shutdown(void)
{
  
}

int main() {

    //picojson::value v;    
    //string err = picojson::parse(v, json, json + strlen(json));

    //picojson::array products = v.get("Products").get<picojson::array>();//.get("Powermode");//.get<string>();

    struct iio_context *ctx = iio_create_context_from_uri("serial:UART0,115200n8");
    
    if (!ctx) {
        DEBUG_MESSAGE("Unable to create context: %i\r\n", errno);
        while (true)
            Delay_ms(300);
    }

//  DEBUG_MESSAGE("IIO context created with %s backend.\r\n",iio_context_get_name(ctx));
      
    DEBUG_MESSAGE("===================================\r\n");
    DEBUG_MESSAGE("Writing configuration to the device\r\n\r\n");
    
    dev = iio_context_get_device(ctx, 0); // first device in context ?
    /*
    char var[255];
    char temp_var[50];
    var = products[0].get("OperationalMode").get<string>();
    DEBUG_MESSAGE("OperationalMode - %s\r\n",var.c_str());
    write_attribute_string(dev,"OperationalMode",var.c_str());

    var = v.get("DisplayTemperatureAs").get<string>();
    DEBUG_MESSAGE("TemperatureUnit - %s\r\n",var.c_str());
    write_attribute_string(dev,"TemperatureUnit",var.c_str());

    var = products[0].get("PowerMode").get<string>();
    DEBUG_MESSAGE("PowerMode - %s\r\n",var.c_str());
    write_attribute_string(dev,"PowerMode",var.c_str());


    var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("FilterType").get<string>();
    var = var.substr(0,var.find('.'));
    DEBUG_MESSAGE("FilterType - %s\r\n",var.c_str());
    write_attribute_string(dev,"FilterType",var.c_str());

    if(var.find("FIR")!=var.npos) //  found
    {
        var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("FilterType").get<string>();
        var = var.substr(var.find('.')+1);
        DEBUG_MESSAGE("FirFrequency - %s\r\n",var.c_str());
        write_attribute_string(dev,"FirFrequency",var.c_str());
    //FIRFREQUENCY
    }
    var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("FS").get<string>();
    if(var.compare(" ")!= 1 && var.size()!=0){
    DEBUG_MESSAGE("FS - %s\r\n",var.c_str());
    write_attribute_string(dev,"FS",var.c_str());}

    var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("SensorType").get<string>();
    var = var.substr(var.find('.')+1);
    DEBUG_MESSAGE("thermocouple\\SensorType - %s\r\n",var.c_str());
    write_attribute_string(dev,"thermocouple\\SensorType",var.c_str());

    var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("Gain").get<string>();
    DEBUG_MESSAGE("thermocouple\\Gain - %s\r\n",var.c_str());
    write_attribute_string(dev,"thermocouple\\Gain",var.c_str());

    var = products[0].get("HardwareParameters").get<picojson::array>()[0].get("VBiasEnable").get<string>();
    if(var=="True") var = "1";
    else var = "0";
    DEBUG_MESSAGE("thermocouple\\VBiasEnable - %s\r\n",var.c_str());
    write_attribute_string(dev,"thermocouple\\VBiasEnable",var.c_str());

    // temp min & temp max
    var =  products[0].get("HardwareParameters").get<picojson::array>()[0].get("TemperatureMin").get<string>();
    DEBUG_MESSAGE("thermocouple\\TemperatureMin - %s\r\n",var.c_str());
    write_attribute_string(dev,"thermocouple\\TemperatureMin",var.c_str());

    var =  products[0].get("HardwareParameters").get<picojson::array>()[0].get("TemperatureMax").get<string>();
    DEBUG_MESSAGE("thermocouple\\TemperatureMax - %s\r\n",var.c_str());
    write_attribute_string(dev,"thermocouple\\TemperatureMax",var.c_str());


    var = products[0].get("HardwareParameters").get<picojson::array>()[1].get("SensorType").get<string>();
    var = var.substr(0,var.find('.'));
    DEBUG_MESSAGE("cold_junction\\Sensor - %s\r\n",var.c_str());
    write_attribute_string(dev,"cold_junction\\Sensor",var.c_str());

    var = products[0].get("HardwareParameters").get<picojson::array>()[1].get("SensorType").get<string>();
    var = var.substr(var.find('.')+1);
    DEBUG_MESSAGE("cold_junction\\SensorType - %s\r\n",var.c_str());
    write_attribute_string(dev,"cold_junction\\SensorType",var.c_str());

    var = products[0].get("HardwareParameters").get<picojson::array>()[1].get("Gain").get<string>();
    DEBUG_MESSAGE("cold_junction\\Gain - %s\r\n",var.c_str());
    write_attribute_string(dev,"cold_junction\\Gain",var.c_str());
    wait_ms(5);

    // exc current formula
    var = products[0].get("HardwareParameters").get<picojson::array>()[1].get("ExcitationCurrent").get<string>();
    DEBUG_MESSAGE("cold_junction\\ExcitationCurrent - %s\r\n",var.c_str());
    double exc_current = mA( atof(var.c_str()) );
    snprintf(temp_var,10,"%f",exc_current);
    var = temp_var;
    write_attribute_string(dev,"cold_junction\\ExcitationCurrent",var.c_str());
    // reference resistor
    var = products[0].get("HardwareParameters").get<picojson::array>()[1].get("ReferenceResistor").get<string>();
    DEBUG_MESSAGE("cold_junction\\ReferenceResistor - %s\r\n",var.c_str());
    write_attribute_string(dev,"cold_junction\\ReferenceResistor",var.c_str());
    */
    unsigned int major, minor;
    char git_tag[8];
    DEBUG_MESSAGE("=====================================\r\n");
    DEBUG_MESSAGE("Reading configuration from the device\r\n\r\n");
    int ret = iio_context_get_version(ctx, &major, &minor, git_tag);
    if (!ret)
        DEBUG_MESSAGE("Backend version: %u.%u (git tag: %s)\r\n", major, minor,git_tag);
    else
        fprintf(stderr, "Unable to get backend version: %i\r\n", ret);

    DEBUG_MESSAGE("Backend description string: %s\r\n",iio_context_get_description(ctx));

    unsigned int nb_devices = iio_context_get_devices_count(ctx);
    DEBUG_MESSAGE("IIO context has %u devices:\r\n", nb_devices);


    for (unsigned int i = 0; i < nb_devices; i++) {
        dev = iio_context_get_device(ctx, i);
        const char *name = iio_device_get_name(dev);
        DEBUG_MESSAGE("\t%s: %s\r\n", iio_device_get_id(dev), name);

        unsigned int nb_channels = iio_device_get_channels_count(dev);
        DEBUG_MESSAGE("\t\t%u channels found:\r\n", nb_channels);

        unsigned int j;

        //*ch = (iio_channel*)malloc(nb_channels * sizeof(void*));

        for (j = 0; j < nb_channels; j++) {
            ch[j] = iio_device_get_channel(dev, j);
            const char *type_name;

            if (iio_channel_is_output(ch[j]))
                type_name = "output";
            else
                type_name = "input";

            // enable the channels
            iio_channel_enable(ch[j]);

            name = iio_channel_get_name(ch[j]);
            DEBUG_MESSAGE("\t\t\t%s: %s (%s)\r\n", iio_channel_get_id(ch[j]),
                    name ? name : "", type_name);

            unsigned int nb_attrs = iio_channel_get_attrs_count(ch[j]);
            if (!nb_attrs)
                continue;

            DEBUG_MESSAGE("\t\t\t%u channel-specific attributes found:\r\n", nb_attrs);

            unsigned int k;
            for (k = 0; k < nb_attrs; k++) {
                const char *attr = iio_channel_get_attr(ch[j], k);
                char buf[1024];
                ret = (int) iio_channel_attr_read(ch[j], attr, buf, sizeof(buf));
                if (ret > 0) {
                    DEBUG_MESSAGE("\t\t\t\tattr %u: %s"
                            " value: %s\r\n", k, attr, buf);
                } else if (ret == -ENOSYS) {
                    DEBUG_MESSAGE("\t\t\t\tattr %u: %s\r\n", k, attr);
                } else {
                    iio_strerror(-ret, buf, sizeof(buf));

                    fprintf(stderr, "Unable to read attribute %s: %s\r\n", attr,
                            buf);
                }
            }
        }

        unsigned int nb_attrs = iio_device_get_attrs_count(dev);
        if (!nb_attrs)
            continue;

        DEBUG_MESSAGE("\t\t%u device-specific attributes found:\r\n", nb_attrs);
        for (j = 0; j < nb_attrs; j++) {
            const char *attr = iio_device_get_attr(dev, j);
            char buf[1024];
            ret = (int) iio_device_attr_read(dev, attr, buf, sizeof(buf));
            if (ret > 0) {
                DEBUG_MESSAGE("\t\t\t\tattr %u: %s value: %s\r\n", j, attr, buf);
            } else if (ret == -ENOSYS) {
                DEBUG_MESSAGE("\t\t\t\tattr %u: %s\r\n", j, attr);
            } else {
                iio_strerror(-ret, buf, sizeof(buf));

                fprintf(stderr, "Unable to read attribute %s: %s\r\n", attr,
                        buf);
            }
        }

        nb_attrs = iio_device_get_debug_attrs_count(dev);
        if (nb_attrs) {
            DEBUG_MESSAGE("\t\t%u debug attributes found:\r\n", nb_attrs);
            for (j = 0; j < nb_attrs; j++) {
                const char *attr = iio_device_get_debug_attr(dev, j);
                char buf[1024];

                ret = (int) iio_device_debug_attr_read(dev, attr, buf,
                        sizeof(buf));
                if (ret > 0)
                    DEBUG_MESSAGE("\t\t\t\tdebug attr %u: %s value: %s\r\n", j, attr,
                            buf);
                else if (ret == -ENOSYS)
                    DEBUG_MESSAGE("\t\t\t\tdebug attr %u: %s\r\n", j, attr);
            }
        }

        const struct iio_device *trig;
        ret = iio_device_get_trigger(dev, &trig);
        if (ret == 0) {
            if (trig == NULL) {
                DEBUG_MESSAGE("\t\tNo trigger assigned to device\r\n");
            } else {
                name = iio_device_get_name(trig);
                DEBUG_MESSAGE("\t\tCurrent trigger: %s(%s)\r\n",
                        iio_device_get_id(trig), name ? name : "");
            }
        }
    }

    ssize_t nbytes_rx;

    rxbuf = iio_device_create_buffer(dev, NUMBER_OF_SAMPLES, false);
    if (!rxbuf) {
        perror("Could not create RX buffer");
        shutdown();
    }
    DEBUG_MESSAGE("==============================\r\n");
    DEBUG_MESSAGE("Acquiring data from the device\r\n");
    //iio_context_set_timeout(ctx,CTX_TIMEOUT);
    while (true) {
        void *p_dat, *p_end;
        ptrdiff_t p_inc;

        nbytes_rx = iio_buffer_refill(rxbuf);
        if (nbytes_rx < 0) {
            DEBUG_MESSAGE("Error refilling buf %d\n", (int) nbytes_rx);
            shutdown();
        }
        p_inc = iio_buffer_step(rxbuf);
        p_end = iio_buffer_end(rxbuf);
        
        for (p_dat = iio_buffer_first(rxbuf, ch[0]); p_dat < p_end; *(char*)p_dat += p_inc)
        {
            DEBUG_MESSAGE("%f, %f \r\n",((float*)p_dat)[0], ((float*)p_dat)[1]);

        }
        Delay_ms(1000);
        }

    }

#if 0
    extern "C" {
    // Libxml2 relies on getcwd. But newlib does not provide it. 
    char *getcwd(char *buf, size_t size) {
        return NULL;
    }
    }
#endif
