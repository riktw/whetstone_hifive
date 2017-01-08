# whetstone_hifive
A whetstone benchmark for the hifive riscv board

Useage:

After checking out the freedom-e-sdk, place this directory inside of the software directory of the SDK.
In the SDK folder, build this program using: make software PROGRAM=whetstone_hifive BOARD=freedom-e300-hifive1
And flash it using: make upload PROGRAM=Whetstone BOARD=freedom-e300-hifive1

In a serial terminal like screen or cutecom that is connected to the on board ftdi serial the following should show up:

 My Benchmark example for Whetstones 

Beginning Whetstone benchmark at default 260 MHz ...

 kraj 

Loops: 25000 , \0x09 Iterations: 1, \0x09 Duration: 203.000 sec. 

C Converted Double Precision Whetstones: 12.315 MIPS 