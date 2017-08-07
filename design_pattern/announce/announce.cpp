#include "announce.h"
#include <iostream>

using namespace cvte::ui;

Announce::Announce(AnnounceMessages messages ,
                   std::shared_ptr<IAnnounceListener> listener)
	: messages_(messages)
	, announce_listener_(listener) {
	for (const auto& message : messages_) {
		AnnounceManager::GetInstance()->RegisterListener(message , announce_listener_);
    }
}

Announce::Announce(AnnounceMessages messages, const AnnounceCallback &callback):
    Announce(messages, std::make_shared<LambdaAnnounceListener>(callback)){}

Announce::~Announce() {
	for (const auto& message : messages_) {
		AnnounceManager::GetInstance()->UnRegisterListener(message , announce_listener_);
	}
	messages_.clear();
}

void Announce::PublishMessage(const AnnounceMessage& message, void* data) {
	AnnounceManager::GetInstance()->PublishMessage(message, data);
}

AnnounceManager* AnnounceManager::GetInstance() {
	static AnnounceManager instance;
	return &instance;
}

AnnounceManager::AnnounceManager()
	: listener_() {
}
AnnounceManager::~AnnounceManager() {
}


void AnnounceManager::RegisterListener(const AnnounceMessage& message ,
        std::shared_ptr<IAnnounceListener> handler) {
    listener_.Register(message, handler);
}

void AnnounceManager::UnRegisterListener(const AnnounceMessage& message ,
 std::shared_ptr<IAnnounceListener> handler) {
    listener_.UnRegister(message, handler);
}

void AnnounceManager::PublishMessage(const AnnounceMessage& message, void *data) {
    listener_.Publish(message);
}

LambdaAnnounceListener::LambdaAnnounceListener():
    LambdaAnnounceListener(AnnounceCallback()){}

LambdaAnnounceListener::LambdaAnnounceListener(const AnnounceCallback &callback):
    callback_(callback){}

void LambdaAnnounceListener::OnAnnounce(const AnnounceMessage &message, void *data){
    if(callback_){
        callback_(message);
    }
}

void IAnnounceListener::OnAction(const AnnounceMessage &message, void *data) {
    OnAnnounce(message, data);
}
