/**
 * @file   kbdscan.c
 * @author Henry Leinen
 * @date   10 March 2018
 * @brief  A kernel module for scanning a keyboard matrix which is connected via 9x9 gpio lines.
 * The sysfs entry appears at /sys/ebb/gpio115
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>       // Required for the GPIO functions
#include <linux/kobject.h>    // Using kobjects for the sysfs bindings
#include <linux/kthread.h>
#include <linux/delay.h>
#include "mygpio.h"

#define  THREAD_TIME   5  ///< The thread execution frequency -- 5ms

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Henry Leinen");
MODULE_DESCRIPTION("A simple Linux keyboard matrix driver");
MODULE_VERSION("0.2");

//static const char* gpioRowsNames[9] = { "Row0", "Row1", "Row2", "Row3", "Row4", "Row5", "Row6", "Row7", "Row8" };
//static const char* gpioColsNames[9] = { "Col0", "Col1", "Col2", "Col3", "Col4", "Col5", "Col6", "Col7", "Col8" };

static unsigned int gpioCols[9] = { 18, 23, 24, 25, 12, 16, 20, 26, 19 };
static unsigned int gpioRows[9] = {  2,  3,  4, 17, 27, 22, 10,  9, 11 };
static unsigned int cols_argc = 0, rows_argc = 0;
module_param_array(gpioCols, uint, &cols_argc, S_IRUGO);
MODULE_PARM_DESC(gpioCols, " GPIO Column Pins number array");
module_param_array(gpioRows, uint, &rows_argc, S_IRUGO);
MODULE_PARM_DESC(gpioRows, " GPIO Rows Pins number array");


static char   attrGrpName[8] = "kbdscan";      ///< Null terminated default string -- just in case
static bool   isDebounce = 1;               ///< Use to store the debounce state (on by default)
static int    numberPresses = 0;            ///< For information, store the number of button presses
static int    keyNum = 0;		    ///< The key which was last detected to be pressed
static int    currentRow = 0;
static bool   singleLine = 0;			// do not automatically advance to nex keyboard matrix line. This way, the current scan line is selected by the user
static int    kbdscan_major = 0;	///	Device driver major number

#include "mygpio.c"

static ssize_t keyNum_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return keyNum == 0 ? sprintf(buf, "\n") : sprintf(buf, "%d\n", keyNum);
}

/** @brief A callback function to output the numberPresses variable
 *  @param kobj represents a kernel object device that appears in the sysfs filesystem
 *  @param attr the pointer to the kobj_attribute struct
 *  @param buf the buffer to which to write the number of presses
 *  @return return the total number of characters written to the buffer (excluding null)
 */
static ssize_t numberPresses_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", numberPresses);
}

/** @brief A callback function to read in the numberPresses variable
 *  @param kobj represents a kernel object device that appears in the sysfs filesystem
 *  @param attr the pointer to the kobj_attribute struct
 *  @param buf the buffer from which to read the number of presses (e.g., reset to 0).
 *  @param count the number characters in the buffer
 *  @return return should return the total number of characters used from the buffer
 */
static ssize_t numberPresses_store(struct kobject *kobj, struct kobj_attribute *attr,
                                   const char *buf, size_t count){
	sscanf(buf, "%du", &numberPresses);
	return count;
}

static ssize_t kbdscan_major_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	return sprintf(buf, "%d\n", kbdscan_major);
}

static ssize_t singleLine_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	return sprintf(buf, "%d\n", singleLine);
}

static ssize_t singleLine_store(struct kobject *kobj, struct kobj_attribute *attr,
				const char *buf, size_t count) {
	unsigned int temp;
	sscanf(buf, "%du", &temp);
	if (temp == 0)
		singleLine = FALSE;
	else
		singleLine = TRUE;
	return count;
}


/** @brief A callback function to output the currentRow variable
 *  @param kobj represents a kernel object device that appears in the sysfs filesystem
 *  @param attr the pointer to the kobj_attribute struct
 *  @param buf the buffer to which to write the current selected row number
 *  @return return the total number of characters written to the buffer (excluding null)
 **/
static ssize_t currentRow_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	return sprintf(buf, "%d\n", currentRow);
}

/** @brief A callback function to read in the currentRow variable
 *  @param kobj represents a kernal object device that appears in the sysfs filesystem
 *  @param attr the pointer to the kobj_attribute struct
 *  @param buf the buffer from which to read the currently selected row
 **/
static ssize_t currentRow_store(struct kobject *kobj, struct kobj_attribute *attr,
			 	const char *buf, size_t count) {
	sscanf(buf, "%du", &currentRow);
	return count;
}


/** @brief Displays if button debouncing is on or off */
static ssize_t isDebounce_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", isDebounce);
}

/** @brief Stores and sets the debounce state */
static ssize_t isDebounce_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
	unsigned int temp;
	sscanf(buf, "%du", &temp);                // use a temp varable for correct int->bool


	isDebounce = temp;
	if(isDebounce) {
		printk(KERN_INFO "KBDSCAN : Debounce on\n");
	}
	else {
		printk(KERN_INFO "KBDSCAN: Debounce off\n");
	}
	return count;
}

/**  Use these helper macros to define the name and access levels of the kobj_attributes
 *  The kobj_attribute has an attribute attr (name and mode), show and store function pointers
 *  The count variable is associated with the numberPresses variable and it is to be exposed
 *  with mode 0666 using the numberPresses_show and numberPresses_store functions above
 */
/* warning! need write-all permission so overriding check */ 
//#undef VERIFY_OCTAL_PERMISSIONS
//#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)
static struct kobj_attribute count_attr = __ATTR(numberPresses, 0644, numberPresses_show, numberPresses_store);
static struct kobj_attribute debounce_attr = __ATTR(isDebounce, 0644, isDebounce_show, isDebounce_store);
static struct kobj_attribute currentRow_attr = __ATTR(currentRow, 0644, currentRow_show, currentRow_store);
static struct kobj_attribute singleLine_attr = __ATTR(singleLine, 0644, singleLine_show, singleLine_store);

/**  The __ATTR_RO macro defines a read-only attribute. There is no need to identify that the
 *  function is called _show, but it must be present. __ATTR_WO can be  used for a write-only
 *  attribute but only in Linux 3.11.x on.
 */
static struct kobj_attribute keyNum_attr = __ATTR_RO(keyNum);     ///< the keyNum kobject attr
static struct kobj_attribute kbdscan_major_attr = __ATTR_RO(kbdscan_major);

/**  The ebb_attrs[] is an array of attributes that is used to create the attribute group below.
 *  The attr property of the kobj_attribute is used to extract the attribute struct
 */
static struct attribute *ebb_attrs[] = {
      &count_attr.attr,                  ///< The number of button presses
      &keyNum_attr.attr,                  ///< Is the LED on or off?
      &debounce_attr.attr,               ///< Is the debounce state true or false
      &currentRow_attr.attr,
      &singleLine_attr.attr,
      &kbdscan_major_attr.attr,
      NULL,
};

/**  The attribute group uses the attribute array and a name, which is exposed on sysfs -- in this
 *  case it is gpio115, which is automatically defined in the ebbButton_init() function below
 *  using the custom kernel parameter that can be passed when the module is loaded.
 */
static struct attribute_group attr_group = {
      .name  = attrGrpName,                 ///< The name is generated in ebbButton_init()
      .attrs = ebb_attrs,                ///< The attributes array defined just above
};

static struct kobject *ebb_kobj;
static struct task_struct *task;	// The pointer to the thread task



/** @brief The keyboard scan main kthread loop
 *
 *  @param arg A void pointer used in order to pass data to the thread
 *  @return returns 0 if successful
 */
static int kbdScan(void *arg) {
	unsigned int row = 0;
	unsigned int col = 0;
	static unsigned int act_key_num = 0;
	printk (KERN_INFO "KBDSCAN: Thread has started running\n");
	while (!kthread_should_stop()) {
		set_current_state(TASK_RUNNING);

		/*  do stuff   */
		numberPresses ++;
		/* read the column values and interpret any pressed key */
		if (singleLine)
			act_key_num = 0;
		else if (currentRow==0)
			act_key_num = 0;
		for (col = 0 ; col < 9 ; col++) {
			if (mygpio_digital_read(gpioCols[col]) == 1) {
				/* key press detected */
				act_key_num = col<<4 | currentRow;
			}
		}
		/* advance to next row and set the outputs */
		if (!singleLine)
			currentRow++;
		if (currentRow > 8) {
			currentRow = 0;
			keyNum = act_key_num;
		}
		for (row = 0 ; row < 9 ; row++)
			mygpio_digital_write(gpioRows[row], row == currentRow ? 1 : 0);

		set_current_state(TASK_INTERRUPTIBLE);
		msleep(THREAD_TIME);				/* wait for (default) 20 ms */
	}
	printk (KERN_INFO "KBDSCAN: Thread has run to completion\n");
	return 0;
}



static ssize_t kbdscan_file_read( struct file *file_ptr, char __user *user_Buffer, size_t count, loff_t *position) {

	char key = (char)keyNum;

	if (keyNum == 0) {
		return 0;
	}

	put_user(key, user_Buffer);

	return 1;
}







static struct file_operations	kbdscan_driver_fops = {
	.owner		= THIS_MODULE,
	.read		= kbdscan_file_read,
};


/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point. In this example this
 *  function sets up the GPIOs and the IRQ
 *  @return returns 0 if successful
 */
static int __init kbdScan_init(void){
	int result = 0;
	int col, row;

	printk(KERN_INFO "KBDSCAN: Initializing the Keyboard Matrix LKM\n");
	sprintf(attrGrpName, "matrix0");           // Create the kbdscan name for /sys/kbdscan/matrix0

	// create the kobject sysfs entry at /sys/ebb -- probably not an ideal location!
	ebb_kobj = kobject_create_and_add("kbdscan", kernel_kobj->parent); // kernel_kobj points to /sys/kernel
	if(!ebb_kobj){
		printk(KERN_ALERT "KBDSCAN: failed to create kobject mapping\n");
		return -ENOMEM;
	}
	// add the attributes to /sys/ebb/ -- for example, /sys/ebb/gpio115/numberPresses
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if(result) {
		printk(KERN_ALERT "KBDSCAN: failed to create sysfs group\n");
		kobject_put(ebb_kobj);                          // clean up -- remove the kobject sysfs entry
		return result;
	}

	// Initialize and register the char device driver structure
	printk(KERN_INFO "KBDSCAN: Trying to register device driver \n");
	result = register_chrdev( 0, attrGrpName, &kbdscan_driver_fops );
	if (result < 0) {
		printk (KERN_WARNING "KBDSCAN: can't register character device with errorcode = %i\n", result);
	} else {
		kbdscan_major = result;
		printk (KERN_NOTICE "KBDSCAN: registered character device with major numer = %i and minor numbers 0...255\n", kbdscan_major );
	}

	// Initialize my GPIO
	mygpio_setup();

	// Setup the keyboard matrix IOs for operation
	for (row = 0 ; row < 9 ; row++) {
		mygpio_pin_mode(gpioRows[row], OUTPUT);
		mygpio_digital_write(gpioRows[row], 0);
	}
	for (col = 0 ; col < 9 ; col++) {
		mygpio_pin_mode(gpioCols[col], INPUT);
		mygpio_pull_up_dn_control(gpioCols[col], PUD_DOWN);
	}

	//	Start the keyboard scan thread
	task = kthread_run(kbdScan, NULL, "Kbd_Scan_thread");
	if (IS_ERR(task)) {
		printk (KERN_ALERT "KBDSCAN: failed to create the task\n");
		return PTR_ERR(task);
	}

	return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit kbdScan_exit(void){

	unregister_chrdev(kbdscan_major, attrGrpName);
	kthread_stop(task);
	kobject_put(ebb_kobj);                   // clean up -- remove the kobject sysfs entry
	mygpio_exit();
	printk(KERN_INFO "KBDSCAN: Goodbye from the KBDSCAN LKM!\n");
}


// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(kbdScan_init);
module_exit(kbdScan_exit);
