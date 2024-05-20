#include "rflink.h"
#include "spi.h"
#include "main.h"
#include <string.h>
#include <array>
#include <vector>

void RfLink::init() {
	// init RF mobule #1
	Pin rf1NresetPin(GPIOA, GPIO_PIN_10);
	Pin rf1NcsPin(GPIOA, GPIO_PIN_4);
	Pin rf1BusyPin(GPIOB, GPIO_PIN_12);
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
		sendPacket();
	} else  {
		enterRx();
	}
	HAL_Delay(5000);
	// switch (state) {
	// case START: 
	// 	// 49 us
	// 	packetNumber++;
	// 	if (packetNumber >= 1000) {
	// 		packetNumber = 0;
	// 	}
	// 	break;
	// case SEND_OR_ENTER_RX:
	// 	// sendPacket xxx us, enterRx: 153 us
	// 	upLink() ? sendPacket() : enterRx();
	// 	state = WAITING_FOR_SYNC;
	// 	break;
	// case TRANSMITTED:
	// 	state = DONE;
	// 	break;
	// case SYNC_RECEIVED:
	// 	lostSync = 0;
	//     adjustTimerToTrackTx();
	//     state = WAITING_FOR_RECEIVE;
	//     break;
	// case RECEIVED: {
	// 	// xxx us
	// 	lostPacket = 0;

	// 	uint8_t payload[127];
	// 	uint8_t maxSize = sizeof(payload);
	// 	uint8_t size;
	// 	memset(payload, 0, 127);
	// 	rf1Module->getPayload(payload, &size, maxSize);
	// 	if (size == 41) {
	// 		if (onReceive != nullptr) {
	// 			Packet packet;
	// 			memcpy(&packet.status, &payload[0], 2);
	// 			memcpy(&packet.payload[0], &payload[2], 6);
	// 			onReceive(packet);
	// 		}
	// 	}
	// 	state = DONE;
	// }
	// 	break;
	// case DONE:
	// 	state = WAITING_FOR_NEXT_HOP;
	// 	break;
	// default:
	// 	break;
	// }
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

void RfLink::sendPacket(void) {
	DEBUG("sending message...\n");
	if (onTransmit == nullptr) { return; }
	rf1TxEnable.high();
    Packet packet;
    packet.status.packetNumber = packetNumber;

	char *message = "hello world!";
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

