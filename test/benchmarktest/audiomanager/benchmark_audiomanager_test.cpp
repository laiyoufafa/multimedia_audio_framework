/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include "audio_info.h"
#include "audio_errors.h"
#include "audio_system_manager.h"

using namespace std;
using namespace OHOS;
using namespace OHOS::AudioStandard;

namespace
{
    class AudioManagerTest : public benchmark::Fixture
    {
    public:
        AudioManagerTest()
        {
            Iterations(iterations);
            Repetitions(repetitions);
            ReportAggregatesOnly();
        }

        ~AudioManagerTest() override = default;

        void SetUp(const ::benchmark::State &state) override
        {
            sleep(1);
            instance = AudioSystemManager::GetInstance();
            sleep(1);
        }

        void TearDown(const ::benchmark::State &state) override
        {
        }

    protected:
        AudioSystemManager *instance;
        const int32_t RENDERER_FLAG = 0;
        const int32_t repetitions = 3;
        const int32_t iterations = 300;
        const int32_t MAX_VOL = 15;
        const int32_t MIN_VOL = 0;
    };

    // SetDeviceActiveAbility
    BENCHMARK_F(AudioManagerTest, SetDeviceActiveAbilityTestCase)
    (
        benchmark::State &state)
    {
        while (state.KeepRunning())
        {
            auto ret = instance->SetDeviceActive(ActiveDeviceType::SPEAKER, true);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetDeviceActiveAbilityTestCase audioManager SetDeviceActive true failed.");
            }
            state.PauseTiming();
            ret = instance->SetDeviceActive(ActiveDeviceType::SPEAKER, false);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetDeviceActiveAbilityTestCase audioManager SetDeviceActive false failed.");
            }
            state.ResumeTiming();
        }
    }

    // SetVolumeAbility
    BENCHMARK_F(AudioManagerTest, SetVolumeAbilityTestCase)
    (
        benchmark::State &state)
    {
        while (state.KeepRunning())
        {
            auto ret = instance->SetVolume(AudioVolumeType::STREAM_RING, MAX_VOL);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetVolumeAbilityTestCase audioManager SetVolume MAX_VOL failed.");
            }
            state.PauseTiming();
            ret = instance->SetVolume(AudioVolumeType::STREAM_RING, MIN_VOL);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetVolumeAbilityTestCase audioManager SetVolume MIN_VOL failed.");
            }
            state.ResumeTiming();
        }
    }

    // SetMuteAbility
    BENCHMARK_F(AudioManagerTest, SetMuteAbilityTestCase)
    (
        benchmark::State &state)
    {
        while (state.KeepRunning())
        {
            state.PauseTiming();
            int32_t ret = instance->SetMute(AudioVolumeType::STREAM_RING, true);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetVolumeAbilityTestCase audioManager SetDeviceActive false failed.");
            }
            state.ResumeTiming();
            ret = instance->SetMute(AudioVolumeType::STREAM_RING, false);
            if (ret != SUCCESS)
            {
                state.SkipWithError("SetVolumeAbilityTestCase audioManager SetDeviceActive false failed.");
            }
        }
    }

}

// Run the benchmark
BENCHMARK_MAIN();
