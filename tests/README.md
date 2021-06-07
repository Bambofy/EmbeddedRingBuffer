#About

This folder contains the tests we perform to verify the buffer is working 
correctly.

#Listings

| Filename | Name | Description | Result |
| -------- | ---- | ----------- | ------ |
| ReadWrite_Test1.cpp | Read Write Test Number 1 | This test checks that 10 values can be appended and read correctly. | |
| ReadWrite_Test2.cpp | Read Write Test Number 2 | This test checks that 10 integer series can be added in two batched, then a read of 10 succeeds. | |
| ReadWrite_Test3.cpp | Read Write Test Number 3 | This test checks that unordered appending and reading succeeds. | |
| SpeedTest_FastFill.cpp | Speed Test - Fast Fill | This test checks whether the ring buffer can handle quick writes, for example from sampling an ADC 16,000 Hz. | |
| SpeedTest_SlowFill.cpp | Speed Test - Slow Fill | This test checks that the ring buffer performs correctly when written to slowly, for example from a temperature sensor every hour. | |
| SpeedTest_FastFillInterrupt.cpp | Speed Test - Fast Fill With Interrupt | This test checks that the ring buffer performs correctly when samples are added quickly from an ISR, for example an interurpt handle on ADC samples. | |
| SpeedTest_SlowFillInterrupt.cpp | Speed Test - Slow Fill With Interrupt | This test checks that the ring buffer performs correctly when written to slowly from an ISR. | |
| VolumeTest_OverflowResponse.cpp | Volume Test - Overflow Response | This test checks that the ring buffer correctly detects overflows and handles them correctly | |
| VolumeTest_IntermittentFill.cpp | Volume Test - Intermittent Fill | This test checks that the ring buffer operates normally when written to sporadically for example from a sampler with no fixed samplerate. | |
| DataLossTest.cpp | Data Loss Test | This test checks how the ring buffer operates when data is lost | |
| DataCorruptionTest.cpp | Data Corruption Test | This test checks how the ring buffer operates when data is corrupted. | | 

