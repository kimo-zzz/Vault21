#include "stdafx.h"
#include "Engine.h"

bool BuffEntry::IsAlive() {
	auto time = Engine::GetGameTime();
	return this->GetBuffStartTime() < time && time < this->GetBuffEndTime();
}