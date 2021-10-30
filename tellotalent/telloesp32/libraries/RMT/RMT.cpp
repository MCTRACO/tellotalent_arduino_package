#include "RMT.h"






RxTaskClass* taskClass[RMT_MAX_CHANNELS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

RmtChannel RMTClass::txInit(int pin, float tick, uint8_t blockNum)
{
	RmtChannel channel = getChannel(pin);
	RMTObj[channel].mode = RMT_MODE_TX;
	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = (rmt_channel_t)channel;
	config.gpio_num = (gpio_num_t)pin;
	config.mem_block_num = blockNum;
	config.tx_config.loop_en = false;
	config.tx_config.carrier_en = false;
	config.tx_config.idle_output_en = true;
	config.tx_config.idle_level = (rmt_idle_level_t)0;
	config.clk_div = getClkDiv(tick);

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
	return channel;
}

RmtChannel RMTClass::rxInit(int pin, float tick, uint32_t timeOut, uint8_t blockNum)
{
	uint8_t clkdiv = getClkDiv(tick);
	RmtChannel channel = getChannel(pin);
	RMTObj[channel].mode = RMT_MODE_RX;
    rmt_config_t config;
    config.rmt_mode = RMT_MODE_RX;
	config.channel = (rmt_channel_t)channel;
	config.gpio_num = (gpio_num_t)pin;
    config.clk_div = clkdiv;
    config.mem_block_num = blockNum;
    config.rx_config.filter_en = true;
    config.rx_config.filter_ticks_thresh = 100;
    config.rx_config.idle_threshold = timeOut / 10 * (80000000/clkdiv/100000);
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 1000, 0));
	return channel;
}

uint32_t RMTClass::rxStart(RmtChannel ch, RMTRxCallback rxCb, void * const parameters)
{
	if (RMTObj[ch].handleRxTask == NULL && RMTObj[ch].allocated){
		RMTObj[ch].parameters = parameters;
		char name[] = "RxTaskChannelX";
		name[13] = ch + '0';
		RMTObj[ch].rxCb = rxCb;
		taskClass[ch] = new RxTaskClass(ch);
        xTaskCreatePinnedToCore(taskClass[ch]->RxTaskChannel, name, 8192, taskClass[ch], 2, &RMTObj[ch].handleRxTask, 1);
	}
}

uint8_t RMTClass::getClkDiv(float tick){
    int apb_div = RMT_LIMIT(tick/12.5, 256);
    int ref_div = RMT_LIMIT(tick/1000, 256);

    float apb_tick = 12.5 * apb_div;
    float ref_tick = 1000.0 * ref_div;

    if (RMT_ABS(apb_tick - tick) <= RMT_ABS(ref_tick - tick)) {
        return apb_div & 0xFF;
    } else {
        return ref_div & 0xFF;
    }
}

float RMTClass::setTick(RmtChannel ch, float tick)
{
    int apb_div = RMT_LIMIT(tick/12.5, 256);
    int ref_div = RMT_LIMIT(tick/1000, 256);

    float apb_tick = 12.5 * apb_div;
    float ref_tick = 1000.0 * ref_div;

    if (RMT_ABS(apb_tick - tick) < RMT_ABS(ref_tick - tick)) {
        rmt_set_clk_div((rmt_channel_t)ch, apb_div & 0xFF);
        return apb_tick;
    } else {
        rmt_set_clk_div((rmt_channel_t)ch, ref_div & 0xFF);
        return ref_tick;
    }
}

uint32_t RMTClass::rxStop(RmtChannel ch)
{
	if (RMTObj[ch].handleRxTask){
	    vTaskDelete(RMTObj[ch].handleRxTask);
		RMTObj[ch].handleRxTask = NULL;
		RMTObj[ch].rxCb = NULL;
		RMTObj[ch].parameters = NULL;
		delete taskClass[ch];
	}
}

void RMTClass::rxDeinit(RmtChannel ch)
{
    if (ch < RMT_CHANNEL_MAX) {
		if (RMTObj[ch].handleRxTask)
			rxStop(ch);
		rmt_driver_uninstall((rmt_channel_t)ch);
		//gpio_matrix_in(0x30, RMT_SIG_IN0_IDX + ch, false);
		pinMode(RMTObj[ch].pin, INPUT);
		RMTObj[ch].pin = -1;
		RMTObj[ch].allocated = false;
		RMTObj[ch].mode = RMT_MODE_MAX;
	}
}

void RMTClass::txDeinit(RmtChannel ch)
{
    if (ch < RMT_CHANNEL_MAX) {
        rmt_driver_uninstall((rmt_channel_t)ch);
        pinMode(RMTObj[ch].pin, INPUT);
        pinMatrixOutDetach(RMTObj[ch].pin, false, false);
        RMTObj[ch].pin = -1;
        RMTObj[ch].allocated = false;
        RMTObj[ch].mode = RMT_MODE_MAX;
    }
}

bool RMTClass::write(RmtChannel ch, rmt_item32_t* data, size_t size)
{
    ESP_ERROR_CHECK(rmt_write_items((rmt_channel_t)ch, data, size, false));
}

bool RMTClass::writeAsync(RmtChannel ch, rmt_item32_t* data, size_t size)
{
    ESP_ERROR_CHECK(rmt_write_items((rmt_channel_t)ch, data, size, true));
}

RmtChannel RMTClass::getChannel(int pin)
{
	for (int i=0; i<RMT_MAX_CHANNELS; i++) {
		if(RMTObj[i].allocated && RMTObj[i].pin == pin)
		{
			return RMTObj[i].channel;
		}
	}
	for (int i=0; i<RMT_MAX_CHANNELS; i++) {
		if(!RMTObj[i].allocated)
		{
			RMTObj[i].pin = pin;
			RMTObj[i].allocated = true;
			return RMTObj[i].channel;
		}
	}
}

RMTClass::RMTClass() {
	
}
RMTClass::~RMTClass(){}

RMTClass Rmt;

