# lx_task3

This repository includes 3 file:-
2 Application related file:-  
    1. Makefile : To build application  
    2. lxapp.c : Application source file  
1 Driver file :-  
    1. /task3_driver_file/lis3dh_acc.h : To set the default threshold as 100 (as instructed under task3)  

Instructions:  
1. Driver build:  
- Driver should be build by replacing the header with the file available in this repository at path  /task3_driver_file/  
This file set the default threshold as 100.  
2. Application build:    
- Application can be build using make command  
make
- To clean the application build run make clean  
make clean

3. To change threshold from user space:-  
Default threshold is 100  
To change the threshold to new value run echo command as given below:  
echo -e "db,70\0" > /dev/lxaccess  
    where 70 is the new desired threshold  
    
Please note below command to set threshold will not work  
echo "db,100" > /dev/lxaccell  
 Because the string passed using echo is not null terminated by default thus inside the write() function of driver kstrtoul() will fail with Invalid argument error.  

4. If it is required to change the threshold using command like     echo "db,100" > /dev/lxaccell  
Below modification would be required in write() function of driver file "lx_acc_driver.c" to copy the user buffer to kernel space and to make it null terminated before fetching the new threshold value using kstrtoul() function.  
Modified write() function is given below:  


static ssize_t lx_accell_device_write(struct file *file, const char __user *buffer,size_t length, loff_t *offset)  
{  
    struct lx_accell_private_data *pdata = misc_get_drvdata(file);
    unsigned long threshold = 0;
    char tempkbuf[10];
    int ret = 0;
    /* copy 6 bytes from user space */
    /* threshold will be provided from user in last 3 bytes */
    ret = copy_from_user(tempkbuf, buffer, 6);
    if (ret)
       printk("Error in copy_from_user()\n");
    /* make the buffer null terminated for processing THS */
    tempkbuf[6] = '\0';
    if (strstr(tempkbuf, "db,") && !kstrtoul(tempkbuf+3, 0, &threshold)) {
        printk("Applying new threshold: %lu\n", threshold);
        lis3dh_acc_int1_set_threshold(pdata->client, threshold);
    }
    return length;
}







