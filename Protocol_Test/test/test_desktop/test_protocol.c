/*
 Copyright (c) 2014-present PlatformIO <contact@platformio.org>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
**/

#include <unity.h>
#include "protocol.h"
#include "crc.h"

protocol_frame_t frame_expected;
uint8_t data_received[64];
protocol_frame_t frame_read;
crc_status_t crc_status;

void setUp(void) 
{
    frame_expected.address = 0x01;
    frame_expected.actionCode = 2;
    frame_expected.flagStatus = 1;
    frame_expected.payload_temp = 450;
    frame_expected.payload_hum = 980;
   
    protocol_buildFrame(data_received, 64, &frame_expected);

    protocol_readFrame(data_received, &frame_read);
}

void tearDown(void) 
{
    // clean stuff up here
}

void test_protocol_readFrameHumidity(void)
{
    TEST_ASSERT_EQUAL(frame_expected.payload_hum, frame_read.payload_hum);
}

void test_protocol_readFrameTemperature(void)
{
    TEST_ASSERT_EQUAL(frame_expected.payload_temp, frame_read.payload_temp);
}

void test_protocol_CRC(void)
{
    TEST_ASSERT_EQUAL(crc_status, CRC_OK);
}

int main(int argc, char **argv) 
{
    UNITY_BEGIN();
    RUN_TEST(test_protocol_readFrameHumidity);
    RUN_TEST(test_protocol_readFrameTemperature);
    RUN_TEST(test_protocol_CRC);
    UNITY_END();

    return 0;
}