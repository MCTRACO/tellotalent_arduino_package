#ifndef RMT_H
#define RMT_H

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#include "driver/rmt.h"

#define RMT_MAX_CHANNELS 8
#define RMT_ABS(a) (a>0?a:-a)
#define RMT_LIMIT(a,b) (a>b?b:a)

enum RmtChannel {
	ch0 = 0,
	ch1,
	ch2,
	ch3,
	ch4,
	ch5,
	ch6,
	ch7,
};

typedef void (*RMTRxCallback) (rmt_item32_t* item, uint32_t size, void* parameters);


struct RMTObj_t {
	bool allocated;
    int pin;
    RmtChannel channel;
	rmt_mode_t mode;
	TaskHandle_t handleRxTask;
	RMTRxCallback rxCb;
	void* parameters;
};
static struct RMTObj_t RMTObj[RMT_MAX_CHANNELS] = {
    { false, -1, ch0, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch1, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch2, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch3, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch4, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch5, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch6, RMT_MODE_MAX, NULL, NULL, NULL },
    { false, -1, ch7, RMT_MODE_MAX, NULL, NULL, NULL },
};

class RxTaskClass{
public:
	RxTaskClass(RmtChannel ch): channle(ch){};
	static void RxTaskChannel(void* parameters)
	{
		RxTaskClass* obj = (RxTaskClass*) parameters;
		RingbufHandle_t rb = NULL;
		rmt_get_ringbuf_handle((rmt_channel_t)RMTObj[obj->channle].channel, &rb);
		rmt_rx_start((rmt_channel_t)RMTObj[obj->channle].channel, 1);

		while(rb) 
		{
			size_t rx_size = 0;
			rmt_item32_t* item = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 3000);
			if (item)
			{
				if(RMTObj[obj->channle].rxCb){
					RMTObj[obj->channle].rxCb(item, rx_size, RMTObj[obj->channle].parameters);
				}
				vRingbufferReturnItem(rb, (void*)item);
			};
		};
		RMTObj[obj->channle].handleRxTask = NULL;
		vTaskDelete(NULL); 
	}
private:
	int channle;
};

class RMTClass
{
private:
    uint8_t getClkDiv(float tick);
public:
    RMTClass();
    ~RMTClass();

    RmtChannel txInit(int pin, float tick, uint8_t blockNum=1);
    float      setTick(RmtChannel ch, float tick);
    void       txDeinit(RmtChannel ch);
	bool       write(RmtChannel ch, rmt_item32_t* data, size_t size);
	bool       writeAsync(RmtChannel ch, rmt_item32_t* data, size_t size);
	RmtChannel getChannel(int pin);

    RmtChannel rxInit(int pin, float tick, uint32_t timeOut, uint8_t blockNum=1);
    uint32_t   rxStart(RmtChannel ch, RMTRxCallback rxCb, void * const parameters);
	uint32_t   rxStop(RmtChannel ch);
    void       rxDeinit(RmtChannel ch);
};

extern RMTClass Rmt;

#endif