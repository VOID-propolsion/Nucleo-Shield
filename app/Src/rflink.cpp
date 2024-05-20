#include "rflink.h"
#include "spi.h"
#include "main.h"
#include <string.h>
#include <array>
#include <vector>

void RfLink::init() {
	// init RF mobule #1
	Pin rf1NresetPin(RF_Reset_GPIO_Port, RF_Reset_Pin);
	Pin rf1NcsPin(RF_NSS_GPIO_Port, RF_NSS_Pin);
	Pin rf1BusyPin(RF_Busy_GPIO_Port, RF_Busy_Pin);
	this->rf1Module = new SX1280(&hspi1, rf1NresetPin, rf1NcsPin, rf1BusyPin);

	rf1Module->onTxDone = [this]() {
		rf1TxEnable.low();
		state = TRANSMITTED; 
	};

	rf1Module->onSyncWordDone = [this]() {
	    state = SYNC_RECEIVED;
	};

	rf1Module->onRxDone = [this]() {
		rf1RxEnable.low();
		state = RECEIVED;
	};

	rf1Module->init();
	rf1Module->setAddress(0x6969);

    __HAL_TIM_SET_AUTORELOAD(heartBeatTimer, transmitter ? trackingHopRate : acqusitionHopRate);
}

void RfLink::sendPacket(char *message ) {
	DEBUG("sending message...\n");
	if (onTransmit == nullptr) { 
		return; 
	}
	rf1TxEnable.high();
    Packet packet;
    packet.status.packetNumber = packetNumber;

	// Clear the payload
    memset(packet.payload, 0, sizeof(packet.payload));

    // Copy the message into the payload
    strncpy((char*)packet.payload, message, sizeof(packet.payload) - 1);

    onTransmit(packet);
    rf1Module->send((uint8_t *)&packet, sizeof(Packet));
	DEBUG("message sent successfully!\n");
}

void RfLink::enterRx(void) {
	rf1RxEnable.high();
    rf1Module->enterRx();
}

void RfLink::changeMode(void) {
	this->sender = !sender;
}

void RfLink::processHeartBeat(TIM_HandleTypeDef *htim) {
	if (htim != heartBeatTimer) { return; }

	heartBeatTimeout = true;
}

void RfLink::processIrqs(Pin  pin) {
//	static const Pin irqPin(RF1IRQ_GPIO_Port, RF1IRQ_Pin);
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

