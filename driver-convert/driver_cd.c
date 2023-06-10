#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/init.h>

#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/fs.h>




struct class *device_class;
struct cdev *char_device;
dev_t dev_num;
static int flag_data_send_app = 0;
static char data_send_app[65];

#define CD_2TO8         '0'
#define CD_2TO10        '1'
#define CD_2TO16        '2'
#define CD_8TO2         '3'
#define CD_8TO10        '4'
#define CD_8TO16        '5'
#define CD_16TO2        '6'
#define CD_16TO8        '7'
#define CD_16TO10       '8'

/*8->2*/
int convert8_to_2(char *oct,char *bin)
{
    int i=0;
    while(oct[i])
    {
        switch(oct[i])
        {
            case '0':
                strcat(bin,"000"); break;
            case '1':
                strcat(bin,"001"); break;
            case '2':
                strcat(bin,"010"); break;
            case '3':
                strcat(bin,"011"); break;
            case '4':
                strcat(bin,"100"); break;
            case '5':
                strcat(bin,"101"); break;
            case '6':
                strcat(bin,"110"); break;
            case '7':
                strcat(bin,"111"); break;
            default:
                printk("\n Invalid octal digit %c ", oct[i]);
                return 1;
        }
        i++;
    }
    return 0;
}

/**/

/*16->8*/

int pow(int value, int temp)
{
    int i;
    int t = value;
    if(temp == 0)   return 1;
    if(temp == 1)   return value;
    for(i=1;i<temp;i++)
    {
        value = value * t;
    }

    return value;
}


int convert16_to_8(char hexa[]) {
    int i, size, deci = 0, octa = 0;
    for (size = 0; hexa[size] != '\0'; size++) { } //this loop calculates size of hexadecimal number
    for (i = 0; hexa[i] != '\0'; i++, size--) {
        if (hexa[i] >= '0' && hexa[i] <= '9') {
            deci = deci + (hexa[i] - '0') * pow(16, size - 1);
        }
        if (hexa[i] >= 'A' && hexa[i] <= 'F') {
            deci = deci + (hexa[i] - 55) * pow(16, size - 1);
        }
        if (hexa[i] >= 'a' && hexa[i] <= 'f') {
            deci = deci + (hexa[i] - 87) * pow(16, size - 1);
        }
    } // deci contains the decimal value of given hexadecimal number.
    i = 1;
    while (deci != 0) {
        octa = octa + (deci % 8) * i;
        deci = deci / 8;
        i = i * 10;
    }
    return octa;
}


/*16->2*/

void convert16_to_2(char *hex,char *bin)
{
    int i;
    /* Extract first digit and find binary of each hex digit */
    for(i=0; hex[i]!='\0'; i++)
    {
        switch(hex[i])
        {
            case '0':
                strcat(bin, "0000");
                break;
            case '1':
                strcat(bin, "0001");
                break;
            case '2':
                strcat(bin, "0010");
                break;
            case '3':
                strcat(bin, "0011");
                break;
            case '4':
                strcat(bin, "0100");
                break;
            case '5':
                strcat(bin, "0101");
                break;
            case '6':
                strcat(bin, "0110");
                break;
            case '7':
                strcat(bin, "0111");
                break;
            case '8':
                strcat(bin, "1000");
                break;
            case '9':
                strcat(bin, "1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                break;
            default:
                printk("Invalid hexadecimal input.");
        }
    }
}

/**/

/*convert 2,8,16 -> 10*/
int val(char c)
{
    if (c >= '0' && c <= '9')
        return (int)c - '0';
    else
        return (int)c - 'A' + 10;
}
 
int toDeci(char *str, int base)
{
    int len = strlen(str);
    int power = 1; // Initialize power of base
    int num = 0;  // Initialize result
    int i;
 
    // Decimal equivalent is str[len-1]*1 +
    // str[len-2]*base + str[len-3]*(base^2) + ...
    for (i = len - 1; i >= 0; i--)
    {
        // A digit in input number must be
        // less than number's base
        if (val(str[i]) >= base)
        {
           printk("Invalid Number");
           return -1;
        }
 
        num += val(str[i]) * power;
        power = power * base;
    }
 
    return num;
}
/*end*/

static int open_fun(struct inode *inode, struct file *file)
{
    return 0;
}

static int release_fun(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t read_fun(struct file *file, char *user_buf, size_t len, loff_t *off)
{
    
    if(flag_data_send_app = 1)
    {
        flag_data_send_app=0;
        copy_to_user(user_buf, data_send_app, strlen(data_send_app));
    }
    return 0;
}

static ssize_t write_fun(struct file *file, const char *user_buff, size_t len, loff_t *off)
{
    char buffer[20];
    char value_convert[19];
    int data_cv = 0;
    memset(buffer, '\0', sizeof(buffer));
    copy_from_user(buffer, user_buff, len);
    memcpy(value_convert,&buffer[1],(sizeof(buffer) - 1));
    switch(buffer[0])
    {
        case CD_2TO8:
            data_cv = toDeci(value_convert,2);
            sprintf(data_send_app,"0x%02x",data_cv);
            data_cv = convert16_to_8(data_send_app);
            sprintf(data_send_app,"%d",data_cv);
            break;
        case CD_2TO10:
            data_cv = toDeci(value_convert,2);
            sprintf(data_send_app,"%d",data_cv);
            break;
        case CD_2TO16:
            data_cv = toDeci(value_convert,2);
            sprintf(data_send_app,"0x%02x",data_cv);
            break;
        case CD_8TO2:
            convert8_to_2(value_convert,data_send_app);
            break;
        case CD_8TO10:
            data_cv = toDeci(value_convert,8);
            sprintf(data_send_app,"%d",data_cv);
            break;
        case CD_8TO16:
            data_cv = toDeci(value_convert,8);
            sprintf(data_send_app,"0x%02x",data_cv);
            break;
        case CD_16TO10:
            data_cv = toDeci(value_convert,16);
            sprintf(data_send_app,"%d",data_cv);
            break;
        case CD_16TO2:
            convert16_to_2(value_convert,data_send_app);
            break;
        case CD_16TO8:
            data_cv = convert16_to_8(value_convert);
            sprintf(data_send_app,"%d",data_cv);
        default:
            break;
    }
    flag_data_send_app = 1;
    
    return 0;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = read_fun,
    .write = write_fun,
    .open = open_fun,
    .release = release_fun};


static int __init driver_cd_init(void)
{
    alloc_chrdev_region(&dev_num, 0, 1, "alloc");
    device_class = class_create(THIS_MODULE, "class");
    device_create(device_class, NULL, dev_num, NULL, "driver_chuyendoi");

    char_device = cdev_alloc();
    cdev_init(char_device, &fops);
    cdev_add(char_device, dev_num, 1);
    printk("DRIVER CHUYEN DOI KHOI TAO");
    return 0;
    
}

static void __exit driver_cd_exit(void)

{
    cdev_del(char_device);
   
    device_destroy(device_class, dev_num);
    class_destroy(device_class);
    unregister_chrdev_region(dev_num, 1);
}

module_init(driver_cd_init);

module_exit(driver_cd_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("NHL");

MODULE_DESCRIPTION("Register");
