#include "rflink.h"
#include "spi.h"
#include "main.h"
#include <string.h>
#include <array>
#include <vector>

void RfLink::init() {
	// init RF mobule #1
	Pin rfNresetPin(RF_Reset_GPIO_Port, RF_Reset_Pin);
	Pin rfNcsPin(RF_NSS_GPIO_Port, RF_NSS_Pin);
	Pin rfBusyPin(RF_Busy_GPIO_Port, RF_Busy_Pin);
	this->rfModule = new SX1280(&hspi1, rfNresetPin, rfNcsPin, rfBusyPin);
	rfRxEnable.high();
	rfTxEnable.low();

	rfModule->onTxDone = [this]() {
		rfTxEnable.low();
		state = TRANSMITTED; 
	};

	rfModule->onSyncWordDone = [this]() {
	    state = SYNC_RECEIVED;
	};

	rfModule->onRxDone = [this]() {
		rfRxEnable.low();
		state = RECEIVED;
	};

	rfModule->init();
	rfModule->setAddress(0x6969);

    __HAL_TIM_SET_AUTORELOAD(heartBeatTimer, transmitter ? trackingHopRate : acqusitionHopRate);
}

void RfLink::sendPacket(char *message ) {
	DEBUG("sending message...\n");
	if (onTransmit == nullptr) { 
		return; 
	}
	rfTxEnable.high();
    Packet packet;
    packet.status.packetNumber = packetNumber;

	// Clear the payload
    memset(packet.payload, 0, sizeof(packet.payload));

    // Copy the message into the payload
    strncpy((char*)packet.payload, message, sizeof(packet.payload) - 1);

    onTransmit(packet);
    rfModule->send(&packet);
	DEBUG("message sent successfully!\n");
}

bool RfLink::receivePacket(Packet *buffer) {
	bool response = rfModule->getPayload(buffer);
    return response;
}

void RfLink::enterRx(void) {
	rfRxEnable.high();
    rfModule->enterRx();
}

void RfLink::changeMode(void) {
	this->sender = !sender;
}

void RfLink::processHeartBeat(TIM_HandleTypeDef *htim) {
	if (htim != heartBeatTimer) { return; }

	heartBeatTimeout = true;
}

void RfLink::processIrqs(Pin  pin) {
//	static const Pin irqPin(rfIRQ_GPIO_Port, rfIRQ_Pin);
//	if (pin == irqPin) {
//		lastIrqSource = MODULE1_IRQ;
//	}
}

void RfLink::runLoop(void) {
	if (sender) {
		sendPacket("hello world!");
	} else  {
		enterRx();
	}
	HAL_Delay(10000);
}

//
// Private methods
//

bool RfLink::upLink(void) {
	if (tracking) {
		return transmitter;
	} else {
		return transmitter;
	}
}

void RfLink::setTracking(bool tracking) {
	if (this->tracking != tracking) {
		this->tracking = tracking;
		if (tracking) {
			if (!transmitter) {
				__HAL_TIM_SET_AUTORELOAD(heartBeatTimer, trackingHopRate);
			}
		} else {
			if (!transmitter) {
				__HAL_TIM_SET_AUTORELOAD(heartBeatTimer, transmitter ? trackingHopRate : acqusitionHopRate);
			}
		}
	}
}

void RfLink::adjustTimerToTrackTx(void) {
	__HAL_TIM_SET_COUNTER(heartBeatTimer, timerWhenSynReceived);
	setTracking(true);
}

void RfLink::registerLostPacket(void) {
	lostSync++;
	lostPacket++;

	if (lostSync > transmitter ? lostSyncTreshold / downLinkFrequency : lostSyncTreshold) {
		setTracking(false);
	}
}

