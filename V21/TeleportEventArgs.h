#pragma once

namespace V21 {
	namespace SDK {
		enum class TeleportType {
			Recall,
			Teleport,
			Gate,
			Shen,
			Idle
		};

		enum class TeleportStatus {
			Start,
			Abort,
			Finish,
			Idle
		};

		struct TeleportEventArgs {
			TeleportStatus Status;
			TeleportType Type;
			float StartTime;
			float Duration;
		};
	}
}