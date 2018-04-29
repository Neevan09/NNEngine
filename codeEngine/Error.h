#pragma once

namespace codeEngine
{

	enum err_code
	{
		kErrUnknown = -1,

		kOK = 0,

		kErrConfigLoadingFailed,

		kErrUILoadingFailed,

		kErrAudioFailure,

		kErrBadAudioTrack,

		kErrSceneLoadFailed,

		kErrSceneUnloadFailed,

		kErrGameUpdateFailed,

		kErrGameFixedUpdateFailed,
	};

	

}
