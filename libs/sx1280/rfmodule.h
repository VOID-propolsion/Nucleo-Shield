/*
 * rfmodule.h
 *
 *  Created on: Nov 10, 2018
 *      Author: bodri
 */

#ifndef RFMODULE_H_
#define RFMODULE_H_

#include <functional>
#include <vector>

#define PAYLOAD_SIZE	39

typedef struct {
	struct {
		uint16_t packetType : 3;
		uint16_t rfu : 3;
		uint16_t packetNumber : 10;
	} status;

	uint8_t payload[PAYLOAD_SIZE];
} Packet;

class RfModule {
public:
	RfModule() { };
	virtual ~RfModule() { };

	virtual void init(void) = 0;
	virtual void standBy(void) = 0;
	virtual void setAddress(uint16_t) = 0;
	virtual void setChannel(uint8_t) = 0;
	virtual void enterRx(void) = 0;
	virtual void send(Packet *packet) = 0;

	std::function<void(void)> onSyncWordDone;
	std::function<void(void)> onRxDone;
	std::function<void(void)> onTxDone;
};

#endif /* RFMODULE_H_ */
