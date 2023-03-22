/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "audio_capturer_file_source.h"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "audio_errors.h"
#include "audio_log.h"

using namespace std;

namespace OHOS {
namespace AudioStandard {
AudioCapturerFileSource::AudioCapturerFileSource()
{
}

AudioCapturerFileSource::~AudioCapturerFileSource()
{
    DeInit();
}

int32_t AudioCapturerFileSource::SetVolume(float left, float right)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::GetVolume(float &left, float &right)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::SetMute(bool isMute)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::GetMute(bool &isMute)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::SetInputRoute(DeviceType inputDevice)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::SetAudioScene(AudioScene audioScene, DeviceType activeDevice)
{
    return SUCCESS;
}

uint64_t AudioCapturerFileSource::GetTransactionId(void)
{
    uint64_t res = -1L;
    return res;
}

int32_t AudioCapturerFileSource::Pause(void)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::Resume(void)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::Reset(void)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::Flush(void)
{
    return SUCCESS;
}

bool AudioCapturerFileSource::IsInited(void)
{
    return capturerInited_;
}

void AudioCapturerFileSource::DeInit()
{
    if (filePtr != nullptr) {
        fclose(filePtr);
        filePtr = nullptr;
    }
    capturerInited_ = false;
}

int32_t AudioCapturerFileSource::Init(IAudioSourceAttr &attr)
{
    const char *filePath = attr.filePath;
    char realPath[PATH_MAX + 1] = {0x00};
    std::string sourceFilePath(filePath);
    std::string rootPath;
    std::string fileName;

    auto pos = sourceFilePath.rfind("/");
    if (pos!= std::string::npos) {
        rootPath = sourceFilePath.substr(0, pos);
        fileName = sourceFilePath.substr(pos);
    }

    if ((strlen(sourceFilePath.c_str()) >= PATH_MAX) || (realpath(rootPath.c_str(), realPath) == nullptr)) {
        AUDIO_ERR_LOG("AudioCapturerFileSource:: Invalid path errno = %{public}d", errno);
        return ERROR;
    }

    std::string verifiedPath(realPath);
    filePtr = fopen(verifiedPath.append(fileName).c_str(), "rb");
    if (filePtr == nullptr) {
        AUDIO_ERR_LOG("Error opening pcm test file!");
        return ERROR;
    }

    capturerInited_ = true;
    return SUCCESS;
}

int32_t AudioCapturerFileSource::CaptureFrame(char *frame, uint64_t requestBytes, uint64_t &replyBytes)
{
    if (filePtr == nullptr) {
        AUDIO_ERR_LOG("Invalid filePtr!");
        return ERROR;
    }

    if (feof(filePtr)) {
        AUDIO_INFO_LOG("End of the file reached, start reading from beginning");
        rewind(filePtr);
    }

    replyBytes = fread(frame, 1, requestBytes, filePtr);

    return SUCCESS;
}

int32_t AudioCapturerFileSource::Start(void)
{
    return SUCCESS;
}

int32_t AudioCapturerFileSource::Stop(void)
{
    if (filePtr != nullptr) {
        fclose(filePtr);
        filePtr = nullptr;
    }
    return SUCCESS;
}
} // namespace AudioStandard
} // namespace OHOS
