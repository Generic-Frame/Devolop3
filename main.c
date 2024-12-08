#include "global_def.h"
#include "host_func.h"
#include "slave_func.h"
#include "event_func.h"

int main()
{
	printf("first func\n");
#ifdef SLAVE

	app_gbl_t *resc = app_global_def();

	if(resc)
	{
		bsp_slave_init(resc);
		bsp_event_init(resc);
	}

	Debug("firmware version:%s\n",FIRMWARE_VERSION);

#else

	dev_proc_t * dev_proc = creat_device();

	if(dev_proc)
	{
		auto_scan_slave(dev_proc);
	}

#endif

	while(1)  {

		usleep(2000);
	}
	printf("hello");
	printf("end111\n");
}
